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

itcl::class SCIRun_ChangeMesh_TransformPlanarMesh {
    inherit Module
    
     constructor { {args ""} } {
        eval configure $args
	set name TransformPlanarMesh
    }
    
    method ui {} {
      global $this-axis

      set w .ui[modname]
      if {[winfo exists $w]} {
          return
      }
      
      sci_toplevel $w

      sci_labeledframe $w.axis -labelpos nw -labeltext "Axis of Alignment"
      set axis [$w.axis childsite]

      sci_radiobutton $axis.x -text "X" -width 6 \
          -anchor w -just left -variable $this-axis -value 0
      sci_radiobutton $axis.y -text "Y" -width 6 \
          -anchor w -just left -variable $this-axis -value 1
      sci_radiobutton $axis.z -text "Z" -width 6 \
          -anchor w -just left -variable $this-axis -value 2

      sci_checkbutton $axis.invert -text "Invert" -width 6 \
          -anchor w -just left -variable $this-invert
      
      pack $axis.x $axis.y $axis.z $axis.invert -side left
      pack $w.axis -side top

      sci_labeledframe $w.trans -labelpos nw -labeltext "Transform"
      set trans [$w.trans childsite]

      sci_spinner $trans.x -labeltext "Left-Right" \
        -width 10 -fixed 10 \
        -validate  "$this trans_in %P $this-trans_x" \
        -decrement "$this trans_angle -1 $trans.x $this-trans_x" \
        -increment "$this trans_angle  1 $trans.x $this-trans_x" 

      $trans.x insert 0 [set $this-trans_x]

      sci_spinner $trans.y -labeltext "Up-Down: " \
        -width 10 -fixed 10 \
        -validate  "$this trans_in %P $this-trans_y" \
        -decrement "$this trans_angle -1 $trans.y $this-trans_y" \
        -increment "$this trans_angle  1 $trans.y $this-trans_y" 

      $trans.y insert 0 [set $this-trans_y]
      
      pack $trans.x $trans.y -side left
      pack $w.trans -side top

      makeSciButtonPanel $w $w $this
      moveToCursor $w
    }

    method trans_in {new trans} {
	if {! [regexp "\\A\\d*\\.*\\d+\\Z" $new]} {
	    return 0
	} elseif {$new < -100.0 || 100.0 < $new} {
	    return 0
	} 

	set $trans $new
	$this-c needexecute
	return 1
    }

    method trans_angle {step spin trans} {
	set newtrans [expr [set $trans] + 1 * $step]

	if {$newtrans < -100.0} {
	    set newtrans -100
	} elseif {$newtrans > 100.0} {
	    set newtrans 100
	}   

	set $trans $newtrans
	$spin delete 0 end
	$spin insert 0 [set $trans]
	$this-c needexecute
    }
}
