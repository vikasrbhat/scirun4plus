/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPhilipsRECImageIO.h,v $
  Language:  C++
  Date:      $Date: 2009-03-19 14:20:58 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

/**
 * \file   The code for this file reader was written based on
 *         examination of Philips REC/PAR image files acquired at the
 *         Center for NMR Research at the Penn State Milton S. Hershey
 *         Medical Center.
 *
 *
 * \author Don C. Bigler
 *         The Pennsylvania State University 2005
 *
 * This implementation was contributed as a paper to the Insight Journal
 * http://insight-journal.org/midas/handle.php?handle=1926/1381
 *
 */

#ifndef __itkPhilipsRECImageIO_h
#define __itkPhilipsRECImageIO_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkImageIOBase.h"
#include "itkVectorContainer.h"
#include "vnl/vnl_vector_fixed.h"

namespace itk
{
/** \class PhilipsRECImageIO
 * \ingroup IOFilters
 * \author Don C. Bigler
 * \brief Class that defines how to read Philips REC/PAR image files.
 *  This class supports reading only and not writing.
 */

class ITK_EXPORT PhilipsRECImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef PhilipsRECImageIO       Self;
  typedef ImageIOBase             Superclass;
  typedef SmartPointer<Self>      Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhilipsRECImageIO, Superclass);

  /** Special types used for Philips PAR meta data. */
  typedef VectorContainer< unsigned int, double > EchoTimesContainerType;
  typedef VectorContainer< unsigned int, double > TriggerTimesContainerType;
  typedef VectorContainer< unsigned int, double > RepetitionTimesContainerType;
  typedef vnl_vector_fixed< int, 2 >              ScanResolutionType;
  typedef vnl_vector_fixed< float, 3 >            FOVType;
  typedef vnl_vector_fixed< double, 3 >           AngulationMidSliceType;
  typedef vnl_vector_fixed< double, 3 >           OffCentreMidSliceType;
  typedef vnl_vector_fixed< float, 3 >            PhaseEncodingVelocityType;
  /** Image types: 
   * 0 = Magnitude, 
   * 1 = Real, 
   * 2 = Imaginary, 
   * 3 = Phase,
   * 4 = Special/Processed. */
  typedef vnl_vector_fixed< int, 8 >              ImageTypesType;
  typedef vnl_vector_fixed< int, 8 >              ScanningSequencesType;
  typedef std::vector< int >                      SliceIndexType;
  typedef vnl_vector_fixed< double, 3 >           ImageTypeRescaleValuesType;
  
  typedef VectorContainer< unsigned int, ImageTypeRescaleValuesType >
                            ImageTypeRescaleValuesContainerType;
  typedef ImageTypeRescaleValuesContainerType::Pointer
                            ImageTypeRescaleValuesContainerTypePtr;
  typedef VectorContainer< unsigned int, ImageTypeRescaleValuesContainerTypePtr >
                            ScanningSequenceImageTypeRescaleValuesContainerType;
  typedef double            GradientBvalueType;
  typedef VectorContainer< unsigned int, GradientBvalueType >
                            GradientBvalueContainerType;
  typedef vnl_vector_fixed< double, 3 >
                            GradientDirectionType;
  typedef VectorContainer< unsigned int, GradientDirectionType >
                            GradientDirectionContainerType;
  typedef VectorContainer< unsigned int, int > 
                            LabelTypesASLContainerType;

  /*-------- This part of the interfaces deals with reading data. ----- */

  /** Determine if the file can be read with this ImageIO implementation.
       * \author Don C. Bigler
       * \param FileNameToRead The name of the file to test for reading.
       * \return Returns true if this ImageIO can read the file specified.
       */
  virtual bool CanReadFile(const char* FileNameToRead);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine if the file can be written with this ImageIO implementation.
       * \param FileNameToWrite The name of the file to test for writing.
       * \author Don C. Bigler
       * \post This function will always return false (Not implemented).
       * \return Returns true if this ImageIO can write the file specified.
       */
  virtual bool CanWriteFile( const char * itkNotUsed( FileNameToWrite ) )
    {
    return false;
    }

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation()
    { 
    return;
    }

  /** Writes the data to disk from the memory buffer provided. Make sure
       * that the IORegions has been set properly. */
  virtual void Write( const void * itkNotUsed(buffer) )
    {
    return;
    }

protected:
  PhilipsRECImageIO();
  ~PhilipsRECImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;

private:

  PhilipsRECImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  void SwapBytesIfNecessary(void * buffer, unsigned long numberOfPixels);
  int GetSliceIndex(int index);

  SliceIndexType *          m_SliceIndex;
  ImageIOBase::ByteOrder    m_MachineByteOrder;
};

} // end namespace itk

#endif // __itkPhilipsRECImageIO_h
