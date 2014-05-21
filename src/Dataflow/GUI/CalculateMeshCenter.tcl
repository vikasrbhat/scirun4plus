#
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


catch {rename SCIRun_MiscField_CalculateMeshCenter ""}

itcl::class SCIRun_MiscField_CalculateMeshCenter {
    inherit Module

     constructor { {args ""} } {
        eval configure $args
        set name CalculateMeshCenter
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            raise $w
            return;
        }

        sci_toplevel $w

        sci_labeledframe $w.bound -labeltext "Method for computing center"
        pack $w.bound -expand yes -fill both

        set frame [$w.bound childsite]

        sci_radiobutton $frame.nodecenter -text "Average of Node locations" \
            -variable $this-method -value "node-center"
        sci_radiobutton $frame.elemcenter -text "Average of Element centers" \
            -variable $this-method -value "elem-center"
        sci_radiobutton $frame.weightedelemcenter -text "Volumetric center" \
            -variable $this-method -value "weighted-elem-center"
        sci_radiobutton $frame.boundingboxcenter -text "Bounding box center" \
            -variable $this-method -value "bounding-box-center"
        sci_radiobutton $frame.midnode -text "Middle index node" \
            -variable $this-method -value "mid-node-index"
        sci_radiobutton $frame.midelem -text "Middle index element" \
            -variable $this-method -value "mid-elem-index"
            
       pack $frame.nodecenter $frame.elemcenter  $frame.weightedelemcenter \
            $frame.boundingboxcenter $frame.midnode $frame.midelem  \
            -side top -anchor nw

        makeSciButtonPanel $w $w $this
        moveToCursor $w
    }
}