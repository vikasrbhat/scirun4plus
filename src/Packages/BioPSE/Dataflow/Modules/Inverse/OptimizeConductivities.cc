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
 *  OptimizeConductivities.cc:  Search for the optimal conductivity
 *
 *  Written by:
 *   David Weinstein
 *   Department of Computer Science
 *   University of Utah
 *   May 1999
 *
 */

#include <Core/Util/StringUtil.h>
#include <Core/Containers/Array2.h>
#include <Core/Math/MusilRNG.h>
#include <Core/Math/Gaussian.h>
#include <Core/Thread/Mutex.h>

#include <Core/Datatypes/ColumnMatrix.h>
#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/SparseRowMatrix.h>
#include <Core/Datatypes/Field.h>
#include <Core/Datatypes/FieldInformation.h>
#include <Core/Datatypes/MatrixTypeConverter.h>
#include <Core/Algorithms/FiniteElements/BuildMatrix/BuildFEMatrix.h>

#include <Dataflow/Network/Module.h>
#include <Dataflow/Network/Ports/MatrixPort.h>
#include <Dataflow/Network/Ports/FieldPort.h>
#include <Dataflow/GuiInterface/GuiVar.h>

#include <iostream>


namespace BioPSE {

using namespace SCIRun;  

class OptimizeConductivities : public Module {    
  FieldHandle mesh_in_;
  FieldHandle mesh_out_;
  MatrixHandle cond_params_;
  MatrixHandle cond_vector_;
  MatrixHandle fem_mat_;

  MatrixHandle AmatH_;
  Array1<Array1<double> > data_basis_;
  Array1<double> misfit_;
  Array2<double> conductivities_;
  Array1<int> cell_visited_;
  Array1<double> cell_err_;
  Array1<Vector> cell_dir_;  
  int seed_counter_;
  std::string state_;
  int stop_search_;
  int last_intermediate_;
  Mutex mylock_;
  MusilRNG* mr;

  static int NDIM_;
  static int NSEEDS_;
  static int NCONDUCTIVITIES_;
  static int MAX_EVALS_;
  static double CONVERGENCE_;
  static double OUT_OF_BOUNDS_MISFIT_;

  void build_basis_matrices();
  void initialize_search();
  MatrixHandle build_composite_matrix(int which_conductivity);
  void send_and_get_data(int which_conductivity);
  int pre_search();
  void eval_test_conductivity();
  double simplex_step(Array1<double>& sum, double factor, int worst);
  void simplex_search();
  void read_mesh_and_cond_param_ports(int &valid_data, int &new_data);
public:
  GuiInt seed_gui_;
  OptimizeConductivities(GuiContext *context);
  virtual ~OptimizeConductivities();
  virtual void execute();
  virtual void tcl_command( GuiArgs&, void * );
};


DECLARE_MAKER(OptimizeConductivities)


int OptimizeConductivities::NDIM_ = 3;
int OptimizeConductivities::NSEEDS_ = 4;
int OptimizeConductivities::NCONDUCTIVITIES_ = 5;
int OptimizeConductivities::MAX_EVALS_ = 100;
double OptimizeConductivities::CONVERGENCE_ = 0.001;
double OptimizeConductivities::OUT_OF_BOUNDS_MISFIT_ = 1000000;

OptimizeConductivities::OptimizeConductivities(GuiContext *context)
  : Module("OptimizeConductivities",context, Filter, "Inverse", "BioPSE"), 
  mylock_("pause lock for OptimizeConductivities"), 
  seed_gui_(context->subVar("seed_gui"))
{
  if(sci_getenv("SCI_REGRESSION_TESTING"))
  {
    // Prevent Test from taking too long
    MAX_EVALS_ = 5;
  }

  mylock_.unlock();
  state_ = "SEEDING";
  stop_search_ = 0;
  seed_counter_ = 0;
}


OptimizeConductivities::~OptimizeConductivities()
{
}


void
OptimizeConductivities::build_basis_matrices()
{
  MatrixHandle FEM;
  MatrixHandle cond =  new ColumnMatrix(NDIM_);
  cond->zero();
  
  SCIRunAlgo::BuildFEMatrixAlgo fealgo;
  fealgo.set_progress_reporter(this);
  if(!(fealgo.run(mesh_in_,cond,FEM))) return;
  
  AmatH_ = FEM;
  AmatH_.detach();
  data_basis_.resize(NDIM_);
  
  for (int i=0; i<NDIM_; i++) 
  {
    cond->put(i,0,1.0);

    if(!(fealgo.run(mesh_in_,cond,FEM))) return;
    SparseRowMatrix *m = dynamic_cast<SparseRowMatrix*>(FEM.get_rep());
    data_basis_[i].resize(m->get_nnz());
    for (index_type j=0; j<m->get_nnz(); j++)
      data_basis_[i][j] = m->get_value(j);
 
    cond->put(i,0,0.0);
  }   
}


//! Initialization sets up our conductivity search matrix, our misfit vector,
//!   and our stiffness matrix frame
void
OptimizeConductivities::initialize_search()
{
  // fill in our random conductivities and build our stiffness matrix frame
  misfit_.resize(NCONDUCTIVITIES_);
  conductivities_.resize(NCONDUCTIVITIES_, NDIM_);

  ColumnMatrix *cm;
  cond_vector_ = cm = new ColumnMatrix(NDIM_*2);

  for (int c=0; c<NDIM_*2; c++) (*cm)[c]=0;
  std::vector<std::pair<std::string, Tensor> > conds;
  if (mesh_in_->get_property("conductivity_table", conds)) 
  {
    for( int c=NDIM_; c<NDIM_*2; c++ ) 
    {
        (*cm)[c]=conds[c-NDIM_].second.mat_[0][0];
    }
  }

  int seed = seed_gui_.get();
  seed_gui_.set(seed+1);
  mr = new MusilRNG(seed);
  (*mr)();  // first number isn't really random

  // load Gaussian distribution of conductivity values into our search matrix
  int i, j;
  for (i=0; i<NDIM_; i++) 
  {
    double val, min, max, sigma, avg;
    avg = cond_params_->get(i, 0);
    sigma = cond_params_->get(i, 1);
    min = cond_params_->get(i, 2);
    max = cond_params_->get(i, 3);
    Gaussian g(avg, sigma);
    for (j=0; j<NDIM_+2; j++) 
    {
      do 
      {
        val= conductivities_(j,i) = g.rand();
      } while (val > max || val < min);
    }
  }

  build_basis_matrices();
}


//! Scale the basis matrix data by the conductivities and sum

MatrixHandle
OptimizeConductivities::build_composite_matrix(int which_conductivity)
{
  MatrixHandle fem_mat = AmatH_;
  fem_mat.detach();
  SparseRowMatrix *m = matrix_cast::as_sparse(fem_mat);
  double *sum = m->get_vals();
  for (int i=0; i<NDIM_; i++) 
  {
    double weight = conductivities_(which_conductivity,i);
    for (int j=0; j<data_basis_[i].size(); j++)
      sum[j] += weight*data_basis_[i][j];
  }
  return fem_mat;
}


//! Find the misfit and optimal orientation for a single conductivity
void
OptimizeConductivities::send_and_get_data(int which_conductivity)
{
  if (!mylock_.tryLock()) 
  {
    mylock_.lock();
    mylock_.unlock();
  } 
  else 
  {
    mylock_.unlock();
  }

  // build the new conductivity vector, the new mesh (just new tensors), 
  // and the new fem matrix
  cond_vector_.detach();
  mesh_out_ = mesh_in_;
  mesh_out_.detach();
  std::vector<std::pair<std::string, Tensor> > conds;
  ColumnMatrix *cm = dynamic_cast<ColumnMatrix*>(cond_vector_.get_rep());
  int i;
  for (i=0; i<NDIM_; i++) 
  {
    double c=conductivities_(which_conductivity, i);
    conds.push_back(std::pair<std::string, Tensor>(to_string(i), Tensor(c)));
    (*cm)[i]=c;
  }
  mesh_out_->freeze();
  mesh_out_->set_property("conductivity_table", conds, true);

  fem_mat_ = build_composite_matrix(which_conductivity);
  
  // send out data
  send_output_handle("FiniteElementMesh", mesh_out_, true, true);
  send_output_handle("OldAndNewConductivities", cond_vector_, true, true);
  send_output_handle("FiniteElementMatrix", fem_mat_, true, true);
  last_intermediate_=1;

  // read back data, and set the caches and search matrix
  MatrixHandle mH;
  if (!get_input_handle("TestMisfit", mH)) return;

  misfit_[which_conductivity] = mH->get(0, 0);
}  


//! pre_search gets called once for each seed conductivity.  It sends out
//!   one of the seeds, reads back the results, and fills the data
//!   into the caches and the search matrix
//! return "fail" if any seeds are out of range or if we don't get
//!   back a misfit after a send 

int OptimizeConductivities::pre_search() 
{
  if (seed_counter_ == 0) 
  {
    initialize_search();
  }

  send_and_get_data(seed_counter_);
  seed_counter_++;

  // done seeding, prepare for search phase
  if (seed_counter_ == NSEEDS_) 
  {
    seed_counter_ = 0;
    state_ = "START_SEARCHING";
  }

  return 1;
}


//! Evaluate a test conductivity.
void
OptimizeConductivities::eval_test_conductivity()
{
  
  int in_range=1;
  for (int i=0; i<NDIM_; i++)
    if (conductivities_(NSEEDS_,i) < cond_params_->get(i, 2))
      in_range=0;
    else if (conductivities_(NSEEDS_,i) > cond_params_->get(i, 3))
      in_range=0;

  if (in_range) 
  {
    send_and_get_data(NSEEDS_);
  } 
  else 
  {
    misfit_[NSEEDS_]=OUT_OF_BOUNDS_MISFIT_;
  }
}


//! Take a single simplex step.  Evaluate a new position -- if it's
//! better then an existing vertex, swap them.
double
OptimizeConductivities::simplex_step(Array1<double>& sum, double factor, int worst)
{
  double factor1 = (1 - factor)/NDIM_;
  double factor2 = factor1-factor;
  int i;
  for (i=0; i<NDIM_; i++) 
    conductivities_(NSEEDS_,i) = sum[i]*factor1 - 
      conductivities_(worst,i)*factor2;

  // evaluate the new guess
  eval_test_conductivity();

  // if this is better, swap it with the worst one
  if (misfit_[NSEEDS_] < misfit_[worst]) 
  {
    misfit_[worst] = misfit_[NSEEDS_];
    for (i=0; i<NDIM_; i++) 
    {
      sum[i] = sum[i] + conductivities_(NSEEDS_,i)-conductivities_(worst,i);
      conductivities_(worst,i) = conductivities_(NSEEDS_,i);
    }
  }
  return misfit_[NSEEDS_];
}


//! The simplex has been constructed -- now let's search for a minimal misfit

void
OptimizeConductivities::simplex_search()
{
  Array1<double> sum(NDIM_); // sum of the entries in the search matrix rows
  sum.initialize(0);
  int i, j;
  for (i=0; i<NSEEDS_; i++) 
    for (j=0; j<NDIM_; j++)
      sum[j]+=conductivities_(i,j); 

  double relative_tolerance = 0.0;
  int num_evals = 0;

  while(1) 
  {
    int best, worst, next_worst;
    best = 0;
    if (misfit_[0] > misfit_[1]) 
    {
      worst = 0;
      next_worst = 1;
    } 
    else 
    {
      worst = 1;
      next_worst = 0;
    }
    for (i=0; i<NSEEDS_; i++) 
    {
      if (misfit_[i] <= misfit_[best]) best=i;
      if (misfit_[i] > misfit_[worst]) 
      {
        next_worst = worst;
        worst = i;
      } 
      else if (misfit_[i] > misfit_[next_worst] && (i != worst)) 
        next_worst=i;
      relative_tolerance = 2*(misfit_[worst]-misfit_[best])/
        (misfit_[worst]+misfit_[best]);
    }

    if ((relative_tolerance < CONVERGENCE_) || 
        (num_evals > MAX_EVALS_) || (stop_search_)) 
      break;

    double step_misfit = simplex_step(sum, -1, worst);
    num_evals++;
    if (step_misfit <= misfit_[best]) 
    {
      step_misfit = simplex_step(sum, 2, worst);
      num_evals++;
    } 
    else if (step_misfit >= misfit_[worst]) 
    {
      double old_misfit = misfit_[worst];
      step_misfit = simplex_step(sum, 0.5, worst);
      num_evals++;
      if (step_misfit >= old_misfit) 
      {
        for (i=0; i<NSEEDS_; i++) 
        {
          if (i != best) 
          {
            for (j=0; j<NDIM_; j++)
              conductivities_(i,j) = conductivities_(NSEEDS_,j) = 
                0.5 * (conductivities_(i,j) + conductivities_(best,j));
            eval_test_conductivity();
            misfit_[i] = misfit_[NSEEDS_];
            num_evals++;
          }
        }
      }
      sum.initialize(0);
      for (i=0; i<NSEEDS_; i++) 
      {
        for (j=0; j<NDIM_; j++)
          sum[j]+=conductivities_(i,j); 
      }
    }
    if ((num_evals % 10) == 0) 
    {
      msg_stream_ << "OptimizeConductivities -- Iter "<<num_evals<<":\n";
      for (i=0; i<NSEEDS_; i++) 
      {
        msg_stream_ << "\t";
        for (j=0; j<NDIM_; j++) 
        {
          msg_stream_ << conductivities_(i,j) << " ";
        }
        msg_stream_ << "\n";
      }
    }
  }
  
  ColumnMatrix *cm = dynamic_cast<ColumnMatrix*>(cond_vector_.get_rep());
  msg_stream_ << "OptimizeConductivities -- Original conductivities: \n\t";
  for (i=NDIM_; i<NDIM_*2; i++) msg_stream_ << (*cm)[i] << " ";
  msg_stream_ << "\nConductivitiySearch -- Final conductivities: \n";
  for (i=0; i<NSEEDS_; i++) 
  {
    msg_stream_ << "\t";
    for (j=0; j<NDIM_; j++) 
    {
      msg_stream_ << conductivities_(i,j) << " ";
    }
    msg_stream_ << "(error="<<misfit_[i]<<")\n";
  }
}


//! Read the input fields.  Check whether the inputs are valid,
//! and whether they've changed since last time.
void
OptimizeConductivities::read_mesh_and_cond_param_ports(int &valid_data, 
                                                   int &new_data)
{
  FieldHandle mesh;
  valid_data=1;
  new_data=0;
  get_input_handle("FiniteElementMesh", mesh, false);
 
  FieldInformation fi(mesh);
  
  if (fi.is_constantdata())
  {
    if (!mesh_in_.get_rep() || (mesh_in_->generation != mesh->generation)) 
    {
      new_data=1;
      mesh_in_=mesh;
    } 
    else 
    {
      remark("Same VolumeMesh as previous run.");
    }
  } 
  else 
  {
    valid_data = 0;
    error("Didn't get a valid VolumeMesh.");
    error("Expected TetVolMesh with constant basis and int data.");
  }

  Array1<Tensor> tens;
  
  double dmax;
  int max = 1;
  if (!mesh->vfield()->max(dmax)) valid_data=0;
  max = static_cast<int>(dmax);
  NDIM_ = max+1;
  NSEEDS_ = max+2;
  NCONDUCTIVITIES_ = max+3;

  MatrixHandle cond_params;
  if (get_input_handle("ConductivityParameters", cond_params, false) &&
      (cond_params->nrows() == NDIM_) && (cond_params->ncols() == 4))
  {
    if (!cond_params_.get_rep() || 
          (cond_params_->generation != cond_params->generation))
    {
      new_data = 1;
      cond_params_=cond_params;
    } 
    else 
    {
      remark("Same ConductivityParams as before.");
    }
  } 
  else 
  {
    valid_data=0;
    error("Didn't get valid ConductivityParams.");
  }
}


//! If we have an old solution and the inputs haven't changed, send that
//! one.  Otherwise, if the input is valid, run a simplex search to find
//! a conductivity with minimal misfit.

void
OptimizeConductivities::execute()
{
  int valid_data, new_data;

  read_mesh_and_cond_param_ports(valid_data, new_data);
  if (!valid_data) return;
  if (!new_data) 
  {
    if (mesh_out_.get_rep()) 
    { // if we have valid old data
      // send old data and clear ports
      remark("Sending old data.");
      send_output_handle("FiniteElementMesh", mesh_out_, true);
      send_output_handle("OldAndNewConductivities", cond_vector_, true);
      send_output_handle("FiniteElementMatrix", fem_mat_, true);
      MatrixHandle dummy_mat;
      get_input_handle("TestMisfit", dummy_mat, false);
      return;
    } 
    else 
    {
      return;
    }
  }

  last_intermediate_=0;

  // we have new, valid data -- run the simplex search
  while (1) {
    if (state_ == "SEEDING") 
    {
      if (!pre_search()) break;
    } 
    else if (state_ == "START_SEARCHING") 
    {
      simplex_search();
      state_ = "DONE";
    }
    if (stop_search_ || state_ == "DONE") break;
  }
  if (last_intermediate_) 
  { // last sends were send_intermediates
    // Gotta do final sends and clear the ports.
    send_output_handle("FiniteElementMesh", mesh_out_, true);
    send_output_handle("OldAndNewConductivities", cond_vector_, true);
    send_output_handle("FiniteElementMatrix", fem_mat_, true);
    MatrixHandle dummy_mat;
    get_input_handle("TestMisfit", dummy_mat, false);
  }
  state_ = "SEEDING";
  stop_search_=0;
  seed_counter_=0;
}


//! Commands invoked from the Gui.  Pause/unpause/stop the search.

void
OptimizeConductivities::tcl_command(GuiArgs& args, void* userdata)
{
  if (args[1] == "pause") 
  {
    if (mylock_.tryLock())
      msg_stream_ << "Pausing..."<<std::endl;
    else 
      msg_stream_ << "Can't lock -- already locked"<<std::endl;
  } 
  else if (args[1] == "unpause") 
  {
    if (mylock_.tryLock())
      msg_stream_ << "Can't unlock -- already unlocked"<<std::endl;
    else
      msg_stream_ << "Unpausing"<<std::endl;
    mylock_.unlock();
  } 
  else if (args[1] == "stop") 
  {
    stop_search_=1;
  } 
  else if (args[1] == "exec") 
  {
    mesh_in_=0;
    want_to_execute();
  } 
  else 
  {
    Module::tcl_command(args, userdata);
  }
}

} // End namespace BioPSE
