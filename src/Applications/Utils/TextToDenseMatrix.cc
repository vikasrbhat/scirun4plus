/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2009 Scientific Computing and Imaging Institute,
   University of Utah.

   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/


/*
 *  TextToDenseMatrix.cc
 *
 *  Written by:
 *   David Weinstein
 *   Department of Computer Science
 *   University of Utah
 *   April 2003
 *
 */

// This program will read in a .txt file specifying a matrix.  The .txt
// file will have white-space separated matrix entries and a one line 
// header indicating the number of rows and number of columns of the 
// matrix (unless the user has specified the -noHeader flag, in which 
// case the next two command line arguments must be the number of rows 
// and the number of columns in the matrix).
// The SCIRun .mat file will be saved as ASCII by default, unless the
// user specified the -binOutput flag.

#include <Core/Datatypes/DenseMatrix.h>
#include <Core/Persistent/Pstreams.h>
#include <Core/Datatypes/MatrixTypeConverter.h>

#include <Core/Init/init.h>
#include <Core/Util/FileUtils.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using std::cerr;
using std::ifstream;
using std::endl;

using namespace SCIRun;

bool header;
bool binOutput;
bool debugOn;
int nr;
int nc;

void setDefaults() {
  header=true;
  binOutput=false;
  debugOn=false;
  nr=0;
  nc=0;
}

int parseArgs(int argc, char *argv[]) {
  int currArg = 3;
  while (currArg < argc) {
    if (!strcmp(argv[currArg],"-noHeader")) {
      header=false;
      currArg++;
      nr=atoi(argv[currArg]);
      currArg++;
      nc=atoi(argv[currArg]);
      currArg++;
    } else if (!strcmp(argv[currArg],"-binOutput")) {
      binOutput=true;
      currArg++;
    } else if (!strcmp(argv[currArg],"-debugOn")) {
      debugOn=true;
      currArg++;
    } else {
      cerr << "Error - unrecognized argument: "<<argv[currArg]<<"\n";
      return 0;
    }
  }
  return 1;
}

void printUsageInfo(char *progName) {
  cerr << "\n Usage: "<<progName<<" textfile DenseMatix [-noHeader nrows ncols] [-binOutput] [-debugOn]\n\n";
  cerr << "\t This program will read in a .txt file specifying a matrix. \n";
  cerr << "\t The .txt file will have white-space separated matrix entries \n";
  cerr << "\t and  a one line header indicating the number of rows and \n";
  cerr << "\t number of columns of the matrix (unless the user has \n";
  cerr << "\t specified the -noHeader flag, in which case the next two \n";
  cerr << "\t command line arguments must be the number of rows and the \n";
  cerr << "\t number of columns in the matrix).  The SCIRun .mat file will \n";
  cerr << "\t be saved as ASCII by default, unless the user specified the \n";
  cerr << "\t -binOutput flag.\n\n";
}

int
main(int argc, char **argv) {
  if (argc < 3 || argc > 8) {
    printUsageInfo(argv[0]);
    return 2;
  }
  SCIRunInit();
  setDefaults();

  char *textfileName = argv[1];
  char *matrixName = argv[2];
  if (!parseArgs(argc, argv)) {
    printUsageInfo(argv[0]);
    return 2;
  }

  ifstream matstream(textfileName);
  if (matstream.fail()) {
    cerr << "Error -- Could not open file " << textfileName << "\n";
    return 2;
  }
  if (header) matstream >> nr >> nc;

  cerr << "nrows="<<nr<<" ncols="<<nc<<"\n";
  DenseMatrix *dm = new DenseMatrix(nr,nc);

  int r, c;
  for (r=0; r<nr; r++)
    for (c=0; c<nc; c++) {
      double d;
      matstream >> d;
      (*dm)[r][c]=d;
      if (debugOn) 
	cerr << "matrix["<<r<<"]["<<c<<"]="<<d<<"\n";
    }
  cerr << "done building matrix.\n";

  MatrixHandle mH(dm);
  mH->set_raw(false);

  if (binOutput) {
    BinaryPiostream out_stream(matrixName, Piostream::Write);
    Pio(out_stream, mH);
  } else {
    TextPiostream out_stream(matrixName, Piostream::Write);
    Pio(out_stream, mH);
  }
  return 0;  
}    
