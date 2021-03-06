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
 *  BoxWidget.h
 *
 *  Written by:
 *   James Purciful
 *   Department of Computer Science
 *   University of Utah
 *   Jan. 1995
 *
 */

#ifndef SCI_project_ScaledBox_Widget_h
#define SCI_project_ScaledBox_Widget_h 1

#include <Dataflow/Widgets/BaseWidget.h>
#include <Core/Datatypes/Clipper.h>

#include <Dataflow/Widgets/share.h>

namespace SCIRun {

class SCISHARE BoxWidget : public BaseWidget {
public:
  BoxWidget( Module* module, CrowdMonitor* lock, double widget_scale,
	     bool is_aligned = false, bool is_slideable = false );
  BoxWidget( const BoxWidget& );
  virtual ~BoxWidget();

  virtual void redraw();
  virtual void geom_moved(GeomPickHandle, int, double, const Vector&, int,
			  const BState&, const Vector &pick_offset);
  virtual void geom_pick(GeomPickHandle, ViewWindow*, int, const BState& bs);

  virtual void MoveDelta( const Vector& delta );
  virtual Point ReferencePoint() const;

  void SetPosition( const Point& center, const Point& R, const Point& D,
		    const Point& I );
  void GetPosition( Point& center, Point& R, Point& D, Point& I );

  void SetRatioR( const double ratio );
  double GetRatioR() const;
  void SetRatioD( const double ratio );
  double GetRatioD() const;
  void SetRatioI( const double ratio );
  double GetRatioI() const;

  void RestrictTranslationXYZ()
    { restrict_translation_rdi_ = false; 
      restrict_translation_xyz_ = true; 
      execute(0); }
  void SetRestrictX( const bool val )
    { restrict_x_ = val; execute(0); }
  void SetRestrictY( const bool val )
    { restrict_y_ = val; execute(0); }
  void SetRestrictZ( const bool val )
    { restrict_z_ = val; execute(0); }
  void RestrictTranslationRDI()
    { restrict_translation_rdi_ = true; 
      restrict_translation_xyz_ = false; 
      execute(0); }
  void SetRestrictR( const bool val )
    { restrict_r_ = val; execute(0); }
  void SetRestrictD( const bool val )
    { restrict_d_ = val; execute(0); }
  void SetRestrictI( const bool val )
    { restrict_i_ = val; execute(0); }
  void UnRestrictTranslation()
    { restrict_translation_rdi_ = false; 
      restrict_translation_xyz_ = false; 
      execute(0); }

  const Vector& GetRightAxis();
  const Vector& GetDownAxis();
  const Vector& GetInAxis();

  bool IsAxisAligned() const { return is_aligned_; }
  bool IsSlideable() const { return is_slideable_; }

  // Variable indexs
  enum { CenterVar, PointRVar, PointDVar, PointIVar,
	 DistRVar, DistDVar, DistIVar, HypoRDVar, HypoDIVar, HypoIRVar,
	 SDistRVar, RatioRVar, SDistDVar, RatioDVar, SDistIVar, RatioIVar,
	 NumVars };

  // Material indexs
  enum { PointMatl, EdgeMatl, ResizeMatl, SliderMatl, NumMatls };

  ClipperHandle get_clipper();

protected:
  virtual std::string GetMaterialName( const Index mindex ) const;   
   
private:
  bool is_aligned_;
  bool is_slideable_;

  bool restrict_translation_xyz_;
  bool restrict_x_;
  bool restrict_y_;
  bool restrict_z_;
  
  bool restrict_translation_rdi_;
  bool restrict_r_;
  bool restrict_d_;
  bool restrict_i_;

  Vector oldrightaxis, olddownaxis, oldinaxis;
  Point pick_centervar_;
  Point pick_pointrvar_;
  Point pick_pointdvar_;
  Point pick_pointivar_;

  Vector rot_start_ray_;
};


} // End namespace SCIRun

#endif
