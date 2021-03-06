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

//    File   : TendAnplot.cc
//    Author : Martin Cole
//    Date   : Mon Sep  8 09:46:49 2003

#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/NrrdPort.h>
#include <teem/ten.h>

namespace SCITeem {

using namespace SCIRun;

class TendAnplot : public Module {
  public:
    TendAnplot(SCIRun::GuiContext *ctx);
    virtual ~TendAnplot() {}
    virtual void execute();

  private:
    GuiInt          resolution_;
    GuiInt          horizontal_flip_;
    GuiInt          whole_;
    GuiInt          values_;
    GuiString       anisotropy_;

    unsigned int get_anisotropy(const std::string &an);
};

DECLARE_MAKER(TendAnplot)

TendAnplot::TendAnplot(SCIRun::GuiContext *ctx) : 
  Module("TendAnplot", ctx, Filter, "Tend", "Teem"),
  resolution_(get_ctx()->subVar("resolution"), 256),
  horizontal_flip_(get_ctx()->subVar("hflip"), 0),
  whole_(get_ctx()->subVar("whole"), 0),
  values_(get_ctx()->subVar("values"), 0),
  anisotropy_(get_ctx()->subVar("anisotropy"), "cl1")
{
}


void 
TendAnplot::execute()
{
  // Force Teem to be locked befoer calling the Teem library
  NrrdGuard nrrd_guard;

  // Inform module that execution started
  update_state(Executing);

  NrrdDataHandle nrrd_handle;
  Nrrd *nout = nrrdNew();

  if (tenAnisoPlot(nout,
                   get_anisotropy(anisotropy_.get()), 
		               resolution_.get(),
                   horizontal_flip_.get(),
                   whole_.get(),
                   values_.get())) 
  {
    char *err = biffGetDone(TEN);
    error(std::string("Error in TendAnplot: ") + err);
    free(err);
    return;
  }

  NrrdDataHandle out(new NrrdData(nout));
  send_output_handle("OutputNrrd", out);
}


unsigned int
TendAnplot::get_anisotropy(const std::string &an)
{
  if (an == "cl1")
    return tenAniso_Cl1;
  else if (an == "cl2")
    return tenAniso_Cl2;
  else if (an == "cp1")
    return tenAniso_Cp1;
  else if (an == "cp2")
    return tenAniso_Cp2;
  else if (an == "ca1")
    return tenAniso_Ca1;
  else if (an == "ca2")
    return tenAniso_Ca2;
  else if (an == "cs1")
    return tenAniso_Cs1;
  else if (an == "cs2")
    return tenAniso_Cs2;
  else if (an == "ct1")
    return tenAniso_Ct1;
  else if (an == "ct2")
    return tenAniso_Ct1;
  else if (an == "ra")
    return tenAniso_RA;
  else if (an == "fa")
    return tenAniso_FA;
  else if (an == "vf")
    return tenAniso_VF;
  else if (an == "tr")
    return tenAniso_Tr;
  else 
  {
    error("Unkown anisotropy metric.  Using trace");
    return tenAniso_Tr;
  }
}

} // End namespace SCITeem



