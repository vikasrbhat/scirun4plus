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

#include <sstream>
#include <string>
#include <vector>

#include <Dataflow/Network/Module.h>

#include <Core/Datatypes/Bundle.h>
#include <Core/Datatypes/String.h>

#include <Dataflow/Network/Ports/BundlePort.h>
#include <Dataflow/Network/Ports/StringPort.h>

#include <Core/Matlab/matlabfile.h>
#include <Core/Matlab/matlabarray.h>
#include <Core/Matlab/matlabconverter.h>
#include <Dataflow/GuiInterface/GuiVar.h>
#include <Core/Datatypes/MatrixTypeConverter.h>


namespace MatlabIO {

using namespace SCIRun;

class ExportBundlesToMatlab : public Module 
{

  public:

    // Constructor
    ExportBundlesToMatlab(GuiContext*);

    // Destructor
    virtual ~ExportBundlesToMatlab();

    // Std functions for each module
    // execute():
    //   Execute the module and put data on the output port
    
    virtual void execute();
    
  private:

    // Support functions for converting between TCL and C++
    // converttcllist:
    // TCL lists are stings in which the elements are separated by {}.
    // In a C++ environment it is easier to have them as a STL vector
    // object. This function converts the TCL list into an STL object
    // convertdataformat:
    // In TCL the dataformat is a string this function is used to convert
    // it into its matlab enum counterpart.
    std::vector<std::string> converttcllist(std::string str);
    
    // Support functions for the GUI
    // displayerror:
    // Directly reporting an error to the user (not in the error log)
    // overwrite:
    // Ask for confirmation to overwrite the file if it already exists
    
    void displayerror(std::string str);
    bool overwrite();

  private:

    enum { NUMPORTS = 6};
    
    // GUI variables
    GuiFilename guifilename_;     // .mat filename (import from GUI)
    GuiString   guifilenameset_;
    GuiString   guimatrixname_;   // A list of the matrix names
    GuiString   guidataformat_;   // A list of the dataformat for each matrix (int, double etc.)
    GuiString   guimatrixformat_; // A list of the matlabarray format (numeric array, structured array)
    GuiInt      guioverwrite_;    // Overwrite matlab file
};

DECLARE_MAKER(ExportBundlesToMatlab)

// Constructor:
// Initialise all the variables shared between TCL and SCIRun
// matrixname contains a list of matrix names.
// dataformat contains a list of the format of each matrix (int32,single,double, etc...)
// matrixformat contains a list of the way the object is represented in matlab
// e.g. as a structured object or an object with the dataarray only

ExportBundlesToMatlab::ExportBundlesToMatlab(GuiContext* ctx)
  : Module("ExportBundlesToMatlab", ctx, Sink, "DataIO", "MatlabInterface"),
    guifilename_(get_ctx()->subVar("filename")),
    guifilenameset_(get_ctx()->subVar("filename-set", false)),
    guimatrixname_(get_ctx()->subVar("matrixname")),     
    guidataformat_(get_ctx()->subVar("dataformat")),    
	  guimatrixformat_(get_ctx()->subVar("matrixformat")),
    guioverwrite_(get_ctx()->subVar("overwrite"), 1)
{
}

// Destructor:
// All my objects have descrutors and hence nothing needs
// explicit descruction
ExportBundlesToMatlab::~ExportBundlesToMatlab()
{
}

// Execute:
// Inner workings of this module

void ExportBundlesToMatlab::execute()
{
  matlabconverter translate(dynamic_cast<SCIRun::ProgressReporter*>(this));

	StringHandle stringH;
	get_input_handle("Filename",stringH,false);
	if (stringH.get_rep())
	{
		std::string filename = stringH->get();
		guifilename_.set(filename);
		get_ctx()->reset();
	}

  
  bool porthasdata[NUMPORTS];
  SCIRun::BundleHandle matrixhandle[NUMPORTS];
  
  for (int p=0; p<NUMPORTS; p++)
  {
    porthasdata[p] = get_input_handle(p,matrixhandle[p],false);
  }

  // Reorder the TCL input and put them
  // in orderly STL style vectors.
  
  // First update the GUI to C++ interface
  TCLInterface::execute(get_id()+" Synchronise");
  get_ctx()->reset();

  // Get the contents of the filename entrybox
  std::string filename = guifilename_.get();

  // If the filename is empty, launch an error
  if (filename.empty())
  {
    error("ExportBundlesToMatlab: No file name was specified");
    return;
  }
  
  // Make sure we have a .mat extension
  size_t filenamesize = filename.size();
  if (filenamesize < 4) 
  {   
    filename += ".mat";
  }
  else
  {
    if (filename.substr(filenamesize-4,filenamesize) != ".mat") filename += ".mat";
  }

  if (! overwrite()) return;

  update_state(Executing);

  // get all the settings from the GUI
  
  std::vector<std::string> matrixname = converttcllist(guimatrixname_.get());
  std::vector<std::string> dataformat = converttcllist(guidataformat_.get());
  std::vector<std::string> matrixformat = converttcllist(guimatrixformat_.get());
  
  // Check the validity of the matrixnames

  for (int p=0;p<static_cast<int>(matrixname.size());p++)
  {
    if (porthasdata[p] == false) continue; // Do not check not used ports
    if (!translate.isvalidmatrixname(matrixname[p]))
    {
      error("ExportBundlesToMatlab: The matrix name specified is invalid");
      return;
    }
    for (int q=0;q<p;q++)
    {
      if (porthasdata[q] == false) continue;
      if (matrixname[q] == matrixname[p])
      {
        error("ExportBundlesToMatlab: A matrix name is used twice");
        return;
      }
    }
  }
  
  try
  {
    matlabfile mfile;   // matlab file object contains all function for reading and writing matlab arrayd
    matlabarray ma;		// matlab style formatted array (can be stored in a matlabfile object)
    mfile.open(filename,"w");   // open file for writing
    
    // Add an information tag to the data, so the origin of the file is known
    // There are 124 bytes of free data storage at the header of the file.
    // Do not start the file with 'SCI ', otherwise the file looks like a
    // native SCIRun file which uses the same extension.
    
    mfile.setheadertext("Matlab V5 compatible file generated by SCIRun [module ExportBundlesToMatlab version 1.1]");
    
    for (int p=0;p<NUMPORTS;p++)
    {
      if (porthasdata[p] == false) continue; // if the port is not connected skip to the next one
      
      // Convert the SCIRun matrixobject to a matlab object
      

      if (matrixformat[p] == "struct array")
      {   
        // translate the matrix into a matlab structured array, which
        // can also store some data from the property manager
        translate.converttostructmatrix();
      }

      if (matrixformat[p] == "numeric array")
      {
        // only store the numeric parts of the data
        translate.converttonumericmatrix();
      }

      translate.sciBundleTOmlArray(matrixhandle[p],ma);
              
      if (ma.isempty())
      {
        warning("One of the matrices is empty");
        continue; // Do not write empty matrices
      }
      // Every thing seems OK, so proceed and store the matrix in the file
      mfile.putmatlabarray(ma,matrixname[p]);
    }
    
    mfile.close();
  }	

  // in case something went wrong
  catch (matlabconverter::matlabconverter_error)
  {
      error("ExportBundlesToMatlab: Error in the SCIRun to Matlab converter");
  }          
  catch (matlabfile::could_not_open_file)
  {
      error("ExportBundlesToMatlab: Could not open file");
  }
  catch (matlabfile::invalid_file_format)
  {
      error("ExportBundlesToMatlab: Invalid file format");
  }
  catch (matlabfile::io_error)
  {   // IO error from ferror
      error("ExportBundlesToMatlab: IO error");
  }
  catch (matlabfile::unknown_type)
  {
    error("ExportBundlesToMatlab: Unknow type encountered");
  }
  catch (matlabfile::empty_matlabarray)
  {
    error("ExportBundlesToMatlab: Empty Matlab array encountered");  
  }
  catch (matlabfile::out_of_range)
  {
    error("ExportBundlsToMatlab: Out of Range error");
  }
  catch (matlabfile::invalid_file_access)
  {
    error("ExportBundlesToMatlab: Invalid file access");
  }
  catch (matlabfile::compression_error)
  { 
    error("ExportBundlesToMatlab: Compression error");
  }  
  catch (matlabfile::matfileerror) 
  {   // All other errors are classified as internal
      // matfileerrror is the base class on which all
      // other exceptions are based.
      error("ExportBundlesToMatlab: Internal error in writer");
  }
  // No handling of the SCIRun errors here yet, most SCIRun functions used
  // do not use exceptions yet.
}

// Additional support functions :
// To help coordinate between the GUI in TCL and
// the functions in this module on the C++ site.
// Some of the following functions are TCL specific!

// converttcllist:
// converts a TCL formatted list into a STL array
// of strings

std::vector<std::string> ExportBundlesToMatlab::converttcllist(std::string str)
{
  std::string result;
  std::vector<std::string> list(0);
  int lengthlist = 0;
  
  // Yeah, it is TCL dependent:
  // TCL::llength determines the length of the list
  TCLInterface::eval("llength { "+str + " }",result);	
  std::istringstream iss(result);
  iss >> lengthlist;
  if (lengthlist < 0) return(list);
  
  list.resize(lengthlist);
  for (int p = 0;p<lengthlist;p++)
  {
    std::ostringstream oss;
    // TCL dependency:
    // TCL::lindex retrieves the p th element from the list
    oss << "lindex { " << str <<  " } " << p;
    TCLInterface::eval(oss.str(),result);
    list[p] = result;
  }
  return(list);
}

// overwrite:
// Ask the user whether the file should be overwritten

bool ExportBundlesToMatlab::overwrite()
{
  std::string result;
  // call overwrite Tcl function
  TCLInterface::eval(get_id()+" overwrite",result);
  if (result == std::string("0")) 
  {
    warning("User chose to not save.");
    return false;
  }
  return true;
}

// displayerror:
// This function should be replaced with a more
// general function in SCIRun for displaying errors

void ExportBundlesToMatlab::displayerror(std::string str)
{
  // Explicit call to TCL
  TCLInterface::execute("tk_messageBox -icon error -type ok -title {ERROR} -message {" + str + "}");
}


} // End namespace MatlabIO
