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

#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/DenseMatrix.h>
#include <Dataflow/Network/Ports/MatrixPort.h>
#include <Dataflow/Network/Module.h>

namespace SCIRun {

class CreateDataArrayFromIndices : public Module {
  public:
    CreateDataArrayFromIndices(GuiContext*);
    virtual ~CreateDataArrayFromIndices() {}
    virtual void execute();
};


DECLARE_MAKER(CreateDataArrayFromIndices)

CreateDataArrayFromIndices::CreateDataArrayFromIndices(GuiContext* ctx)
  : Module("CreateDataArrayFromIndices", ctx, Source, "DataArrayMath", "SCIRun")
{
}

void
CreateDataArrayFromIndices::execute()
{
  MatrixHandle Index, Template, Array;
  
  get_input_handle("Indices",Index,true);
  get_input_handle("Template",Template,true);
  
  if (inputs_changed_ || !oport_cached("DataArray"))
  {    
    update_state(Executing);
    MatrixHandle temp = Template->dense();
    MatrixHandle indices = Index->dense();
    
    if (temp == 0)
    {
      error("Cannot read matrix on the 'Template' input port");
      return;
    }
    
    if (indices == 0)
    {
      error("Cannot read matrix on the 'Indices' input port");
      return;
    }
    
    size_type temp_m = temp->nrows(); 
    size_type temp_n = temp->ncols();
    double* temp_data = temp->get_data_pointer();

    size_type indices_m = indices->nrows(); 
    size_type indices_n = indices->ncols();
    double* indices_data = indices->get_data_pointer();
    
    if (temp_data == 0)
    {
      error("No data in the Template matrix");
      return;
    }

    if (indices_data == 0)
    {
      error("No data in the Indices matrix");
      return;
    }
    
    // transpose on paper
    if ((indices_m == 1)&&(indices_n > 1)) { indices_m = indices_n; indices_n = 1;}
    
    if ((indices_m == 0)||(indices_n == 0))
    {
      error("Indices matrix is empty");
      return;  
    }

    if ((temp_m == 0)||(temp_n == 0))
    {
      error("Template matrix is empty");
      return;  
    }

    if (indices_n != 1)
    {
      error("Number of columns of index vector is larger than one");
      return;
    }
      
    Array = new DenseMatrix(indices_m,temp_n);
    double* array_data = Array->get_data_pointer();
    
    for (index_type p = 0; p < indices_m; p++)
    {
      index_type index = static_cast<index_type>(indices_data[p]);
      if ((index < 0)||(index >= temp_m))
      {
        for (index_type q = 0; q < temp_n ;q++) array_data[p*temp_n+q] = 0.0;
      }
      else
      {
        double* t_data = temp_data + temp_n*index;
        for (index_type q = 0; q < temp_n ;q++) array_data[p*temp_n+q] = t_data[q];
      }
    }  
    
    send_output_handle("DataArray", Array);
  }
}


} // End namespace SCIRun


