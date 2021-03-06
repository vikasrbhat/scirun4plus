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

//    File   : TendSten.cc
//    Author : Martin Cole
//    Date   : Mon Sep  8 09:46:49 2003

#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/NrrdPort.h>
#include <teem/ten.h>

namespace SCITeem {

using namespace SCIRun;

class TendSten : public Module {
  public:
    TendSten(SCIRun::GuiContext *ctx);
    virtual ~TendSten() {}
    virtual void execute();

  private:
    GuiInt          diffscale_;
    GuiInt          intscale_;
    GuiInt          factor_;
};

DECLARE_MAKER(TendSten)

TendSten::TendSten(SCIRun::GuiContext *ctx) : 
  Module("TendSten", ctx, Filter, "Tend", "Teem"),
  diffscale_(get_ctx()->subVar("diffscale"), 1),
  intscale_(get_ctx()->subVar("intscale"), 2),
  factor_(get_ctx()->subVar("factor"), 1)
{
}


void 
TendSten::execute()
{
  update_state(NeedData);

  NrrdDataHandle nrrd_handle;
  get_input_handle("InputNrrd", nrrd_handle);

  if (inputs_changed_ || diffscale_.changed() || intscale_.changed() ||
      factor_.changed() || !oport_cached("OutputNrrd"))
  {
    Nrrd *nin = nrrd_handle->nrrd_;
    Nrrd *nout = nrrdNew();

    if (gageStructureTensor(nout, nin, diffscale_.get(), intscale_.get(), 
          factor_.get()))
    {
      char *err = biffGetDone(TEN);
      error(std::string("Error calculating tensors: ") + err);
      free(err);
      return;
    }

    NrrdDataHandle out(new NrrdData(nout));

    send_output_handle("OutputNrrd", out);
  }
}


} // End namespace SCITeem

