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

itcl::class SCIRun_ChangeFieldData_SelectAndSetFieldData {
   inherit Module

     constructor { {args ""} } {
        eval configure $args
        set name SelectAndSetFieldData
        set_defaults
    }

    method set_defaults {} {
      global $this-function
      global $this-selection
      global $this-format
      global $this-help

      set $this-selection1 "DATA < 1.0"
      set $this-function1 "abs(DATA)"
      set $this-selection2 "DATA > 1.0"
      set $this-function2 "-abs(DATA)"
      set $this-selection3 ""
      set $this-function3 ""
      set $this-selection4 ""
      set $this-function4 ""
      set $this-functiondef "0"

      set $this-format "Scalar"
      set $this-help ""
      
    }

    method update_text {} {
      set w .ui[modname]
      if {[winfo exists $w]} {
        set ss [$w.ss childsite]
        set $this-function1 [$ss.function1 get 1.0 end]
        set $this-function2 [$ss.function2 get 1.0 end]
        set $this-function3 [$ss.function3 get 1.0 end]
        set $this-function4 [$ss.function4 get 1.0 end]
        set $this-functiondef [$ss.function5 get 1.0 end]
        set $this-selection1 [$ss.selection1 get 1.0 end]
        set $this-selection2 [$ss.selection2 get 1.0 end]
        set $this-selection3 [$ss.selection3 get 1.0 end]
        set $this-selection4 [$ss.selection4 get 1.0 end]
        }
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }

        sci_toplevel $w

        sci_labeledframe $w.inf -labeltext "Create New Field Data"
        set infoframe [$w.inf childsite]
        sci_frame $infoframe.info
        pack $infoframe.info -side left
        set info $infoframe.info
        sci_label $info.info1 -text "Selection: function(DATA,A,B,C,...)"
        sci_label $info.info1a -text "Function: function(DATA,A,B,C,...)"
        sci_label $info.info2 -text "Input array: DATA (scalar/vector/tensor: data from field port) "
        sci_label $info.info3 -text "Input array: X, Y, Z (scalar: Cartensian coordinates of node/element)"
        sci_label $info.info4 -text "Input array: POS (vector: vector with node/element position)"
        sci_label $info.info5 -text "Input array: A, B, C, ... (scalar/vector/tensor: data from matrix ports)"
        sci_label $info.info6 -text "Input array: INDEX (scalar: number of the element)"
        sci_label $info.info7 -text "Input array: SIZE (scalar: number of elements)"
        sci_label $info.info8 -text "Input array: ELEMENT (element: object containing element properties)"
        sci_label $info.info9 -text "Output array: RESULT (scalar/vector/tensor)"

        grid $info.info1 -row 0 -column 0 -columnspan 2 -sticky w
        grid $info.info1a -row 1 -column 0 -columnspan 2 -sticky w
        grid $info.info2 -row 2 -column 0 -sticky w
        grid $info.info3 -row 3 -column 0 -sticky w
        grid $info.info4 -row 4 -column 0 -sticky w
        grid $info.info5 -row 5 -column 0 -sticky w
        grid $info.info6 -row 2 -column 1 -sticky w
        grid $info.info7 -row 3 -column 1 -sticky w
        grid $info.info8 -row 4 -column 1 -sticky w
        grid $info.info9 -row 5 -column 1 -sticky w

        pack $w.inf -side top -anchor w -fill x

        sci_labeledframe $w.of -labeltext "output type"
        set otype [$w.of childsite]
        pack $w.of -side top -anchor w -fill x
        
        labelcombo $otype.otype "Field Output Data Type" \
          {Scalar Vector Tensor "Same as Input" "char" "unsigned char" \
					"short" "unsigned short" "int" "unsigned int" "float" "double"} \
          $this-format

        sci_labeledframe $w.ss -labeltext "SELECT => SET OUTPUT"
        set ss [$w.ss childsite]
        option add *textBackground white	
        
        sci_label $ss.sellabel -text "SELECTION = "
        grid $ss.sellabel -row 0 -column 0 -sticky news -padx 2 -pady 2

        sci_label $ss.funlabel -text "FUNCTION = "
        grid $ss.funlabel -row 0 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2

        sci_label $ss.deflabel -text "DEFAULT OUTPUT FUNCTION ="
        grid $ss.deflabel -row 5 -column 0 -sticky news -padx 2 -pady 2
        
        iwidgets::scrolledtext $ss.selection1 -height 30 -width 300 -hscrollmode dynamic -vscrollmode none
        $ss.selection1 insert end [set $this-selection1]
        grid $ss.selection1 -row 1 -column 0 -sticky news -padx 2 -pady 2

        sci_scrolledtext $ss.selection2 -height 30 -width 300 -hscrollmode dynamic -vscrollmode none
        $ss.selection2 insert end [set $this-selection2]
        grid $ss.selection2 -row 2 -column 0 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.selection3 -height 30 -width 300 -hscrollmode dynamic -vscrollmode none
        $ss.selection3 insert end [set $this-selection3]
        grid $ss.selection3 -row 3 -column 0 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.selection4 -height 30 -width 300 -hscrollmode dynamic -vscrollmode none
        $ss.selection4 insert end [set $this-selection4]
        grid $ss.selection4 -row 4 -column 0 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.function1 -height 30 -width 400 -hscrollmode dynamic -vscrollmode none
        $ss.function1 insert end [set $this-function1]
        grid $ss.function1 -row 1 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.function2 -height 30 -width 400 -hscrollmode dynamic -vscrollmode none
        $ss.function2 insert end [set $this-function2]
        grid $ss.function2 -row 2 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.function3 -height 30 -width 400 -hscrollmode dynamic -vscrollmode none
        $ss.function3 insert end [set $this-function3]
        grid $ss.function3 -row 3 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.function4 -height 30 -width 400 -hscrollmode dynamic -vscrollmode none
        $ss.function4 insert end [set $this-function4]
        grid $ss.function4 -row 4 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2 

        sci_scrolledtext $ss.function5 -height 30 -width 400 -hscrollmode dynamic -vscrollmode none
        $ss.function5 insert end [set $this-functiondef]
        grid $ss.function5 -row 5 -column 1 -columnspan 2 -sticky news -padx 2 -pady 2 

        pack $w.ss -side top -anchor w -fill both 

        sci_button $w.help -text "Parser Help" -command "$this showhelp"
        pack $w.help -side top -anchor e    

        makeSciButtonPanel $w $w $this
    }
    
    method showhelp { } {

      # Create a unique name for the file selection window
      set w [format "%s-functionhelp" .ui[modname]]

      if { [winfo exists $w] } {
        if { [winfo ismapped $w] == 1} {
          raise $w
        } else {
          wm deiconify $w
        }
	    	return
      }
	
      sci_toplevel $w -class TkFDialog
            
      sci_labeledframe $w.hf -labeltext "Parser Help"
      set help [$w.hf childsite]
      option add *textBackground white	
      sci_scrolledhtml $help.help -height 60 -hscrollmode dynamic -width 500p -height 300p        
      
      set helpfile [file join [netedit getenv SCIRUN_SRCDIR] Dataflow GUI ArrayMathFunctionHelp.html]
      $help.help import $helpfile
      
      pack $help.help -side top -anchor w -fill both -expand yes
      pack $w.hf -side top -anchor w -fill both -expand yes
    } 
    
    method labelcombo { win text1 arglist var} {
      sci_frame $win 
      pack $win -side top -padx 5
      sci_label $win.l1 -text $text1 \
              -anchor w -just left
      sci_label $win.colon  -text ":" -width 2 -anchor w -just left
      sci_optionmenu $win.c -foreground darkred \
        -command " $this comboget $win.c $var "

      set i 0
      set found 0
      set length [llength $arglist]
      for {set elem [lindex $arglist $i]} {$i<$length} \
          {incr i 1; set elem [lindex $arglist $i]} {
          if {"$elem"=="[set $var]"} {
        set found 1
          }
          $win.c insert end $elem
      }

      if {!$found} {
          $win.c insert end [set $var]
      }

      $win.c select [set $var]

      sci_label $win.l2 -text "" -width 20 -anchor w -just left

      # hack to associate optionmenus with a textvariable
      # bind $win.c <Map> "$win.c select {[set $var]}"

      pack $win.l1 $win.colon -side left
      pack $win.c $win.l2 -side left	
    }

    method comboget { win var } {
      if {![winfo exists $win]} {
          return
      }
      if { "$var"!="[$win get]" } {
          set $var [$win get]
      }
    }

    method set_combobox { win var name1 name2 op } {
      set w .ui[modname]
      set menu $w.$win
      if {[winfo exists $menu]} {
          $menu select $var
      }
    }

}


