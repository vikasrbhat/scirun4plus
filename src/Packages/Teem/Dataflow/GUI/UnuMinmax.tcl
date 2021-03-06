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

#    File   : UnuMinmax.tcl
#    Author : Darby Van Uitert
#    Date   : April 2004

itcl::class Teem_UnuAtoM_UnuMinmax {
    inherit Module
     constructor { {args ""} } {
        eval configure $args
        set name UnuMinmax
        set_defaults
    }

    method set_defaults {} {
        global $this-nrrds
        global $this-old_nrrds

        set $this-nrrds 0
        set $this-old_nrrds 0
    }

    method init_axes { } {
        for {set i 0} {$i < [set $this-nrrds]} {incr i} {

            if { [catch { set t [set $this-mins$i] } ] } {
              set $this-mins$i 0
            }
            if { [catch { set t [set $this-maxs$i]}] } {
              set $this-maxs$i 0
            }
        }
    }
    
    method make_min_max {} {
        set w .ui[modname]

        if {[winfo exists $w]} {
            
            if {[winfo exists $w.f.mmf.t]} {
          destroy $w.f.mmf.t
            }
            
            # clear old ones using old_dim
            for {set i 0} {$i < [set $this-old_nrrds]} {incr i} {
          if {[winfo exists $w.f.mmf.a$i]} {
              destroy $w.f.mmf.a$i
          }
	    }
	    
	    set $this-old_nrrds [set $this-nrrds]
	    
	    # create new ones
	    for {set i 0} {$i < [set $this-nrrds]} {incr i} {
		if {![winfo exists $w.f.mmf.a$i]} {
		    create_min_max_info $w.f.mmf.a$i $i [set $this-min$i] [set $this-max$i]
		}
	    }
	}
    }
    
    method create_min_max_info {w which min max} {
        sci_labeledframe $w -labeltext "Nrrd $which" \
            -labelpos nw
        pack $w -side top -anchor nw -pady 3

        set n [$w childsite]
        
        sci_frame $n.f 
        pack $n.f -side top -anchor nw
        
        sci_label $n.f.minl -text "Min: $min"
        sci_label $n.f.maxl -text "Max: $max"
        pack $n.f.minl $n.f.maxl -side left  -padx 8
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }
        sci_toplevel $w

        wm minsize $w 150 80
        sci_frame $w.f
        pack $w.f -padx 2 -pady 2 -side top -expand yes
        
        sci_frame $w.f.mmf
        pack $w.f.mmf -padx 2 -pady 2 -side top -expand yes
        
        if {[set $this-nrrds] == 0} {
            sci_label $w.f.mmf.t -text "Need to Execute to know the number of Nrrds."
            pack $w.f.mmf.t
        } else {
            init_axes 
            make_min_max
        }
        
        
        pack $w.f -expand 1 -fill x
        makeSciButtonPanel $w $w $this
        moveToCursor $w

    }
}


