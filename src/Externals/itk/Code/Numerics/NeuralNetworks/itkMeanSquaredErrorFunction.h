/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMeanSquaredErrorFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-01-24 21:33:49 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMeanSquaredErrorFunction_h
#define __itkMeanSquaredErrorFunction_h

#include "itkErrorFunctionBase.h"
#include "vnl/vnl_matrix.h"
#include "vnl/vnl_vector.h"

namespace itk
{
namespace Statistics
{

template<class TMeasurementVector, class ScalarType>
class MeanSquaredErrorFunction : public ErrorFunctionBase<TMeasurementVector, ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef MeanSquaredErrorFunction                          Self;
  typedef ErrorFunctionBase<TMeasurementVector, ScalarType> Superclass;
  typedef SmartPointer<Self>                                Pointer;
  typedef SmartPointer<const Self>                          ConstPointer;
  typedef typename Superclass::ErrorVectorType              ErrorVectorType;
  typedef typename Superclass::InternalVectorType           InternalVectorType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanSquaredErrorFunction, FunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Evaluate at the specified Error position */
  virtual ScalarType Evaluate(const TMeasurementVector& Errors) const;

  virtual InternalVectorType EvaluateDerivative(const TMeasurementVector& Errors) const;

protected:

  MeanSquaredErrorFunction();
  virtual ~MeanSquaredErrorFunction();

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const;
};

} // end namespace Statistics
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
  #include "itkMeanSquaredErrorFunction.txx"
#endif


#endif
