#! /bin/sh

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

#
# repair.sh <bad_include>
#
# The purpose of this script is to find .d (Makefile dependency files
# that have bad (old) include files in them and to delete the .d and
# corresponding .o file.

if test $# != 1 || test $1 = "-h" || test $1 = "-help" || test $1 = "--help"; then
  echo ""
  echo "Usage: $0 <bad_include>"
  echo ""
  echo "  bad_include - the name of the bad file (or some unique text"
  echo "                that is grep'd for in the .d files.)"
  echo ""
  echo "This script will run through all the .d (Makefile Dependency)"
  echo "files in the current directory and below.  If any of those files"
  echo "have the 'bad_include' in them, it will delete the .d and"
  echo "corresponding .o file."
  echo ""
  echo "If, for example, you updated to the newest thirdparty (version 1.22)"
  echo "and are now getting:"
  echo ""
  echo "  gmake: No rule to make target ???, needed by ___.o"
  echo ""
  echo "You could run this script like this:"
  echo ""
  echo "cd .../SCIRun/<bin>"
  echo "../src/scripts/repair.sh Thirdparty/1.20"
  echo ""
  echo "And then run your 'gmake' again."
  echo ""
  echo "Example 2:"
  echo ""
  echo "If the error is something like:"
  echo ""
  echo "    No rule to make target `../src/Dataflow/Modules/Render/SCIBaWGL.h',"
  echo "       needed by `Dataflow/Modules/Render/OpenGL.o'.  Stop."
  echo ""
  echo "Then you would use:"
  echo ""
  echo "../src/scripts/repair.sh SCIBaWGL.h"
  echo ""
  exit
fi

bad_inc=$1

files=`find . -name "*.d" -o -name "depend.mk" | xargs grep -l $bad_inc`

file_found=no

for file in $files; do
   file_found=yes

   filename=`echo $file | sed "s%.*/%%"`

   if test "$filename" = "depend.mk"; then
     c_files=`grep $bad_inc $file | cut -f1 -d":"`
   else
     c_files=$file
   fi

   for cfile in $c_files; do
     base=`echo $cfile | sed "s%\.d%%" | sed "s%\.o%%"`
     echo "rm -rf $base.o"
           rm -rf $base.o
     if test "$filename" != "depend.mk"; then
        echo "rm -rf $base.d"
              rm -rf $base.d
     else
        # remove the individual bad line from the __depend.mk__ file.
        rm -rf $file.temp
        grep -v $bad_inc $file > $file.temp
        mv $file.temp $file
     fi 
   done
done

if test $file_found = "no"; then
   echo ""
   echo "No matching files found. (Perhaps your 'bad_include' was incorrect?)"
   echo "If you continue to have problems with this script, please contact"
   echo "J. Davison de St. Germain (dav@sci.utah.edu)."
   echo ""
fi



