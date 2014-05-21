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
 *  SparseMatrixFunctions.h:  Sparse Row Matrix Functions
 *
 *  Written by:
 *   Steven G. Parker
 *   Department of Computer Science
 *   University of Utah
 *   November 1994
 *
 * See http://math.nist.gov/MatrixMarket/formats.html#MMformat
 * for more information about this sparse matrix format.
 */


#ifndef CORE_DATATYPES_SPARSEROWMATRIXFUNCTIONS_H
#define CORE_DATATYPES_SPARSEROWMATRIXFUNCTIONS_H 

#include <Core/Datatypes/MatrixFwd.h>

#include <Core/Datatypes/share.h>

namespace SCIRun {

namespace SparseMatrixFunctions
{
  void sparse_mult(const SparseRowMatrix& matrix, const DenseMatrix& x, DenseMatrix& b);
  void sparse_mult_transXB(const SparseRowMatrix& matrix, const DenseMatrix& x, DenseMatrix& b);

  MatrixHandle sparse_sparse_mult(const SparseRowMatrix& matrix, const SparseRowMatrix &x);

  SparseRowMatrix *AddSparse(const SparseRowMatrix &a, const SparseRowMatrix &b);
  SparseRowMatrix *SubSparse(const SparseRowMatrix &a, const SparseRowMatrix &b);
}

} // End namespace SCIRun

#endif