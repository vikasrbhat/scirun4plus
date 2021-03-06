#-----------------------------------------------------------------------------
# Rebuild gdcm whenever a file starting with gdcm* is modified
INCLUDE_REGULAR_EXPRESSION("^gdcm.*$")


# Add proper includes
INCLUDE_DIRECTORIES(
  ${GDCM_SOURCE_DIR}/src
  ${GDCM_BINARY_DIR} #for gdcmConfigure.h
  ${GDCM_BINARY_DIR}/src #for gdcmjpeg 8, 12 and 16 
)

SET(libgdcm_la_SOURCES
   gdcmArgMgr.cxx
   gdcmBase.cxx
   gdcmBinEntry.cxx
   gdcmContentEntry.cxx
   gdcmDebug.cxx
   gdcmDicomDir.cxx
   gdcmDicomDirElement.cxx
   gdcmDicomDirImage.cxx
   gdcmDicomDirMeta.cxx
   gdcmDicomDirObject.cxx
   gdcmDicomDirPatient.cxx
   gdcmDicomDirVisit.cxx
   gdcmDicomDirSerie.cxx
   gdcmDicomDirStudy.cxx
   gdcmDict.cxx
   gdcmDictEntry.cxx
   gdcmDictGroupName.cxx
   gdcmDictSet.cxx
   gdcmDirList.cxx
   gdcmDocEntry.cxx
   gdcmDocEntryArchive.cxx
   gdcmDocEntrySet.cxx
   gdcmDocument.cxx
   gdcmElementSet.cxx
   gdcmException.cxx
   gdcmFile.cxx
   gdcmFileHelper.cxx
   gdcmGlobal.cxx
   gdcmJPEGFragment.cxx
   gdcmJPEGFragmentsInfo.cxx
   gdcmJpeg8.cxx
   gdcmJpeg12.cxx
   gdcmJpeg16.cxx
   gdcmJpeg2000.cxx
   gdcmOrientation.cxx
   gdcmPixelReadConvert.cxx
   gdcmPixelWriteConvert.cxx
   gdcmRLEFrame.cxx
   gdcmRLEFramesInfo.cxx
   gdcmSeqEntry.cxx
   gdcmSerieHelper.cxx
   gdcmSQItem.cxx
   gdcmTS.cxx
   gdcmUtil.cxx
   gdcmValEntry.cxx
   gdcmValidator.cxx
   gdcmVR.cxx
   ${GDCM_BINARY_DIR}/src/gdcmDefaultDicts.cxx
   )

ADD_LIBRARY(itkgdcm STATIC ${libgdcm_la_SOURCES})
TARGET_LINK_LIBRARIES(itkgdcm 
  itkjpeg8 
  itkjpeg12
  itkjpeg16
  itkopenjpeg
  ${UUID_LIBRARIES}
)
SET_TARGET_PROPERTIES(itkgdcm PROPERTIES DEFINE_SYMBOL gdcm_EXPORTS)
IF(WIN32)
  IF(NOT BORLAND)
    # rpcrt4 -> UuidCreate
    TARGET_LINK_LIBRARIES(itkgdcm wsock32 snmpapi rpcrt4)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

# Need nsl to resolve gethostbyname on SunOS-5.8
# and socket also
IF(CMAKE_SYSTEM MATCHES "SunOS.*")
  TARGET_LINK_LIBRARIES(itkgdcm socket nsl)
ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")

FILE(GLOB __files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
#INSTALL(FILES ${__files}
#  DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24}/gdcm/src
#  COMPONENT Development)
IF(ITK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(itkgdcm PROPERTIES ${ITK_LIBRARY_PROPERTIES})
ENDIF(ITK_LIBRARY_PROPERTIES)
#INSTALL(TARGETS itkgdcm
#  RUNTIME DESTINATION ${ITK_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
#  LIBRARY DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
#  ARCHIVE DESTINATION ${ITK_INSTALL_LIB_DIR_CM24} COMPONENT Development)

#
# Turn off GCC warnings on files that use gdcmAssert
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(gdcmPixelWriteConvert.cxx PROPERTIES COMPILE_FLAGS -w)
  SET_SOURCE_FILES_PROPERTIES(gdcmFile.cxx PROPERTIES COMPILE_FLAGS -w)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(gdcmDocument.cxx PROPERTIES COMPILE_FLAGS -fno-strict-aliasing)
  SET_SOURCE_FILES_PROPERTIES(gdcmSerieHelper.cxx PROPERTIES COMPILE_FLAGS -Wno-uninitialized)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
