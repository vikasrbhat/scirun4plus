##
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

itcl::class SCIRun_String_ReportStringInfo {
    inherit Module

     constructor { {args ""} } {
        eval configure $args
        set name ReportStringInfo
    }

    method ui {} {
    
        global $this-inputstring

        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }
        sci_toplevel $w
        sci_frame $w.f     
        pack $w.f -expand yes -fill both

        sci_scrolledtext $w.f.str -vscrollmode dynamic \
            -labeltext "String Contents"  
        $w.f.str insert end [set $this-inputstring]
        pack $w.f.str -fill both -expand yes

        makeSciButtonPanel $w $w $this
        moveToCursor $w
    }

    method update {} {
    
        global $this-inputstring
        set w .ui[modname]
        if {[winfo exists $w]} {
            $w.f.str clear
            $w.f.str insert end [set $this-inputstring]
         }
    }

}


