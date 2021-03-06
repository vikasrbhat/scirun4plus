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


# example of Writer
# by Samsonov Alexei
# October 2000 
# NOTE: if file to be splitted, uncomment corresponding lines in the file

catch {rename SCIRun_DataIO_WriteColorMap2D ""}

itcl::class SCIRun_DataIO_WriteColorMap2D {
    inherit Module

     constructor { {args ""} } {
        eval configure $args
	set name WriteColorMap2D
    }

    method overwrite {} {
        global $this-confirm $this-confirm-once $this-filename $this-old-filename
				
				if {![info exists $this-old-filename]} {
					set $this-old-filename ""
				}
				
				if {[set $this-confirm-once] == 1} {
					if { [string equal [set $this-filename] [set $this-old-filename] ]} {
					  return 1
					}
				}
        if {[info exists $this-confirm] && [info exists $this-filename] && \
                [set $this-confirm] && [file exists [set $this-filename]] } {
          set value [createSciDialog \
              -title "Overwrite file ?" \
              -message "File [set $this-filename] already exists.\nWould you like to overwrite it?" \
              -button1 "No" -button3 "Yes" ]
          if {$value == 1} { return 0 }
        }
				
				set $this-old-filename [set $this-filename]
        return 1
    }

    
    method ui {} {
      set w .ui[modname]
      if {[winfo exists $w]} {
          # Refresh UI
          biopseFDialog_RefreshCmd $w
          return
      }
      
      sci_toplevel $w -class TkFDialog
      # place to put preferred data directory
      # it's used if $this-filename is empty
      set initdir [netedit getenv SCIRUN_DATA]

      #######################################################
      # to be modified for particular reader

      # extansion to append if no extension supplied by user
      set defext ".cmap2"
      
      # name to appear initially
      set defname "MyColorMap"
      set title "Save colormap file"

      # Unwrap $this-types into a list.
      set tmp1 [set $this-types]
      set tmp2 [eval "set tmp3 $tmp1"]
      
      ######################################################
      
      makeSaveFilebox \
        -parent $w \
        -filevar $this-filename \
        -setcmd "wm withdraw $w" \
        -command "$this-c needexecute;" \
        -cancel "wm withdraw $w" \
        -title $title \
        -filetypes $tmp2 \
        -initialfile $defname \
        -initialdir $initdir \
        -defaultextension $defext \
        -formatvar $this-filetype \
        -formats {None} \
        -confirmvar $this-confirm \
        -confirmoncevar $this-confirm-once \
        -selectedfiletype $this-exporttype

      moveToCursor $w
    }
}
