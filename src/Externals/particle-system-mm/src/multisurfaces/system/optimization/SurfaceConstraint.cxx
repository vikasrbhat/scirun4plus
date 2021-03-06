#include <cstdlib>
#include <iostream>
#include <features/mtxlib.h>
#include <system/defines.h>
#include <system/particles/DynamicSurfacePoint.h>
#include <system/domain/Domain.h>
#include <system/optimization/SurfaceConstraint.h>

#ifdef _WIN32
#pragma warning (disable : 4244)
#endif

#include <system/ParticleSystem.h>

using namespace particle_sys;
using namespace std;

void SurfaceConstraint::optimize(custom_class::svector<DynamicSurfacePoint*> &points)
{ 
	cout << "SurfaceConstraint::optimize [start]" << endl;
	init(points); 
	_optimized=true; 
	cout << "SurfaceConstraint::optimize [end]" << endl;
};
    
void SurfaceConstraint::init( svector<DynamicSurfacePoint*> &points,
                              int num_iterations )
{ 
  for ( int j = 0; j < num_iterations; j++ )
  {
    for ( int i = 0; i < points.size(); i++ )
    {
      projectOntoSurface(points[i]);

      // check if the Point needs to be removed from the System
      if ( points[i]->moved_outside() )
      {
        (points[i]->system())->removePoint( i );
	      i--;
      }
    }
  }

  // now, make sure the points are all within the threshold of the
  //   surface
  if ( !points.empty() )
  {
    cout << points.size() << endl;
    points[0]->system()->cleanUpSystem( _F_threshold );
  }

}

//------------------------------------------------------------------------
// Function    : projectOntoSurface
// Description : projects the DynamicSufacePoint onto the Surface and
//               updates the position
//------------------------------------------------------------------------
void 
SurfaceConstraint::projectOntoSurface(DynamicSurfacePoint *point, 
                                      float threshold)
{
  // compute the feedback term (that pushes the Point towards the 
  //   Surface)
  int num_projections = 10;
  vector_type feedback;
  for ( int i = 0; i < num_projections; i++ )
  {
	 //P.Petrov 2014
	 //this function is hardcoded to return false :( WTF
    //if ( !point->domain()->surface()->projectOntoSurface(point, feedback) )
      feedback = (-point->F()/(point->Fx().length() + EPSILON)) * point->normal();

    // project the Point onto the Surface
    vector_type pos = point->position() + feedback;
   
    // update the Point's position
    // P.Petrov 2014
    //maybe we can compute only the energy function and avoid moving the point  ??
    point->move( pos );
    if (fabs(point->F()) < threshold) return;
   }

}









  

