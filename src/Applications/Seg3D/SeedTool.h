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
//    File   : SeedTool.h
//    Author : Michael Callahan
//    Date   : March 2008

#ifndef SEG3D_SeedTool_h
#define SEG3D_SeedTool_h

#include <string>
#include <Core/Events/BaseTool.h>
#include <Core/Thread/ThreadLock.h>
#include <Applications/Seg3D/VolumeFilter.h>

namespace SCIRun {

class SeedTool : public virtual BaseTool, public PointerTool
{
public:
  SeedTool(const string &name, Painter *painter);
  virtual ~SeedTool();

  propagation_state_e   pointer_down(int, int, int, unsigned int, int);
  propagation_state_e   pointer_up(int, int, int, unsigned int, int);
  propagation_state_e   pointer_motion(int, int, int, unsigned int, int);
  propagation_state_e   process_event(event_handle_t);

  void set_just_one_seed_mode(bool flag) { just_one_seed_mode_ = flag; }
  bool get_just_one_seed_mode() { return just_one_seed_mode_; }

  // Set which button is used to place seedpoints.
  void set_button(int b) { button_ = b; }

protected:
  virtual void          seed_change_callback();

  void                  recreate_seed_geom();

  typedef vector<Point> seeds_t;

  virtual void          run_filter() = 0;
  virtual void          draw_gl(SliceWindow &window);

  void                  draw_seed(const Point &seed, SliceWindow &);

  void                  convert_seeds_to_indices(vector<vector<int> > &iseeds,
                                                 NrrdVolumeHandle &vol);

  Painter *             painter_;

  seeds_t               seeds_;

  ThreadLock            seed_lock_;

  // Only one seed point, autoreplaces itself.
  bool                  just_one_seed_mode_;

  // Which button adds seeds.  Usually 1 but 2 for Brush.
  int                   button_;

  // For 2D seed operations, do the operation where the seed was put.
  SliceWindow          *last_seed_window_;

private:
  int                   last_seed_index_;
};
  
  
}

#endif
