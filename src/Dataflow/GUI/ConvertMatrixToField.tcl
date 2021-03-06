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

itcl::class SCIRun_Converters_ConvertMatrixToField {
    inherit Module
     constructor { {args ""} } {
        eval configure $args
        set name ConvertMatrixToField
        set_defaults
    }

    method set_defaults {} {
        global $this-datalocation
        
        set $this-datalocation "Node"
    }

    method ui {} {
        set w .ui[modname]
        if {[winfo exists $w]} {
            return
        }
        sci_toplevel $w


        labelcombo $w.datalocation "Data Location" \
          {"Node" "Element"} \
          $this-datalocation


        makeSciButtonPanel $w $w $this
        moveToCursor $w
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


