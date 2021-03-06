/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGDCMImageIO.h,v $
  Language:  C++
  Date:      $Date: 2009-04-25 12:25:42 $
  Version:   $Revision: 1.39 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGDCMImageIO_h
#define __itkGDCMImageIO_h

#include "itkImageIOBase.h"
#include <fstream>
#include <string>

namespace itk
{

/** \class GDCMImageIO
 *
 *  \brief ImageIO class for reading and writing DICOM V3.0 and ACR/NEMA 1&2 uncompressed images
 *  This class is only an adaptor to the gdcm library (currently gdcm 1.2.x is used by default):
 *
 * GDCM 1.2 can be found at:
 *  http://creatis-www.insa-lyon.fr/Public/Gdcm/
 *
 *  CREATIS INSA - Lyon 2003-2008
 *    http://www.creatis.insa-lyon.fr
 *
 * Using the CMake variable: ITK_USE_SYSTEM_GDCM it is now possible to use a system installed
 * GDCM 2.x release. GDCM 2.x is now being developped on sourceforge.net :
 *
 *         http://gdcm.sourceforge.net
 *
 * Documentation:
 * - http://apps.sourceforge.net/mediawiki/gdcm/index.php?title=ITK_USE_SYSTEM_GDCM
 * - http://apps.sourceforge.net/mediawiki/gdcm/index.php?title=GDCM_Release_2.0
 *
 *  \warning There are several restrictions to this current writer:
 *           -  Eventhough during the writing process you pass in a DICOM file as input
 *              The output file may not contains ALL DICOM field from the input file.
 *              In particular:
 *                             - The SeQuence DICOM field (SQ).
 *                             - Fields from Private Dictionary 
 *           -  Some very long (>0xfff) binary fields are not loaded (typically 0029|0010), 
 *              you need to explicitely set the maximum length of elements to load to be bigger
 *              (see Get/SetMaxSizeLoadEntry)
 *           - GDCMImageIO was not handling rescale slope/intercept properly. This is fixed as of 11/12/2007
 *           - In DICOM some field are stored directly using there binary representation. When loaded into
 *             the MetaDataDict some fields are converted to ASCII (only VR: OB/OW/OF and UN are encoded as
 *             mime64).
 *
 *  \ingroup IOFilters
 *
 */
class InternalHeader;
class ITK_EXPORT GDCMImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef GDCMImageIO         Self;
  typedef ImageIOBase         Superclass;
  typedef SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GDCMImageIO, Superclass);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);
  
  /** Set the spacing and dimesion information for the current filename. */
  virtual void ReadImageInformation();
  
  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. GDCM triggers on ".dcm" and ".dicom". */
  virtual bool CanWriteFile(const char*);

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();
  
  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);
  
  /** Macro to access Rescale Slope and Rescale Intercept. Which are
   * needed to rescale properly image when needed. User then need to 
   * Always check those value when access value from the DICOM header */
  itkGetConstMacro(RescaleSlope, double);
  itkGetConstMacro(RescaleIntercept, double);

  /** Macro to access the DICOM UID prefix. By default this is the ITK
   *  root id. This default can be overriden if the exam is for example
   *  part of an existing study.
   */
  itkGetStringMacro(UIDPrefix);
  itkSetStringMacro(UIDPrefix);

  /** Access the generated DICOM UID's. */
  itkGetStringMacro(StudyInstanceUID);
  itkGetStringMacro(SeriesInstanceUID);
  itkGetStringMacro(FrameOfReferenceInstanceUID);

  /** Preserve the original DICOM UID of the input files
   */
  itkSetMacro(KeepOriginalUID,bool);
  itkGetConstMacro(KeepOriginalUID,bool);
  itkBooleanMacro(KeepOriginalUID);

  /** Convenience methods to query patient information and scanner
   * information. These methods are here for compatibility with the
   * DICOMImageIO2 class and as such should not be used in any new code.
   * They rely on properly preallocated buffer, which is not a good practice.
   * Instead user are encourage to use directly the GetValueFromTag function
   */
  void GetPatientName(char* name);
  void GetPatientID(char* id);
  void GetPatientSex(char* sex);
  void GetPatientAge(char* age);
  void GetStudyID(char* id);
  void GetPatientDOB(char* dob);
  void GetStudyDescription(char* desc);
  void GetBodyPart(char* part);
  void GetNumberOfSeriesInStudy(char* series);
  void GetNumberOfStudyRelatedSeries(char* series);
  void GetStudyDate(char* date);
  void GetModality(char* modality);
  void GetManufacturer(char* manu);
  void GetInstitution(char* ins);
  void GetModel(char* model);
  void GetScanOptions(char *options);

  /** More general method to retrieve an arbitrary DICOM value based
   * on a DICOM Tag (eg "0123|4567").
   * WARNING: You need to use the lower case for hex 0x[a-f], for instance:
   * "0020|000d" instead of "0020|000D" (the latter won't work)
   */
  bool GetValueFromTag(const std::string & tag, std::string & value);

  /** Method for consulting the DICOM dictionary and recovering the text
   * description of a field using its numeric tag represented as a string.  If
   * the tagkey is not found in the dictionary then this static method return
   * false and the value "Unknown " in the labelId. If the tagkey is found then
   * this static method returns true and the actual string descriptor of the
   * tagkey is returned in the variable labelId. */
  static bool GetLabelFromTag( const std::string & tag, 
                               std::string & labelId );

  /** A DICOM file can contains multiple binary stream that can be very long
   * For example an Overlay on the image. Most of the time user do not want to load
   * this binary structure in memory since it can consume lot of memory. Therefore
   * any field that is bigger than the default value 0xfff is discarded and just seek'd 
   * This method allow advanced user to force the reading of such field
   */
  itkSetMacro(MaxSizeLoadEntry, long);

  /** Parse any sequences in the DICOM file. Defaults to the value of
   *  LoadSequencesDefault. Loading DICOM files is faster when
   *  sequences are not needed.
   */
  itkSetMacro(LoadSequences, bool);
  itkGetConstMacro(LoadSequences, bool);
  itkBooleanMacro(LoadSequences);

  /** Parse any private tags in the DICOM file. Defaults to the value
   * of LoadPrivateTagsDefault. Loading DICOM files is faster when
   * private tags are not needed.
   */
  itkSetMacro(LoadPrivateTags, bool);
  itkGetConstMacro(LoadPrivateTags, bool);
  itkBooleanMacro(LoadPrivateTags);  

  /** Global method to define the default value for
   * LoadSequences. When instances of GDCMImageIO are created, the
   * ivar LoadSequences is initialized to the value of
   * LoadSequencesDefault.  This method is useful when relying on the
   * IO factory mechanism to load images rather than specifying a
   * particular ImageIO object on the readers. Default is false. */
  static void SetLoadSequencesDefault(bool b)
    { m_LoadSequencesDefault = b; }
  static void LoadSequencesDefaultOn()
    { m_LoadSequencesDefault = true; }
  static void LoadSequencesDefaultOff()
    { m_LoadSequencesDefault = false; }
  static bool GetLoadSequencesDefault()
    { return m_LoadSequencesDefault; }

  /** Global method to define the default value for
   * LoadPrivateTags. When instances of GDCMImageIO are created, the
   * ivar LoadPrivateTags is initialized to the value of
   * LoadPrivateTagsDefault.  This method is useful when relying on the
   * IO factory mechanism to load images rather than specifying a
   * particular ImageIO object on the readers. Default is false. */
  static void SetLoadPrivateTagsDefault(bool b)
    { m_LoadPrivateTagsDefault = b; }
  static void LoadPrivateTagsDefaultOn()
    { m_LoadPrivateTagsDefault = true; }
  static void LoadPrivateTagsDefaultOff()
    { m_LoadPrivateTagsDefault = false; }
  static bool GetLoadPrivateTagsDefault()
    { return m_LoadPrivateTagsDefault; }

  /** Set/Get a boolean to use the JPEG2000 compression or not. */
  typedef enum { JPEG = 0, JPEG2000 } TCompressionType;
  itkSetEnumMacro(CompressionType,TCompressionType);
  itkGetEnumMacro(CompressionType,TCompressionType);

protected:
  GDCMImageIO();
  ~GDCMImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;

  bool OpenGDCMFileForReading(std::ifstream& os, const char* filename);
  bool OpenGDCMFileForWriting(std::ofstream& os, const char* filename);
  void InternalReadImageInformation(std::ifstream& file);

  double m_RescaleSlope;
  double m_RescaleIntercept;

  std::string m_UIDPrefix;
  std::string m_StudyInstanceUID;
  std::string m_SeriesInstanceUID;
  std::string m_FrameOfReferenceInstanceUID;
  bool        m_KeepOriginalUID;
  long        m_MaxSizeLoadEntry;

private:
  GDCMImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_PatientName;
  std::string m_PatientID;
  std::string m_PatientDOB;
  std::string m_StudyID;   
  std::string m_StudyDescription;
  std::string m_BodyPart;
  std::string m_NumberOfSeriesInStudy;
  std::string m_NumberOfStudyRelatedSeries;
  std::string m_PatientSex;
  std::string m_PatientAge;
  std::string m_StudyDate;
  std::string m_Modality;
  std::string m_Manufacturer;
  std::string m_Institution;
  std::string m_Model;
  std::string m_ScanOptions;

  bool        m_LoadSequences;
  bool        m_LoadPrivateTags;
  static bool m_LoadSequencesDefault;
  static bool m_LoadPrivateTagsDefault;
  
  /** defines whether this image is a 2D out of a 2D image
   *  or a 2D out of a 3D image. */
  unsigned int     m_GlobalNumberOfDimensions;
  TCompressionType m_CompressionType;
  
  ImageIOBase::IOComponentType m_InternalComponentType;
  InternalHeader *             m_DICOMHeader;
};

} // end namespace itk

#endif // __itkGDCMImageIO_h
