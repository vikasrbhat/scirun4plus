//
//  For more information, please see: http://software.sci.utah.edu
//
//  The MIT License
//
//  Copyright (c) 2009 Scientific Computing and Imaging Institute,
//  University of Utah.
//
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//    File   : FrameBufferPickTool.h
//    Author : Martin Cole
//    Date   : Tue Jul 18 14:05:48 2006

#if !defined(FrameBufferPickTool_h)
#define FrameBufferPickTool_h

#include <Core/Events/OGLView/ViewToolInterface.h>
#include <Core/Events/BaseTool.h>
#include <Core/Geom/View.h>
#include <Core/Events/OGLView/Ball.h>
#include <Core/Events/OGLView/BallMath.h>
#include <Core/Geometry/Transform.h>
#include <vector>
#include <set>

namespace SCIRun {

class FBInterface
{
public:
  FBInterface() {}
  virtual ~FBInterface() {}
  virtual int width() = 0;
  virtual int height() = 0;
  virtual std::vector<unsigned char> &img() = 0;
  virtual void do_pick_draw() = 0;
  virtual void add_selection(unsigned int) = 0;
  virtual void remove_selection(unsigned int) = 0;
};


class FrameBufferPickTool : public PointerTool
{
public:
  FrameBufferPickTool(const std::string& name, FBInterface *fbi);
  virtual ~FrameBufferPickTool();

  //! which == button number, x,y in window at event time
  virtual propagation_state_e pointer_down(int which, int x, int y,
                                           unsigned int modifiers,
                                           int time);
  //! which == button number, x,y in window at event time
  virtual propagation_state_e pointer_motion(int which, int x, int y,
                                             unsigned int modifiers,
                                             int time);
  //! which == button number, x,y in window at event time
  virtual propagation_state_e pointer_up(int which, int x, int y,
                                         unsigned int modifiers,
                                         int time);

  virtual propagation_state_e process_event(event_handle_t e);

private:
  void add_pick_to_selection(int x, int y);
  void remove_pick_from_selection(int x, int y);
  bool get_index_at_selection(int x, int y, unsigned int &idx);

  FBInterface                      *fbi_;
};


} // namespace SCIRun

#endif //FrameBufferPickTool_h
