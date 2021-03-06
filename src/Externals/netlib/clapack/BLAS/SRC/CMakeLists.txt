#
#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2010 Scientific Computing and Imaging Institute,
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

#######################################################################
#  This is the makefile to create a library for the BLAS.
#  The files are grouped as follows:
#
#       SBLAS1 -- Single precision real BLAS routines
#       CBLAS1 -- Single precision complex BLAS routines
#       DBLAS1 -- Double precision real BLAS routines
#       ZBLAS1 -- Double precision complex BLAS routines
#
#       CB1AUX -- Real BLAS routines called by complex routines
#       ZB1AUX -- D.P. real BLAS routines called by d.p. complex
#                 routines
#
#      ALLBLAS -- Auxiliary routines for Level 2 and 3 BLAS
#
#       SBLAS2 -- Single precision real BLAS2 routines
#       CBLAS2 -- Single precision complex BLAS2 routines
#       DBLAS2 -- Double precision real BLAS2 routines
#       ZBLAS2 -- Double precision complex BLAS2 routines
#
#       SBLAS3 -- Single precision real BLAS3 routines
#       CBLAS3 -- Single precision complex BLAS3 routines
#       DBLAS3 -- Double precision real BLAS3 routines
#       ZBLAS3 -- Double precision complex BLAS3 routines
#
#  The library can be set up to include routines for any combination
#  of the four precisions.  To create or add to the library, enter make
#  followed by one or more of the precisions desired.  Some examples:
#       make single
#       make single complex
#       make single double complex complex16
#  Note that these commands are not safe for parallel builds.
#
#  Alternatively, the commands
#       make all
#  or
#       make
#  without any arguments creates a library of all four precisions.
#  The name of the library is held in BLASLIB, which is set in the
#  top-level make.inc
#
#  To remove the object files after the library is created, enter
#       make clean
#  To force the source files to be recompiled, enter, for example,
#       make single FRC=FRC
#
#---------------------------------------------------------------------
#
#  Edward Anderson, University of Tennessee
#  March 26, 1990
#  Susan Ostrouchov, Last updated September 30, 1994
#  ejr, May 2006.
#
#######################################################################

project(blas)
message(STATUS "Configure BLAS library...")

#---------------------------------------------------------------------
#  Level 1 BLAS
#---------------------------------------------------------------------
set(blas_srcs_SBLAS1
  isamax.c
  sasum.c
  saxpy.c
  scopy.c
  sdot.c
  snrm2.c
  srot.c
  #srotg.c
  sscal.c
  sswap.c
  sdsdot.c
  srotmg.c
  srotm.c)

set(blas_srcs_CBLAS1
  scabs1.c
  scasum.c
  scnrm2.c
  icamax.c
  caxpy.c
  ccopy.c
  cdotc.c
  cdotu.c
  csscal.c
  #crotg.c
  cscal.c
  cswap.c
  csrot.c)

set(blas_srcs_DBLAS1
  idamax.c
  dasum.c
  daxpy.c
  dcopy.c
  ddot.c
  dnrm2.c
  drot.c
  #drotg.c
  dscal.c
  dsdot.c
  dswap.c
  drotmg.c
  drotm.c)

set(blas_srcs_ZBLAS1
  dcabs1.c
  dzasum.c
  dznrm2.c
  izamax.c
  zaxpy.c
  zcopy.c
  zdotc.c
  zdotu.c
  zdscal.c
  #zrotg.c
  zscal.c
  zswap.c
  zdrot.c)

set(blas_srcs_CB1AUX
  isamax.c
  sasum.c
  saxpy.c
  scopy.c
  snrm2.c
  sscal.c)

set(blas_srcs_ZB1AUX
  idamax.c
  dasum.c
  daxpy.c
  dcopy.c
  dnrm2.c
  dscal.c)

#---------------------------------------------------------------------
#  The following line defines auxiliary routines needed by both the
#  Level 2 and Level 3 BLAS.  Comment it out only if you already have
#  both the Level 2 and 3 BLAS.
#---------------------------------------------------------------------
set(blas_srcs_ALLBLAS
  lsame.c
  xerbla.c
  xerbla_array.c)

#---------------------------------------------------------
#  Level 2 BLAS.
#---------------------------------------------------------
set(blas_srcs_SBLAS2
  sgemv.c
  sgbmv.c
  ssymv.c
  ssbmv.c
  sspmv.c
  strmv.c
  stbmv.c
  stpmv.c
  strsv.c
  stbsv.c
  stpsv.c
  sger.c
  ssyr.c
  sspr.c
  ssyr2.c
  sspr2.c)

set(blas_srcs_CBLAS2
  cgemv.c
  cgbmv.c
  chemv.c
  chbmv.c
  chpmv.c
  ctrmv.c
  ctbmv.c
  ctpmv.c
  ctrsv.c
  ctbsv.c
  ctpsv.c
  cgerc.c
  cgeru.c
  cher.c
  chpr.c
  cher2.c
  chpr2.c)

set(blas_srcs_DBLAS2
  dgemv.c
  dgbmv.c
  dsymv.c
  dsbmv.c
  dspmv.c
  dtrmv.c
  dtbmv.c
  dtpmv.c
  dtrsv.c
  dtbsv.c
  dtpsv.c
  dger.c
  dsyr.c
  dspr.c
  dsyr2.c
  dspr2.c)

set(blas_srcs_ZBLAS2
  zgemv.c
  zgbmv.c
  zhemv.c
  zhbmv.c
  zhpmv.c
  ztrmv.c
  ztbmv.c
  ztpmv.c
  ztrsv.c
  ztbsv.c
  ztpsv.c
  zgerc.c
  zgeru.c
  zher.c
  zhpr.c
  zher2.c
  zhpr2.c)

#---------------------------------------------------------
#  Level 3 BLAS.
#---------------------------------------------------------
set(blas_srcs_SBLAS3
  sgemm.c
  ssymm.c
  ssyrk.c
  ssyr2k.c
  strmm.c
  strsm.c)

set(blas_srcs_CBLAS3
  cgemm.c
  csymm.c
  csyrk.c
  csyr2k.c
  ctrmm.c
  ctrsm.c
  chemm.c
  cherk.c
  cher2k.c)

set(blas_srcs_DBLAS3
  dgemm.c
  dsymm.c
  dsyrk.c
  dsyr2k.c
  dtrmm.c
  dtrsm.c)

set(blas_srcs_ZBLAS3
  zgemm.c
  zsymm.c
  zsyrk.c
  zsyr2k.c
  ztrmm.c
  ztrsm.c
  zhemm.c
  zherk.c
  zher2k.c)

add_library(${SCI_BLAS_LIBRARY} STATIC
  ${blas_srcs_SBLAS1}
  ${blas_srcs_SBLAS2}
  ${blas_srcs_SBLAS3}
  ${blas_srcs_DBLAS1}
  ${blas_srcs_DBLAS2}
  ${blas_srcs_DBLAS3}
  ${blas_srcs_CBLAS1}
  ${blas_srcs_CBLAS2}
  ${blas_srcs_CBLAS3}
  ${blas_srcs_ZBLAS1}
  ${blas_srcs_ZBLAS2}
  ${blas_srcs_ZBLAS3}
  ${blas_srcs_ALLBLAS})

target_link_libraries(${SCI_BLAS_LIBRARY} ${SCI_LIBF2C_LIBRARY})
  
if(UNIX AND NOT APPLE AND NOT CYGWIN)
  set_target_properties(${SCI_BLAS_LIBRARY}
                        PROPERTIES
                          COMPILE_FLAGS "-fPIC")
endif(UNIX AND NOT APPLE AND NOT CYGWIN)
