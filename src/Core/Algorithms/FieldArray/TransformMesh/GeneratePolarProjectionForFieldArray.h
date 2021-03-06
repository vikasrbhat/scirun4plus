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

#ifndef CORE_ALGORITHMS_FIELDARRAY_TRANSFORMFIELD_GENERATEPOLARPORJECTIONFORFIELDARRAY_H
#define CORE_ALGORITHMS_FIELDARRAY_TRANSFORMFIELD_GENERATEPOLARPORJECTIONFORFIELDARRAY_H 1

//! Datatypes that the algorithm uses
#include <Core/Datatypes/FieldArray.h>

//! Base class for algorithm
#include <Core/Algorithms/Util/AlgoBase.h>

//! for Windows support
#include <Core/Algorithms/FieldArray/share.h>

namespace SCIRunAlgo {

using namespace SCIRun;

class SCISHARE GeneratePolarProjectionForFieldArrayAlgo : public AlgoBase
{
  public:
    //! Set defaults
    GeneratePolarProjectionForFieldArrayAlgo()
    {
      add_vector("direction",Vector(0.0,0.0,10));
      add_point("origin",Point(0.0,0.0,0.0));
      add_scalar("spacing",0.0);    
    }
  
    //! run the algorithm
    bool run(FieldArrayHandle input,  FieldArrayHandle& output);

};


} // end namespace SCIRunAlgo

#endif 

