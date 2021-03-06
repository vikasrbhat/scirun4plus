/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkVoxBoCUBImageIO.h,v $
  Language:  C++
  Date:      $Date: 2008-06-18 19:28:57 $
  Version:   $1.0$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVoxBoCUBImageIO_h
#define __itkVoxBoCUBImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <fstream>
#include <string>
#include <map>
#include "itkImageIOBase.h"
#include "itkSpatialOrientation.h"
#include <stdio.h>

namespace itk
{

// Forward declaration
class GenericCUBFileAdaptor;

/** \class VoxBoCUBImageIO
 *
 *  \brief Read VoxBoCUBImage file format.
 *
 * A generic reader and writer object for VoxBo files. Basically it
 * provides uniform access to gzip and normal files
 *
 * \author Burstein, Pablo D.; Yushkevich, Paul; Gee, James C. 
 *
 * This implementation was contributed as a paper to the Insight Journal
 * http://insight-journal.org/midas/handle.php?handle=1926/303
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT VoxBoCUBImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef VoxBoCUBImageIO             Self;
  typedef ImageIOBase                 Superclass;
  typedef SmartPointer<Self>          Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VoxBoCUBImageIO, Superclass);

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer);


  VoxBoCUBImageIO();
  ~VoxBoCUBImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  VoxBoCUBImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool CheckExtension(const char*, bool &isCompressed);
  GenericCUBFileAdaptor *CreateReader(const char *filename);
  GenericCUBFileAdaptor *CreateWriter(const char *filename);
  GenericCUBFileAdaptor *m_Reader, *m_Writer;

  // Initialize the orientation map (from strings to ITK)
  void InitializeOrientationMap();

  // Orientation stuff
  typedef SpatialOrientation::ValidCoordinateOrientationFlags OrientationFlags;
  typedef std::map<std::string, OrientationFlags>             OrientationMap;
  typedef std::map<OrientationFlags, std::string>             InverseOrientationMap;

  OrientationMap            m_OrientationMap;
  InverseOrientationMap     m_InverseOrientationMap;

  // Method to swap bytes in read buffer
  void SwapBytesIfNecessary(void *buffer, unsigned long numberOfBytes);

  // Strings used in VoxBo files
  static const char * m_VB_IDENTIFIER_SYSTEM;
  static const char * m_VB_IDENTIFIER_FILETYPE;
  static const char * m_VB_DIMENSIONS;
  static const char * m_VB_SPACING;
  static const char * m_VB_ORIGIN;
  static const char * m_VB_DATATYPE;
  static const char * m_VB_BYTEORDER;
  static const char * m_VB_ORIENTATION;
  static const char * m_VB_BYTEORDER_MSB;
  static const char * m_VB_BYTEORDER_LSB;
  static const char * m_VB_DATATYPE_BYTE;
  static const char * m_VB_DATATYPE_INT;
  static const char * m_VB_DATATYPE_FLOAT;
  static const char * m_VB_DATATYPE_DOUBLE;
};

} // end namespace itk

#endif // __itkVoxBoCUBImageIO_h
