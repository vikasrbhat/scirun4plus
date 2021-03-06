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
//    File   : ITKCannyEdgeDetectionImageFilterTool.cc
//    Author : David Brayford
//    Date   : May 2008

#include <Applications/Seg3D/ITKCannyEdgeDetectionImageFilterTool.h>

#include <Applications/Seg3D/Seg3DFrame.h>
#include <Applications/Seg3D/BrushTool.h>
#include <Applications/Seg3D/Painter.h>
#include <Applications/Seg3D/VolumeOps.h>
#include <Core/Util/Assert.h>

#include <Applications/Seg3D/GuiCode/itkCannyEdgeDetectionImageFilter.h>
#include <Applications/Seg3D/Seg3DwxGuiUtils.h>
#include <wx/string.h>
#include <wx/variant.h>

namespace SCIRun {

ITKCannyEdgeDetectionImageFilterTool::
ITKCannyEdgeDetectionImageFilterTool(Painter *painter) :
  BaseTool("ITK Canny Edge Detection ImageFilter Tool"),
  painter_(painter),
  Variance_(painter->get_vars(), "itkCannyEdgeDetectionImageFilter::mVariance"),
  Max_Error_(painter->get_vars(), "itkCannyEdgeDetectionImageFilter::mMaxError"),
  Threshold_(painter->get_vars(), "itkCannyEdgeDetectionImageFilter::mThreshold"),
  filter_()
{
}


BaseTool::propagation_state_e 
ITKCannyEdgeDetectionImageFilterTool::process_event
(event_handle_t event)
{
  // check for the FinishEvent signal generated by the start button
  if (dynamic_cast<FinishEvent *>(event.get_rep())) {
    if (!painter_->check_for_active_data_volume("Canny Edge Detection filter"))
    {
      return STOP_E;
    }
    run_filter();
  }
 
  return CONTINUE_E;
}


void
ITKCannyEdgeDetectionImageFilterTool::run_filter()
{
  NrrdVolumeHandle new_layer =
    painter_->copy_current_layer("Canny Edge Detection");

  filter_.set_volume( new_layer );

  // Set the filter parameters.
  const double variance = Variance_;
  const double max_error = Max_Error_/100.0;
  const double threshold = Threshold_;

  filter_->SetVariance(variance);
  filter_->SetMaximumError(max_error);
  filter_->SetUpperThreshold(threshold);

  painter_->set_status("Running canny edge detection filter.");
  painter_->start_progress();
  filter_();
  painter_->finish_progress();

  painter_->current_volume_->reset_data_range();

  // Change the result from an image to a label
  painter_->volume_lock_.lock();
  new_layer->set_label(1);
  new_layer->set_label_color( NrrdVolume::get_next_label_color() );
  // To go directly to a labeled volume like what is done in
  // ITKOtsuImageFilterTool the results must be negative and
  // postive. For the edges it is either 0 or 1 so instead change the
  // type using a threshold at 1.

  new_layer->change_type_from_float_to_bit(1.0);
  painter_->volume_lock_.unlock();

  UndoHandle undo =
    new UndoReplaceLayer(painter_, "Undo Canny Edge Detection",
                         0, painter_->current_volume_, 0);
  painter_->push_undo(undo);

  // Rebuild so the layer buttoms show up correctly.
  painter_->rebuild_layer_buttons();

  // Redraw everything after completion.
  painter_->extract_all_window_slices();
  painter_->redraw_all();

  painter_->hide_tool_panel();
}


}
