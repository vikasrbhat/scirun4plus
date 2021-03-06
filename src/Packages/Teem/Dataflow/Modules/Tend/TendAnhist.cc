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

//    File   : TendAnhist.cc
//    Author : Martin Cole
//    Date   : Mon Sep  8 09:46:49 2003

#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/NrrdPort.h>
#include <teem/ten.h>

namespace SCITeem {

using namespace SCIRun;

class TendAnhist : public Module {
public:
  TendAnhist(SCIRun::GuiContext *ctx);
  virtual ~TendAnhist() {}
  virtual void execute();

private:
  GuiInt          right_;
  GuiInt          westin_;
  GuiInt          resolution_;
};

DECLARE_MAKER(TendAnhist)

TendAnhist::TendAnhist(SCIRun::GuiContext *ctx) : 
  Module("TendAnhist", ctx, Filter, "Tend", "Teem"),
  right_(ctx->subVar("right"), 0),
  westin_(ctx->subVar("westin"), 1),
  resolution_(ctx->subVar("resolution"), 256)
{
}


void 
TendAnhist::execute()
{
  update_state(NeedData);

  NrrdDataHandle nrrd_handle;
  get_input_handle("InputNrrd", nrrd_handle,true);

  // weights nrrd optional
  NrrdDataHandle weight_handle;
  get_input_handle("WeightNrrd", weight_handle,true);

  if (inputs_changed_ || right_.changed() || westin_.changed() ||
      resolution_.changed() || !oport_cached("OutputNrrd"))
  {
    // Force Teem to be locked befoer calling the Teem library
    NrrdGuard nrrd_guard;

    // Inform module that execution started
    update_state(Executing);
      
    Nrrd *nin = nrrd_handle->nrrd_;
    Nrrd *nout = nrrdNew();
    Nrrd *weight = 0;
    if (weight_handle.get_rep())
    {
      weight = weight_handle->nrrd_;
    }

    if (tenAnisoHistogram(nout, nin, weight, right_.get(),
                          westin_.get(), resolution_.get()))
    {
      char *err = biffGetDone(TEN);
      error(std::string("Error generating barycentric histograms of anisotropy: ")
            + err);
      free(err);
      return;
    }

    NrrdDataHandle out(new NrrdData(nout));
    send_output_handle("OutputNrrd", out);
  }
}

} // End namespace SCITeem



