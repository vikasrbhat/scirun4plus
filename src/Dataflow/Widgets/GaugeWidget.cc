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
 *  GaugeWidget.cc
 *
 *  Written by:
 *   James Purciful
 *   Department of Computer Science
 *   University of Utah
 *   Aug. 1994
 *
 */


#include <Dataflow/Widgets/GaugeWidget.h>
#include <Dataflow/Constraints/DistanceConstraint.h>
#include <Dataflow/Constraints/RatioConstraint.h>
#include <Core/Geom/GeomCylinder.h>
#include <Core/Geom/GeomSphere.h>

#include <Dataflow/Network/Module.h>

namespace SCIRun {



const Index NumCons = 2;
const Index NumVars = 5;
const Index NumGeoms = 6;
const Index NumPicks = 6;
const Index NumMatls = 4;
const Index NumSwitches = 4;
const Index NumSchemes = 3;

enum { ConstDist, ConstRatio };
enum { GeomPointL, GeomPointR, GeomShaft, GeomSlider, GeomResizeL, GeomResizeR };
enum { PickSphL, PickSphR, PickCyl, PickSlider, PickResizeL, PickResizeR };

/***************************************************************************
 * The constructor initializes the widget's constraints, variables,
 *      geometry, picks, materials, modes, switches, and schemes.
 * Variables and constraints are initialized as a function of the
 *      widget_scale.
 * Much of the work is accomplished in the BaseWidget constructor which
 *      includes some consistency checking to ensure full initialization.
 */
GaugeWidget::GaugeWidget( Module* module, CrowdMonitor* lock,
			  double widget_scale, bool is_slideable ) :
  BaseWidget(module, lock, "GaugeWidget", NumVars, NumCons, NumGeoms,
	     NumPicks, NumMatls, 0, NumSwitches, widget_scale),
  oldaxis(1, 0, 0),
  is_slideable_(is_slideable)
{
  double INIT = 10.0*widget_scale;
  // Scheme3 is for resizing.
  variables[PointLVar] = new PointVariable("PntL", solve, Scheme1, Point(0, 0, 0));
  variables[PointRVar] = new PointVariable("PntR", solve, Scheme1, Point(INIT, 0, 0));
  variables[DistVar] = new RealVariable("Dist", solve, Scheme1, INIT);
  variables[SDistVar] = new RealVariable("SDistVar", solve, Scheme2, INIT/2.0);
  variables[RatioVar] = new RealVariable("Ratio", solve, Scheme1, 0.5);
   
  constraints[ConstDist] = new DistanceConstraint("ConstDist",
						     NumSchemes,
						     variables[PointLVar],
						     variables[PointRVar],
						     variables[DistVar]);
  constraints[ConstDist]->VarChoices(Scheme1, 1, 0, 1);
  constraints[ConstDist]->VarChoices(Scheme2, 1, 0, 1);
  constraints[ConstDist]->VarChoices(Scheme3, 2, 2, 2);
  constraints[ConstDist]->Priorities(P_Highest, P_Highest, P_Default);
  constraints[ConstRatio] = new RatioConstraint("ConstRatio",
						   NumSchemes,
						   variables[SDistVar],
						   variables[DistVar],
						   variables[RatioVar]);
  constraints[ConstRatio]->VarChoices(Scheme1, 0, 0, 0);
  constraints[ConstRatio]->VarChoices(Scheme2, 2, 2, 2);
  constraints[ConstRatio]->VarChoices(Scheme3, 0, 0, 0);
  constraints[ConstRatio]->Priorities(P_Highest, P_Highest, P_Highest);

  // Shaft geometry.
  geometries[GeomShaft] = new GeomCappedCylinder;
  picks_[PickCyl] =
    new GeomPick(geometries[GeomShaft], this, PickCyl);
  picks(PickCyl)->set_highlight(DefaultHighlightMaterial);
  materials[ShaftMatl] = new GeomMaterial(picks_[PickCyl],
					     DefaultEdgeMaterial);
  CreateModeSwitch(0, materials[ShaftMatl]);

  // Rotate ball geometry.
  GeomGroup* sphs = new GeomGroup;
  geometries[GeomPointL] = new GeomSphere;
  picks_[PickSphL] = new GeomPick(geometries[GeomPointL],
				    this, PickSphL);
  picks(PickSphL)->set_highlight(DefaultHighlightMaterial);
  sphs->add(picks_[PickSphL]);
  geometries[GeomPointR] = new GeomSphere;
  picks_[PickSphR] = new GeomPick(geometries[GeomPointR],
				    this, PickSphR);
  picks(PickSphR)->set_highlight(DefaultHighlightMaterial);
  sphs->add(picks_[PickSphR]);
  materials[PointMatl] = new GeomMaterial(sphs, DefaultPointMaterial);
  CreateModeSwitch(1, materials[PointMatl]);

  // Resize geometry
  GeomGroup* resizes = new GeomGroup;
  geometries[GeomResizeL] = new GeomCappedCylinder;
  picks_[PickResizeL] = new GeomPick(geometries[GeomResizeL],
				       this, PickResizeL);
  picks(PickResizeL)->set_highlight(DefaultHighlightMaterial);
  resizes->add(picks_[PickResizeL]);
  geometries[GeomResizeR] = new GeomCappedCylinder;
  picks_[PickResizeR] = new GeomPick(geometries[GeomResizeR],
				       this, PickResizeR);
  picks(PickResizeR)->set_highlight(DefaultHighlightMaterial);
  resizes->add(picks_[PickResizeR]);
  materials[ResizeMatl] = new GeomMaterial(resizes, DefaultResizeMaterial);
  CreateModeSwitch(2, materials[ResizeMatl]);

  // Slider geometry.
  geometries[GeomSlider] = new GeomCappedCylinder;
  picks_[PickSlider] = new GeomPick(geometries[GeomSlider],
				      this, PickSlider);
  picks(PickSlider)->set_highlight(DefaultHighlightMaterial);
  materials[SliderMatl] = new GeomMaterial(picks_[PickSlider],
					      DefaultSliderMaterial);
  CreateModeSwitch(3, materials[SliderMatl]);

  // Switch0 are the bars
  // Switch1 are the rotation points
  // Switch2 are the resize cylinders
  // Switch3 are the sliders
  if (is_slideable_)
  {
    SetNumModes(8);
    SetMode(Mode0, Switch0|Switch1|Switch2|Switch3);
    SetMode(Mode1, Switch0);
    SetMode(Mode2, Switch0|Switch1);
    SetMode(Mode3, Switch0|Switch2);
    SetMode(Mode4, Switch0|Switch1|Switch2);
    SetMode(Mode5, Switch0|Switch3);
    SetMode(Mode6, Switch0|Switch1|Switch3);
    SetMode(Mode7, Switch0|Switch2|Switch3);
  }
  else
  {
    SetNumModes(4);
    SetMode(Mode0, Switch0|Switch1|Switch2);
    SetMode(Mode1, Switch0);
    SetMode(Mode2, Switch0|Switch1);
    SetMode(Mode3, Switch0|Switch2);
  }

  FinishWidget();
}


/***************************************************************************
 * The destructor frees the widget's allocated structures.
 * The BaseWidget's destructor frees the widget's constraints, variables,
 *      geometry, picks, materials, modes, switches, and schemes.
 * Therefore, most widgets' destructors will not need to do anything.
 */
GaugeWidget::~GaugeWidget()
{
}


/***************************************************************************
 * The widget's redraw method changes widget geometry to reflect the
 *      widget's variable values and its widget_scale.
 * Geometry should only be changed if the mode_switch that displays
 *      that geometry is active.
 * Redraw should also set the principal directions for all picks.
 * Redraw should never be called directly; the BaseWidget execute method
 *      calls redraw after establishing the appropriate locks.
 */
void
GaugeWidget::redraw()
{
  Point L(variables[PointLVar]->point()), R(variables[PointRVar]->point()),
    S(L+GetAxis()*variables[SDistVar]->real());

  // Draw the cylinder
  if (mode_switches[0]->get_state())
  {
    geometry<GeomCappedCylinder*>(GeomShaft)->move(L, R, 0.5*widget_scale_);
  }

  // Draw the balls
  if (mode_switches[1]->get_state())
  {
    geometry<GeomSphere*>(GeomPointL)->move(L, widget_scale_);
    geometry<GeomSphere*>(GeomPointR)->move(R, widget_scale_);
  }

  if (mode_switches[2]->get_state())
  {
    geometry<GeomCappedCylinder*>(GeomResizeL)->
      move(L, L - (GetAxis() * 1.5 * widget_scale_), 0.5*widget_scale_);
    geometry<GeomCappedCylinder*>(GeomResizeR)->
      move(R, R + (GetAxis() * 1.5 * widget_scale_), 0.5*widget_scale_);
  }

  if (mode_switches[3]->get_state())
  {
    geometry<GeomCappedCylinder*>(GeomSlider)->
      move(S - (GetAxis() * 0.3 * widget_scale_),
	   S + (GetAxis() * 0.3 * widget_scale_),
	   1.1*widget_scale_);
  }
   
  Vector v(GetAxis()), v1, v2;
  v.find_orthogonal(v1,v2);
  for (Index geom = 0; geom < NumPicks; geom++)
  {
    if ((geom == PickSlider) || (geom == PickResizeL) || (geom == PickResizeR))
    {
      picks(geom)->set_principal(v);
    }
    else
    {
      picks(geom)->set_principal(v, v1, v2);
    }
  }
}



void
GaugeWidget::geom_pick(GeomPickHandle p, ViewWindow *vw,
		       int data, const BState &bs)
{
  BaseWidget::geom_pick(p, vw, data, bs);
  pick_pointlvar_ = variables[PointLVar]->point();
  pick_pointrvar_ = variables[PointRVar]->point();
  pick_distvar_ = variables[DistVar]->real();
  pick_sdistvar_ = variables[SDistVar]->real();
  pick_ratiovar_ = variables[RatioVar]->real();
}



/***************************************************************************
 * The widget's geom_moved method receives geometry move requests from
 *      the widget's picks.  The widget's variables must be altered to
 *      reflect these changes based upon which pick made the request.
 * No more than one variable should be Set since this triggers solution of
 *      the constraints--multiple Sets could lead to inconsistencies.
 *      The constraint system only requires that a variable be Set if the
 *      change would cause a constraint to be invalid.  For example, if
 *      all PointVariables are moved by the same delta, then no Set is
 *      required.
 * The last line of the widget's geom_moved method should call the
 *      BaseWidget execute method (which calls the redraw method).
 */
void
GaugeWidget::geom_moved( GeomPickHandle, int axis, double dist,
			 const Vector& /*delta*/, int pick, const BState&,
			 const Vector &offset)
{
  ((DistanceConstraint*)constraints[ConstDist])->SetDefault(GetAxis());

  switch(pick)
  {
  case PickSphL:
    variables[PointLVar]->Move(pick_pointlvar_);
    variables[PointRVar]->Move(pick_pointrvar_);
    variables[DistVar]->Move(pick_distvar_);
    variables[SDistVar]->Move(pick_sdistvar_);
    variables[RatioVar]->Move(pick_ratiovar_);
    variables[PointLVar]->SetDelta(offset);
    break;

  case PickSphR:
    variables[PointLVar]->Move(pick_pointlvar_);
    variables[PointRVar]->Move(pick_pointrvar_);
    variables[DistVar]->Move(pick_distvar_);
    variables[SDistVar]->Move(pick_sdistvar_);
    variables[RatioVar]->Move(pick_ratiovar_);
    variables[PointRVar]->SetDelta(offset);
    break;

  case PickResizeL:
    variables[PointLVar]->Move(pick_pointlvar_);
    variables[PointRVar]->Move(pick_pointrvar_);
    variables[DistVar]->Move(pick_distvar_);
    variables[SDistVar]->Move(pick_sdistvar_);
    variables[RatioVar]->Move(pick_ratiovar_);
    variables[PointLVar]->SetDelta(offset, Scheme3);
    break;

  case PickResizeR:
    variables[PointLVar]->Move(pick_pointlvar_);
    variables[PointRVar]->Move(pick_pointrvar_);
    variables[DistVar]->Move(pick_distvar_);
    variables[SDistVar]->Move(pick_sdistvar_);
    variables[RatioVar]->Move(pick_ratiovar_);
    variables[PointRVar]->SetDelta(offset, Scheme3);
    break;

  case PickSlider:
    if (is_slideable_)
    {
      if (axis==1) { dist*=-1.0; }
      double sdist(variables[SDistVar]->real()+dist);
      if (sdist<0.0) { sdist=0.0; }
      else if (sdist>variables[DistVar]->real())
      {
	sdist=variables[DistVar]->real();
      }
      variables[SDistVar]->Set(sdist);
    }
    break;

  case PickCyl:
    variables[PointLVar]->Move(pick_pointlvar_);
    variables[PointRVar]->Move(pick_pointrvar_);
    variables[DistVar]->Move(pick_distvar_);
    variables[SDistVar]->Move(pick_sdistvar_);
    variables[RatioVar]->Move(pick_ratiovar_);
    MoveDelta(offset);
    break;
  }
  execute(0);
}


/***************************************************************************
 * This standard method simply moves all the widget's PointVariables by
 *      the same delta.
 * The last line of this method should call the BaseWidget execute method
 *      (which calls the redraw method).
 */
void
GaugeWidget::MoveDelta( const Vector& delta )
{
  variables[PointLVar]->MoveDelta(delta);
  variables[PointRVar]->MoveDelta(delta);

  execute(1);
}


/***************************************************************************
 * This standard method returns a reference point for the widget.  This
 *      point should have some logical meaning such as the center of the
 *      widget's geometry.
 */
Point
GaugeWidget::ReferencePoint() const
{
  return (variables[PointLVar]->point() +
	  (variables[PointRVar]->point() - variables[PointLVar]->point())*0.5);
}


void
GaugeWidget::SetRatio( const double ratio )
{
  ASSERT((ratio>=0.0) && (ratio<=1.0));
  variables[RatioVar]->Set(ratio);
   
  execute(0);
}


double
GaugeWidget::GetRatio() const
{
  return (variables[RatioVar]->real());
}


void
GaugeWidget::SetEndpoints( const Point& end1, const Point& end2 )
{
  variables[PointLVar]->Move(end1);
  variables[PointRVar]->Set(end2, Scheme3);
   
  execute(0);
}


void
GaugeWidget::GetEndpoints( Point& end1, Point& end2 ) const
{
  end1 = variables[PointLVar]->point();
  end2 = variables[PointRVar]->point();
}


const Vector&
GaugeWidget::GetAxis()
{
  Vector axis(variables[PointRVar]->point() - variables[PointLVar]->point());
  if (axis.length2() <= 1e-6)
    return oldaxis;
  else 
    return (oldaxis = axis.safe_normal());
}


/***************************************************************************
 * This standard method returns a string describing the functionality of
 *      a widget's material property.  The string is used in the 
 *      BaseWidget UI.
 */
std::string
GaugeWidget::GetMaterialName( const Index mindex ) const
{
  ASSERT(mindex<materials.size());
   
  switch(mindex)
  {
  case 0:
    return "Point";
  case 1:
    return "Shaft";
  case 2:
    return "Resize";
  case 3:
    return "Slider";
  default:
    return "UnknownMaterial";
  }
}

} // End namespace SCIRun


