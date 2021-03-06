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
 *  FieldSetProperty: Set a property for a Field
 *
 *  Written by:
 *   David Weinstein
 *   Department of Computer Science
 *   University of Utah
 *   June 1999
 *
 */

#include <Dataflow/Network/Module.h>
#include <Core/Util/StringUtil.h>
#include <Core/Containers/Handle.h>
#include <iostream>

namespace SCIRun {

template< class HANDLE_TYPE >
class SetPropertyModule : public Module {

public:
  SetPropertyModule(const std::string& module_name,
		    GuiContext* ctx, SchedClass,
		    const std::string& catagory  = "unknown",
		    const std::string& package   = "unknown",
		    const std::string& port_name = "unknown" );

  virtual ~SetPropertyModule();
  virtual void execute();

private:
  std::string port_name_;

  GuiInt    gui_entries_;
  GuiString gui_property_;
  GuiString gui_type_;
  GuiString gui_value_;
  GuiInt    gui_read_only_;

  std::vector< GuiString* > gui_properties_;
  std::vector< GuiString* > gui_types_;
  std::vector< GuiString* > gui_values_;
  std::vector< GuiInt*    > gui_read_only_flags_;

  std::vector< std::string > properties_;
  std::vector< std::string > types_;
  std::vector< std::string > values_;
  std::vector< int    > read_only_flags_;

  HANDLE_TYPE output_handle_;

  bool execute_error_;
};

template< class HANDLE_TYPE >
SetPropertyModule< HANDLE_TYPE >::SetPropertyModule(const std::string& module_name,
						    GuiContext* context,
						    SchedClass,
						    const std::string& catagory,
						    const std::string& package,
						    const std::string& port_name )
  : Module(module_name, context, Filter, catagory, package),
    port_name_(port_name),
    gui_entries_(context->subVar("num-entries"), 0),
    gui_property_(context->subVar("property"), ""),
    gui_type_(context->subVar("type"), "unknown"),
    gui_value_(context->subVar("value"), ""),
    gui_read_only_(context->subVar("readonly"), 0),
    output_handle_(0),
    execute_error_(false)
{
}


template< class HANDLE_TYPE >
SetPropertyModule< HANDLE_TYPE >::~SetPropertyModule()
{
}

template< class HANDLE_TYPE >
void
SetPropertyModule< HANDLE_TYPE >::execute()
{
  HANDLE_TYPE input_handle;

  if( !get_input_handle( port_name_, input_handle, true ) )
    return;

  //! New field so update the GUI
  if( inputs_changed_ ) 
  {
    //! Add the current properties to the display.
    for( unsigned int ic=0; ic<input_handle->nproperties(); ic++ ) 
    {
      bool           p_bool;
      unsigned char  p_uchar;
      char           p_char;
      unsigned short p_ushort;
      short          p_short;
      unsigned int   p_uint;
      int            p_int;
      float          p_float;
      double         p_double;
      std::string         p_string;

      std::string pname = input_handle->get_property_name( ic );
      std::string type("other");
      std::string value("Can not display");
      int readonly = 1;

      if( input_handle->get_property( pname, p_bool ) ) 
      {
        value = to_string(p_bool);
        type = "bool";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_uchar ) ) 
      {
        value = to_string(p_uchar);
        type = "unsigned char";
      } 
      else if( input_handle->get_property( pname, p_char ) ) 
      {
        value = to_string(p_char);
        type = "char";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_ushort ) ) 
      {
        value = to_string(p_ushort);
        type = "unsigned short";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_short ) ) 
      {
        value = to_string(p_short);
        type = "short";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_uint ) ) 
      {
        value = to_string(p_uint);
        type = "unsigned int";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_int ) ) 
      {
        value = to_string(p_int);
        type = "int";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_float ) ) 
      {
        value = to_string(p_float);
        type = "float";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_double ) ) 
      {
        value = to_string(p_double);
        type = "double";
        readonly = 0;
      } 
      else if( input_handle->get_property( pname, p_string ) ) 
      {
        type = "string";
        value = p_string;
        readonly = 0;
      }

      std::ostringstream str;
      str << get_id() << " setEntry {";
      str << pname << "} {";
      str << type << "} {";
      str << value << "} ";
      str << readonly << " ";

      TCLInterface::execute(str.str());
    }
  }

  // Save off the defaults
  if( gui_entries_.changed( true ) ) 
  {
    inputs_changed_ = true;
  }

  int entries = gui_properties_.size();         // # GUI vars entries

  // Remove the GUI entries that are not needed.
  for( int ic=entries-1; ic>=gui_entries_.get(); ic-- ) 
  {
    delete( gui_properties_[ic] );
    delete( gui_types_[ic] );
    delete( gui_values_[ic] );
    delete( gui_read_only_flags_[ic] );

    gui_properties_.pop_back();
    gui_types_.pop_back();
    gui_values_.pop_back();
    gui_read_only_flags_.pop_back();

    properties_.pop_back();
    types_.pop_back();
    values_.pop_back();
    read_only_flags_.pop_back();
  }

  // Add new GUI entries that are needed.
  for( int ic=entries; ic<gui_entries_.get(); ic++ ) 
  {
    char idx[24];

    sprintf( idx, "property-%d", ic );
    gui_properties_.push_back(new GuiString(get_ctx()->subVar(idx)) );

    sprintf( idx, "type-%d", ic );
    gui_types_.push_back(new GuiString(get_ctx()->subVar(idx)) );

    sprintf( idx, "value-%d", ic );
    gui_values_.push_back(new GuiString(get_ctx()->subVar(idx)) );

    sprintf( idx, "readonly-%d", ic );
    gui_read_only_flags_.push_back(new GuiInt(get_ctx()->subVar(idx)) );

    properties_.push_back("");
    types_.push_back("");
    values_.push_back("");
    read_only_flags_.push_back(0);
  }


  // Look through the properties to see if any have changed.
  for( int ic=0; ic<gui_entries_.get(); ic++ ) 
  {
    gui_properties_[ic]->reset();
    gui_types_[ic]->reset();
    gui_values_[ic]->reset();

    std::string tmpStr;

    // Compare with the current stored properties.
    tmpStr = gui_properties_[ic]->get();
    if( tmpStr != properties_[ic] ) 
    {
      properties_[ic] = tmpStr;
      inputs_changed_ = true;
    }

    tmpStr = gui_types_[ic]->get();
    if( tmpStr != types_[ic] ) 
    {
      types_[ic] = tmpStr;
      inputs_changed_ = true;
    }

    tmpStr = gui_values_[ic]->get();
    if( tmpStr != values_[ic] ) 
    {
      values_[ic] = tmpStr;
      inputs_changed_ = true;
    }

    // Compare with the current properties.
    bool           p_bool;
    unsigned char  p_uchar;
    char           p_char;
    unsigned short p_ushort;
    short          p_short;
    unsigned int   p_uint;
    int            p_int;
    float          p_float;
    double         p_double;
    std::string         p_string;

    if( types_[ic] == std::string( "bool" ) &&
      input_handle->get_property( properties_[ic], p_bool ) ) 
    {
      bool val; from_string(values_[ic],val);
      if( p_bool != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "unsigned char" ) &&
      input_handle->get_property( properties_[ic], p_uchar ) ) 
    {
      unsigned char val; from_string(values_[ic],val);
      if( p_uchar != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( " char" ) &&
      input_handle->get_property( properties_[ic], p_char ) ) 
    {
      char val; from_string(values_[ic],val);
      if( p_char != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "unsigned short" ) &&
      input_handle->get_property( properties_[ic], p_ushort ) ) 
    {
      unsigned short val; from_string(values_[ic],val);
      if( p_ushort != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "short" ) &&
      input_handle->get_property( properties_[ic], p_short ) ) 
    {
      short val; from_string(values_[ic],val);
      if( p_short != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "unsigned int" ) &&
      input_handle->get_property( properties_[ic], p_uint ) ) 
    {
      unsigned int val; from_string(values_[ic],val);
      if( p_uint != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "int" ) &&
      input_handle->get_property( properties_[ic], p_int ) ) 
    {
      int val; from_string(values_[ic],val);
      if( p_int != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "float" ) &&
              input_handle->get_property( properties_[ic], p_float ) ) 
    {
      float val; from_string(values_[ic],val);
      if( p_float != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "double" ) &&
              input_handle->get_property( properties_[ic], p_double ) ) 
    {
      double val; from_string(values_[ic],val);
      if( p_double != val ) inputs_changed_ = true;
    } 
    else if( types_[ic] == std::string( "string" ) &&
                input_handle->get_property( properties_[ic], p_string ) ) 
    {
      if( p_string != values_[ic] ) inputs_changed_ = true;
    }
  }

  // Something changed so update the properties.
  if( inputs_changed_ ||
      !output_handle_.get_rep() ||
      execute_error_ == true  ) 
  {
    update_state(Executing);

    execute_error_ = false;

    for( int ic=0; ic<gui_entries_.get(); ic++ ) 
    {
      if( types_[ic] == "unsigned char" ) 
      {
        unsigned char p_uchar;
        from_string(values_[ic],p_uchar);
        input_handle->set_property(properties_[ic], p_uchar, false);
      } 
      else if( types_[ic] == "char" ) 
      {
        char p_char;
        from_string(values_[ic],p_char);
        input_handle->set_property(properties_[ic], p_char, false);
      } 
      else if( types_[ic] == "unsigned short" ) 
      {
        unsigned short p_ushort;
        from_string(values_[ic],p_ushort);
        input_handle->set_property(properties_[ic], p_ushort, false);
      } 
      else if( types_[ic] == "short" ) 
      {
        short p_short;
        from_string(values_[ic],p_short);
        input_handle->set_property(properties_[ic], p_short, false);
      } 
      else if( types_[ic] == "unsigned int" ) 
      {
        unsigned int p_uint;
        from_string(values_[ic],p_uint);
        input_handle->set_property(properties_[ic], p_uint, false);
      } 
      else if( types_[ic] == "int" ) 
      {
        int p_int;
        from_string(values_[ic],p_int);
        input_handle->set_property(properties_[ic], p_int, false);
      } 
      else if( types_[ic] == "float" ) 
      {
        float p_float;
        from_string(values_[ic],p_float);
        input_handle->set_property(properties_[ic], p_float, false);
      } 
      else if( types_[ic] == "double" ) 
      {
        double p_double;
        from_string(values_[ic],p_double);
        input_handle->set_property(properties_[ic], p_double, false);
      } 
      else if( types_[ic] == "string" ) 
      {
        std::string p_string = (std::string) values_[ic];
        input_handle->set_property(properties_[ic], p_string, false);
      } 
      else if( types_[ic] == "unknown" ) 
      {
        error( properties_[ic] + " has an unknown type" );
        execute_error_ = true;
        return;
      }
    }

    // Remove the deleted properties.
    for( unsigned int ic=0; ic<input_handle->nproperties(); ic++ ) {
      std::string pname = input_handle->get_property_name( ic );

      int jc;

      // If the property is in the entries keep it.
      for( jc=0; jc<gui_entries_.get(); jc++ ) {
	if( pname == properties_[jc] )
	  break;
      }

      // Otherwise if not found remove it.
      if( jc == gui_entries_.get() )
	input_handle->remove_property( pname );
    }

    // Update the handles and generation numbers.
    // NOTE: This is done in place, e.g. the field is not copied.
    output_handle_ = input_handle;
    output_handle_->generation = output_handle_->compute_new_generation();
  }

  // Send the data downstream
  send_output_handle( port_name_, output_handle_, true );
}

} // End namespace SCIRun
