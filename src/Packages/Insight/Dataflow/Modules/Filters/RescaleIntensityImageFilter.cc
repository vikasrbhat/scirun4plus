/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
   University of Utah.

   
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
 * RescaleIntensityImageFilter.cc
 *
 *   Auto Generated File For itk::RescaleIntensityImageFilter
 *
 */


#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/ITKDatatypePort.h>

#include <itkVector.h>

#include <itkRescaleIntensityImageFilter.h>

#include <itkCommand.h>

namespace Insight 
{

using namespace SCIRun;

class RescaleIntensityImageFilter : public Module 
{
public:

  typedef itk::MemberCommand< RescaleIntensityImageFilter > RedrawCommandType;

  // Filter Declaration
  itk::Object::Pointer filter_;

  // Declare GuiVars
  GuiDouble  gui_minimum_;
  GuiDouble  gui_maximum_;
  
  bool execute_;
  
  // Declare Ports
  ITKDatatypeHandle inhandle_InputImage_;
  int last_InputImage_;

  ITKDatatypeHandle outhandle_OutputImage_;

  RescaleIntensityImageFilter(GuiContext*);

  virtual ~RescaleIntensityImageFilter();

  virtual void execute();

  // Run function will dynamically cast data to determine which
  // instantiation we are working with. The last template type
  // refers to the last template type of the filter intstantiation.
  template<class InputImageType, class OutputImageType > 
  bool run( itk::Object*   );

  // progress bar
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event );
  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event );
  void Observe( itk::Object *caller );
  RedrawCommandType::Pointer m_RedrawCommand;
};


template<class InputImageType, class OutputImageType>
bool 
RescaleIntensityImageFilter::run( itk::Object *obj_InputImage) 
{
  InputImageType *data_InputImage = dynamic_cast<  InputImageType * >(obj_InputImage);
  
  if( !data_InputImage ) {
    return false;
  }

  typedef typename itk::RescaleIntensityImageFilter< InputImageType, OutputImageType > FilterType;

  // Check if filter_ has been created
  // or the input data has changed. If
  // this is the case, set the inputs.

  if(!filter_  || 
     inhandle_InputImage_->generation != last_InputImage_)
  {
     last_InputImage_ = inhandle_InputImage_->generation;

     // create a new one
     filter_ = FilterType::New();

     // attach observer for progress bar
     Observe( filter_.GetPointer() );

     // set inputs 
     
     dynamic_cast<FilterType* >(filter_.GetPointer())->SetInput( data_InputImage );
  }

  // reset progress bar
  update_progress(0.0);

  // set filter parameters
  dynamic_cast<FilterType* >(filter_.GetPointer())->SetOutputMinimum( gui_minimum_.get() ); 
  
  dynamic_cast<FilterType* >(filter_.GetPointer())->SetOutputMaximum( gui_maximum_.get() ); 
  

  // execute the filter
  try {

    dynamic_cast<FilterType* >(filter_.GetPointer())->Update();

  } catch ( itk::ExceptionObject & err ) {
     error("ExceptionObject caught!");
     error(err.GetDescription());
  }

  // get filter output
  ITKDatatype* out_OutputImage_ = new ITKDatatype; 
  
  out_OutputImage_->data_ = dynamic_cast<FilterType* >(filter_.GetPointer())->GetOutput();
  
  outhandle_OutputImage_ = out_OutputImage_; 
  send_output_handle("OutputImage", outhandle_OutputImage_, true);

  return true;
}


DECLARE_MAKER(RescaleIntensityImageFilter)

RescaleIntensityImageFilter::RescaleIntensityImageFilter(GuiContext* ctx)
  : Module("RescaleIntensityImageFilter", ctx, Source, "Filters", "Insight"),
     gui_minimum_(get_ctx()->subVar("minimum")),
     gui_maximum_(get_ctx()->subVar("maximum")), 
     last_InputImage_(-1)
{
  filter_ = 0;


  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &RescaleIntensityImageFilter::ProcessEvent );
  m_RedrawCommand->SetCallbackFunction( this, &RescaleIntensityImageFilter::ConstProcessEvent );
}


RescaleIntensityImageFilter::~RescaleIntensityImageFilter() 
{
}


void 
RescaleIntensityImageFilter::execute() 
{
  // check input ports
  if (!get_input_handle("InputImage", inhandle_InputImage_)) return;

  // get input
  itk::Object* data_InputImage = inhandle_InputImage_.get_rep()->data_.GetPointer();
  
  // can we operate on it?
  if(0) { }
  else if(run< itk::Image<float, 2>, itk::Image<float, 2> >( data_InputImage )) {} 
  else if(run< itk::Image<float, 3>, itk::Image<float, 3> >( data_InputImage )) {} 
  else {
    // error
    error("Incorrect input type");
    return;
  }
}


// Manage a Progress event 
void 
RescaleIntensityImageFilter::ProcessEvent( itk::Object * caller, const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent )   ==  typeid( event ) )
  {
    ::itk::ProcessObject::Pointer  process = 
        dynamic_cast< itk::ProcessObject *>( caller );

    const double value = static_cast<double>(process->GetProgress() );
    update_progress( value );
  }
}


// Manage a Progress event 
void 
RescaleIntensityImageFilter::ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent )   ==  typeid( event ) )
  {
    ::itk::ProcessObject::ConstPointer  process = 
        dynamic_cast< const itk::ProcessObject *>( caller );

    const double value = static_cast<double>(process->GetProgress() );
    update_progress( value );
  }
}


// Manage a Progress event 
void 
RescaleIntensityImageFilter::Observe( itk::Object *caller )
{
  caller->AddObserver(  itk::ProgressEvent(), m_RedrawCommand.GetPointer() );
  caller->AddObserver(  itk::IterationEvent(), m_RedrawCommand.GetPointer() );
}


} // End of namespace Insight
