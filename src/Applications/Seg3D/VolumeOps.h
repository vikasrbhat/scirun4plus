//  
//  For more information, please see: http://software.sci.utah.edu
//  
//  The MIT License
//  
//  Copyright (c) 2009 Scientific Computing and Imaging Institute,
//  University of Utah.
//  
//  License for the specific language governing rights and limitations under
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//  
//    File   : VolumeOps.h
//    Author : McKay Davis
//    Date   : Tue Oct 31 17:41:48 2006 (spooky!)

#ifndef SEG3D_VolumeOps_H
#define SEG3D_VolumeOps_H 1

#include <Core/Datatypes/NrrdData.h>
#include <Core/Util/Assert.h>

namespace SCIRun {

#define LabelNrrdType nrrdTypeUChar
typedef unsigned char label_type;

class NrrdStats {
public:
  NrrdStats(double min, double max, double mean, double deviation, double median, unsigned int n, double volume)
  : min_(min), max_(max), mean_(mean), deviation_(deviation), n_(n), volume_(volume) {}

  inline double min() { return min_; }
  inline double max() { return max_; }
  inline double mean() { return mean_; }
  inline double stddev() { return deviation_; }
  inline double median() { return median_; }
  inline unsigned int n() { return n_; }
  inline double volume() { return volume_; }

private:
  double min_;
  double max_;
  double mean_;
  double deviation_;
  double median_;
  unsigned int n_;
  double volume_;
};


class VolumeOps {
public:
  static size_t                 nrrd_elem_count(NrrdDataHandle &);
  static size_t                 nrrd_data_size(NrrdDataHandle &);
  static size_t                 nrrd_type_size(NrrdDataHandle &);

  static NrrdDataHandle         float_to_bit(NrrdDataHandle &,
                                             float ref = 0.0,
                                             unsigned int mask = 0);


  static NrrdDataHandle convert_to(NrrdDataHandle &ninh,
				   unsigned int type);

  static NrrdDataHandle bit_to_float(NrrdDataHandle &,
				     unsigned int mask = 0,
				     float ref = 0.0);
                                            
  static void bit_set(NrrdDataHandle &dst, unsigned int dbit);
  static void bit_clear(NrrdDataHandle &dst, unsigned int dbit);
  static void bit_copy(NrrdDataHandle &dst, unsigned int dbit,
                       NrrdDataHandle &src, unsigned int sbit);
  static void bit_invert(NrrdDataHandle &dst, unsigned int dbit,
                         NrrdDataHandle &src, unsigned int sbit);
  static void bit_and(NrrdDataHandle &dst, unsigned int dbit,
                      NrrdDataHandle &src1, unsigned int sbit1,
                      NrrdDataHandle &src2, unsigned int sbit2);
  static void bit_or(NrrdDataHandle &dst, unsigned int dbit,
                     NrrdDataHandle &src1, unsigned int sbit1,
                     NrrdDataHandle &src2, unsigned int sbit2);
  static void bit_xor(NrrdDataHandle &dst, unsigned int dbit,
                      NrrdDataHandle &src1, unsigned int sbit1,
                      NrrdDataHandle &src2, unsigned int sbit2);
  static void bit_andnot(NrrdDataHandle &dst, unsigned int dbit,
                         NrrdDataHandle &src1, unsigned int sbit1,
                         NrrdDataHandle &src2, unsigned int sbit2);

  static size_t bit_count(NrrdDataHandle &src, unsigned int sbit);

  static void merge_label_into_segmentation(NrrdDataHandle &seg,
                                            unsigned int svalue,
                                            NrrdDataHandle &label,
                                            unsigned int lbit);

  static void extract_label_from_segmentation(NrrdDataHandle &label,
                                              unsigned int lbit,
                                              NrrdDataHandle &seg,
                                              unsigned int svalue);

  static void threshold(NrrdDataHandle &dst, unsigned int dbit,
                        NrrdDataHandle &src, float minval, float maxval);

  // This sets all the values in dst to newval outside of the mask region.
  static void mask_data(NrrdDataHandle &dst, NrrdDataHandle &src,
                        NrrdDataHandle &mask, unsigned int mbit, float newval);

  static NrrdDataHandle         clear_nrrd(NrrdDataHandle &);
  static NrrdDataHandle         create_clear_nrrd(NrrdDataHandle &, 
                                                  unsigned int type = 0);
  static NrrdDataHandle         create_nrrd(NrrdDataHandle &, 
                                            unsigned int type = 0);

  // Returns true if the nrrd info is similar enough between nrrds a and b.
  // For now the similarity just means that a is a suitable label for b.
  static bool compare_nrrd_info(NrrdDataHandle &a, NrrdDataHandle &b);

  static void dice_32_into_8(NrrdDataHandle &src,
                             vector<NrrdDataHandle> &dst);

  static size_t index_to_offset(Nrrd *nrrd, const vector<int> &index)
  {
    ASSERT((unsigned int)(index.size()) == nrrd->dim);
    size_t position = index[0];
    size_t mult_factor = nrrd->axis[0].size;
    for (size_t a = 1; a < nrrd->dim; ++a) {
      position += index[a] * mult_factor;
      mult_factor *= nrrd->axis[a].size;
    }
    return position;
  }

  template <class T>
  static
  void                          nrrd_get_value(Nrrd *,
                                               const vector<int> &index,
                                               T &value);
  
  template <class T>
  static 
  void                          nrrd_set_value(Nrrd *,
                                               const vector<int> &index,
                                               T value,
                                               unsigned int mask=0);

  template <class T>
  static 
  void                          nrrd_set_value_unmasked(Nrrd *,
                                                        const vector<int> &index,
                                                        T value);

  template <class T>
  static 
  void                          nrrd_and_value_unmasked(Nrrd *,
                                                        const vector<int> &index,
                                                        T value);

  template <class T>
  static 
  void                          nrrd_or_value_unmasked(Nrrd *,
                                                        const vector<int> &index,
                                                        T value);

  static NrrdStats nrrd_statistics(NrrdDataHandle &nrrdh, NrrdDataHandle &maskh, unsigned char bitmask = 0xFF);

};


template<class T>
void
VolumeOps::nrrd_get_value(Nrrd *nrrd, 
                          const vector<int> &index, 
                          T &value)
{
  const size_t position = index_to_offset(nrrd, index);

  switch (nrrd->type) {
  case nrrdTypeChar: {
    char *slicedata = (char *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeUChar: {
    unsigned char *slicedata = (unsigned char *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeShort: {
    short *slicedata = (short *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeUShort: {
    unsigned short *slicedata = (unsigned short *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeInt: {
    int *slicedata = (int *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeUInt: {
    unsigned int *slicedata = (unsigned int *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeLLong: {
    signed long long *slicedata = (signed long long *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeULLong: {
    unsigned long long *slicedata = (unsigned long long *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeFloat: {
    float *slicedata = (float *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  case nrrdTypeDouble: {
    double *slicedata = (double *)nrrd->data;
    value = (T)slicedata[position];
  } break;
  default: {
    throw "Unsupported nrrd data type ";
  } break;
  }
}



template <class T>
void
VolumeOps::nrrd_set_value(Nrrd *nrrd,
                          const vector<int> &index, 
                          T val, 
                          unsigned int label_mask)
{
  const size_t position = index_to_offset(nrrd, index);

  switch (nrrd->type) {
  case nrrdTypeChar: {
    char *slicedata = (char *)nrrd->data;
    slicedata[position] = (char)val;
  } break;
  case nrrdTypeUChar: {
    unsigned char *slicedata = (unsigned char *)nrrd->data;
    slicedata[position] = (unsigned char)val;
    } break;
  case nrrdTypeShort: {
    short *slicedata = (short *)nrrd->data;

    slicedata[position] = (short)val;
    } break;
  case nrrdTypeUShort: {
    unsigned short *slicedata = (unsigned short *)nrrd->data;
    slicedata[position] = (unsigned short)val;
    } break;
  case nrrdTypeInt: {
    int *slicedata = (int *)nrrd->data;
    slicedata[position] = (int)val;
    } break;
  case nrrdTypeUInt: {
    unsigned int *slicedata = (unsigned int *)nrrd->data;
    if (slicedata[position] == label_mask) {
      slicedata[position] = (unsigned int)val;
    }
    } break;
  case nrrdTypeLLong: {
    signed long long *slicedata = (signed long long *)nrrd->data;
    slicedata[position] = (signed long long)val;
    } break;
  case nrrdTypeULLong: {
    unsigned long long *slicedata = (unsigned long long *)nrrd->data;
    slicedata[position] = (unsigned long long)val;
    } break;
  case nrrdTypeFloat: {
    float *slicedata = (float *)nrrd->data;
    slicedata[position] = (float)val;
    } break;
  case nrrdTypeDouble: {
    double *slicedata = (double *)nrrd->data;
    slicedata[position] = (double)val;
    } break;
  default: { 
    throw "Unsupported nrrd data type ";
    } break;
  }
}



template <class T>
void
VolumeOps::nrrd_set_value_unmasked(Nrrd *nrrd,
                                   const vector<int> &index, 
                                   T val)
{
  const size_t position = index_to_offset(nrrd, index);

  switch (nrrd->type) {
  case nrrdTypeChar: {
    char *slicedata = (char *)nrrd->data;
    slicedata[position] = (char)val;
  } break;
  case nrrdTypeUChar: {
    unsigned char *slicedata = (unsigned char *)nrrd->data;
    slicedata[position] = (unsigned char)val;
    } break;
  case nrrdTypeShort: {
    short *slicedata = (short *)nrrd->data;

    slicedata[position] = (short)val;
    } break;
  case nrrdTypeUShort: {
    unsigned short *slicedata = (unsigned short *)nrrd->data;
    slicedata[position] = (unsigned short)val;
    } break;
  case nrrdTypeInt: {
    int *slicedata = (int *)nrrd->data;
    slicedata[position] = (int)val;
    } break;
  case nrrdTypeUInt: {
    unsigned int *slicedata = (unsigned int *)nrrd->data;
    slicedata[position] = (unsigned int)val;
    } break;
  case nrrdTypeLLong: {
    signed long long *slicedata = (signed long long *)nrrd->data;
    slicedata[position] = (signed long long)val;
    } break;
  case nrrdTypeULLong: {
    unsigned long long *slicedata = (unsigned long long *)nrrd->data;
    slicedata[position] = (unsigned long long)val;
    } break;
  case nrrdTypeFloat: {
    float *slicedata = (float *)nrrd->data;
    slicedata[position] = (float)val;
    } break;
  case nrrdTypeDouble: {
    double *slicedata = (double *)nrrd->data;
    slicedata[position] = (double)val;
    } break;
  default: { 
    throw "Unsupported nrrd data type ";
    } break;
  }
}


template <class T>
void
VolumeOps::nrrd_and_value_unmasked(Nrrd *nrrd,
                                   const vector<int> &index, 
                                   T val)
{
  const size_t position = index_to_offset(nrrd, index);

  switch (nrrd->type) {
  case nrrdTypeChar: {
    char *slicedata = (char *)nrrd->data;
    slicedata[position] = (char)val;
  } break;
  case nrrdTypeUChar: {
    unsigned char *slicedata = (unsigned char *)nrrd->data;
    slicedata[position] &= (unsigned char)val;
    } break;
  case nrrdTypeShort: {
    short *slicedata = (short *)nrrd->data;

    slicedata[position] &= (short)val;
    } break;
  case nrrdTypeUShort: {
    unsigned short *slicedata = (unsigned short *)nrrd->data;
    slicedata[position] &= (unsigned short)val;
    } break;
  case nrrdTypeInt: {
    int *slicedata = (int *)nrrd->data;
    slicedata[position] &= (int)val;
    } break;
  case nrrdTypeUInt: {
    unsigned int *slicedata = (unsigned int *)nrrd->data;
    slicedata[position] &= (unsigned int)val;
    } break;
  case nrrdTypeLLong: {
    signed long long *slicedata = (signed long long *)nrrd->data;
    slicedata[position] &= (signed long long)val;
    } break;
  case nrrdTypeULLong: {
    unsigned long long *slicedata = (unsigned long long *)nrrd->data;
    slicedata[position] &= (unsigned long long)val;
    } break;
  default: { 
    throw "Unsupported nrrd data type ";
    } break;
  }
}


template <class T>
void
VolumeOps::nrrd_or_value_unmasked(Nrrd *nrrd,
                                  const vector<int> &index, 
                                  T val)
{
  const size_t position = index_to_offset(nrrd, index);

  switch (nrrd->type) {
  case nrrdTypeChar: {
    char *slicedata = (char *)nrrd->data;
    slicedata[position] |= (char)val;
  } break;
  case nrrdTypeUChar: {
    unsigned char *slicedata = (unsigned char *)nrrd->data;
    slicedata[position] |= (unsigned char)val;
    } break;
  case nrrdTypeShort: {
    short *slicedata = (short *)nrrd->data;
    slicedata[position] |= (short)val;
    } break;
  case nrrdTypeUShort: {
    unsigned short *slicedata = (unsigned short *)nrrd->data;
    slicedata[position] |= (unsigned short)val;
    } break;
  case nrrdTypeInt: {
    int *slicedata = (int *)nrrd->data;
    slicedata[position] |= (int)val;
    } break;
  case nrrdTypeUInt: {
    unsigned int *slicedata = (unsigned int *)nrrd->data;
    slicedata[position] |= (unsigned int)val;
    } break;
  case nrrdTypeLLong: {
    signed long long *slicedata = (signed long long *)nrrd->data;
    slicedata[position] |= (signed long long)val;
    } break;
  case nrrdTypeULLong: {
    unsigned long long *slicedata = (unsigned long long *)nrrd->data;
    slicedata[position] |= (unsigned long long)val;
    } break;
  default: { 
    throw "Unsupported nrrd data type ";
    } break;
  }
}


}
#endif
