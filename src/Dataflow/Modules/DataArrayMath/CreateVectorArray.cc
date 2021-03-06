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

class CreateVectorArray : public Module {
  public:
    CreateVectorArray(GuiContext*);
    virtual ~CreateVectorArray() {}
    virtual void execute();
};


DECLARE_MAKER(CreateVectorArray)
CreateVectorArray::CreateVectorArray(GuiContext* ctx)
  : Module("CreateVectorArray", ctx, Source, "DataArrayMath", "SCIRun")
{
}


void
CreateVectorArray::execute()
{
  MatrixHandle X,Y,Z,V;
  
  get_input_handle("X",X,true);
  get_input_handle("Y",Y,true);
  get_input_handle("Z",Z,true);

  if (inputs_changed_ || !oport_cached("VectorArray"))
  {
    update_state(Executing);
    // This needs to go to algorithms
    int n;
    int Xn, Yn, Zn;
    Xn = X->nrows();
    Yn = Y->nrows();
    Zn = Z->nrows();
    
    n = Xn;
    if (Yn > n) n = Yn;
    if (Zn > n) n = Zn;
    
    if (((Xn!=1)&&(Yn!=1)&&(Xn!=Yn))||((Yn!=1)&&(Zn!=1)&&(Yn!=Zn))||(Xn==0)||(Yn==0)||(Zn==0))
    {
      error("Improper matrix dimensions: all the matrices should have the same number of rows");
      return;
    }

    if (X->ncols()!=1)
    {
      error("X matrix should have only one column");
      return;
    }

    if (Y->ncols()!=1)
    {
      error("Y matrix should have only one column");
      return;
    }

    if (Z->ncols()!=1)
    {
      error("Z matrix should have only one column");
      return;
    }

    X = X->dense();
    Y = Y->dense();
    Z = Z->dense();

    V = new DenseMatrix(n,3);
    
    double* xptr = X->get_data_pointer();
    double* yptr = Y->get_data_pointer();
    double* zptr = Z->get_data_pointer();
    double* vptr = V->get_data_pointer();
    
    for (int p=0;p<n;p++)
    {
      vptr[0] = *xptr;
      vptr[1] = *yptr;
      vptr[2] = *zptr;
      
      if (Xn > 1) xptr++;
      if (Yn > 1) yptr++;
      if (Zn > 1) zptr++;
      
      vptr += 3;
    }

    send_output_handle("VectorArray", V);
  }
}



} // End namespace SCIRun


