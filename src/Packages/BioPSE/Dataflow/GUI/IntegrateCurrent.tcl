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


##
 #  IntegrateCurrent.tcl: Set theta and phi for the dipole
 #
 #  Written by:
 #   David Weinstein
 #   Department of Computer Science
 #   University of Utah
 #   June 1999
 #
 #  Log Information:
 #
 ##

catch {rename BioPSE_Forward_IntegrateCurrent ""}

itcl::class BioPSE_Forward_IntegrateCurrent {
    inherit Module
     constructor { {args ""} } {
        eval configure $args
        set name IntegrateCurrent
        set_defaults
    }
    method set_defaults {} {
        global $this-current
        set $this-current 0
    }
    method make_entry {w text v c} {
        sci_frame $w
        sci_label $w.l -text "$text"
        pack $w.l -side left
        global $v
        sci_entry $w.e -textvariable $v
        bind $w.e <Return> $c
        pack $w.e -side right
    }
    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            raise $w
            return;
        }

        sci_toplevel $w
        wm minsize $w 150 30
        sci_frame $w.f
        global $this-current
        sci_frame $w.f.c
        sci_label $w.f.c.l -text "Computed current: "
        sci_label $w.f.c.c -textvariable $this-current
        sci_label $w.f.c.a -text "amps"
        pack $w.f.c.l $w.f.c.c $w.f.c.a -side left -fill x -expand yes
        pack $w.f.c -side top -fill x -expand yes
        pack $w.f -side top -fill x -expand yes
    }
}
