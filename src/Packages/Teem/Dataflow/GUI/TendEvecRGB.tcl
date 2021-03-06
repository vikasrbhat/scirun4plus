#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2009 Scientific Computing and Imaging Institute,
#  University of Utah.
# 
#  
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#

#    File   : TendEvecRGB.tcl
#    Author : Martin Cole
#    Date   : Mon Sep  8 09:46:23 2003

catch {rename Teem_Tend_TendEvecRGB ""}

itcl::class Teem_Tend_TendEvecRGB {
    inherit Module

     constructor { {args ""} } {
        eval configure $args
        set name TendEvecRGB
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            raise $w
            return;
        }

        sci_toplevel $w

        sci_frame $w.f
        pack $w.f -padx 2 -pady 2 -side top -expand yes
        
        sci_frame $w.f.options
        pack $w.f.options -side top -expand yes

        sci_frame $w.f.options.f1 -borderwidth 2 -relief groove
        make_labeled_radio $w.f.options.f1.evec \
            "Eigenvector to use" "" top 1 $this-evec \
            {{"Major" 0}
          {"Medium" 1}
          {"Minor" 2}}
        pack $w.f.options.f1.evec -side top -expand yes -fill x
        pack $w.f.options.f1 -side top -expand yes -fill x

        sci_frame $w.f.options.f2 -borderwidth 2 -relief groove
        make_labeled_radio $w.f.options.f2.aniso_metric \
            "Anisotropy Metric"  "" top 1 $this-aniso_metric \
            {{"Westin's linear (first version)" "tenAniso_Cl1"} 
          {"Westin's planar (first version)" "tenAniso_Cp1"} 
          {"Westin's linear + planar (first version)" "tenAniso_Ca1"}
          {"Westin's spherical (first version)" "tenAniso_Cs1"}
          {"gk's anisotropy type (first version)" "tenAniso_Ct1"}
          {"Westin's linear (second version)" "tenAniso_Cl2"}
          {"Westin's planar (second version)" "tenAniso_Cp2"}
          {"Westin's linear + planar (second version)" "tenAniso_Ca2"}
          {"Westin's spherical (second version)" "tenAniso_Cs2"}
          {"gk's anisotropy type (second version)" "tenAniso_Ct2"}
          {"Bass+Pier's relative anisotropy" "tenAniso_RA"}
          {"(Bass+Pier's fractional anisotropy)/sqrt(2)" "tenAniso_FA"}
          {"volume fraction = 1-(Bass+Pier's volume ratio)" "tenAniso_VF"}
          {"plain old trace" "tenAniso_Tr"}}
        pack $w.f.options.f2.aniso_metric -side top -expand yes -fill x
        pack $w.f.options.f2 -side top -expand yes -fill x

        sci_entryfield $w.f.options.background -labeltext \
          "Background:" -textvariable $this-background
        pack $w.f.options.background -side top -expand yes -fill x
        Tooltip $w.f.options.background "Gray level to use\nfor voxels who's\nconfidence is zero"

        sci_entryfield $w.f.options.gray -labeltext \
          "Gray:" -textvariable $this-gray
        pack $w.f.options.gray -side top -expand yes -fill x
        Tooltip $w.f.options.gray "Gray level to desaturate\ntowards an anisotropy"

        sci_entryfield $w.f.options.gamma -labeltext \
          "Gamma:" -textvariable $this-gamma
        pack $w.f.options.gamma -side top -expand yes -fill x
        Tooltip $w.f.options.gamma "Gamma to use on color components"

        sci_entryfield $w.f.options.threshold -labeltext \
          "Threshold:" -textvariable $this-threshold
        pack $w.f.options.threshold -side top -expand yes -fill x
        Tooltip $w.f.options.threshold "Confidence Threshold"

        makeSciButtonPanel $w $w $this
        moveToCursor $w

        pack $w.f -expand 1 -fill x
    }
}
