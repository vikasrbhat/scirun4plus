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

#    File   : UnuCCfind.tcl
#    Author : Darby Van Uitert
#    Date   : April 2004

itcl::class Teem_UnuAtoM_UnuCCfind {
    inherit Module
     constructor { {args ""} } {
        eval configure $args
        set name UnuCCfind
        set_defaults
    }

    method set_defaults {} {
        global $this-connectivity
        set $this-connectivity 1

        global $this-type
        set $this-type nrrdTypeInt

        global $this-usetype
        set $this-usetype 1

        trace variable $this-type w "$this set_type"
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }
        sci_toplevel $w

        sci_frame $w.f
        pack $w.f -padx 2 -pady 2 -side top -expand yes
        
        sci_frame $w.f.options
        pack $w.f.options -side top -expand yes

        sci_optionmenu $w.f.options.type -labeltext "Type:" \
            -labelpos w -command "$this update_type $w.f.options.type"
        $w.f.options.type insert end nrrdTypeUChar \
            nrrdTypeUShort nrrdTypeInt 
        pack $w.f.options.type -side top -anchor nw -padx 3 -pady 3
        $w.f.options.type select [set $this-type]

        sci_checkbutton $w.f.options.usetype -text "Use smallest for output type" \
            -variable $this-usetype
        pack $w.f.options.usetype -side top -anchor nw -padx 3 -pady 3 

        sci_entryfield $w.f.options.connectivity -labeltext "Connectivity:" -textvariable $this-connectivity
        pack $w.f.options.connectivity -side top -expand yes -fill x


        pack $w.f -expand 1 -fill x
        makeSciButtonPanel $w $w $this
        moveToCursor $w

    }

    method update_type {menu} {
        global $this-type
        set which [$menu get]
        set $this-type $which
          }

          method set_type { name1 name2 op } {
        set w .ui[modname]
        set menu $w.f.options.type
        if {[winfo exists $menu]} {
            $menu select [set $this-type]
        }
    }
}



