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
 *  NrrdPort.cc
 *
 *  Written by:
 *   David Weinstein
 *   Department of Computer Science
 *   University of Utah
 *   February 2001
 *
 */

#include <Dataflow/Network/Ports/NrrdPort.h>


#undef SCISHARE
#if defined(_WIN32) && !defined(BUILD_SCIRUN_STATIC)
#define SCISHARE __declspec(dllexport)
#else
#define SCISHARE
#endif

namespace SCIRun {

extern "C" {
  SCISHARE IPort* make_NrrdIPort(Module* module, const std::string& name) {
  return new SimpleIPort(module,name,"Nrrd","cyan2");
}
  SCISHARE OPort* make_NrrdOPort(Module* module, const std::string& name) {
  return new SimpleOPort(module,name,"Nrrd","cyan2");
}
}

} // End namespace SCIRun

