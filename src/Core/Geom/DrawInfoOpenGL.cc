/*
  For more information, please see: http://software.sci.utah.edu

  The MIT License

  Copyright (c) 2009 Scientific Computing and Imaging Institute,
  University of Utah.

  
  Permission is hereby granted, free of charge, to any person obtaining a
  copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.
*/


/*
 *  DrawInfoOpenGL.cc: OpenGL State Machine
 *
 *  Written by:
 *   McKay Davis
 *   Scientific Computing and Imaging Institute
 *   University of Utah
 *   November, 2005
 *
 */

#include <sci_gl.h>
#include <sci_glx.h>

#include <Core/Geom/DrawInfoOpenGL.h>
#include <Core/Geom/GeomDL.h>
#include <Core/Datatypes/Material.h>
#include <Core/Math/MiscMath.h>

#include <iostream>

#include <sci_defs/x11_defs.h>

#if defined(HAVE_X11)
#  include <X11/X.h>
#  include <X11/Xlib.h>
#elif defined(_WIN32)
#  include <windows.h>
#endif

using std::map;
using std::cerr;
using std::endl;
using std::pair;

namespace SCIRun {

GLvoid
quad_error(GLenum code)
{
//  cerr << "WARNING: Quadric Error (" << (char*)gluErrorString(code) << ")" << endl;
}


DrawInfoOpenGL::DrawInfoOpenGL() :
  polycount_(0),
  lighting_(1),
  show_bbox_(0),
  currently_lit_(1),
  pickmode_(1),
  pickchild_(0),
  npicks_(0),
  fog_(0),
  cull_(0),
  display_list_p_(0),
  mouse_action_(0),
  check_clip_(1),
  clip_planes_(0),
  current_matl_(0),
  ignore_matl_(0),
  qobj_(0),
  axis_(0),
  dir_(0),
  ambient_scale_(1.0),
  diffuse_scale_(1.0),
  specular_scale_(1.0),
  emission_scale_(1.0),
  shininess_scale_(1.0),
  point_size_(1.0),
  line_width_(1.0),
  polygon_offset_factor_(0.0),
  polygon_offset_units_(0.0),
  using_cmtexture_(0),
  cmtexture_(0),
  drawtype_(Gouraud)
{
  for (int i=0; i < GEOM_FONT_COUNT; i++)
  {
    fontstatus_[i] = 0;
    fontbase_[i] = 0;
  }

  qobj_=gluNewQuadric();

  if ( !qobj_ )
  {
    printf( "Error in GeomOpenGL.cc: DrawInfoOpenGL(): gluNewQuadric()\n" );
  }

#if defined(_WIN32)
  gluQuadricCallback(qobj_, /* FIX (GLenum)GLU_ERROR*/ 0, (void (__stdcall*)())quad_error);
#elif defined(__APPLE__) && !defined(APPLE_LEOPARD)
  // TODO: see if this still applies to more recent OS X versions...
  gluQuadricCallback(qobj_, (GLenum)GLU_ERROR, (GLvoid (*)(...))quad_error);
#else
  gluQuadricCallback(qobj_, (GLenum)GLU_ERROR, (void(*)())quad_error);
#endif
}


void
DrawInfoOpenGL::reset()
{
  polycount_ = 0;
  current_matl_ = 0;
  ignore_matl_ = 0;
  fog_ = 0;
  cull_ = 0;
  check_clip_ = 0;
  pickmode_ = 0;
  pickchild_ = 0;
  npicks_ = 0;
}


DrawInfoOpenGL::~DrawInfoOpenGL()
{
  map<GeomDL *, pair<unsigned int, unsigned int> >::iterator loc;
  loc = dl_map_.begin();
  while (loc != dl_map_.end())
  {
    (*loc).first->dl_unregister(this);
    ++loc;
  }
}


DrawInfoOpenGL::DrawType
DrawInfoOpenGL::get_drawtype()
{
  return drawtype_;
}

void
DrawInfoOpenGL::set_drawtype(DrawType dt)
{
  drawtype_ = dt;
  switch(drawtype_)
  {
  case DrawInfoOpenGL::WireFrame:
    gluQuadricDrawStyle(qobj_, (GLenum)GLU_LINE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    break;
  case DrawInfoOpenGL::Flat:
    gluQuadricDrawStyle(qobj_, (GLenum)GLU_FILL);
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT_AND_BACK,(GLenum)GL_FILL);
    break;
  case DrawInfoOpenGL::Gouraud:
    gluQuadricDrawStyle(qobj_, (GLenum)GLU_FILL);
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK,(GLenum)GL_FILL);
    break;
  }
}


void
DrawInfoOpenGL::init_lighting(int use_light)
{
  if (use_light)
  {
    glEnable(GL_LIGHTING);
    switch(drawtype_)
    {
    case DrawInfoOpenGL::WireFrame:
      gluQuadricNormals(qobj_, (GLenum)GLU_SMOOTH);
      break;
    case DrawInfoOpenGL::Flat:
      gluQuadricNormals(qobj_, (GLenum)GLU_FLAT);
      break;
    case DrawInfoOpenGL::Gouraud:
      gluQuadricNormals(qobj_, (GLenum)GLU_SMOOTH);
      break;
    }
  }
  else
  {
    glDisable(GL_LIGHTING);
    gluQuadricNormals(qobj_,(GLenum)GLU_NONE);
  }
  if (fog_)
    glEnable(GL_FOG);
  else
    glDisable(GL_FOG);
  if (cull_)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
}



void
DrawInfoOpenGL::init_clip(void)
{
  for (int num = 0; num < 6; num++) 
  {
    GLdouble plane[4];
    planes_[num].get(plane); 
    glClipPlane((GLenum)(GL_CLIP_PLANE0+num),plane); 
    if (check_clip_ && clip_planes_ & (1 << num))
      glEnable((GLenum)(GL_CLIP_PLANE0+num));
    else 
      glDisable((GLenum)(GL_CLIP_PLANE0+num));
  }
}


void
DrawInfoOpenGL::set_material(Material* matl)
{
  if (matl==current_matl_ || ignore_matl_)
  {
    return;     
  }
  float color[4];
  (matl->ambient*ambient_scale_).get_color(color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
  (matl->diffuse*diffuse_scale_).get_color(color);
  if (matl->transparency < 1.0)
  {
    color[3] = matl->transparency;
  }
  glColor4fv(color);
  (matl->specular*specular_scale_).get_color(color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, color);
  (matl->emission*emission_scale_).get_color(color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, color);
  if (!current_matl_ || matl->shininess != current_matl_->shininess)
  {
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matl->shininess*shininess_scale_);
  }     
  current_matl_=matl;
}


bool
DrawInfoOpenGL::dl_lookup(GeomDL *obj, unsigned int &state, unsigned int &dl)
{
  map<GeomDL *, pair<unsigned int, unsigned int> >::iterator loc;
  loc = dl_map_.find(obj);
  if (loc != dl_map_.end())
  {
    dl = (*loc).second.first;
    state = (*loc).second.second;
    return true;
  }
  return false;
}


bool
DrawInfoOpenGL::dl_addnew(GeomDL *obj, unsigned int state, unsigned int &dl)
{
  if (!dl_freelist_.empty())
  {
    dl = dl_freelist_.front();
    dl_freelist_.pop_front();
  }
  else
  {
    dl = glGenLists(1);
  }
  if (dl)
  {
    dl_map_[obj] = pair<unsigned int, unsigned int>(dl, state);
    obj->dl_register(this);
    return true;
  }
  return false;
}


bool
DrawInfoOpenGL::dl_update(GeomDL *obj, unsigned int state)
{
  map<GeomDL *, pair<unsigned int, unsigned int> >::iterator loc;
  loc = dl_map_.find(obj);
  if (loc != dl_map_.end())
  {
    (*loc).second.second = state;
    return true;
  }
  return false;
}


bool
DrawInfoOpenGL::dl_remove(GeomDL *obj)
{
  map<GeomDL *, pair<unsigned int, unsigned int> >::iterator loc;
  loc = dl_map_.find(obj);
  if (loc != dl_map_.end())
  {
    dl_freelist_.push_front((*loc).second.first);
    dl_map_.erase(loc);
    return true;
  }
  return false;
}


// this is for transparent rendering stuff.

void
DrawInfoOpenGL::init_view( double /*znear*/, double /*zfar*/,
                           Point& /*eyep*/, Point& /*lookat*/ )
{
}

}
