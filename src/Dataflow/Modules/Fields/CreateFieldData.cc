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


// Include all code for the dynamic engine
#include <Core/Datatypes/String.h>
#include <Core/Datatypes/Matrix.h>
#include <Core/Datatypes/Field.h>
#include <Core/Parser/ArrayMathEngine.h>

#include <Dataflow/Network/Module.h>
#include <Dataflow/Network/Ports/MatrixPort.h>
#include <Dataflow/Network/Ports/FieldPort.h>
#include <Dataflow/Network/Ports/StringPort.h>

namespace SCIRun {

class CreateFieldData : public Module {
  public:
    CreateFieldData(GuiContext*);
    virtual ~CreateFieldData() {}
    virtual void execute();
    virtual void presave();

    virtual void tcl_command(GuiArgs&, void*);

  private:
    GuiString guifunction_;     // function code
    GuiString guiformat_;       // scalar, vector, or tensor ?
    GuiString guibasis_;       // constant, linear, quadratic, ....

};


DECLARE_MAKER(CreateFieldData)
CreateFieldData::CreateFieldData(GuiContext* ctx)
  : Module("CreateFieldData", ctx, Source, "ChangeFieldData", "SCIRun"),
  guifunction_(get_ctx()->subVar("function")),
  guiformat_(get_ctx()->subVar("format")),  
  guibasis_(get_ctx()->subVar("basis"))
{
}


void
CreateFieldData::execute()
{
  FieldHandle field;
  StringHandle func;
  std::vector<MatrixHandle> matrices;
  
  get_input_handle("Field",field,true);
  if (get_input_handle("Function",func,false))
  {
    if (func.get_rep())
    {
      guifunction_.set(func->get());
      get_ctx()->reset();  
    }
  }
  get_dynamic_input_handles("DataArray",matrices,false);

  TCLInterface::eval(get_id()+" update_text");
  
  if (inputs_changed_ || guiformat_.changed() || guibasis_.changed() 
                    || guifunction_.changed() || !oport_cached("Field"))
  {
    update_state(Executing);
    
    // Get number of matrix ports with data (the last one is always empty)
    size_t numinputs = matrices.size();
    if (numinputs > 23)
    {
      error("This module cannot handle more than 23 input matrices.");
      return;
    }

    NewArrayMathEngine engine;
    engine.set_progress_reporter(this);
    
    std::string format = guiformat_.get();
    if (format == "") format = "double";
    std::string basis = guibasis_.get();
    if (basis == "") basis = "Linear";

    // Add as well the output object
    int basis_order = 0;
    if (basis == "Linear") basis_order = 1;

    if(!(engine.add_input_fielddata_location("POS",field,basis_order))) return;

    if(!(engine.add_input_fielddata_coordinates("X","Y","Z",field,basis_order))) return;

    if(!(engine.add_input_fielddata_element("ELEMENT",field,basis_order))) return;

    // Loop through all matrices and add them to the engine as well
    char mname = 'A';
    std::string matrixname("A");
  
    for (size_t p = 0; p < matrices.size(); p++)
    {
      if (matrices[p].get_rep() == 0)
      {
        error("No matrix was found on input port.");
        return;      
      }

      matrixname[0] = mname++;
      if (!(engine.add_input_matrix(matrixname,matrices[p]))) return;
    }

    if(!(engine.add_output_fielddata("RESULT",field,basis_order,format))) return;

    // Add an object for getting the index and size of the array.

    if(!(engine.add_index("INDEX"))) return;
    if(!(engine.add_size("SIZE"))) return;

    std::string function = guifunction_.get();
    if(!(engine.add_expressions(function))) return;
    
    // Actual engine call, which does the dynamic compilation, the creation of the
    // code for all the objects, as well as inserting the function and looping 
    // over every data point

    if (!(engine.run())) return;
    
    // Get the result from the engine
    FieldHandle ofield;    
    engine.get_field("RESULT",ofield);

    // send new output if there is any: 
    send_output_handle("Field", ofield);
  }
}


void
CreateFieldData::tcl_command(GuiArgs& args, void* userdata)
{
  if(args.count() < 2)
  {
    args.error("CreateDataField needs a minor command");
    return;
  }

  if( args[1] == "gethelp" )
  {
    return;
  }
  else
  {
    Module::tcl_command(args, userdata);
  }
}

void
CreateFieldData::presave()
{
  // update gui_function_ before saving.
  TCLInterface::execute(get_id() + " update_text");
}


} // End namespace SCIRun


