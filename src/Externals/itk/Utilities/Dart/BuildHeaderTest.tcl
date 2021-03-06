# =========================================================================
# 
#   Program:   Insight Segmentation & Registration Toolkit
#   Module:    $RCSfile: BuildHeaderTest.tcl,v $
#   Language:  Tcl
#   Date:      $Date: 2008-05-26 11:47:38 $
#   Version:   $Revision: 1.9 $
# 
#  Copyright (c) Insight Software Consortium. All rights reserved.
#  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.
#
#     This software is distributed WITHOUT ANY WARRANTY; without even
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#     PURPOSE.  See the above copyright notices for more information.
#
#
# =========================================================================

set Header {/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Language:  C++

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include <cstdlib>
// This file has been generated by BuildHeaderTest.tcl
// Test to include each header file for Insight
}

set Trailer "
int main ( int , char* [] )
{
  
  return EXIT_SUCCESS;
}
"

set D [lsort [glob -nocomplain Code/*]]
set Directories ""
foreach d $D \
{
  if { [file tail $d] != "CVS" && [file isdirectory $d]} \
  {
    lappend Directories [file tail $d]
  }
}

foreach Directory $Directories \
{
  if { ![file exists [file join Testing Code $Directory]] } \
  {
    continue
  }
  set f [open [file join Testing Code $Directory itk${Directory}HeaderTest.cxx] w]
  puts $f $Header
  foreach File [lsort [glob -nocomplain [file join Code $Directory *.h]]] \
  {
      if {[file tail $File] == "itkTestMain.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWindows.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWin32Header.h"} {
	  continue;
      }
      if {[file tail $File] == "itkWin32OutputWindow.h"} {
	  continue;
      }
      if {[file tail $File] == "itkImageViewerWindow.h"} {
	  continue;
      }
#
# include the .h if there is no .txx
# otherwise include the .txx
#
      set txxFile "[file rootname $File].txx"
      if {[file tail $txxFile] == "itkImageViewer.txx"} {
	  continue;
      }
      if { [file exists $txxFile] } {
          puts $f "#include \"[file tail $txxFile]\""
      } else {
        puts $f "#include \"[file tail $File]\""
      }
  }
  puts $f $Trailer
  close $f
}

