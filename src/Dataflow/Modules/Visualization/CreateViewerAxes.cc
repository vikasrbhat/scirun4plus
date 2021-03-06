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
 *  CreateViewerAxes.cc: Choose one input field to be passed downstream
 *
 *  Written by:
 *   McKay Davis
 *   Department of Computer Science
 *   University of Utah
 *   November 2003
 *
 */

#include <Dataflow/Network/Module.h>
#include <Dataflow/Network/Ports/FieldPort.h>
#include <Dataflow/Network/Ports/GeometryPort.h>
#include <Core/Util/StringUtil.h>
#include <Core/Containers/Handle.h>
#include <Core/Geom/ColorMapTex.h>
#include <Core/Geom/GeomCull.h>
#include <Core/Geom/GeomDL.h>
#include <Core/Geom/GeomGrid.h>
#include <Core/Geom/GeomGroup.h>
#include <Core/Geom/GeomPoint.h>
#include <Core/Geom/GeomLine.h>
#include <Core/Geom/GeomQuads.h>
#include <Core/Geom/GeomSticky.h>
#include <Core/Geom/GeomSwitch.h>
#include <Core/Geom/GeomText.h>
#include <Core/Geom/GeomMaterial.h>
#include <Core/Geom/GeomTransform.h>
#include <Core/Geom/GeomTriangles.h>
#include <Core/Geometry/BBox.h>

#include <Core/Math/MiscMath.h>

#include <Dataflow/GuiInterface/GuiVar.h>
#include <iostream>
#include <map>


namespace SCIRun {

class CreateViewerAxes : public Module {
public:
  CreateViewerAxes(GuiContext* ctx);

  virtual ~CreateViewerAxes();

  virtual void execute();

  virtual void tcl_command(GuiArgs& args, void* userdata);

private:
  std::vector< GeomHandle >          geometry_out_handles_;

  // The bounding box of all the input fields
  BBox				bbox_;
  
  // Some basic colors, mainly for debugging
  MaterialHandle		white_;
  MaterialHandle		red_;
  MaterialHandle		green_;

    
  GeomHandle			generateAxisLines(int,int,int);
  GeomHandle			generateLines(const Point  start,
					      const Vector delta,
					      const Vector dir,
					      const double num);

  
  typedef std::map<std::string, GuiVar*>	var_map_t;
  typedef var_map_t::iterator	var_iter_t;
  var_map_t			vars_;
  void				add_GuiString(const std::string &str);
  void				add_GuiDouble(const std::string &str);
  void				add_GuiInt(const std::string &str);
  void				add_GuiBool(const std::string &str);


  std::string			get_GuiString(const std::string &str);
  double			get_GuiDouble(const std::string &str);
  int		                get_GuiInt(const std::string &str);
  bool		                get_GuiBool(const std::string &str);
};

DECLARE_MAKER(CreateViewerAxes)
  


CreateViewerAxes::CreateViewerAxes(GuiContext* ctx)
  : Module("CreateViewerAxes", ctx, Filter, "Visualization", "SCIRun"),
    bbox_(Point(-1.,-1.,-1.),Point(1.,1.,1.)),
    white_(new Material(Color(0,0,0), Color(1,1,1), Color(1,1,1), 20)),
    red_(new Material(Color(0,0,0), Color(1,0.4,0.4), Color(1,0.4,0.4), 20)),
    green_(new Material(Color(0,0,0), Color(0.4,1.,0.4), Color(0.4,1.,0.4), 20)),
    vars_()
{
  add_GuiString("title");
  add_GuiString("font");
  add_GuiInt("precision");
  add_GuiDouble("squash");
  add_GuiString("valuefont");
  add_GuiString("labelfont");
  add_GuiInt("valuerez");
  add_GuiInt("labelrez");

  for (int i = 0; i < 3; i++) {
    for (int dir = 0; dir < 2; dir++) {
      int axis = i/3*3+(i%3+1)/2+1;
      if (!dir) axis=i/3*3+i%3/2;
      
      std::ostringstream str;
    str << "Plane-" << i/3*3+i%3/2 << i/3*3+(i%3+1)/2+1 
	<< "-" << axis << "-Axis-";
    const std::string base = str.str();

    add_GuiString(base+"text");

    add_GuiDouble(base+"absolute");
    add_GuiDouble(base+"divisions");
    add_GuiBool(base+"offset");
    
    add_GuiDouble(base+"range-first");
    add_GuiDouble(base+"range-second");
    add_GuiDouble(base+"min-absolute");
    add_GuiDouble(base+"max-absolute");

    add_GuiInt(base+"minplane");
    add_GuiInt(base+"maxplane");
    add_GuiInt(base+"lines");
    add_GuiInt(base+"minticks");
    add_GuiInt(base+"maxticks");
    add_GuiInt(base+"minlabel");
    add_GuiInt(base+"maxlabel");
    add_GuiInt(base+"minvalue");
    add_GuiInt(base+"maxvalue");

    add_GuiDouble(base+"width");
    add_GuiDouble(base+"double");
    add_GuiDouble(base+"tickangle");
    add_GuiDouble(base+"ticktilt");
    add_GuiDouble(base+"ticksize");
    add_GuiDouble(base+"labelangle");
    add_GuiDouble(base+"labelheight");
    add_GuiDouble(base+"valuesize");
    add_GuiDouble(base+"valuesquash");

    }
  }
}

CreateViewerAxes::~CreateViewerAxes()
{
}

void
CreateViewerAxes::execute()
{
  std::vector< FieldHandle > field_input_handles;

  if( !get_dynamic_input_handles( "Field", field_input_handles, true ) )
    return;

  if( !geometry_out_handles_.size() == 0 || inputs_changed_ ) 
  {
    // Inform module that execution started
    update_state(Executing);

    geometry_out_handles_.clear();

    bbox_.reset();
    for( unsigned int i=0; field_input_handles.size(); i++ )
      bbox_.extend(field_input_handles[i]->vmesh()->get_bounding_box());

    std::vector< std::string > names;

    geometry_out_handles_.push_back(generateAxisLines(0,1,2));
    geometry_out_handles_.push_back(generateAxisLines(1,2,0));
    geometry_out_handles_.push_back(generateAxisLines(0,2,1));

    names.push_back("XY Plane");
    names.push_back("XZ Plane");
    names.push_back("YZ Plane");

    send_output_handle( std::string("Axes"), geometry_out_handles_, names );
  }
}


//#define DEBUGPRINT  std::cerr << "[set " << str << "] { " << ret << " } " << std::endl; std::cerr.flush();
#define DEBUGPRINT

std::string
CreateViewerAxes::get_GuiString(const std::string &str) {
  std::string ret;
  var_iter_t pos = vars_.find(str);
  if (pos == vars_.end()) return ret;  
  if (GuiString *var = dynamic_cast<GuiString *>((*pos).second)) {
    ret = var->get();
  }
  DEBUGPRINT;
  return ret;
}


double
CreateViewerAxes::get_GuiDouble(const std::string &str) {
  double ret = 0.0;
  var_iter_t pos = vars_.find(str);
  if (pos == vars_.end()) return ret;  
  if (GuiDouble *var = dynamic_cast<GuiDouble *>((*pos).second)) {
    ret = var->get();
  }
  DEBUGPRINT;
  return ret;
}


int
CreateViewerAxes::get_GuiInt(const std::string &str) {
  int ret = 0;
  var_iter_t pos = vars_.find(str);
  if (pos == vars_.end()) return ret;  
  if (GuiInt *var = dynamic_cast<GuiInt *>((*pos).second)) {
    ret = var->get();
  }
  DEBUGPRINT;
  return ret;
}

bool
CreateViewerAxes::get_GuiBool(const std::string &str) {
  bool ret = false;
  var_iter_t pos = vars_.find(str);
  if (pos == vars_.end()) return ret;  
  if (GuiInt *var = dynamic_cast<GuiInt *>((*pos).second)) {
    ret = (var->get()?true:false);
  }
  DEBUGPRINT;
  return ret;
}

//#define DEBUGPRINT std::cerr << "adding GuiVar: " << str << std::endl; std::cerr.flush();
//#define DEBUGPRINT
void
CreateViewerAxes::add_GuiString(const std::string &str) {
  DEBUGPRINT;
  vars_.insert(std::make_pair(str,new GuiString(get_ctx()->subVar(str))));
}


void
CreateViewerAxes::add_GuiDouble(const std::string &str) {
  DEBUGPRINT;
  vars_.insert(std::make_pair(str,new GuiDouble(get_ctx()->subVar(str))));
}


void
CreateViewerAxes::add_GuiInt(const std::string &str) {
  DEBUGPRINT;
  vars_.insert(std::make_pair(str,new GuiInt(get_ctx()->subVar(str))));
}

void
CreateViewerAxes::add_GuiBool(const std::string &str) {
  DEBUGPRINT;
  add_GuiInt(str);
}

GeomHandle
CreateViewerAxes::generateLines(const Point  start,
		       const Vector delta,
		       const Vector dir,
		       const double num)
{
  GeomLines *lines = new GeomLines();
  for (int i = 0; i <= Round(num); i++) {
    const Point pos = start + delta*i;    
    lines->add(pos, pos + dir);
  }
  
  return new GeomMaterial(lines, white_);
}

void 
CreateViewerAxes::tcl_command(GuiArgs& args, void* userdata) {
  if(args.count() < 2){
    args.error("ShowField needs a minor command");
    return;
  } else {
    Module::tcl_command(args, userdata);
  }
}




GeomHandle
CreateViewerAxes::generateAxisLines(int prim, int sec, int ter)
{
  GeomGroup *group = new GeomGroup();

  const double scale = bbox_.diagonal().maxComponent();
  const double radians = 2.0*M_PI/360.0;
    
  for (int dir = 0; dir < 2; dir++) {
    int axisleft   = (dir?prim:sec);
    int axisup     = (dir?sec:prim);
    int axisnormal = ter;

    std::ostringstream str;   
    str << "Plane-" << prim << sec << "-" << axisleft << "-Axis-";
    const std::string base = str.str();

    // ********** THIS DOESNT BELONG HERE ***************
    var_iter_t pos = vars_.find(base+"range-first");
    GuiDouble *var = NULL;
    if (pos != vars_.end() && (var = dynamic_cast<GuiDouble *>((*pos).second))) {
      var->set(bbox_.min().asVector()[axisleft]);
    }

    pos = vars_.find(base+"range-second");
    if (pos != vars_.end() && (var = dynamic_cast<GuiDouble *>((*pos).second))) {
      var->set(bbox_.max().asVector()[axisleft]);
    }
    // **************************************************

   
    Point origin(bbox_.min());
    Vector originv(bbox_.min().asVector());    
    originv[axisleft] = get_GuiDouble(base+"range-first");
    origin = originv.asPoint();

    Vector primary(0.,0.,0.);
    primary[axisleft] = get_GuiDouble(base+"range-second")-get_GuiDouble(base+"range-first");
    primary[axisup] = 0.0;
    primary[axisnormal] = 0.0;
    if (primary.length() < 10e-6) return group;

    const Vector left = primary * (1.0/primary.length());

    Vector secondary(bbox_.diagonal());
    secondary[axisleft] = 0.0;
    secondary[axisnormal] = 0.0;

    if (secondary.length() < 10e-6) return group;

    const Vector up = secondary * (1.0/secondary.length());
    
    Vector tertiary(bbox_.diagonal());
    tertiary[axisleft] = 0.0;
    tertiary[axisup] = 0.0;
        
    Vector normal = tertiary;
    if (normal.length() < 10e-6)
      normal = Cross(left,up);
    normal.normalize();
    Vector nnormal = normal;
    nnormal *= -1.0;

    const int num = Round(get_GuiDouble(base+"divisions"));

    Vector delta = primary;
    delta.normalize();
    GuiDouble *absolute_var = dynamic_cast<GuiDouble *>(vars_[base+"absolute"]);
    if (absolute_var->valid()) 
      delta *= absolute_var->get();
    else 
      delta = primary/num;

    Vector nsecondary = -secondary;
    
    GeomHandle obj;
    
    GeomLines *lines = new GeomLines();
    double line_width = get_GuiDouble(base+"width");
    if (line_width < 0.05) line_width = 0.05;
    lines->setLineWidth(static_cast<float>(line_width));


    for (int i = 0; i <= num; i++) {
      const Point pos = origin + delta*i;    
      lines->add(pos, pos + secondary);
    }
    
    obj = new GeomMaterial(lines, white_);


    int show = get_GuiInt(base+"lines");
    GeomGroup *grid = new GeomGroup();
    grid->add(new GeomSwitch(new GeomCull(obj,(show<2)?0:&normal),show>0));

    // Create Tickmarks
    Vector tick_vec = secondary;
    tick_vec[axisleft] = 0.0;
    tick_vec[axisnormal] = 0.0;
    tick_vec[axisup] = secondary[axisup]<0.0?-1.:1.;

    Point Zero(0.,0.,0.), One = tick_vec.asPoint();

    GeomLines *tickline = new GeomLines();
    tickline->add(Zero, white_,One,white_);
    tickline->setLineWidth(static_cast<float>(line_width));

    double tilt = radians*get_GuiDouble(base+"ticktilt");
    Transform trans;
    Vector max_offset = tick_vec;
    trans.pre_scale(Vector(1.,1.,1.)*scale*get_GuiDouble(base+"ticksize")/100.);
    trans.project_inplace(max_offset);
    trans.pre_rotate(tilt,normal);
    trans.project_inplace(tick_vec);


    GeomTransform *tilted_tick = new GeomTransform(tickline, trans);

    Vector textup = secondary;
    textup.normalize();
    textup *= scale*get_GuiDouble(base+"valuesize")/100.0;

    max_offset.normalize();
    //    max_offset *= textup.length()*1.1;
    max_offset *= 1.05*scale*(get_GuiDouble(base+"ticksize")+
			     get_GuiDouble(base+"valuesize")*1.1)/100.0;


    GeomGroup *ticks = new GeomGroup();
    Vector squash(1.,1.,1.);
    squash[axisleft] = 
      get_GuiDouble(base+"valuesquash")*get_GuiDouble("squash");
    
    for (int i = 0; i <= num; i++) {
      GeomTransform *ttick = new GeomTransform(tilted_tick);
      ttick->translate(delta*i);
      ticks->add(ttick);
      
    }
    
    //ticks->add(values);
    
    double angle = radians*get_GuiDouble(base+"tickangle");
    trans.load_identity();
    trans.pre_rotate(angle,left);
    trans.pre_translate((origin+secondary).asVector());

    obj = new GeomTransform(ticks, trans);
    show = get_GuiInt(base+"maxticks");
    obj = new GeomSwitch(new GeomCull(obj, show<2?0:&secondary),show>0);
    grid->add(new GeomSwitch(new GeomCull(obj, show<2?0:&normal),show>0));

    // Min Ticks
    trans.load_identity();
    tick_vec[axisleft] = 0.0;
    tick_vec[axisnormal] = 0.0;
    tick_vec[axisup] = (secondary[axisup]>0.0)?-1.:1.;

    One = tick_vec.asPoint();

    tickline = new GeomLines();
    tickline->add(Zero, white_,One,white_);
    tickline->setLineWidth(static_cast<float>(line_width));

    Vector min_offset = tick_vec;
    trans.pre_scale(Vector(1.,1.,1.)*scale*get_GuiDouble(base+"ticksize")/100.);
    trans.project_inplace(min_offset);
    trans.pre_rotate(-tilt,normal);
    trans.project_inplace(tick_vec);

    min_offset.normalize();
    min_offset *= scale*(get_GuiDouble(base+"ticksize")+
			 get_GuiDouble(base+"valuesize")*1.1)/100.0;

//    min_offset *= textup.length()*1.1;

    tilted_tick = new GeomTransform(tickline, trans);
    ticks = new GeomGroup();    

    for (int i = 0; i <= num; i++) {
      GeomTransform *ttick = new GeomTransform(tilted_tick);
      ttick->translate(delta*i);
      ticks->add(ttick);

    }

    angle = radians*get_GuiDouble(base+"tickangle");
    trans.load_identity();
    trans.pre_rotate(-angle,left);
    trans.pre_translate(origin.asVector());

    obj = new GeomTransform(ticks, trans);
    show = get_GuiInt(base+"minticks");
    obj = new GeomSwitch(new GeomCull(obj, show<2?0:&nsecondary),show>0);
    grid->add(new GeomSwitch(new GeomCull(obj, show<2?0:&normal),show>0));

// TODO: delete this?
#if 0
    ticks = new GeomGroup();
    for (int i = 0; i <= Round(divisions); i++) {
      GeomTransform *ttick = new GeomTransform(tick);
      tick->rotate(M_PI,primary);
      ttick->translate((origin+delta*i).asVector());
      ticks->add(ttick);
    }

    show = get_GuiInt(base+"minticks");
    obj = new GeomSwitch(new GeomCull(ticks, show<2?0:&nsecondary),show>0);
    grid->add(new GeomSwitch(new GeomCull(obj, show<2?0:&normal),show>0));
#endif

    obj = grid;
    GeomTransform *mirror = new GeomTransform(grid);
    Vector center = origin.asVector()+bbox_.diagonal()/2.;
    
    mirror->translate(-center);
    mirror->rotate(M_PI,up);
    mirror->translate(center);
    
    //    show = get_GuiInt(base+"minplane");
    //obj = new GeomSwitch(new GeomCull(grid, show<2?0:&normal),show>0);
    //group->add(obj);

    //show = get_GuiInt(base+"maxplane");
    //obj = new GeomSwitch(new GeomCull(mirror, show<2?0:&nnormal),show>0);
    //group->add(obj);
    
    //    grid->add(new GeomSwitch(new GeomCull(obj, show<2?0:&normal),show>0));
    group->add(grid);
    group->add(mirror);
  }

  GeomPoints *points = new GeomPoints();
  points->add(bbox_.min());
  group->add(points);

  return group;
}
		   
} // End namespace SCIRun

