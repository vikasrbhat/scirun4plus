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
//    File   : ITKOtsuImageFilterTool.h
//    Author : David Brayford
//    Date   : May 2008

#ifndef SEG3D_ITKOTSUImageFilterTool_h
#define SEG3D_ITKOTSUImageFilterTool_h

#include <sci_defs/insight_defs.h>

#include <string>
#include <Core/Events/BaseTool.h>
#include <Core/Datatypes/ITKDatatype.h>
#include <Applications/Seg3D/VolumeFilter.h>
#include <itkImageToImageFilter.h>
#include <itkCommand.h>
#include <itkOtsuThresholdImageFilter.h>

namespace SCIRun {

class Painter;
class NrrdVolume;

  
class ITKOtsuImageFilterTool : public virtual BaseTool
{
public:
  ITKOtsuImageFilterTool(Painter *painter);
  propagation_state_e   process_event(event_handle_t);

private:
  void                  run_filter();
  Painter *             painter_;
  NrrdVolumeHandle      source_volume_;
  string                prefix_;

 
  typedef itk::OtsuThresholdImageFilter
    < ITKImageFloat3D, ITKImageLabel3D > FilterType;

  VolumeFilter<FilterType>      filter_;
};
  
  
}

#endif
