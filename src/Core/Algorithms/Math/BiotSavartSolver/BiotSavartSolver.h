
#ifndef CORE_ALGORITHMS_MATH_BIOTSAVARTSOLVER_H
#define CORE_ALGORITHMS_MATH_BIOTSAVARTSOLVER_H 1

//! Datatypes that the algorithm uses
#include <Core/Datatypes/Mesh.h>
#include <Core/Datatypes/Field.h>
#include <Core/Datatypes/Matrix.h>

//! Base class for algorithm
#include <Core/Algorithms/Util/AlgoBase.h>

//! for Windows support
#include <Core/Algorithms/Fields/share.h>

namespace SCIRunAlgo {

using namespace SCIRun;

class SCISHARE BiotSavartSolverAlgo : public AlgoBase
{
  public:
    BiotSavartSolverAlgo()
    {
      // Number of processors to use
      add_int("num_processors",-1);
    }
    
    //! Solve magnetic B-field via Biot-Savart piece-wise linear intergation
    bool run(FieldHandle& mesh, FieldHandle& coil,FieldHandle& outmesh, MatrixHandle& dataB, MatrixHandle& dataA);
    //TODO split this in varaints with A, A+B and maybe ignore outmesh

};

} // end namespace BiotSavartSolverAlgo

#endif
