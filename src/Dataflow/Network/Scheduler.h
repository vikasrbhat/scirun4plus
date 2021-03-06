/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/



/*
 *  Scheduler.h: 
 *
 *  Written by:
 *   Steven G. Parker
 *   Department of Computer Science
 *   University of Utah
 *   April 2002
 *
 */

#ifndef SCIRun_Dataflow_Network_Scheduler_h
#define SCIRun_Dataflow_Network_Scheduler_h

#include <Dataflow/Comm/MessageBase.h>
#include <Dataflow/GuiInterface/GuiCallback.h>
#include <Core/Thread/Mailbox.h>
#include <Core/Thread/Runnable.h>

#include <Core/Containers/LockingHandle.h>
#include <string>
#include <list>

#include <Dataflow/Network/share.h>

namespace SCIRun {

class Connection;
class MessageBase;
class Module;
class Network;
class OPort;

typedef LockingHandle<OPort>      OPortHandle;
typedef LockingHandle<Module>     ModuleHandle;
typedef LockingHandle<Connection> ConnectionHandle;

struct SerialSet
{
  int base;
  int size;
  int callback_count;

  SerialSet(int base, int s) :
    base(base), size(s), callback_count(0)
  {}
};



// Return whether or not we should fall through to next callback or
// not.  For instance if you reschedule you should probably return
// false because you wouldn't want the lower priority callbacks to run
// until after you were finished.
typedef bool (*SchedulerCallback)(void *);

class SCISHARE Scheduler : public Runnable
{
  Network* net;
  bool first_schedule;
  bool schedule;
  unsigned int serial_id;
  std::list<SerialSet> serial_set;

  struct SCData
  {
    int priority;
    SchedulerCallback callback;
    void *data;

    // Self test for equality.  Used to remove callbacks from queue.
    bool operator()(const SCData &a)
    {
      return callback == a.callback && data == a.data;
    }
  };

  std::vector<SCData> callbacks_;
  std::vector<SCData> start_callbacks_; // prior to execution of any module
  Mutex callback_lock_;
  Mutex need_execute_lock_;

  virtual void run();
  void main_loop();
  void multisend_real(OPortHandle);
  void do_scheduling_real(ModuleHandle);
  void report_execution_finished_real(unsigned int serial);
  void report_execution_started();

  Mailbox<MessageBase*> mailbox;

public:

  Scheduler(Network*);
  ~Scheduler();
    
  // Prevent modules from changing their need_execute_ state while
  //   the dataflow dependency tree is being walked
  inline void lockNeedExecute() { need_execute_lock_.lock(); }
  inline void unlockNeedExecute() { need_execute_lock_.unlock(); }

  // Turns scheduler on and off
  // Returns: true if scheduler is now on, false if scheduler is now off
  bool toggleOnOffScheduling();

  void do_scheduling();
  void request_multisend(OPortHandle);

  // msg must be of type ModuleExecute.
  void report_execution_finished(const MessageBase *msg);
  void report_execution_finished(unsigned int serial);

  // The callbacks will be called in order from highest priority to
  // lowest priority.  If a callback returns false then the callbacks
  // with lower priority will not be called.
  void add_callback(SchedulerCallback cv, void *data, int priority = 0);
  void remove_callback(SchedulerCallback cv, void *data);

  // These get called prior to execution of any module with the same priority
  // scheme as for end execution callbacks.
  void add_start_callback(SchedulerCallback cv, void *data, int priority = 0);
  void remove_start_callback(SchedulerCallback cv, void *data);  
};


class Scheduler_Module_Message : public MessageBase {
public:
  ConnectionHandle conn;
  unsigned int serial;
  Scheduler_Module_Message(unsigned int serial, bool ignored);
  Scheduler_Module_Message(unsigned int serial);
  Scheduler_Module_Message(ConnectionHandle conn);
  virtual ~Scheduler_Module_Message();
};


class Module_Scheduler_Message : public MessageBase {
public:
  OPortHandle p1;
  unsigned int serial;
  Module_Scheduler_Message();
  Module_Scheduler_Message(OPortHandle);
  Module_Scheduler_Message(unsigned int serial);
  virtual ~Module_Scheduler_Message();
};


} // End namespace SCIRun

#endif

