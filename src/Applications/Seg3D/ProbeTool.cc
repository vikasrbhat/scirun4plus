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
//    File   : ProbeTool.cc
//    Author : McKay Davis
//    Date   : Sat Oct 14 16:11:09 2006

#include <Applications/Seg3D/ProbeTool.h>
#include <Applications/Seg3D/Painter.h>

namespace SCIRun {

ProbeTool::ProbeTool(Painter *painter) : 
  PointerTool("Probe"),
  painter_(painter)
{
}


BaseTool::propagation_state_e
ProbeTool::pointer_down(int, int, int, unsigned int, int)
{
  painter_->set_probe();
  return STOP_E;
}

BaseTool::propagation_state_e
ProbeTool::pointer_motion(int, int, int, unsigned int, int)
{
  painter_->set_probe();
  return STOP_E;
}


BaseTool::propagation_state_e
ProbeTool::pointer_up(int, int, int, unsigned int, int)
{
  return QUIT_AND_STOP_E;
}


}  
