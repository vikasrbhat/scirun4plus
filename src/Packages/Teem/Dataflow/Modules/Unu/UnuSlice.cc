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

//    File   : UnuSlice.cc
//    Author : Martin Cole
//    Date   : Mon Sep  8 09:46:49 2003

#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/NrrdPort.h>

#include <sstream>
#include <iostream>
using std::endl;
#include <stdio.h>

namespace SCITeem {

using namespace SCIRun;

class UnuSlice : public Module {
public:
  UnuSlice(SCIRun::GuiContext *ctx);
  virtual ~UnuSlice() {}
  virtual void execute();

private:
  GuiInt       axis_;
  GuiInt       position_;
};

DECLARE_MAKER(UnuSlice)

UnuSlice::UnuSlice(SCIRun::GuiContext *ctx) : 
  Module("UnuSlice", ctx, Filter, "UnuNtoZ", "Teem"), 
  axis_(get_ctx()->subVar("axis"), 0),
  position_(get_ctx()->subVar("position"), 0)
{
}


void 
UnuSlice::execute()
{
  update_state(NeedData);

  NrrdDataHandle nrrd_handle;
  get_input_handle("nin", nrrd_handle,true);

  if (inputs_changed_ || axis_.changed() || position_.changed() ||
      !oport_cached("nout"))
  {
    // Force Teem to be locked befoer calling the Teem library
    NrrdGuard nrrd_guard;

    // Inform module that execution started
    update_state(Executing);
      
    reset_vars();

    Nrrd *nin = nrrd_handle->nrrd_;
    Nrrd *nout = nrrdNew();
    
    if (nrrdSlice(nout, nin, axis_.get(), position_.get())) {
      char *err = biffGetDone(NRRD);
      error(std::string("Error Slicing nrrd: ") + err);
      free(err);
    }

    NrrdDataHandle out(new NrrdData(nout));
    send_output_handle("nout", out);
  }
}


} // End namespace SCITeem
