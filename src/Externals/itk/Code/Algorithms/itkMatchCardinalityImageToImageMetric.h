/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatchCardinalityImageToImageMetric.h,v $
  Language:  C++
  Date:      $Date: 2009-04-23 03:53:36 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatchCardinalityImageToImageMetric_h
#define __itkMatchCardinalityImageToImageMetric_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptMatchCardinalityImageToImageMetric.h"
#else

#include "itkImageToImageMetric.h"
#include "itkCovariantVector.h"
#include "itkPoint.h"
#include "itkMultiThreader.h"
#include <vector>

namespace itk
{
/** \class MatchCardinalityImageToImageMetric
 * \brief Computes similarity between two objects to be registered
 *
 * This Class is templated over the type of the fixed and moving
 * images to be compared.
 *
 * This metric computes cardinality of the set of pixels that match
 * exactly between the moving and fixed images. The spatial
 * correspondance between both images is established through a
 * Transform. Pixel values are taken from the Moving image. Their
 * positions are mapped to the Fixed image and result in general in
 * non-grid position on it. Values at these non-grid position of the
 * Fixed image are interpolated using a user-selected Interpolator.
 *
 * This metric is designed for matching label maps. All pixel
 * mismatches are considered equal whether they are between label 1
 * and label 2 or between label 1 and label 500.  In other words, the
 * magnitude of an individual label mismatch is not relevant, or the
 * occurence of a label mismatch is important.
 *
 * Given the nature of label maps, a nearest neighbor interpolator is
 * the preferred interpolator.
 *
 * The metric measure can measure the number of pixel matches (pixels
 * with exactly the same label) or pixel mismatches (pixels with
 * different labels). The returned metric value is the number of pixel
 * matches (or mismatches) normalized by the number of pixels
 * considered. The number of pixel considered is a function of the
 * number of pixels in the overlap of the fixed and moving image
 * buffers conditional on any assigned masks.
 *
 * \ingroup RegistrationMetrics
 */
template < class TFixedImage, class TMovingImage > 
class ITK_EXPORT MatchCardinalityImageToImageMetric : 
    public ImageToImageMetric< TFixedImage, TMovingImage>
{
public:

  /** Standard class typedefs. */
  typedef MatchCardinalityImageToImageMetric              Self;
  typedef ImageToImageMetric<TFixedImage, TMovingImage >  Superclass;
  typedef SmartPointer<Self>                              Pointer;
  typedef SmartPointer<const Self>                        ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(MatchCardinalityImageToImageMetric, ImageToImageMetric);

 
  /** Types transferred from the base class */
  typedef typename Superclass::RealType                 RealType;
  typedef typename Superclass::TransformType            TransformType;
  typedef typename Superclass::TransformPointer         TransformPointer;
  typedef typename Superclass::TransformParametersType  TransformParametersType;
  typedef typename Superclass::TransformJacobianType    TransformJacobianType;
  typedef typename Superclass::GradientPixelType        GradientPixelType;
  typedef typename Superclass::InputPointType           InputPointType;
  typedef typename Superclass::OutputPointType          OutputPointType;

  typedef typename Superclass::MeasureType              MeasureType;
  typedef typename Superclass::DerivativeType           DerivativeType;
  typedef typename Superclass::FixedImageType           FixedImageType;
  typedef typename Superclass::MovingImageType          MovingImageType;
  typedef typename Superclass::FixedImageConstPointer   FixedImageConstPointer;
  typedef typename Superclass::MovingImageConstPointer  MovingImageConstPointer;
  typedef typename Superclass::FixedImageRegionType     FixedImageRegionType;


  /** Get the derivatives of the match measure. */
  void GetDerivative( const TransformParametersType &,
                      DerivativeType & derivative ) const
    {
    itkWarningMacro(<< "This metric does not provide metric derivatives.");
    derivative.Fill( NumericTraits<ITK_TYPENAME DerivativeType::ValueType>::Zero );
    }

  /**  Get the value of the metric at a particular parameter
   *  setting. The metric value is the number of pixel matches (or
   *  mis-matches, see SetMeasureMatches()) normalized by the number
   *  of pixels under consideration (within the buffer and if
   *  specified within a mask). In other words, the metric measure the
   *  percentage of pixel matches or mismatches. */
  MeasureType GetValue( const TransformParametersType & parameters ) const;

  /** Set/Get whether this metric measures pixel matches or pixel
   * mismatches. Note the GetValue() returns the number of matches (or
   * mismatches) normalized by the number of pixels considered. In
   * other words, the metric measures the percentage of pixel matches
   * or mismatches. The default is to measure matches
   * (MeasureMatchesOn). */
  itkSetMacro(MeasureMatches, bool);
  itkBooleanMacro(MeasureMatches);
  itkGetConstMacro(MeasureMatches, bool);
  
  /** Get/Set the number of threads to create when executing. */
  itkSetClampMacro( NumberOfThreads, int, 1, ITK_MAX_THREADS );
  itkGetConstReferenceMacro( NumberOfThreads, int );
  
  /** Return the multithreader used by this class. */
  MultiThreader * GetMultiThreader()
    {return m_Threader;}

protected:
  MatchCardinalityImageToImageMetric();
  virtual ~MatchCardinalityImageToImageMetric() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /**
   * Non-const version of GetValue().  This is a hack around various
   * const issues with trying to spawn threads from the const version
   * of GetValue().
   */
  MeasureType GetNonconstValue( const TransformParametersType & parameters );

  /**
   * Thread worker routine to calculate the contribution of the a
   * subregion to the overall metric.  Can only be called from
   * GetValue(). */
  virtual
  void ThreadedGetValue(const FixedImageRegionType& outputRegionForThread,
                        int threadId );

  /** Split the FixedImageRegion into "num" pieces, returning
   * region "i" as "splitRegion". This method is called "num" times. The
   * regions must not overlap. The method returns the number of pieces that
   * the routine is capable of splitting the FixedImageRegion,
   * i.e. return value is less than or equal to "num". */
  virtual
  int SplitFixedRegion(int i,int num, FixedImageRegionType& splitRegion);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGetValue(). */
  static ITK_THREAD_RETURN_TYPE ThreaderCallback( void *arg );

  /** Internal structure used for passing image data into the threading library */
  struct ThreadStruct
    {
    Pointer Metric;
    };

private:
  MatchCardinalityImageToImageMetric(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                       m_MeasureMatches;
  std::vector<MeasureType>   m_ThreadMatches;
  std::vector<unsigned long> m_ThreadCounts;
  
  /** Support processing data in multiple threads. Used by subclasses
   * (e.g., ImageSource). */
  MultiThreader::Pointer m_Threader;
  int                    m_NumberOfThreads;
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMatchCardinalityImageToImageMetric.txx"
#endif

#endif

#endif
