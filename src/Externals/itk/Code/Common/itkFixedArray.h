/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFixedArray.h,v $
  Language:  C++
  Date:      $Date: 2009-07-09 18:06:39 $
  Version:   $Revision: 1.45 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFixedArray_h
#define __itkFixedArray_h

#include "itkMacro.h"

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4284) // operator-> returning pointer to non-aggregate
#endif

namespace itk
{

/**
 * Due to a bug in MSVC, an enum value cannot be accessed out of a template
 * parameter until the template class opens.  In order for templated classes
 * to access the dimension of a vector template parameter in defining their
 * own dimension, this class is needed as a work-around.
 */
template <typename TVector>
struct GetVectorDimension
{
  itkStaticConstMacro(VectorDimension, unsigned int, TVector::Dimension);
}; 


  
/** \class FixedArray
 *  \brief Simulate a standard C array with copy semnatics.
 *
 * Simulates a standard C array, except that copy semantics are used instead
 * of reference semantics.  Also, arrays of different sizes cannot be
 * assigned to one another, and size information is known for function
 * returns.
 *
 * Template parameters for class FixedArray:
 * - TValueType = Element type stored at each location in the array.
 * - VLength    = Length of the array.
 *
 * The length of the array is fixed at compile time. If you wish to
 * specify the length of the array at run-time, use the class itk::Array.
 * If you wish to change to change the length of the array at run-time,
 * you're best off using std::vector<>.
 *
 * \ingroup DataRepresentation 
 */
template <typename TValueType, unsigned int VLength=3>
class FixedArray
{
public:
  /** Length constant */
  itkStaticConstMacro(Length, unsigned int, VLength);
  
  /** Dimension constant */
  itkStaticConstMacro(Dimension, unsigned int, VLength);
  
  /** The element type stored at each location in the FixedArray. */
  typedef TValueType  ValueType;
  
  /** A type representing the C-array version of this FixedArray. */
  typedef ValueType         CArray[VLength];
  
  /** An iterator through the array. */
  typedef ValueType*        Iterator;

  /** A const iterator through the array. */
  typedef const ValueType*  ConstIterator;

  class ConstReverseIterator;

  /** \class ReverseIterator 
   * \brief A reverse iterator through the array. */
  class ReverseIterator
    {
    public:
      explicit ReverseIterator(Iterator i): m_Iterator(i) {}
      Iterator operator++()        { return --m_Iterator; }
      Iterator operator++(int)     { return m_Iterator--; }
      Iterator operator--()        { return ++m_Iterator; }
      Iterator operator--(int)     { return m_Iterator++; }
      Iterator operator->() const  { return (m_Iterator-1); }
      ValueType& operator*() const { return *(m_Iterator-1); }
      bool operator!=(const ReverseIterator &rit) const {return m_Iterator != rit.m_Iterator;}
      bool operator==(const ReverseIterator &rit) const {return m_Iterator == rit.m_Iterator;}
    private:
      Iterator m_Iterator;
      friend class ConstReverseIterator;
    };
  
  /** \class ConstReverseIterator 
   * \brief A const reverse iterator through the array. */
  class ConstReverseIterator
    {
    public:
      explicit ConstReverseIterator(ConstIterator i): m_Iterator(i) {}
      ConstReverseIterator(const ReverseIterator& rit) { m_Iterator = rit.m_Iterator; }
      ConstIterator operator++()         { return --m_Iterator; }
      ConstIterator operator++(int)      { return m_Iterator--; }
      ConstIterator operator--()         { return ++m_Iterator; }
      ConstIterator operator--(int)      { return m_Iterator++; }
      ConstIterator operator->() const   { return (m_Iterator-1); }
      const ValueType& operator*() const { return *(m_Iterator-1); }
      bool operator!=(const ConstReverseIterator &rit) const {return m_Iterator != rit.m_Iterator;}
      bool operator==(const ConstReverseIterator &rit) const {return m_Iterator == rit.m_Iterator;}
    private:
      ConstIterator m_Iterator;
    };  
  
  /** A pointer to the ValueType. */
  typedef ValueType *        pointer;

  /** A const pointer to the ValueType. */
  typedef const ValueType*  const_pointer;

  /** A reference to the ValueType. */
  typedef ValueType&        reference;

  /** A const reference to the ValueType. */
  typedef const ValueType&  const_reference;
  
  typedef unsigned int   SizeType;
  
public:
  /** Constructors */
  FixedArray();
  FixedArray(const ValueType r[VLength]);

  /** Constructor to initialize a fixed array from another of any data type */
  template< class TFixedArrayValueType >
  FixedArray(const FixedArray< TFixedArrayValueType, VLength >& r)
    {
    typename FixedArray< TFixedArrayValueType, VLength >::ConstIterator input = r.Begin();
    Iterator i = this->Begin();
    while( i != this->End() ) 
      {
      *i++ = static_cast< TValueType >(*input++);
      }
    }
  

  /** This destructor is not virtual for performance reasons. However, this
   * means that subclasses cannot allocate memory.
   *
   * The destructor is PURPOSELY NOT DEFINED, in order to prevent inefficient
   * byte alignment of arrays of this object.
   *
   * ~FixedArray();
   *
   * For a full discussion, see 
   * http://www.itk.org/mailman/private/insight-developers/2008-June/010480.html
   *
   */

  /** Operator= defined for a variety of types. */
  template< class TFixedArrayValueType >
  FixedArray& operator= (const FixedArray< TFixedArrayValueType, VLength > & r)
    {
    if((void *)r.Begin() == (void *)m_InternalArray) return *this;
    typename FixedArray< TFixedArrayValueType, VLength >::ConstIterator input = r.Begin();
    Iterator i = this->Begin();
    while( i != this->End() ) 
      {
      *i++ = static_cast< TValueType >(*input++);
      }
    return *this;
    }

  FixedArray& operator= (const ValueType r[VLength]);
    
  /** Operators == and != are used to compare whether two arrays are equal.
   * Note that arrays are equal when the number of components (size) is the
   * same, and each component value is equal. */
  bool operator==(const FixedArray& r ) const;
  bool operator!=(const FixedArray& r ) const
    { return !operator==(r); }
  
  /** Allow the FixedArray to be indexed normally.  No bounds checking is done.
   * The separate versions are a work-around for an integer conversion bug in
   * Visual C++. */
        reference operator[](short index)                { return m_InternalArray[index]; }
  const_reference operator[](short index) const          { return m_InternalArray[index]; }
        reference operator[](unsigned short index)       { return m_InternalArray[index]; }
  const_reference operator[](unsigned short index) const { return m_InternalArray[index]; }
        reference operator[](int index)                  { return m_InternalArray[index]; }
  const_reference operator[](int index) const            { return m_InternalArray[index]; }
        reference operator[](unsigned int index)         { return m_InternalArray[index]; }
  const_reference operator[](unsigned int index) const   { return m_InternalArray[index]; }
        reference operator[](long index)                 { return m_InternalArray[index]; }
  const_reference operator[](long index) const           { return m_InternalArray[index]; }
        reference operator[](unsigned long index)        { return m_InternalArray[index]; }
  const_reference operator[](unsigned long index) const  { return m_InternalArray[index]; }
  
  /** Set/Get element methods are more convenient in wrapping languages */
  void SetElement( unsigned short index, const_reference value )
                                  { m_InternalArray[ index ] = value; }
  const_reference GetElement( unsigned short index ) const { return m_InternalArray[index]; }
  
  /** Return a pointer to the data. */
  ValueType* GetDataPointer() { return m_InternalArray; }
  const ValueType* GetDataPointer() const { return m_InternalArray; }
    
  /** Get various iterators to the array. */
  Iterator      Begin();
  ConstIterator Begin() const;
  Iterator      End();
  ConstIterator End() const;
  ReverseIterator      rBegin();
  ConstReverseIterator rBegin() const;
  ReverseIterator      rEnd();
  ConstReverseIterator rEnd() const;
  SizeType      Size() const;
  void Fill(const ValueType&);
    
private:
  /** Internal C array representation. */
  CArray  m_InternalArray;
  
public:
 
  static FixedArray Filled(const ValueType&);
};
  
template <typename TValueType, unsigned int VLength>
std::ostream & operator<<(std::ostream &os, const FixedArray<TValueType,VLength> &arr);

} // namespace itk

#ifdef _MSC_VER
# pragma warning (pop)
#endif

// Define instantiation macro for this template.
#define ITK_TEMPLATE_FixedArray(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT FixedArray< ITK_TEMPLATE_2 x >)) \
  _(1(EXPORT std::ostream& operator<<(std::ostream&, \
                                      const FixedArray< ITK_TEMPLATE_2 x >&))) \
  namespace Templates { typedef FixedArray< ITK_TEMPLATE_2 x > FixedArray##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkFixedArray+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkFixedArray.txx"
#endif

#include "itkNumericTraitsFixedArrayPixel.h"

#endif
