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

# CMakeLists.txt for Packages/MatlabInterface/Dataflow/Modules/DataIO

SET(Packages_MatlabInterface_Dataflow_Modules_DataIO_SRCS
  ImportColorMapsFromMatlab.cc
  ImportDatatypesFromMatlab.cc
  ExportDatatypesToMatlab.cc
  ImportFieldsFromMatlab.cc
  ExportFieldsToMatlab.cc
  ImportMatricesFromMatlab.cc
  ExportMatricesToMatlab.cc
  ImportNrrdsFromMatlab.cc
  ExportNrrdsToMatlab.cc
  ImportBundlesFromMatlab.cc
  ExportBundlesToMatlab.cc
)

SCIRUN_ADD_LIBRARY(Packages_MatlabInterface_Dataflow_Modules_DataIO ${Packages_MatlabInterface_Dataflow_Modules_DataIO_SRCS})

TARGET_LINK_LIBRARIES(Packages_MatlabInterface_Dataflow_Modules_DataIO
  Core_Persistent
  Core_Util
  Core_Exceptions
  Core_Thread
  Dataflow_GuiInterface
  Core_Datatypes
  Core_Geometry
  Core_GeomInterface
  Dataflow_TkExtensions
  Dataflow_Network
  Core_XMLUtil
  Core_Matlab
  Core_Services
  Core_ICom
  Core_SystemCall
  ${SCI_TEEM_LIBRARY}
  ${SCI_ZLIB_LIBRARY}
  ${M_LIBRARY}
  ${TK_LIBRARY}
  ${GL_LIBRARY}
)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DBUILD_Packages_MatlabInterface_Dataflow_Modules_DataIO)
ENDIF(BUILD_SHARED_LIBS)
