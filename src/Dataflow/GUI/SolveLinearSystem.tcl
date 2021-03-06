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


catch {rename SolveLinearSystem ""}

itcl::class SCIRun_Math_SolveLinearSystem {
    inherit Module

     constructor { {args ""} } {
        eval configure $args
        set name SolveLinearSystem

        global this-graphs
        set $this-graphs {}
    }
    
    method switchmethod {} {
      global $this-method
      set w .ui[modname]
      set meth [set $this-method]
      
      if {($meth == "Conjugate Gradient & Precond. (SCI)") || \
          ($meth == "BiConjugate Gradient & Precond. (SCI)") || \
          ($meth == "Jacobi & Precond. (SCI)") || \
          ($meth == "MINRES & Precond. (SCI)") || \
          ($meth == "GMRES & Precond. (SCI)")} {
          pack $w.converg $w.graph -side top -padx 2 -pady 2 -fill x
        }
      }
    
    
    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }
        
        sci_toplevel $w
        wm minsize $w 300 20
        set n "$this-c needexecute "
        
        sci_tabnotebook  $w.tabs -raiseselect true -tabpos n\
                               -width 400 -height 150
        pack $w.tabs -side top -fill x
        set methods [$w.tabs add -label "Methods"]
        set precons [$w.tabs add -label "Preconditioners"]

        $w.tabs view 0

        sci_scrolledframe $methods.f -hscrollmode none \
                                -vscrollmode dynamic
        sci_scrolledframe $precons.f -hscrollmode none \
                                -vscrollmode dynamic

        pack $methods.f $precons.f -f both -e y

        set meth [$methods.f childsite]
        set prec [$precons.f childsite]

        make_labeled_radio $meth.f "" "$this switchmethod" \
          top 1 $this-method\
          {{"Conjugate Gradient & Precond. (SCI)" "Conjugate Gradient & Precond. (SCI)"}\
          {"BiConjugate Gradient & Precond. (SCI)" "BiConjugate Gradient & Precond. (SCI)"}\
          {"Jacobi & Precond. (SCI)" "Jacobi & Precond. (SCI)"}\
          {"MINRES & Precond. (SCI)" "MINRES & Precond. (SCI)"}}

        make_labeled_radio $prec.f "" ""\
                      top 1 $this-precond\
                      {{jacobi jacobi}
                       {none none}}
                         
        pack $meth.f -side top -fill x -pady 2
        pack $prec.f -side top -fill x -pady 2
        
        global $this-target_error
        expscale $w.target_error -orient horizontal -label "Target error:" \
          -variable $this-target_error -command ""

        pack $w.target_error -side top -fill x -pady 2

        sci_scale $w.maxiter -orient horizontal -label "Maximum Iterations:" \
          -variable $this-maxiter -from 0 -to 20000
        pack $w.maxiter -side top -fill x -pady 2

        sci_checkbutton $w.emit -variable $this-emit_partial \
          -text "Emit partial solutions"
        pack $w.emit -side top -fill x -pady 2

        sci_scale $w.emititer -orient horizontal \
          -label "Partial Solution Emitted Every:" \
          -variable $this-emit_iter -from 1 -to 2000
        pack $w.emititer -side top -fill x -pady 2
        
        sci_checkbutton $w.use_prev -variable $this-use_previous_soln \
          -text "Use previous solution as initial guess"
        pack $w.use_prev -side top -fill x -pady 2
	
        sci_frame $w.converg -borderwidth 2 -relief ridge
        pack $w.converg -side top -padx 2 -pady 2 -fill x
	
	      sci_frame $w.converg.iter
        pack $w.converg.iter -side top -fill x
        sci_label $w.converg.iter.lab -text "Iteration: "
        pack $w.converg.iter.lab -side left
        sci_label $w.converg.iter.val -textvariable $this-iteration
        pack $w.converg.iter.val -side right
        
        sci_frame $w.converg.first
        pack $w.converg.first -side top -fill x
        sci_label $w.converg.first.lab -text "Original Error: "
        pack $w.converg.first.lab -side left
        sci_label $w.converg.first.val -textvariable $this-orig_error
        pack $w.converg.first.val -side right
        
        sci_frame $w.converg.current
        pack $w.converg.current -side top -fill x
        sci_label $w.converg.current.lab -text "Current Error: "
        pack $w.converg.current.lab -side left
        sci_label $w.converg.current.val -textvariable $this-current_error
        pack $w.converg.current.val -side right
        
        global $this-target_error
        set err [set $this-target_error]

        global Color
        blt::graph $w.graph -title "Convergence" -height 250 \
          -plotbackground $Color(EditBackGround) -background $Color(UIBackGround)
      
        if {$err == 0} {
          set err 1e-20
        }
        $w.graph yaxis configure -title "Error (RMS)"  -min [expr log($err/10)/log(10)] -max 1 -loose true
        $w.graph xaxis configure -title "Iteration" \
          -loose true
        bind $w.graph <ButtonPress-1> "$this select_error $w.graph %x %y"
        bind $w.graph <Button1-Motion> "$this move_error $w.graph %x %y"
        bind $w.graph <ButtonRelease-1> "$this deselect_error $w.graph %x %y"
        set iter 1
        $w.graph element create "Current Target" -linewidth 0
        $w.graph element configure "Current Target" -data "0 $err" \
          -symbol diamond
        pack $w.graph -fill x
        add_graph $w.graph

        reset_graph
        switchmethod

        makeSciButtonPanel $w $w $this
        moveToCursor $w
    }
    
    protected variable error_selected false
    protected variable tmp_error
    
    method select_error {graph wx wy} {
      global $this-target_error $this-iteration
      set err [set $this-target_error]
      set iter [set $this-iteration]
      if {$err == 0} {
        set err 1e-20
      }      
      set errpos [$graph transform $iter [expr log($err)/log(10)]]
      set erry [lindex $errpos 1]
      set errx [lindex $errpos 0]
      if {abs($wy-$erry) < 5} {
        foreach g [set $this-graphs] {
          $g element configure "Current Target" -color yellow
        }
        set error_selected true
      }
    }
    
    method move_error {graph wx wy} {
      global $this-target_error
      if {$error_selected == "true"} {
        set newerror [lindex [$graph invtransform $wx $wy] 1]
        foreach g [set $this-graphs] {
          $g element configure "Current Target" -ydata $newerror
        }
      }
    }
    
    method deselect_error {graph wx wy} {
      if {$error_selected == "true"} {
        foreach g [set $this-graphs] {
          $g element configure "Current Target" -color blue
        }
        set error_selected false
        set newerror [lindex [$graph invtransform $wx $wy] 1]
        global $this-target_error
        set $this-target_error [expr pow(10,$newerror)]
      }
    }
    
    protected variable min_error

    method add_graph {graph} {
        global $this-graphs
        set $this-graphs [concat [set $this-graphs] $graph]
    }

    method reset_graph_once {graph} {
      if {![winfo exists $graph]} {
        return
      }
    
      catch "$graph element delete {Target Error}"
      catch "$graph element delete {Current Error}"

      $graph element create "Target Error" -linewidth 2 -color blue -symbol ""
      $graph element create "Current Error" -linewidth 2 -color red -symbol ""
      global $this-target_error
      set err [set $this-target_error]
      if {$err == 0} {
        set err 1e-20
      }
      set lerr [expr log($err)/log(10)]
      set iter 1
      $graph element configure "Target Error" -data "0 $lerr $iter $lerr"
      set min_error $err
      
      $graph yaxis configure -min [expr log($err/10)/log(10)] -max 1
      $graph element configure "Current Target" -data "0 $lerr"
    }
    
    method reset_graph {} {
      foreach graph [set $this-graphs] {
        reset_graph_once $graph
      }
    }

    method append_graph_once {graph iter values errvalues} {
      if {![winfo exists $graph]} {
        return
      }
      if {$values != ""} {
        set xvals [$graph element cget "Current Error" -xdata]
        set yvals [$graph element cget "Current Error" -ydata]
        
        for {set i 0} {$i<[llength $values]} {incr i} {
          lappend xvals [lindex $values $i]
          incr i
          lappend yvals [lindex $values $i]
        }
        $graph element configure "Current Error" -xdata $xvals
        $graph element configure "Current Error" -ydata $yvals
      }
      if {$errvalues != ""} {
        set xvals [$graph element cget "Target Error" -xdata]
        set yvals [$graph element cget "Target Error" -ydata]
        
        for {set i 0} {$i<[llength $errvalues]} {incr i} {
          lappend xvals [lindex $errvalues $i]
          incr i
          lappend yvals [lindex $errvalues $i]
        }
        $graph element configure "Target Error" -xdata $xvals
        $graph element configure "Target Error" -ydata $yvals
      }
      global $this-target_error
      set err [set $this-target_error]
      if {$err < $min_error} {
          set min_error $err
      }

      $graph yaxis configure -min [expr log($min_error/10)/log(10)]
      $graph element configure "Current Target" -xdata [expr $iter - 1]
    }

    method append_graph {iter values errvalues} {
      foreach graph [set $this-graphs] {
        append_graph_once $graph $iter $values $errvalues
      }
    }

    method finish_graph_once {graph} {
	if {![winfo exists $graph]} {
	    return
	}
	$graph element configure "Current Error" -color green
    }        

    method finish_graph {} {
        foreach graph [set $this-graphs] {
            finish_graph_once $graph
        }
    }
}
