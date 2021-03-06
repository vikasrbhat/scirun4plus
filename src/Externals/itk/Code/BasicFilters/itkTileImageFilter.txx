/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTileImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2008-10-18 16:11:15 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTileImageFilter_txx
#define __itkTileImageFilter_txx
#include "itkTileImageFilter.h"

#include "itkExceptionObject.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkPasteImageFilter.h"
#include "itkImportImageContainer.h"
#include "itkNumericTraitsRGBPixel.h"

namespace itk {

template<class TInputImage, class TOutputImage>
TileImageFilter<TInputImage, TOutputImage>
::TileImageFilter()
{
  m_Layout.Fill(0);
  m_DefaultPixelValue = NumericTraits<OutputPixelType>::ZeroValue();
}

template<class TInputImage, class TOutputImage>
void
TileImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  typename TOutputImage::Pointer output = this->GetOutput();
  typedef Image<InputPixelType, OutputImageDimension> TempImageType;

  // Allocate the output and initialize to default value
  this->AllocateOutputs();
  output->FillBuffer( m_DefaultPixelValue);

  ImageRegionIterator<TileImageType> it(m_TileImage, m_TileImage->GetBufferedRegion());
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    if (it.Get().m_ImageNumber >= 0)
      {
      typename PasteImageFilter<TOutputImage,TempImageType>::Pointer paste = PasteImageFilter<TOutputImage,TempImageType>::New();
      paste->SetDestinationImage(output);
      paste->InPlaceOn();

      // Create a temporary image that has the same dimensions as the
      // output image. The additional dimensions are set to 1. The
      // temporary image will use the same container as the input
      // image. This way we avoid copying the data.
      typename TempImageType::Pointer tempImage = TempImageType::New();
      OutputSizeType tempSize;
      OutputIndexType tempIndex;

      for (unsigned int i = 0; i < InputImageDimension; i++)
        {
        tempSize[i] = this->GetInput(it.Get().m_ImageNumber)->GetBufferedRegion().GetSize()[i];
        tempIndex[i] = this->GetInput(it.Get().m_ImageNumber)->GetBufferedRegion().GetIndex()[i];
        }
      for (unsigned int i = InputImageDimension; i < OutputImageDimension; i++)
        {
        tempSize[i] = 1;
        tempIndex[i] = 0;
        }
      OutputImageRegionType tempRegion(tempIndex, tempSize);
      tempImage->SetRegions(tempRegion);
      tempImage->SetPixelContainer(const_cast<ImportImageContainer<unsigned long, InputPixelType> *>(this->GetInput(it.Get().m_ImageNumber)->GetPixelContainer()));
      paste->SetSourceImage(tempImage);
      paste->SetDestinationIndex(it.Get().m_Region.GetIndex());
      paste->SetSourceRegion(tempRegion);
      paste->Update();
      output = paste->GetOutput();
      }
    ++it;
    }
  this->GraftOutput(output);
}

template<class TInputImage, class TOutputImage>
void
TileImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  for (unsigned int i = 0; i < this->GetNumberOfInputs(); i++)
    {
    TInputImage * input = const_cast<TInputImage *>(this->GetInput(i));
    if( input )
      {
      input->SetRequestedRegionToLargestPossibleRegion();
      }
    }
}

template<class TInputImage, class TOutputImage>
void
TileImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  // Supply spacing, origin and LargestPossibleRegion for the output.
  // This method does most of the work for this filter. In addition to
  // the spacing, origin and region, this method computes the source
  // region and destination index for multiple invocations of the
  // PasteImageFilter.
  OutputImagePointer      outputPtr = this->GetOutput();
  InputImagePointer inputPtr = const_cast<TInputImage*>(this->GetInput());

  if( !outputPtr || !inputPtr )
    {
    return;
    }

  // Spacing(Origin): use the spacing(origin) of the first input for
  // all of the matching output dimensions. For remaining dimensions
  // use 1.0 (0.0).
  float spacing[OutputImageDimension];
  float origin[OutputImageDimension];

  for (unsigned i = 0; i < OutputImageDimension; i++)
    {
    if (i < InputImageDimension)
      {
      spacing[i] = this->GetInput(0)->GetSpacing()[i];
      origin[i] = this->GetInput(0)->GetOrigin()[i];
      }
    else
      {
      spacing[i] = 1.0;
      origin[i] = 0.0;
      }
    }

  outputPtr->SetSpacing (spacing);
  outputPtr->SetOrigin (origin);

  // Create an image that has one element per tile. Each element of
  // this tile image will hold a class that defines the image number
  // for the tile, the size of the image and the destination index for
  // the image in the final composed image.
  typedef ImageRegion<OutputImageDimension>  RegionType;
  m_TileImage = TileImageType::New();

  // Determine the last dimension for the tile image. This dimension will
  // be large enough to accommodate left-over images.
  OutputSizeType outputSize; outputSize.Fill(1);
  OutputIndexType outputIndex; outputIndex.Fill(0);

  if (m_Layout[OutputImageDimension - 1] == 0)
    {
    int used = 1;
    for (unsigned int d = 0; d < OutputImageDimension - 1; d++)
      {
      used *= m_Layout[d];
      }
    outputSize[OutputImageDimension - 1] = (this->GetNumberOfInputs() - 1) / used + 1;
    if (outputSize[OutputImageDimension - 1] < 1)
      {
      outputSize[OutputImageDimension - 1] = 1;
      }
    m_Layout[OutputImageDimension - 1] = outputSize[OutputImageDimension - 1];
    }

  OutputSizeType tileSize; tileSize.Fill(1);
  OutputIndexType tileIndex; tileIndex.Fill(0);

  for (unsigned int i = 0; i < OutputImageDimension; i++)
    {
    tileSize[i] = m_Layout[i];
    }

  // Determine the size of the output. Each "row" size is determined
  // and the maximum size for each "row" will be the size for that
  // dimension.
  RegionType tileRegion(tileIndex, tileSize);
  m_TileImage->SetRegions(tileRegion);
  m_TileImage->Allocate();

  // Initialize the tile image with an image number for each tile. If
  // there is no corresponding input image for a tile, then set the
  // image number to -1.
  ImageRegionIteratorWithIndex<TileImageType> it(m_TileImage, m_TileImage->GetBufferedRegion());

  it.GoToBegin();
  unsigned int input = 0;
  TileInfo info;
  while( !it.IsAtEnd() ) 
    {
    if (input < this->GetNumberOfInputs())
      {
      info.m_ImageNumber = input;
      it.Set (info );
      }
    else
      {
      info.m_ImageNumber = -1;
      it.Set (info );
      }
    ++input;
    ++it;
    }

  // Find the size of the largest cell for each "row" in each dimension.
  ImageLinearConstIteratorWithIndex<TileImageType> tit( m_TileImage, m_TileImage->GetRequestedRegion() );
  int value;

  std::vector<std::vector<int> > sizes, offsets;

  sizes.resize(OutputImageDimension);
  offsets.resize(OutputImageDimension);
  for (unsigned int i = 0; i < OutputImageDimension; i++)
    {
    offsets[i].resize(m_Layout[i]);
    sizes[i].resize(m_Layout[i]);
    for (unsigned int l = 0; l < m_Layout[i]; l++)
      {
      sizes[i][l] = 1;
      }
    }
  for (unsigned int i = 0; i < OutputImageDimension; i++)
    {
    tit.SetDirection(i);
    tit.GoToBegin();
    while( !tit.IsAtEnd() )
      {
      int dsize;
      int count = 0;
      while( !tit.IsAtEndOfLine() )
        {
        value = tit.Get().m_ImageNumber;
        if (value != -1)
          {
          if (i < InputImageDimension)
            {
            dsize = this->GetInput(value)->GetLargestPossibleRegion().GetSize()[i];
            if (dsize > sizes[i][count])
              {
              sizes[i][count] = dsize;
              }
            }
          }
        ++tit;
        ++count;
        }
      tit.NextLine();
      } 
    }

  // Convert the sizes to offsets.
  for (unsigned int i = 0; i < OutputImageDimension; i++)
    {
    offsets[i][0] = 0;
    for (unsigned int t = 0; t < m_Layout[i] - 1; t++)
      {
      offsets[i][t+1] = offsets[i][t] + sizes[i][t];
      }
    // The size for each dimension is the value of the last offset
    outputSize[i] = offsets[i][m_Layout[i] - 1] + sizes[i][m_Layout[i] - 1];
    }

  // Now create an region for each tile that has an image
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    value = it.Get().m_ImageNumber;
    if (value >= 0)
      {
      typename TileImageType::IndexType tileIndex2 = it.GetIndex();

      OutputSizeType regionSize;
      OutputIndexType regionIndex;
      for (unsigned int i = 0; i < OutputImageDimension; i++)
        {
        regionIndex[i] = offsets[i][tileIndex2[i]];
        if (i < InputImageDimension)
          {
          regionSize[i] = this->GetInput(value)->GetLargestPossibleRegion().GetSize()[i];
          }
        else
          {
          regionSize[i] = 1;
          }
        }
      OutputImageRegionType region(regionIndex,regionSize);
      info = it.Get();
      info.m_Region = region;
      it.Set(info);
      }
    ++it;
    }

  typename TOutputImage::RegionType outputLargestPossibleRegion;

  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputIndex );
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template <class TInputImage, class TOutputImage>
void 
TileImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  std::cout << "DefaultPixelValue: "
            << static_cast<typename NumericTraits<OutputPixelType>::PrintType>(m_DefaultPixelValue)
            << std::endl;
  std::cout << "Layout: " << m_Layout << std::endl;
}


}// end namespace itk
#endif
