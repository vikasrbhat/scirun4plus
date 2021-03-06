/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanSquaresImageToImageMetric.h,v $
  Language:  C++
  Date:      $Date: 2009-07-15 08:45:50 $
  Version:   $Revision: 1.33 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeanSquaresImageToImageMetric_h
#define __itkMeanSquaresImageToImageMetric_h

// First make sure that the configuration is available.
// This line can be removed once the optimized versions
// gets integrated into the main directories.
#include "itkConfigure.h"

#ifdef ITK_USE_OPTIMIZED_REGISTRATION_METHODS
#include "itkOptMeanSquaresImageToImageMetric.h"
#else

#include "itkImageToImageMetric.h"
#include "itkCovariantVector.h"
#include "itkPoint.h"


namespace itk
{
/** \class MeanSquaresImageToImageMetric
 * \brief Computes similarity between two objects to be registered
 *
 * This Class is templated over the type of the fixed and moving
 * images to be compared.
 *
 * This metric computes the mean of squared differences between pixels in
 * the moving image and pixels in the fixed image. The spatial correspondance 
 * between both images is established through a Transform. Pixel values are
 * taken from the Moving image. Their positions are mapped to the Fixed image
 * and result in general in non-grid position on it. Values at these non-grid
 * position of the Fixed image are interpolated using a user-selected Interpolator.
 *
 * \ingroup RegistrationMetrics
 */
template < class TFixedImage, class TMovingImage > 
class ITK_EXPORT MeanSquaresImageToImageMetric : 
    public ImageToImageMetric< TFixedImage, TMovingImage>
{
public:

  /** Standard class typedefs. */
  typedef MeanSquaresImageToImageMetric                  Self;
  typedef ImageToImageMetric<TFixedImage, TMovingImage > Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanSquaresImageToImageMetric, ImageToImageMetric);

 
  /** Types transferred from the base class */
  typedef typename Superclass::RealType                 RealType;
  typedef typename Superclass::TransformType            TransformType;
  typedef typename Superclass::TransformPointer         TransformPointer;
  typedef typename Superclass::TransformParametersType  TransformParametersType;
  typedef typename Superclass::TransformJacobianType    TransformJacobianType;
  typedef typename Superclass::GradientPixelType        GradientPixelType;
  typedef typename Superclass::GradientImageType        GradientImageType;
  typedef typename Superclass::InputPointType           InputPointType;
  typedef typename Superclass::OutputPointType          OutputPointType;

  typedef typename Superclass::MeasureType              MeasureType;
  typedef typename Superclass::DerivativeType           DerivativeType;
  typedef typename Superclass::FixedImageType           FixedImageType;
  typedef typename Superclass::MovingImageType          MovingImageType;
  typedef typename Superclass::FixedImageConstPointer   FixedImageConstPointer;
  typedef typename Superclass::MovingImageConstPointer  MovingImageConstPointer;


  /** Get the derivatives of the match measure. */
  void GetDerivative( const TransformParametersType & parameters,
                      DerivativeType & derivative ) const;

  /**  Get the value for single valued optimizers. */
  MeasureType GetValue( const TransformParametersType & parameters ) const;

  /**  Get value and derivatives for multiple valued optimizers. */
  void GetValueAndDerivative( const TransformParametersType & parameters,
                              MeasureType& Value, DerivativeType& Derivative ) const;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(MovingPixelTypeHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TMovingImage::PixelType>));
  itkConceptMacro(MovingRealTypeAdditivieOperatorsCheck,
    (Concept::AdditiveOperators<
     typename NumericTraits<typename TMovingImage::PixelType>::RealType>));
  itkConceptMacro(MovingRealTypeMultiplyOperatorCheck,
    (Concept::MultiplyOperator<
     typename NumericTraits<typename TMovingImage::PixelType>::RealType>));

  /** End concept checking */
#endif

protected:
  MeanSquaresImageToImageMetric();
  virtual ~MeanSquaresImageToImageMetric() {};

private:
  MeanSquaresImageToImageMetric(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeanSquaresImageToImageMetric.txx"
#endif

#endif

#endif
