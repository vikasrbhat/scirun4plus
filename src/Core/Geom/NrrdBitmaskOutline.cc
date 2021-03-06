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
//    File   : NrrdBitmaskOutline.cc
//    Author : McKay Davis
//    Date   : Mon Oct 23 18:40:04 2006


#include <sci_gl.h>
#include <Core/Datatypes/NrrdData.h>
#include <slivr/ShaderProgramARB.h>
#include <Core/Geom/NrrdBitmaskOutline.h>
#include <Core/Math/MiscMath.h>
#include <Core/Util/StringUtil.h>
#include <teem/air.h>
#include <limits.h>

#include <Core/Thread/MutexPool.h>

namespace SCIRun {


#define BITMASK(X) ((X >> *biter) & 1)


NrrdBitmaskOutline::NrrdBitmaskOutline(NrrdDataHandle &nin_handle) :
  lock("NrrdBitmaskOutline"),
  ref_cnt(0),
  nrrd_handle_(nin_handle),
  line_coords_(32),
  point_coords_(32),
  bit_verts_(32),
  dirty_(true),
  fat_lines_(true)
{
  SLIVR::ColorMap *scm = ColorMap::create_pseudo_random(201);
  colormap_ = new SCIRun::ColorMap(*scm);
  delete scm;

  for (int i = 0; i < 6; ++i) {
    bit_priority_[i] = i;
  }
  bit_priority_[6] = -1;
  resize_iso_glyphs_2D();
}


NrrdBitmaskOutline::~NrrdBitmaskOutline()
{
}


void
NrrdBitmaskOutline::set_colormap(ColorMapHandle &cmap)
{
  colormap_ = cmap;
}


void
NrrdBitmaskOutline::draw_lines(float width, unsigned int mask)
{
  const size_t xdim =  nrrd_handle_->nrrd_->axis[1].size;
  const size_t ydim =  nrrd_handle_->nrrd_->axis[2].size;
  if (!xdim*ydim) return;

  if (mask) {
    unsigned int bit = 0;
    while (!(mask & (1<<bit))) ++bit;
    bit_priority_[0] = bit;
    bit_priority_[1] = -1;
  }

  if (dirty_) {
    compute_iso_lines();
    dirty_ = false;
  }

  glLineWidth(width);
  glPointSize(width);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  const int cols = nrrd_handle_->nrrd_->axis[1].size;
  const int rows = nrrd_handle_->nrrd_->axis[2].size;
  const Vector dx = xdir_ / double(cols);
  const Vector dy = ydir_ / double(rows);

  double mat[16];
  mat[0] = dx.x();
  mat[1] = dx.y();
  mat[2] = dx.z();
  mat[3] = 0.0;
  mat[4] = dy.x();
  mat[5] = dy.y();
  mat[6] = dy.z();
  mat[7] = 0.0;
  mat[8] = 0.0;
  mat[9] = 0.0;
  mat[10] = 1.0;
  mat[11] = 0.0;
  mat[12] = min_.x();
  mat[13] = min_.y();
  mat[14] = min_.z();
  mat[15] = 1.0;

  glMultMatrixd(mat);

  const float *rgba = colormap_->get_rgba();
  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    glColor4fv(rgba+4*(*biter+1));

    verts_t &lines = line_coords_[*biter];
    if (!lines.empty()) {
      glVertexPointer(2, GL_FLOAT, 0, &(lines.front()));
      glEnableClientState(GL_VERTEX_ARRAY);
      if (fat_lines_)
      {
        glDrawArrays(GL_LINES, 0, lines.size()/2);
      }
      else
      {
        glDrawArrays(GL_QUADS, 0, lines.size()/2);
      }
    }

    verts_t &points = point_coords_[*biter];
    if (!points.empty()) {
      glVertexPointer(2, GL_FLOAT, 0, &(points.front()));
      glEnableClientState(GL_VERTEX_ARRAY);
      glDrawArrays(GL_POINTS, 0, points.size()/2);
    }
  }

  glPopMatrix();

  glLineWidth(1);
  glPointSize(1);
}

void
NrrdBitmaskOutline::compute_iso_lines()
{
  switch (nrrd_handle_->nrrd_->type)
  {
  case nrrdTypeUChar:
    if (fat_lines_)
    {
      compute_iso_lines_fatT<unsigned char>();
    }
    else
    {
      compute_iso_lines_thinT<unsigned char>();
    }
    break;

  case nrrdTypeUInt:
    if (fat_lines_)
    {
      compute_iso_lines_fatT<unsigned int>();
    }
    else
    {
      compute_iso_lines_thinT<unsigned int>();
    }
    break;

  default:
    ASSERTFAIL("Not implemented.");
  }
}


template <class T>
void
NrrdBitmaskOutline::compute_iso_lines_fatT()
{
  const int cols = nrrd_handle_->nrrd_->axis[1].size;
  const int rows = nrrd_handle_->nrrd_->axis[2].size;

  const int x1 = 0;
  const int x2 = cols;
  const int y1 = 0;
  const int y2 = rows;

  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    line_coords_[*biter].clear();
    point_coords_[*biter].clear();
  }

  T *src = (T *)nrrd_handle_->nrrd_->data;

  // Grab the right set of verts
  verts_t &verts = line_coords_[bit_priority_[0]];
  verts_t &points = point_coords_[bit_priority_[0]];

  const unsigned int mask = 1 << bit_priority_[0];

  for (int j = y1; j <= y2; j++)
  {
    for (int i = x1; i <= x2; i++)
    {
      const bool i0j0 =
        (i < x2) && (j < y2) && (src[i+j*cols] & mask);
      const bool i1j0 =
        ((i-1) >= x1) && (j < y2) && (src[(i-1)+j*cols] & mask);
      const bool i1j1 =
        ((i-1) >= x1) && ((j-1) >= y1) && (src[(i-1)+(j-1)*cols] & mask);
      const bool i0j1 =
        (i < x2) && ((j-1) >= y1) && (src[i+(j-1)*cols] & mask);

      if (i0j0 != i1j0)
      {
        verts.push_back(i);
        verts.push_back(j);
        verts.push_back(i);
        verts.push_back(j+1);
      }
      if (i0j0 != i0j1)
      {
        verts.push_back(i);
        verts.push_back(j);
        verts.push_back(i+1);
        verts.push_back(j);
      }
      if (!(i0j0 == i1j0 && i0j1 == i1j1 ||
            i0j0 == i0j1 && i1j0 == i1j1))
      {
        points.push_back(i);
        points.push_back(j);
      }
    }
  }
}



template <class T>
void
NrrdBitmaskOutline::compute_iso_lines_thinT()
{
  const float off0 = 1.0/3.0;
  const float off1 = 1.0 - off0;

  const int cols = nrrd_handle_->nrrd_->axis[1].size;
  const int rows = nrrd_handle_->nrrd_->axis[2].size;

  const int x1 = 0;
  const int x2 = cols;
  const int y1 = 0;
  const int y2 = rows;

  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    line_coords_[*biter].clear();
    point_coords_[*biter].clear();
  }

  T *src = (T *)nrrd_handle_->nrrd_->data;

  // Grab the right set of verts
  verts_t &verts = line_coords_[bit_priority_[0]];

  const unsigned int mask = 1 << bit_priority_[0];

  for (int j = y1; j <= y2; j++)
  {
    for (int i = x1; i <= x2; i++)
    {
      const bool i0j0 =
        (i < x2) && (j < y2) && (src[i+j*cols] & mask);
      const bool i1j0 =
        ((i-1) >= x1) && (j < y2) && (src[(i-1)+j*cols] & mask);
      const bool i1j1 =
        ((i-1) >= x1) && ((j-1) >= y1) && (src[(i-1)+(j-1)*cols] & mask);
      const bool i0j1 =
        (i < x2) && ((j-1) >= y1) && (src[i+(j-1)*cols] & mask);

      const int count = i0j0 + i1j0 + i1j1 + i0j1;

      if (i0j0 && count < 4)
      {
        verts.push_back(i);
        verts.push_back(j);

        verts.push_back(i);
        verts.push_back(j + off0);

        verts.push_back(i + off0);
        verts.push_back(j + off0);

        verts.push_back(i + off0);
        verts.push_back(j);
      }
      if (i1j0 && count < 4)
      {
        verts.push_back(i);
        verts.push_back(j);

        verts.push_back(i);
        verts.push_back(j + off0);

        verts.push_back(i - off0);
        verts.push_back(j + off0);

        verts.push_back(i - off0);
        verts.push_back(j);
      }
      if (i1j1 && count < 4)
      {
        verts.push_back(i);
        verts.push_back(j);

        verts.push_back(i);
        verts.push_back(j - off0);

        verts.push_back(i - off0);
        verts.push_back(j - off0);

        verts.push_back(i - off0);
        verts.push_back(j);
      }
      if (i0j1 && count < 4)
      {
        verts.push_back(i);
        verts.push_back(j);

        verts.push_back(i);
        verts.push_back(j - off0);

        verts.push_back(i + off0);
        verts.push_back(j - off0);

        verts.push_back(i + off0);
        verts.push_back(j);
      }
      if (i0j0 && !i1j0)
      {
        verts.push_back(i);
        verts.push_back(j + off0);
        
        verts.push_back(i + off0);
        verts.push_back(j + off0);
        
        verts.push_back(i + off0);
        verts.push_back(j + off1);
        
        verts.push_back(i);
        verts.push_back(j + off1);
      }
      if (i0j0 && !i0j1)
      {
        verts.push_back(i + off0);
        verts.push_back(j);
        
        verts.push_back(i + off1);
        verts.push_back(j);
        
        verts.push_back(i + off1);
        verts.push_back(j + off0);
        
        verts.push_back(i + off0);
        verts.push_back(j + off0);
      }
      if (i1j1 && !i0j1)
      {
        verts.push_back(i);
        verts.push_back(j - off0);
        
        verts.push_back(i - off0);
        verts.push_back(j - off0);
        
        verts.push_back(i - off0);
        verts.push_back(j - off1);
        
        verts.push_back(i);
        verts.push_back(j - off1);
      }
      if (i1j1 && !i1j0)
      {
        verts.push_back(i - off0);
        verts.push_back(j);
        
        verts.push_back(i - off1);
        verts.push_back(j);
        
        verts.push_back(i - off1);
        verts.push_back(j - off0);
        
        verts.push_back(i - off0);
        verts.push_back(j - off0);
      }
    }
  }
}


void
NrrdBitmaskOutline::compute_iso_glyphs_2D(int x1, int y1,
                                          int x2, int y2, int b)
{
  switch (nrrd_handle_->nrrd_->type)
  {
  case nrrdTypeUChar:
    compute_iso_glyphs_2DT<unsigned char>(x1, y1, x2, y2, b);
    break;

  case nrrdTypeUInt:
    compute_iso_glyphs_2DT<unsigned int>(x1, y1, x2, y2, b);
    break;

  default:
    ASSERTFAIL("Not implemented.");
  }
}


template <class T>
void
NrrdBitmaskOutline::compute_iso_glyphs_2DT(int x1, int y1, int x2, int y2,
                                           int border)
{
  return;
  size_t cols = nrrd_handle_->nrrd_->axis[1].size;
  size_t rows = nrrd_handle_->nrrd_->axis[2].size;

  if (x1 == -1 && y1 == -1 && x1 == -1 && y1 == -1 && border == -1) {
    x1 = 0;
    y1 = 0;
    x2 = cols;
    y2 = rows;
    border = 1;
  }

  if (x1 > x2) std::swap(x1,x2);
  if (y1 > y2) std::swap(y1,y2);

  x1 = Clamp(x1-border,   0, cols);
  x2 = Clamp(x2+border+1, 0, cols);
  y1 = Clamp(y1-border,   0, rows);
  y2 = Clamp(y2+border+1, 0, rows);


  const int row_width = nrrd_handle_->nrrd_->axis[1].size;
  //const int region_start = row_width * y1 + x1;
  //  const int region_width = x2 - x1;
  //const int region_height = y2 - y1;

  const int max_pos = (nrrd_handle_->nrrd_->axis[1].size *
                       nrrd_handle_->nrrd_->axis[2].size);

  T *src = (T *)nrrd_handle_->nrrd_->data;
  T neighbors[8];
  T nbit[8];
  int pos2;
  for (int row = y1; row < y2; ++row) {
    int start_pos = row*row_width+x1;
    for (int col = x1; col < x2; ++ col) {
      int pos = row*row_width+col;

      const T &val = src[pos];

      if (!val) {
        for (signed char *biter = bit_priority_; *biter >= 0; ++biter)
          for (unsigned int i=0;i < NUM_DIRS; ++i)
            isoglyphs2D_[*biter][i][row][col] = false;
        continue;
      }

      // going in scanline order,

      // SW
      pos2 = pos - row_width-1;
      neighbors[SW] = (pos2 < 0 || pos2 <= start_pos-row_width) ?
        0 : src[pos2];
      // South
      pos2 = pos - row_width;
      neighbors[S] = (pos2 < 0) ? 0 : src[pos2];

      // SE
      pos2 = pos - row_width+1;
      neighbors[SE] = (pos2 < 0 || pos2 >= start_pos) ? 0 : src[pos2];

      // West
      pos2 = pos-1;
      neighbors[W] = (pos2 < start_pos) ? 0 : src[pos2];

      // East
      pos2 = pos+1;
      neighbors[E] = (pos2 > start_pos+row_width) ? 0 : src[pos2];

      // NW
      pos2 = pos + row_width-1;
      neighbors[NW] = (pos2 > max_pos || pos2 < start_pos+row_width) ?
        0 : src[pos2];
      // North
      pos2 = pos + row_width;
      neighbors[N] = (pos2 > max_pos) ? 0 : src[pos2];

      // NE
      pos2 = pos + row_width+1;
      neighbors[NE] = (pos2 > max_pos || pos2 >= start_pos+row_width*2) ?
        0 : src[pos2];

      // Before we iterate through each bit in the value we're looking at,
      // Check to see if we are surrounded by neighbors
      // with exact same bit patterns,
      // this means there are no edge glyphs whatsoever for this pixel,
      // so we set them to be false

//       if (val == neighbors[SW] && val == neighbors[S] &&
//           val == neighbors[SE] && val == neighbors[W] &&
//           val == neighbors[E] && val == neighbors[NW] &&
//           val == neighbors[N] && val == neighbors[NE])
//       {
//         for (signed char *biter = bit_priority; *biter >= 0; ++biter)
//           for (cardinal_t  dir = SW; dir <= NE; ++dir)
//             isoglyphs_[*biter][dir][row][col] = false;
//         continue;
//       }

      for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
        // If this bit is not set at this pixel, then continue
        signed char bit = *biter;

        isoglyphs2D_[bit][C][row][col] = BITMASK(val);

        for (int dir = SW; dir <= NE; ++dir)
          nbit[dir] = BITMASK(neighbors[dir]);

        // South-West corner, point
        isoglyphs2D_[bit][SW][row][col] = !nbit[S] || !nbit[W] || !nbit[SW];

        // South line
        isoglyphs2D_[bit][S][row][col] = !nbit[S];

        // SouthEast point
        isoglyphs2D_[bit][SE][row][col] = !nbit[S] || !nbit[E] || !nbit[SE];

        // West Line
        isoglyphs2D_[bit][W][row][col] = !nbit[W];

        // East Line
        isoglyphs2D_[bit][E][row][col] = !nbit[E];

        // North-West corner, point
        isoglyphs2D_[bit][NW][row][col] = !nbit[N] || !nbit[W] || !nbit[NW];

        // North line
        isoglyphs2D_[bit][N][row][col] = !nbit[N];

        // NorthEast point
        isoglyphs2D_[bit][NE][row][col] = !nbit[N] || !nbit[E] || !nbit[NE];
      }
    }
  }
}


void
NrrdBitmaskOutline::resize_iso_glyphs_2D()
{
  return;
  signed char maxbit = -1;
  for (signed char *biter = bit_priority_; *biter >= 0; ++biter)
    maxbit = *biter;

  size_t cols = nrrd_handle_->nrrd_->axis[1].size;
  size_t rows = nrrd_handle_->nrrd_->axis[2].size;

  isoglyphs2D_.resize(maxbit+1);
  isoglyphs_.resize(maxbit+1);
  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    isoglyphs2D_[*biter].resize(NUM_DIRS);
    isoglyphs_[*biter].resize(NUM_DIRS);
    for (int dir = SW; dir <= C; ++dir) {
      std::vector<std::vector<bool> > &isoglyphs2D = isoglyphs2D_[*biter][dir];
      isoglyphs2D.resize(rows);
      for (unsigned int row = 0; row < rows; ++row) {
        isoglyphs2D[row].resize(cols);
      }
    }
  }
}


void
NrrdBitmaskOutline::compute_iso_glyphs(int x1, int y1, int x2, int y2, int border)
{
  return;
  size_t cols = nrrd_handle_->nrrd_->axis[1].size;
  size_t rows = nrrd_handle_->nrrd_->axis[2].size;
  bool clear = false;
  if (x1 == -1 && y1 == -1 && x1 == -1 && y1 == -1 && border == -1) {
    x1 = 0;
    y1 = 0;
    x2 = cols;
    y2 = rows;
    border = 1;
    clear = true;
  }

  if (x1 > x2) std::swap(x1,x2);
  if (y1 > y2) std::swap(y1,y2);

  x1 = Clamp(x1-border,   0, cols);
  x2 = Clamp(x2+border+1, 0, cols);
  y1 = Clamp(y1-border,   0, rows);
  y2 = Clamp(y2+border+1, 0, rows);

  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    for (int  dir = SW; dir <= C; ++dir) {
      std::vector<std::vector<bool> > &isoglyphs2D = isoglyphs2D_[*biter][dir];
      std::vector<std::pair<size_t, size_t> > &isoglyphs = isoglyphs_[*biter][dir];
      if (clear) isoglyphs.clear();
      for (int row = y1; row < y2; ++row) {
        for (int col = x1; col < x2; ++col) {
          if (isoglyphs2D[row][col]) {
            isoglyphs.push_back(std::make_pair(col, row));
          }
        }
      }
    }
  }
}


void
NrrdBitmaskOutline::draw_glyphs(float sso, float width)
{
  const size_t xdim =  nrrd_handle_->nrrd_->axis[1].size;
  const size_t ydim =  nrrd_handle_->nrrd_->axis[2].size;
  if (!xdim && !ydim) return;


  if (dirty_) {
    compute_iso_glyphs_2D(0,0, xdim, ydim, 0);
    compute_iso_glyphs();
    dirty_ = false;
  }

  Vector dx = xdir_ / double(xdim);
  Vector dy = ydir_ / double(ydim);

  width_ = width;
  sso_ = sso;

  //  glMatrixMode(GL_MODELVIEW);
  //  glScaled(dx.x(), dx.y(), dx.z());
  //  glScaled(dy.x(), dy.y(), dy.z());


  Point v;
  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    for (int dir = SW; dir <= C; ++dir) {
      if (dir != C) continue;
      std::vector<std::pair<size_t, size_t> > &isoglyphs = isoglyphs_[*biter][dir];
      size_t nglyphs = isoglyphs.size();
      glColor4d(1.0, 0.0, 0.0, 1.0);
      glPointSize(width);
      glPointSize(2.0);
      glBegin(GL_POINTS);
      for (size_t i = 0; i < nglyphs; ++i) {
        v = min_ + (float(isoglyphs[i].first + 0.5) * dx +
                    float(isoglyphs[i].second + 0.5) * dy);
        glVertex3dv(&(v(0)));
      }
      glEnd();
    }
  }
}


void
NrrdBitmaskOutline::get_bounds(BBox &bbox)
{
  bbox.extend(min_);
  bbox.extend(min_+xdir_);
  bbox.extend(min_+ydir_);
}


void
NrrdBitmaskOutline::set_line_style(bool fat)
{
  if (fat != fat_lines_)
  {
    fat_lines_ = fat;
    set_dirty();
  }
}


void
NrrdBitmaskOutline::set_coords(const Point &min,
                               const Vector &xdir,
                               const Vector &ydir)
{
  min_ = min;
  xdir_ = xdir;
  ydir_ = ydir;
}


void
NrrdBitmaskOutline::draw_quads(float sso, float width)
{
  const size_t xdim =  nrrd_handle_->nrrd_->axis[1].size;
  const size_t ydim =  nrrd_handle_->nrrd_->axis[2].size;
  if (!xdim && !ydim) return;

  const float *rgba = colormap_->get_rgba();

  width_ = width;
  sso_ = sso;

  if (dirty_) {
    compute_iso_quads();
    dirty_ = false;
  }

  Vector dx = xdir_;
  Vector dy = ydir_;
  dx.normalize();
  dy.normalize();

  Point ll, lr, ul, ur;
  for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
    verts_t &verts = bit_verts_[*biter];
    if (verts.empty()) continue;
    glBegin(GL_QUADS);
    glColor4fv(rgba+4*(*biter+1));
    for (unsigned int v = 0; v < verts.size(); v+= 4) {
      ll = min_ + verts[v]   * dx + verts[v+1] * dy;
      lr = min_ + verts[v+2] * dx + verts[v+1] * dy;
      ur = min_ + verts[v+2] * dx + verts[v+3] * dy;
      ul = min_ + verts[v]   * dx + verts[v+3] * dy;
      glVertex3dv(&(ll(0)));
      glVertex3dv(&(lr(0)));
      glVertex3dv(&(ur(0)));
      glVertex3dv(&(ul(0)));
    }
    glEnd();
  }
}


void
NrrdBitmaskOutline::compute_iso_quads(int x1, int y1, int x2, int y2, int b)
{
  switch (nrrd_handle_->nrrd_->type)
  {
  case nrrdTypeUChar:
    compute_iso_quadsT<unsigned char>(x1, y1, x2, y2, b);
    break;

  case nrrdTypeUInt:
    compute_iso_quadsT<unsigned int>(x1, y1, x2, y2, b);
    break;

  default:
    ASSERTFAIL("Not implemented.");
  }
}


template <class T>
void
NrrdBitmaskOutline::compute_iso_quadsT(int x1, int y1, int x2, int y2,
                                       int border)
{

  size_t cols = nrrd_handle_->nrrd_->axis[1].size;
  size_t rows = nrrd_handle_->nrrd_->axis[2].size;

  if (x1 == -1 && y1 == -1 && x1 == -1 && y1 == -1 && border == -1) {
    x1 = 0;
    y1 = 0;
    x2 = cols;
    y2 = rows;
    border = 0;
    for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
      bit_verts_[*biter].clear();
    }
  }

  if (x1 > x2) std::swap(x1,x2);
  if (y1 > y2) std::swap(y1,y2);
  x1 = Clamp(x1-border,   0, cols);
  x2 = Clamp(x2+border+1, 0, cols);
  y1 = Clamp(y1-border,   0, rows);
  y2 = Clamp(y2+border+1, 0, rows);


  float col_scale = (xdir_/float(cols)).length();
  float row_scale = (ydir_/float(rows)).length();
  float isowidth = sso_*width_;
  float cisowidth = Clamp(isowidth, 0.0f, col_scale);
  float risowidth = Clamp(isowidth, 0.0f, row_scale);

  const int row_width = nrrd_handle_->nrrd_->axis[1].size;
  const int region_start = row_width * y1 + x1;
  const int region_width = x2 - x1;
  const int region_height = y2 - y1;

  const int max_pos = (nrrd_handle_->nrrd_->axis[1].size *
                       nrrd_handle_->nrrd_->axis[2].size);

  T *src = (T *)nrrd_handle_->nrrd_->data;

  T neighbors[8];
  T nbit[8];
  int pos2;
  for (int row = 0; row < region_height; ++row) {
    int start_pos = region_start + row*row_width;
    int end_pos = start_pos + region_width;
    int col = region_start;
    for (int pos = start_pos; pos < end_pos; ++pos, ++col) {
      const T &val = src[pos];
      if (!val) continue;
      // going in scanline order, first we do the south neighbor
      pos2 = pos - row_width;
      neighbors[0] = (pos2 < 0) ? 0 : src[pos2];
      // then the west neighbor
      pos2 = pos-1;
      neighbors[1] = (pos2 < start_pos) ? 0 : src[pos2];
      // east neighbor
      pos2 = pos+1;
      neighbors[2] = (pos2 > start_pos+row_width) ? 0 : src[pos2];
      // last, the north neighbor
      pos2 = pos + row_width;
      neighbors[3] = (pos2 > max_pos) ? 0 : src[pos2];

      // SW
      pos2 = pos - row_width-1;
      neighbors[4] = (pos2 < 0 || pos2 <= start_pos-row_width) ? 0 : src[pos2];

      // SE
      pos2 = pos - row_width+1;
      neighbors[5] = (pos2 < 0 || pos2 >= start_pos) ? 0 : src[pos2];

      // NW
      pos2 = pos + row_width-1;
      neighbors[6] = (pos2 > max_pos || pos2 < start_pos+row_width) ?
        0 : src[pos2];

      // NE
      pos2 = pos + row_width+1;
      neighbors[7] = (pos2 > max_pos || pos2 >= start_pos+row_width*2) ?
        0 : src[pos2];

      // If we are surrounded by neighbors with exact same bit patterns,
      // then there nothing to do for this pixel, so we continue
      if (val == neighbors[0] && val == neighbors[1] &&
          val == neighbors[2] && val == neighbors[3] &&
          val == neighbors[4] && val == neighbors[5] &&
          val == neighbors[6] && val == neighbors[7]) continue;

      for (signed char *biter = bit_priority_; *biter >= 0; ++biter) {
        // If this bit is not set at this pixel, then continue
        if (!BITMASK(val)) continue;
        // Since it is, we need to see if bit is set in our neighbors too...
        nbit[0] = BITMASK(neighbors[0]);
        nbit[1] = BITMASK(neighbors[1]);
        nbit[2] = BITMASK(neighbors[2]);
        nbit[3] = BITMASK(neighbors[3]);

        nbit[4] = BITMASK(neighbors[4]);
        nbit[5] = BITMASK(neighbors[5]);
        nbit[6] = BITMASK(neighbors[6]);
        nbit[7] = BITMASK(neighbors[7]);

        // If we are surrounded by alike neighbors
        // no need for contour in this pixel
        if (nbit[0] && nbit[1] && nbit[2] && nbit[3] &&
            nbit[4] && nbit[5] && nbit[6] && nbit[7]) continue;

        // At this point, we know our bit is set
        // and one of our neighbors bits is not,
        // this means we have to draw an isoline!

        // Grab the right set of verts
        std::vector<float> &verts = bit_verts_[*biter];

        // First, south bit is off, create border poly
        if (!nbit[0]) {
          verts.push_back(col*col_scale+cisowidth);
          verts.push_back(row*row_scale);
          verts.push_back((col+1)*col_scale-cisowidth);
          verts.push_back(row*row_scale+isowidth);
        }

        // west bit is off, create border poly
        if (!nbit[1]) {
          verts.push_back(col*col_scale);
          verts.push_back(row*row_scale+risowidth);
          verts.push_back(col*col_scale+isowidth);
          verts.push_back((row+1)*row_scale-risowidth);
        }

        // east bit is off, create border poly
        if (!nbit[2]) {
          verts.push_back((col+1)*col_scale-isowidth);
          verts.push_back(row*row_scale+risowidth);
          verts.push_back((col+1)*col_scale);
          verts.push_back((row+1)*row_scale-risowidth);
        }

        // north bit is off, create border poly
        if (!nbit[3]) {
          verts.push_back(col*col_scale+cisowidth);
          verts.push_back((row+1)*row_scale-isowidth);
          verts.push_back((col+1)*col_scale-cisowidth);
          verts.push_back((row+1)*row_scale);
        }

        // South-West corner
        if (!nbit[0] || !nbit[1] || !nbit[4]) {
          verts.push_back(col*col_scale);
          verts.push_back(row*row_scale);
          verts.push_back(col*col_scale+cisowidth);
          verts.push_back(row*row_scale+risowidth);
        }

        // South-east corner
        if (!nbit[0] || !nbit[2] || !nbit[5]) {
          verts.push_back((col+1)*col_scale-cisowidth);
          verts.push_back(row*row_scale);
          verts.push_back((col+1)*col_scale);
          verts.push_back(row*row_scale+risowidth);
        }

        // NorthWest corner
        if (!nbit[1] || !nbit[3] || !nbit[6]) {
          verts.push_back(col*col_scale);
          verts.push_back((row+1)*row_scale-risowidth);
          verts.push_back(col*col_scale+cisowidth);
          verts.push_back((row+1)*row_scale);
        }

        // NorthEast corner
        if (!nbit[2] || !nbit[3] || !nbit[7]) {
          verts.push_back((col+1)*col_scale-cisowidth);
          verts.push_back((row+1)*row_scale-risowidth);
          verts.push_back((col+1)*col_scale);
          verts.push_back((row+1)*row_scale);
        }
      }
    }
  }
}


}
