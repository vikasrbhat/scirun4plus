/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkQuadEdgeMeshParamMatrixCoefficients.h,v $
  Language:  C++
  Date:      $Date: 2008-10-03 21:55:06 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkQuadEdgeMeshParamMatrixCoefficients_h
#define __itkQuadEdgeMeshParamMatrixCoefficients_h

#include "itkQuadEdgeMesh.h"
#include "itkCrossHelper.h"
#include "itkTriangleHelper.h"
#include <vnl/vnl_math.h>

namespace itk
{
/** \class MatrixCoefficients
 * \brief Superclass for all the matrix coefficients computation classes.
 * \note  Belongs to the parameterisation package.
 */
template< typename TInputMesh >
class MatrixCoefficients
{
public:
  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::QEType          InputQEType;

  MatrixCoefficients( ){}
  virtual ~MatrixCoefficients( ) {}

  virtual InputCoordRepType operator ( )
      ( InputMeshType* iMesh, InputQEType* iEdge ) const = 0;
};

/** \class OnesMatrixCoefficients
 * \brief Compute a matrix filled by 1s wherever two vertices are connected
 *        by an edge.
 * \note  Belongs to the parameterisation package.
 * \note  See paper:
 */
template< typename TInputMesh >
class OnesMatrixCoefficients : public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh > Superclass;

  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::QEType          InputQEType;

  OnesMatrixCoefficients() {}

  /**
   * \param[in] iMesh
   * \param[in] iEdge
   * \return \f$ 1 \f$
   */
  InputCoordRepType operator ( ) ( InputMeshType * itkNotUsed( iMesh ),
                                   InputQEType *   itkNotUsed( iEdge ) ) const
    {
    return 1.0;
    }
};

/** \class InverseEuclideanDistanceMatrixCoefficients
 * \brief Compute a matrix filed with the inverse of the euclidian distance
 *        wherever two vertices are connected by an edge.
 * \note  Belongs to the parameterisation package.
 * \note  See paper: ...
 */
template< typename TInputMesh >
class InverseEuclideanDistanceMatrixCoefficients :
    public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh > Superclass;

  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::PointType       InputPointType;
  typedef typename InputMeshType::PointIdentifier InputPointIdentifier;
  typedef typename InputMeshType::QEType          InputQEType;
  typedef typename InputMeshType::VectorType      InputVectorType;

  InverseEuclideanDistanceMatrixCoefficients() {}

  /**
   * \param[in] iMesh
   * \param[in] iEdge
   * \return \f$ \frac{1}{\|\boldsymbol{p1} - \boldsymbol{p2} \|} \f$
   */
  InputCoordRepType operator () ( InputMeshType* iMesh, InputQEType* iEdge ) const
    {
    InputPointIdentifier id1 = iEdge->GetOrigin( );
    InputPointIdentifier id2 = iEdge->GetDestination( );

    InputPointType pt1 = iMesh->GetPoint( id1 );
    InputPointType pt2 = iMesh->GetPoint( id2 );

    InputCoordRepType oValue = 1.0 / pt1.EuclideanDistanceTo( pt2 );

    return oValue;
    }
};


/** \class ConformalMatrixCoefficients
 * \brief Compute a matrix filed by Conformal Coefficients of the edge
 *        wherever two vertices are connected by an edge.
 * \note  Belongs to the parameterisation package.
 * \note  See paper ...
 */
template< typename TInputMesh >
class ConformalMatrixCoefficients : public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh > Superclass;

  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::PointType       InputPointType;
  typedef typename InputMeshType::PointIdentifier InputPointIdentifier;
  typedef typename InputMeshType::QEType          InputQEType;

  ConformalMatrixCoefficients() { }

  /**
   * \param[in] iMesh
   * \param[in] iEdge
   * \return \f$ \text{cot} \alpha_{ij} + \text{cot} \beta_{ij} \f$
   */
  InputCoordRepType operator ( ) ( InputMeshType* iMesh, InputQEType* iEdge ) const
    {
    InputPointIdentifier id1 = iEdge->GetOrigin( );
    InputPointIdentifier id2 = iEdge->GetDestination( );
    InputPointType pt1 = iMesh->GetPoint( id1 );
    InputPointType pt2 = iMesh->GetPoint( id2 );
    
    InputCoordRepType oValue( 0.0 );

    if( iEdge->IsLeftSet() )
      {
      InputPointIdentifier idA = iEdge->GetLnext( )->GetDestination( );
      InputPointType ptA = iMesh->GetPoint( idA );
      oValue += TriangleHelper< InputPointType >::Cotangent( pt1, ptA, pt2);
      }
    if( iEdge->IsRightSet() )
      {
      InputPointIdentifier idB = iEdge->GetRnext( )->GetOrigin( );
      InputPointType ptB = iMesh->GetPoint( idB );
      oValue += TriangleHelper< InputPointType >::Cotangent( pt1, ptB, pt2);
      }

    return vnl_math_max( static_cast< InputCoordRepType >( 0.0 ), oValue );
    }
};

/**\class AuthalicMatrixCoefficients
 *
 * \brief Compute a matrix filled with Authalic Coefiicients of the edge,
 *        wherever two vertices are connected with an edge.
 * \note  Belongs to the Parameterisation package.
 * \note  See paper:
 */
template< typename TInputMesh >
class AuthalicMatrixCoefficients : public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh >        Superclass;

  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::PointType       InputPointType;
  typedef typename InputMeshType::PointIdentifier InputPointIdentifier;
  typedef typename InputMeshType::QEType          InputQEType;

  AuthalicMatrixCoefficients() { }

  /**
   * \param[in] iMesh
   * \param[in] iEdge
   * \return \f$ \frac{\text{cot} \gamma_{ij} + \text{cot}
   \delta_{ij}}{\|\boldsymbol{p1} - \boldsymbol{p2} \|} \f$
   */
  InputCoordRepType operator ( ) ( InputMeshType* iMesh, InputQEType* iEdge ) const
    {
    InputPointIdentifier id1 = iEdge->GetOrigin( );
    InputPointType pt1 = iMesh->GetPoint( id1 );
    
    InputPointIdentifier id2 = iEdge->GetDestination( );
    InputPointType pt2 = iMesh->GetPoint( id2 );
    
    InputCoordRepType oValue( 0.0 );

    if( iEdge->IsLeftSet() )
      {
      InputPointIdentifier idA = iEdge->GetLnext( )->GetDestination( );
      InputPointType ptA = iMesh->GetPoint( idA );
      oValue +=
        TriangleHelper< InputPointType >::Cotangent( pt1, pt2, ptA );
      }

    if( iEdge->IsRightSet() )
      {
      InputPointIdentifier idB = iEdge->GetRnext( )->GetOrigin( );
      InputPointType ptB = iMesh->GetPoint( idB );
      oValue += TriangleHelper< InputPointType >::Cotangent( pt1, pt2, ptB );
      }

    return oValue / pt1.EuclideanDistanceTo( pt2 );
    }
};

/** \class IntrinsicMatrixCoefficients
 * \brief Compute a mtrix filled by intrinsic Coefficients of the edge,
 *        wherever two vertices are connected by an edge.
 * \note  Belongs to the parameterization Package.
 * \note  See paper:
 */
template< typename TInputMesh >
class IntrinsicMatrixCoefficients : public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh >        Superclass;

  typedef TInputMesh                              InputMeshType;
  typedef typename InputMeshType::CoordRepType    InputCoordRepType;
  typedef typename InputMeshType::QEType          InputQEType;

  InputCoordRepType m_Lambda;

  IntrinsicMatrixCoefficients( const InputCoordRepType& iLambda ) :
      m_Lambda( iLambda )
    { }

  InputCoordRepType operator ( ) ( InputMeshType* iMesh,
                                  InputQEType* iEdge ) const
    {
    AuthalicMatrixCoefficients< TInputMesh >   authalic;
    ConformalMatrixCoefficients< TInputMesh >  conformal;

    InputCoordRepType oValue = m_Lambda * conformal( iMesh, iEdge )
      + ( 1.0 - m_Lambda ) * authalic( iMesh, iEdge );

    return oValue;
    }
};

/** \class HarmonicMatrixCoefficients
 * \brief Compute a matrix filled with Harmonic coefficients, wherever
 *        two vertices are connected by an edge.
 * \note  Belongs to the parameterization package.
 * \note  See paper:
 */
template< typename TInputMesh >
class HarmonicMatrixCoefficients : public MatrixCoefficients< TInputMesh >
{
public:
  typedef MatrixCoefficients< TInputMesh > Superclass;

  typedef TInputMesh                                InputMeshType;
  typedef typename InputMeshType::CoordRepType      InputCoordRepType;
  typedef typename InputMeshType::PointType         InputPointType;
  typedef typename InputPointType::VectorType       InputVectorType;
  typedef typename InputMeshType::PointIdentifier   InputPointIdentifier;
  typedef typename InputMeshType::QEType            InputQEType;

  itkStaticConstMacro( PointDimension, unsigned int,
      InputPointType::PointDimension );


  HarmonicMatrixCoefficients() { }


  InputCoordRepType operator () ( InputMeshType* iMesh, InputQEType* iEdge ) const
    {
    InputPointIdentifier id1 = iEdge->GetOrigin( );
    InputPointIdentifier id2 = iEdge->GetDestination( );

    InputPointIdentifier idA = iEdge->GetLnext( )->GetDestination( );
    InputPointIdentifier idB = iEdge->GetRnext( )->GetOrigin( );

    InputPointType pt1 = iMesh->GetPoint( id1 );
    InputPointType pt2 = iMesh->GetPoint( id2 );
    InputPointType ptA = iMesh->GetPoint( idA );
    InputPointType ptB = iMesh->GetPoint( idB );

    InputVectorType v1A = ptA - pt1;
    InputVectorType v1B = ptB - pt1;
    InputVectorType v12 = pt2 - pt1;

    InputCoordRepType L1A = v1A * v1A;
    InputCoordRepType L1B = v1B * v1B;
    InputCoordRepType L12 = v12 * v12;

    InputCoordRepType L2A = pt2.SquaredEuclideanDistanceTo( ptA );
    InputCoordRepType L2B = pt2.SquaredEuclideanDistanceTo( ptB );

    CrossHelper< InputVectorType > cross;

    InputCoordRepType AreaA = 0.5 * ( cross( v1A, v12 ).GetNorm( ) );
    InputCoordRepType AreaB = 0.5 * ( cross( v1B, v12 ).GetNorm( ) );

    InputCoordRepType
        oValue = ( L1A + L2A - L12 ) / AreaA + ( L1B + L2B - L12 ) / AreaB;

    return oValue;
    }
};

}
#endif
