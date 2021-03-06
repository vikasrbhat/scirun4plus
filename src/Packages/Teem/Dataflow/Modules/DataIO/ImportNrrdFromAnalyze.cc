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
 * C++ (CC) FILE : ImportNrrdFromAnalyze.cc
 *
 * DESCRIPTION   : This module allows an arbitrary number of Analyze files
 *                 to be read in and converted to 4D nrrds for processing and
 *                 visualization.  The UI associated with this module provides
 *                 a mechanism for selecting Analyze files.
 *                     
 * AUTHOR(S)     : Jenny Simpson
 *                 SCI Institute
 *                 University of Utah
 *         
 * CREATED       : 9/19/2003
 * MODIFIED      : 10/4/2003
 * DOCUMENTATION :
 * NOTES         : 
 *
 */

// SCIRun includes

#include <Dataflow/Network/Module.h>

#include <Dataflow/Network/Ports/NrrdPort.h>
#include <Dataflow/GuiInterface/GuiVar.h>

#include <sci_defs/insight_defs.h> 

#ifdef HAVE_INSIGHT
#include <Core/Algorithms/DataIO/AnalyzeReader.h>
#include <Core/Algorithms/DataIO/AnalyzeImage.h>
#endif

namespace SCITeem {

using namespace SCIRun;

// ****************************************************************************
// *************************** Class: ImportNrrdFromAnalyze ***************************
// ****************************************************************************
 
class  ImportNrrdFromAnalyze : public Module 
{

public:

  //! Constructor
  ImportNrrdFromAnalyze(GuiContext*);

  //! Destructor
  virtual ~ImportNrrdFromAnalyze();

  virtual void execute();

  virtual void tcl_command(GuiArgs&, void*);

#ifdef HAVE_INSIGHT

  int build_nrrds( std::vector<Nrrd*> & array );

  NrrdData * join_nrrds( std::vector<Nrrd*> arr );

#endif

private:

  //! GUI variables
  GuiInt have_insight_;
  GuiString file_;
  GuiString file_del_;
  GuiString messages_;
  GuiInt    num_files_;
  std::vector< GuiString* > filenames_;

  //! Set of Analyze files 
  std::vector<std::string> all_files_;

};

DECLARE_MAKER(ImportNrrdFromAnalyze)

/*===========================================================================*/
// 
// ImportNrrdFromAnalyze
//
// Description : Constructor
//
// Arguments   :
//
// GuiContext* ctx - GUI context
//
ImportNrrdFromAnalyze::ImportNrrdFromAnalyze(GuiContext* ctx)
  : Module("ImportNrrdFromAnalyze", ctx, Source, "DataIO", "Teem"),
    have_insight_(get_ctx()->subVar("have-insight", false)),
    file_(get_ctx()->subVar("file"), ""),
    file_del_(get_ctx()->subVar("file-del"), ""),
    messages_(get_ctx()->subVar("messages"), ""),
    num_files_(get_ctx()->subVar("num-files"), 0)
{
#ifdef HAVE_INSIGHT
  have_insight_.set(1);
#else
  have_insight_.set(0);
#endif
}

/*===========================================================================*/
// 
// ~ImportNrrdFromAnalyze
//
// Description : Destructor
//
// Arguments   : none
//
ImportNrrdFromAnalyze::~ImportNrrdFromAnalyze(){
}


/*===========================================================================*/
// 
// execute 
//
// Description : The execute function for this module.  This is the control
//               center for the module.  This reads a bunch of Analyze files,
//               constructs a nrrd with the Analyze data, and sends the nrrd
//               downstream.
//
// Arguments   : none
//
void ImportNrrdFromAnalyze::execute(){

#ifdef HAVE_INSIGHT

  TCLInterface::execute(get_id() + " sync_filenames");

  // If no Analyze files were specified via the UI, print error and return
  if( all_files_.size() == 0 ) 
  {
    error("(ImportNrrdFromAnalyze::execute) No Analyze files specified -- use 'UI' button to select Analyze files.");
    return;
  }
  
  // Build a vector of nrrds from one or more Analyze files
  std::vector<Nrrd*> arr;
  int ret;
  if( (ret = build_nrrds(arr)) == -1 )
  {
    return;
  }

  // Build single NrrdData object from all nrrds in vector
  NrrdData * sciNrrd = join_nrrds( arr );
  if( sciNrrd == 0 ) 
  {
    error("(ImportNrrdFromAnalyze::execute) Failed to join nrrds.");
    return;
  }

  // Create handle to data
  NrrdDataHandle sciNrrdHandle(sciNrrd);

  // Send nrrd data downstream
  send_output_handle("Nrrd", sciNrrdHandle);

  /*
  Since, in this case, nrrd didn't allocate the data, you might call
  "nrrdNix" to delete the nrrd struct, but not the data it points to
  (assuming someone else will deallocate it).  Or you can effectively
  hand the data off to nrrd and then delete it, along with the nrrd
  struct, with "nrrdNuke".
  */
  //nrrdNix(nrrd);

#else
  error("(ImportNrrdFromAnalyze::execute) Cannot read Analyze files.  Insight module needs to be included.");
  return;
#endif
}

/*===========================================================================*/
// 
// tcl_command
//
// Description : The tcl_command function for this module.
//
// Arguments   :
//
// GuiArgs& args - GUI arguments
//
// void* userdata - ???
// 
void
ImportNrrdFromAnalyze::tcl_command(GuiArgs& args, void* userdata)
{
  if(args.count() < 2){
    args.error("ImportNrrdFromAnalyze needs a minor command");
    return;
  }

  if( args[1] == "add_data" )
  {
#ifdef HAVE_INSIGHT
    if (args.count() == 3) {
      std::string file = args[2];
      
      all_files_.insert( all_files_.end(), file );

      std::ostringstream str;
      str << "filenames" << all_files_.size()-1;
      filenames_.insert(filenames_.end(), new GuiString(get_ctx()->subVar(str.str())));
    }

#endif
  } 
  else if( args[1] == "delete_data" )
  {
#ifdef HAVE_INSIGHT

    // Get the selected file to be deleted
    std::string file_del = args[2];

    // Find the matching entry in the all_files vector and remove it
    int num_files = all_files_.size();
    std::vector<std::string>::iterator iter = all_files_.begin();
    std::vector<GuiString*>::iterator iter2 = filenames_.begin();
     
    for( int i = 0; i < num_files; i++ )
    {
      if( file_del == all_files_[i] )
      {
        // Erase this element from the vector of files
        all_files_.erase( iter );

	// remove the guivar from filenames
	filenames_.erase( iter2 );
      }  
      iter++;
      iter2++;
    }

    // Print out the contents of all_files_
    std::cout << "(ImportNrrdFromAnalyze::tcl_command) File " << file_del << " deleted" 
         << std::endl;
    num_files = all_files_.size();
    for( int j = 0; j < num_files; j++ )
    {
      std::cout << "(ImportNrrdFromAnalyze::tcl_command) all_files_[" << j << "] = " 
           << all_files_[j] << std::endl;
    }    

#endif
  }
  else if ( args[1] == "clear_data" )
    {
#ifdef HAVE_INSIGHT
      all_files_.clear();
#endif      
    }
  else 
  {
    Module::tcl_command(args, userdata);
  }
}

#ifdef HAVE_INSIGHT

/*===========================================================================*/
// 
// build_nrrds 
//
// Description : Given a set of Analyze files, reads all files in and 
//               builds one nrrd object for each file.  Returns 0 on success,
//               -1 on failure.
//
// Arguments   : 
//
// vector<Nrrd*> & array - Empty vector that will be initialized to contain
//                         one nrrd object per file.  
//
int ImportNrrdFromAnalyze::build_nrrds( std::vector<Nrrd*> & array )
{

  // Get the number of files to build nrrds from 
  int num_files = all_files_.size();

  //std::cerr << "(ImportNrrdFromAnalyze::build_nrrds) num_files = " << num_files << std::endl;

  std::vector<Nrrd*> arr( num_files );
  AnalyzeReader reader;

  // Read each Analyze files and build a nrrd object from it
  for( int i = 0; i < num_files; i++ )
  {
    // Get this file
    std::string f = all_files_[i];

    AnalyzeImage image;
    reader.set_file( f );
    int ret;
    if( (ret = reader.read(image)) == -1 )
    {
      error("(ImportNrrdFromAnalyze::build_nrrds) Analyze read failed.  Check command line output for details.");
      return -1;
    }

    image.print_image_info();
  
    // Construct a nrrd from the Analyze data
    char *err;
    Nrrd *nrrd = nrrdNew();

    // The arguments to nrrdWrap:
                                                                               
    // - "data" is a pointer to the raw, contiguous, pixel data.  The
    // parameter type is void*, but you'll be passing a float*, or a
    // double*, or something like that.                                        
                           
    // - I picked "nrrdTypeFloat" out of a hat; you should be choosing from the
    // nrrdType* enum values defined in nrrdEnums.h, to describe the
    // type that data points to.
                                                                               
    // - "3" is the dimension of field being wrapped in the example,
    // and it means that you should pass three numbers (sizeX, sizeY, sizeZ)
    // as the remaining arguments of nrrdWrap().  If this was 2D image data,
    // you pass only two arguments.

    // The nrrd struct does not try to remember "ownership" of the data you
    // give to it: nrrd->data will be set to whatever pointer you give me,
    // and I'll use it assuming that no one is going to free it on me.
    int dim = image.get_dimension();
    if( dim == 3 ) 
    {
      size_t size[3] = {image.get_size(0), image.get_size(1),
			image.get_size(2)};
      if( nrrdWrap_nva(nrrd, image.get_pixel_buffer(), nrrdTypeFloat, 
		       3, size )) 
	{
	  error( "(ImportNrrdFromAnalyze::execute) Error creating nrrd." );
	  err = biffGetDone(NRRD);
	  // There was an error. "err" is a char* error message, pass it
	  // to whatever kind of error handler you are using.  In case
	  // you're picky about memory leaks, its up to you to:
	  free(err);
	}
      
      unsigned int centers[NRRD_DIM_MAX];
      centers[0] = nrrdCenterNode; centers[1] = nrrdCenterNode;
      centers[2] = nrrdCenterNode; centers[3] = nrrdCenterNode;
      nrrdAxisInfoSet_nva( nrrd, nrrdAxisInfoCenter, centers);

      //nrrd->axis[0].label = "Unknown:Scalar";
      nrrd->axis[0].label = airStrdup("x");
      nrrd->axis[1].label = airStrdup("y");
      nrrd->axis[2].label = airStrdup("z");
      nrrd->axis[0].spacing = image.get_spacing(0);
      nrrd->axis[1].spacing = image.get_spacing(1);
      nrrd->axis[2].spacing = image.get_spacing(2);

      nrrdAxisInfoMinMaxSet(nrrd, 0, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(nrrd, 1, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(nrrd, 2, nrrdCenterNode);
      //nrrdAxisInfoMinMaxSet(nrrd, 3, nrrdCenterNode);
    }
    else if( dim == 2 ) 
    {
      size_t size[2] = {image.get_size(0), image.get_size(1)};
      if( nrrdWrap_nva(nrrd, image.get_pixel_buffer(), nrrdTypeFloat, 
		       2, size ) )
      {
        error( "(ImportNrrdFromAnalyze::execute) Error creating nrrd." );
        err = biffGetDone(NRRD);
        // There was an error. "err" is a char* error message, pass it
        // to whatever kind of error handler you are using.  In case
        // you're picky about memory leaks, its up to you to:
        free(err);
      }
  
      unsigned int centers[NRRD_DIM_MAX];
      centers[0] = nrrdCenterNode; centers[1] = nrrdCenterNode;
      centers[2] = nrrdCenterNode; centers[3] = nrrdCenterNode;
      nrrdAxisInfoSet_nva( nrrd, nrrdAxisInfoCenter, centers);

      //nrrd->axis[0].label = "Unknown:Scalar";
      nrrd->axis[0].label = airStrdup("x");
      nrrd->axis[1].label = airStrdup("y");
      nrrd->axis[0].spacing = image.get_spacing(0);
      nrrd->axis[1].spacing = image.get_spacing(1);

      nrrdAxisInfoMinMaxSet(nrrd, 0, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(nrrd, 1, nrrdCenterNode);
      //nrrdAxisInfoMinMaxSet(nrrd, 2, nrrdCenterNode);
    }
    else
    {
      error( "(ImportNrrdFromAnalyze::build_nrrds) Invalid image dimension." );
    }

    // Add this nrrd to the vector of nrrds
    arr[i] = nrrd;
  }

  array = arr;
  return 0;
}

/*===========================================================================*/
// 
// join_nrrds 
//
// Description : Given a vector of 3D nrrd objects, builds a single 4D nrrd
//               that is a combination of all the nrrds.  Uses the nrrdJoin
//               function to do this.  Returns the resulting NrrdData object
//               on success. Returns 0 on failure.
//
// Arguments   : 
//
// vector<Nrrd*> arr - Vector of pointers to nrrd objects. These nrrds should 
//                     all have the same dimensions. 
//
NrrdData * ImportNrrdFromAnalyze::join_nrrds( std::vector<Nrrd*> arr )
{
  int num_nrrds = arr.size();
  //std::cerr << "(ImportNrrdFromAnalyze::join_nrrds) num_nrrds = " << num_nrrds << std::endl;

  if( num_nrrds == 0 )
  {
    error( "(ImportNrrdFromAnalyze::join_nrrds) No nrrds built." );
    return 0;
  }

  // Join all nrrds together into one 4D nrrd object
  NrrdData *sciNrrd = new NrrdData();
  sciNrrd->nrrd_ = nrrdNew();
  
  bool incr = true;
  if (num_nrrds == 1) { incr = false; }
  
  if( nrrdJoin(sciNrrd->nrrd_, &arr[0], num_nrrds, 0, incr) ) 
  {
    char *err = biffGetDone(NRRD);
    error( std::string("(ImportNrrdFromAnalyze::join_nrrds) Join Error: ") +  err );
    free(err);
    return 0;
  }

  unsigned int centers[NRRD_DIM_MAX];
  centers[0] = nrrdCenterNode; centers[1] = nrrdCenterNode;
  centers[2] = nrrdCenterNode; centers[3] = nrrdCenterNode;
  centers[4] = nrrdCenterNode; centers[5] = nrrdCenterNode;
  nrrdAxisInfoSet_nva(sciNrrd->nrrd_, nrrdAxisInfoCenter, centers);

  std::string new_label("");
  for( int i = 0; i < num_nrrds; i++ )
  {
    if (i == 0) 
    {
      new_label += std::string((arr[i])->axis[0].label);
    } 
    else 
    {
      new_label += std::string(",") + std::string(arr[i]->axis[0].label);
    }
  }
  switch (sciNrrd->nrrd_->dim) {
  case 4:
    if (incr) {
      sciNrrd->nrrd_->axis[0].label = airStrdup( new_label.c_str() );
      sciNrrd->nrrd_->axis[1].label = airStrdup( "x" );
      sciNrrd->nrrd_->axis[2].label = airStrdup( "y" );
      sciNrrd->nrrd_->axis[3].label = airStrdup( "z" );
      sciNrrd->nrrd_->axis[1].spacing = arr[0]->axis[0].spacing;
      sciNrrd->nrrd_->axis[2].spacing = arr[0]->axis[1].spacing;
      sciNrrd->nrrd_->axis[3].spacing = arr[0]->axis[2].spacing; 

      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 0, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 1, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 2, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 3, nrrdCenterNode);
    } else {
      return 0;
    }

    break;
  case 3:
    if (incr) {
      sciNrrd->nrrd_->axis[0].label = airStrdup( new_label.c_str() );
      sciNrrd->nrrd_->axis[1].label = airStrdup( "x" );
      sciNrrd->nrrd_->axis[2].label = airStrdup( "y" );
      sciNrrd->nrrd_->axis[1].spacing = arr[0]->axis[0].spacing;
      sciNrrd->nrrd_->axis[2].spacing = arr[0]->axis[1].spacing;
    } else {
      sciNrrd->nrrd_->axis[0].label = airStrdup( "x" );
      sciNrrd->nrrd_->axis[1].label = airStrdup( "y" );
      sciNrrd->nrrd_->axis[2].label = airStrdup( "z" );
      sciNrrd->nrrd_->axis[0].spacing = arr[0]->axis[0].spacing; 
      sciNrrd->nrrd_->axis[1].spacing = arr[0]->axis[1].spacing;
      sciNrrd->nrrd_->axis[2].spacing = arr[0]->axis[2].spacing;
    }
    
    nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 0, nrrdCenterNode);
    nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 1, nrrdCenterNode);
    nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 2, nrrdCenterNode);
    break;
  case 2:
    if (incr) {
      return 0;
    } else {
      sciNrrd->nrrd_->axis[0].label = airStrdup( "x" );
      sciNrrd->nrrd_->axis[1].label = airStrdup( "y" );
      sciNrrd->nrrd_->axis[0].spacing = arr[0]->axis[0].spacing; 
      sciNrrd->nrrd_->axis[1].spacing = arr[0]->axis[1].spacing;

      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 0, nrrdCenterNode);
      nrrdAxisInfoMinMaxSet(sciNrrd->nrrd_, 1, nrrdCenterNode);
    }
    break;
  default:
    return 0;
  }

  return sciNrrd;
}

#endif 

} // End namespace SCITeem
