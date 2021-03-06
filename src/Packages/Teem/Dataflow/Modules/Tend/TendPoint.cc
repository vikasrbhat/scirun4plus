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

//    File   : TendPoint.cc
//    Author : Martin Cole
//    Date   : Mon Sep  8 09:46:49 2003

#include <Dataflow/Network/Module.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <Dataflow/Network/Ports/NrrdPort.h>
#include <Core/Util/StringUtil.h>
#include <teem/ten.h>

namespace SCITeem {

using namespace SCIRun;

class TendPoint : public Module {
  public:
    TendPoint(SCIRun::GuiContext *ctx);
    virtual ~TendPoint() {}
    virtual void execute();

  private:
    GuiInt          x_;
    GuiInt          y_;
    GuiInt          z_;
    GuiDouble       confidence_;
    GuiDouble       tensor1_, tensor2_, tensor3_, tensor4_, tensor5_, tensor6_;
    GuiDouble       eval1_, eval2_, eval3_;
    GuiDouble       evec1_, evec2_, evec3_, evec4_, evec5_;
    GuiDouble       evec6_, evec7_, evec8_, evec9_;
    GuiDouble       angle_;
    GuiDouble       axis1_, axis2_, axis3_;
    GuiDouble       mat1_, mat2_, mat3_, mat4_, mat5_;
    GuiDouble       mat6_, mat7_, mat8_, mat9_;
    GuiDouble       cl1_;
    GuiDouble       cp1_;
    GuiDouble       ca1_;
    GuiDouble       cs1_;
    GuiDouble       ct1_;
    GuiDouble       cl2_;
    GuiDouble       cp2_;
    GuiDouble       ca2_;
    GuiDouble       cs2_;
    GuiDouble       ct2_;
    GuiDouble       ra_;
    GuiDouble       fa_;
    GuiDouble       vf_;
    GuiDouble       b_;
    GuiDouble       q_;
    GuiDouble       r_;
    GuiDouble       s_;
    GuiDouble       skew_;
    GuiDouble       th_;
    GuiDouble       det_;
    GuiDouble       tr_;
};

DECLARE_MAKER(TendPoint)

TendPoint::TendPoint(SCIRun::GuiContext *ctx) : 
  Module("TendPoint", ctx, Filter, "Tend", "Teem"), 
  x_(get_ctx()->subVar("x")),
  y_(get_ctx()->subVar("y")),
  z_(get_ctx()->subVar("z")),
  confidence_(get_ctx()->subVar("confidence")),
  tensor1_(get_ctx()->subVar("tensor1")),
  tensor2_(get_ctx()->subVar("tensor2")),
  tensor3_(get_ctx()->subVar("tensor3")),
  tensor4_(get_ctx()->subVar("tensor4")),
  tensor5_(get_ctx()->subVar("tensor5")),
  tensor6_(get_ctx()->subVar("tensor6")),
  eval1_(get_ctx()->subVar("eval1")),
  eval2_(get_ctx()->subVar("eval2")),
  eval3_(get_ctx()->subVar("eval3")),
  evec1_(get_ctx()->subVar("evec1")),
  evec2_(get_ctx()->subVar("evec2")),
  evec3_(get_ctx()->subVar("evec3")),
  evec4_(get_ctx()->subVar("evec4")),
  evec5_(get_ctx()->subVar("evec5")),
  evec6_(get_ctx()->subVar("evec6")),
  evec7_(get_ctx()->subVar("evec7")),
  evec8_(get_ctx()->subVar("evec8")),
  evec9_(get_ctx()->subVar("evec9")),
  angle_(get_ctx()->subVar("angle")),
  axis1_(get_ctx()->subVar("axis1")),
  axis2_(get_ctx()->subVar("axis2")),
  axis3_(get_ctx()->subVar("axis3")),
  mat1_(get_ctx()->subVar("mat1")),
  mat2_(get_ctx()->subVar("mat2")),
  mat3_(get_ctx()->subVar("mat3")),
  mat4_(get_ctx()->subVar("mat4")),
  mat5_(get_ctx()->subVar("mat5")),
  mat6_(get_ctx()->subVar("mat6")),
  mat7_(get_ctx()->subVar("mat7")),
  mat8_(get_ctx()->subVar("mat8")),
  mat9_(get_ctx()->subVar("mat9")),
  cl1_(get_ctx()->subVar("cl1")),
  cp1_(get_ctx()->subVar("cp1")),
  ca1_(get_ctx()->subVar("ca1")),
  cs1_(get_ctx()->subVar("cs1")),
  ct1_(get_ctx()->subVar("ct1")),
  cl2_(get_ctx()->subVar("cl2")),
  cp2_(get_ctx()->subVar("cp2")),
  ca2_(get_ctx()->subVar("ca2")),
  cs2_(get_ctx()->subVar("cs2")),
  ct2_(get_ctx()->subVar("ct2")),
  ra_(get_ctx()->subVar("ra")),
  fa_(get_ctx()->subVar("fa")),
  vf_(get_ctx()->subVar("vf")),
  b_(get_ctx()->subVar("b")),
  q_(get_ctx()->subVar("q")),
  r_(get_ctx()->subVar("r")),
  s_(get_ctx()->subVar("s")),
  skew_(get_ctx()->subVar("skew")),
  th_(get_ctx()->subVar("th")),
  det_(get_ctx()->subVar("det")),
  tr_(get_ctx()->subVar("tr"))
{
}

void 
TendPoint::execute()
{
  update_state(NeedData);

  NrrdDataHandle nrrd_handle;
  get_input_handle("InputNrrd", nrrd_handle);

  if (inputs_changed_ || x_.changed() || y_.changed() || z_.changed())
  {
    // Force Teem to be locked befoer calling the Teem library
    NrrdGuard nrrd_guard;

    // Inform module that execution started
    update_state(Executing);

    Nrrd *nin = nrrd_handle->nrrd_;

    if (tenTensorCheck(nin, nrrdTypeFloat, AIR_TRUE, AIR_TRUE)) 
    {
      char *err = biffGetDone(TEN);
      error(std::string("Didn't get a valid DT volume: ") + err);
      free(err);
      return;
    }

    int loc[3];
    loc[0] = x_.get();
    loc[1] = y_.get();
    loc[2] = z_.get();

    int sx = 0, sy = 0, sz = 0;
    sx = nin->axis[1].size;
    sy = nin->axis[2].size;
    sz = nin->axis[3].size;

    if ((loc[0] > sx-1) ||
        (loc[0] < 0) ||
        (loc[1] > sy-1) ||
        (loc[1] < 0) ||
        (loc[2] > sz-1) ||
        (loc[2] < 0) ) 
    {
      error(std::string("Location : " + to_string(loc[0]) + ", " +
       to_string(loc[1]) + ", " + to_string(loc[2]) + 
       " not inside volume"));
      return;
    }
    
    std::cerr << "Done with check\n";
    float eval[3], evec[9], c[TEN_ANISO_MAX+1];
    float * tdata =  (float*)(nin->data) + 7*(loc[0] + sx*(loc[1] + sy*loc[2]));
    confidence_.set(tdata[0]);

    tensor1_.set(tdata[1]);
    tensor2_.set(tdata[2]);
    tensor3_.set(tdata[3]);
    tensor4_.set(tdata[4]);
    tensor5_.set(tdata[5]);
    tensor6_.set(tdata[6]);

    tenEigensolve_f(eval, evec, tdata);
    eval1_.set(eval[0]);
    eval2_.set(eval[1]);
    eval3_.set(eval[2]);
    evec1_.set(evec[0]);
    evec2_.set(evec[1]);
    evec3_.set(evec[2]);
    evec4_.set(evec[3]);
    evec5_.set(evec[4]);
    evec6_.set(evec[5]);
    evec7_.set(evec[6]);
    evec8_.set(evec[7]);
    evec9_.set(evec[8]);

    float axis[3], mat[9];
    float angle = ell_3m_to_aa_f(axis, evec);
    angle_.set(angle);
    axis1_.set(axis[0]);
    axis2_.set(axis[1]);
    axis3_.set(axis[2]);

    ell_aa_to_3m_f(mat, angle, axis);
    mat1_.set(mat[0]);
    mat2_.set(mat[1]);
    mat3_.set(mat[2]);
    mat4_.set(mat[3]);
    mat5_.set(mat[4]);
    mat6_.set(mat[5]);
    mat7_.set(mat[6]);
    mat8_.set(mat[7]);
    mat9_.set(mat[8]);

    /*
     * tenAnisoEval_f (or tenAnisoEval_d) is used calculate a particular
     * anisotropy value from pre-computed eigenvalues.
     *
     * Alternatively, use tenAnisoEval_[f|d] to go from the full tensor value
     * to an anisotropy value. 
     * See http://sourceforge.net/forum/forum.php?forum_id=896169 for
     * comments regarding the removal of the tenAnisoCalc_f function.
     */
    c[1] = tenAnisoEval_f(eval, tenAniso_Cl1);
    c[2] = tenAnisoEval_f(eval, tenAniso_Cp1);
    c[3] = tenAnisoEval_f(eval, tenAniso_Ca1);
    c[4] = tenAnisoEval_f(eval, tenAniso_Cs1);
    c[5] = tenAnisoEval_f(eval, tenAniso_Ct1);
    c[6] = tenAnisoEval_f(eval, tenAniso_Cl2);
    c[7] = tenAnisoEval_f(eval, tenAniso_Cp2);
    c[8] = tenAnisoEval_f(eval, tenAniso_Ca2);
    c[9] = tenAnisoEval_f(eval, tenAniso_Cs2);
    c[10] = tenAnisoEval_f(eval, tenAniso_Ct2);
    c[11] = tenAnisoEval_f(eval, tenAniso_RA);
    c[12] = tenAnisoEval_f(eval, tenAniso_FA);
    c[13] = tenAnisoEval_f(eval, tenAniso_VF);
    c[14] = tenAnisoEval_f(eval, tenAniso_B);
    c[15] = tenAnisoEval_f(eval, tenAniso_Q);
    c[16] = tenAnisoEval_f(eval, tenAniso_R);
    c[17] = tenAnisoEval_f(eval, tenAniso_S);
    c[18] = tenAnisoEval_f(eval, tenAniso_Skew);
    c[19] = tenAnisoEval_f(eval, tenAniso_Th);
    c[20] = tenAnisoEval_f(eval, tenAniso_Det);
    c[21] = tenAnisoEval_f(eval, tenAniso_Tr);

    cl1_.set(c[1]);
    cp1_.set(c[2]);
    ca1_.set(c[3]);
    cs1_.set(c[4]);
    ct1_.set(c[5]);
    cl2_.set(c[6]);
    cp2_.set(c[7]);
    ca2_.set(c[8]);
    cs2_.set(c[9]);
    ct2_.set(c[10]);
    ra_.set(c[11]);
    fa_.set(c[12]);
    vf_.set(c[13]);
    b_.set(c[14]);
    q_.set(c[15]);
    r_.set(c[16]);
    s_.set(c[17]);
    skew_.set(c[18]);
    th_.set(c[19]);
    det_.set(c[20]);
    tr_.set(c[21]);
  }
}

} // End namespace SCITeem
