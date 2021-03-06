/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTransferFunctionBase.h,v $
  Language:  C++
  Date:      $Date: 2009-01-28 21:04:59 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkTransferFunctionBase_h
#define __itkTransferFunctionBase_h

#include "itkFunctionBase.h"
#include "itkArray.h"

namespace itk
{
namespace Statistics
{

template<class ScalarType>
class TransferFunctionBase : public FunctionBase<ScalarType, ScalarType>
{
public:

  /** Standard class typedefs. */
  typedef TransferFunctionBase             Self;
  typedef FunctionBase<ScalarType, double> Superclass;
  typedef SmartPointer<Self>               Pointer;
  typedef SmartPointer<const Self>         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(TransferFunctionBase, FunctionBase);

  /** Input type */
  typedef ScalarType InputType;

  /** Output type */
  typedef ScalarType        OutputType;
  typedef Array<ScalarType> ArrayType;

  /** Evaluate at the specified input position */
  virtual OutputType Evaluate(const InputType& input) const = 0;

  /** Evaluate the derivative at the specified input position */
  virtual OutputType EvaluateDerivative(const InputType& input) const = 0;

protected:
  TransferFunctionBase() {};
  ~TransferFunctionBase(){};

  /** Method to print the object. */
  virtual void PrintSelf( std::ostream& os, Indent indent ) const
    {
    os << indent << "TransferFunctionBase(" << this << ")" << std::endl;
    Superclass::PrintSelf( os, indent );
    }

private:

  TransferFunctionBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace Statistics
} // end namespace itk
#endif
