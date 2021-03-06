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
#include <Dataflow/Network/Module.h>
#include <Dataflow/Network/Ports/MatrixPort.h>

#include <teem/ten.h>

namespace SCIRun {

class DecomposeTensorArrayIntoEigenVectors : public Module
{
  public:
    DecomposeTensorArrayIntoEigenVectors(GuiContext*);
    virtual ~DecomposeTensorArrayIntoEigenVectors() {}
    virtual void execute();
};



DECLARE_MAKER(DecomposeTensorArrayIntoEigenVectors)

DecomposeTensorArrayIntoEigenVectors::DecomposeTensorArrayIntoEigenVectors(GuiContext* ctx) :
  Module("DecomposeTensorArrayIntoEigenVectors", ctx, Source, "DataArrayMath", "SCIRun")
{
}

void
DecomposeTensorArrayIntoEigenVectors::execute()
{
  MatrixHandle eigvec1, eigvec2, eigvec3, eigval1, eigval2, eigval3,tensor, temp;
  
  get_input_handle("TensorArray",tensor,true);
  
  if (inputs_changed_ || !oport_cached("EigenVector1") ||
      !oport_cached("EigenVector2") || !oport_cached("EigenVector3") ||
      !oport_cached("EigenValue1") || !oport_cached("EigenValue2") ||
      !oport_cached("EigenValue3"))
  {
    update_state(Executing);
      
    if ((tensor->ncols()!=6)||(tensor->ncols()!=9))
    {
      error("TenosrArray needs to have 6 or 9 columns");
      return;
    }

    if (tensor->nrows()==0)
    {
      error("TensorArray is empty");
      return;
    }

    temp = tensor->dense();
    tensor = temp;

    const int n = tensor->nrows();
    
    eigvec1 = new DenseMatrix(n, 3);
    eigvec2 = new DenseMatrix(n, 3);
    eigvec3 = new DenseMatrix(n, 3);
    
    eigval1 = new DenseMatrix(n, 1);
    eigval2 = new DenseMatrix(n, 1);
    eigval3 = new DenseMatrix(n, 1);
    
    double* eigvec1ptr = eigvec1->get_data_pointer();
    double* eigvec2ptr = eigvec2->get_data_pointer();
    double* eigvec3ptr = eigvec3->get_data_pointer();
    double* eigval1ptr = eigval1->get_data_pointer();
    double* eigval2ptr = eigval2->get_data_pointer();
    double* eigval3ptr = eigval3->get_data_pointer();
    double* tensorptr  = tensor->get_data_pointer();

    double ten[7];
    double eval[3];
    double evec[9];
    bool  is_9tensor = true;
    if (tensor->ncols() == 6) is_9tensor = false;
    
    for (int p=0; p<n; p++)
    {
      ten[0] = 1.0;
      if (is_9tensor)
      {
        ten[1] = tensorptr[0];
        ten[2] = tensorptr[1];
        ten[3] = tensorptr[2];
        ten[4] = tensorptr[4];
        ten[5] = tensorptr[5];
        ten[6] = tensorptr[8];
        tensorptr += 9;
      }
      else
      {
        ten[1] = tensorptr[0];
        ten[2] = tensorptr[1];
        ten[3] = tensorptr[2];
        ten[4] = tensorptr[3];
        ten[5] = tensorptr[4];
        ten[6] = tensorptr[5];
        tensorptr += 6;
      }

      tenEigensolve_d(eval, evec, ten);

      eigvec1ptr[0] = evec[0];
      eigvec1ptr[1] = evec[1];
      eigvec1ptr[2] = evec[2];

      eigvec2ptr[0] = evec[3];
      eigvec2ptr[1] = evec[4];
      eigvec2ptr[2] = evec[5];
      
      eigvec3ptr[0] = evec[6];
      eigvec3ptr[1] = evec[7];
      eigvec3ptr[2] = evec[8];
      
      *eigval1ptr = eval[0];
      *eigval2ptr = eval[1];
      *eigval3ptr = eval[2];
      
      eigvec1ptr +=3;
      eigvec2ptr +=3;
      eigvec3ptr +=3;
      eigval1ptr++;
      eigval2ptr++;
      eigval3ptr++;
    }
    
    send_output_handle("EigenVector1", eigvec1);
    send_output_handle("EigenVector2", eigvec2);
    send_output_handle("EigenVector3", eigvec3);
    send_output_handle("EigenValue1", eigval1);
    send_output_handle("EigenValue2", eigval2);
    send_output_handle("EigenValue3", eigval3);
  }
}


} // End namespace SCIRun


