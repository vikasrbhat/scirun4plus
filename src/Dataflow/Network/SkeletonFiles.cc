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


/* SkeletonFiles.cc */

namespace SCIRun {

char component_skeleton[] = \
"/*\n"
" * The MIT License\n"
" * \n"
" * Copyright (c) 2009 Scientific Computing and Imaging Institute,\n"
" * University of Utah.\n"
" * \n"
" * \n"
" * Permission is hereby granted, free of charge, to any person obtaining a\n"
" * copy of this software and associated documentation files (the \"Software\"),\n"
" * to deal in the Software without restriction, including without limitation\n"
" * the rights to use, copy, modify, merge, publish, distribute, sublicense,\n"
" * and/or sell copies of the Software, and to permit persons to whom the\n"
" * Software is furnished to do so, subject to the following conditions:\n"
" * \n"
" * The above copyright notice and this permission notice shall be included\n"
" * in all copies or substantial portions of the Software.\n"
" * \n"
" * THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n"
" * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
" * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL\n"
" * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
" * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING\n"
" * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n"
" * DEALINGS IN THE SOFTWARE.\n"
" */\n"
"\n"
"\n"
"/*\n"
" *  %s.cc:\n" /* component name */
" *\n"
" *  Written by:\n"
" *   %s\n" /* author name */
" *   %s\n"   /* today's date */
" *\n"
" */\n"
"\n"
"#include <Dataflow/Network/Module.h>\n"
"\n"
"\n"
"namespace %s {\n" /* package name */
"\n"
"using namespace SCIRun;\n"
"\n"
"class %s : public Module {\n" /* component name */
"" 
"public:\n"
"  %s(GuiContext*);\n" /* component name */
"\n"
"  virtual ~%s();\n" /* component name */
"\n"
"  virtual void execute();\n"
"\n"
"  virtual void tcl_command(GuiArgs&, void*);\n"
"};\n"
"\n"
"\n"
"DECLARE_MAKER(%s)\n" /* component name */
"\n"
"%s::%s(GuiContext* ctx) :\n" /* component name, component name */
"  Module(\"%s\", ctx, Source, \"%s\", \"%s\")\n" /* comp, cat, pack */
"{\n"
"}\n"
"\n"
"%s::~%s()" /* component name, component name */
"{\n"
"}\n"
"\n"
"void\n%s::execute()" /* component name */
"{\n"
"}\n"
"\n"
"void\n%s::tcl_command(GuiArgs& args, void* userdata)\n" /* component name */
"{\n"
"  Module::tcl_command(args, userdata);\n"
"}\n"
"\n"
"} // End namespace %s\n" /* package name */
"\n"
"\n";

char gui_skeleton[] = \
"itcl_class %s_%s_%s {\n" /* package name, category name, component name */
"    inherit Module\n"
"    constructor {config} {\n"
"        set name %s\n" /* component name */
"        set_defaults\n"
"    }\n"
"\n"
"    method set_defaults {} {\n"
"    }\n"
"\n"
"    method ui {} {\n"
"        set w .ui[modname]\n"
"        if {[winfo exists $w]} {\n"
"            return\n"
"        }\n"
"        toplevel $w\n"
"        label $w.row1 -text \"This GUI was"
" auto-generated by the Component Wizard.\"\n"
"        label $w.row2 -text {edit the file \"%s\" to modify it.}\n" /*file*/
"        pack $w.row1 $w.row2 -side top -padx 10 -pady 10\n"
"\n"
"        makeSciButtonPanel $w $w $this\n"
"        moveToCursor $w\n"
"    }\n"
"}\n"
"\n"
"\n";

char dllentry_skeleton[] = \
"/* DllEntry.cc */\n"
"\n"
"#ifdef _WIN32\n"
"\n"
"#include <windows.h>\n"
"#include <stdio.h>\n"
"\n"
"BOOL APIENTRY DllMain(HANDLE hModule, \n"
"                      DWORD  ul_reason_for_call, \n"
"                      LPVOID lpReserved)\n"
"{\n"
"#ifdef DEBUG\n"
"  char reason[100]=\"\\0\";\n"
"  printf(\"\\n*** %%sd.dll is initializing {%%s,%%d} ***\\n\",__FILE__,__LINE__);\n" /* package name */
"  printf(\"*** hModule = %%d ***\\n\",hModule);\n"
"  switch (ul_reason_for_call){\n"
"    case DLL_PROCESS_ATTACH:sprintf(reason,\"DLL_PROCESS_ATTACH\"); break;\n"
"    case DLL_THREAD_ATTACH:sprintf(reason,\"DLL_THREAD_ATTACH\"); break;\n"
"    case DLL_THREAD_DETACH:sprintf(reason,\"DLL_THREAD_DETACH\"); break;\n"
"    case DLL_PROCESS_DETACH:sprintf(reason,\"DLL_PROCESS_DETACH\"); break;\n"
"  }\n"
"  printf(\"*** ul_reason_for_call = %%s ***\\n\",reason);\n"
"#endif\n"
"  return TRUE;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"\n";

char package_cmakelists_skeleton[] = \
"\n"
"# CMakeLists.txt for Packages/%s\n"
"\n"
"ADD_SUBDIRECTORY( Core Dataflow )\n"
"\n";

char dataflow_cmakelists_skeleton[] = \
"\n"
"# CMakeLists.txt for Packages/%s/Dataflow\n"
"\n"
"ADD_SUBDIRECTORY( Modules )\n"
"\n";

char core_cmakelists_skeleton[] = \
"\n"
"# CMakeLists.txt for Packages/%s/Core\n"
"\n"
"ADD_SUBDIRECTORY( Datatypes )\n"
"\n";

char modules_cmakelists_skeleton[] = \
"\n"
"# CMakeLists.txt for %sDataflow/Modules\n"
"\n"
"ADD_SUBDIRECTORY()\n"
"\n"
"\n";

char category_cmakelists_skeleton[] = \
"# CMakeLists.txt for %sDataflow/Modules/s\n"
"\n"
"SET( %sDataflow_Modules_%s_SRCS\n"
")\n"
"\n"
"ADD_LIBRARY( %sDataflow_Modules_%s ${%sDataflow_Modules_%s_SRCS})\n"
"\n"
"TARGET_LINK_LIBRARIES( $(TK_LIBRARY) $(GL_LIBRARY) $(M_LIBRARY) )\n"
"\n"
"\n";

char datatypes_cmakelists_skeleton[] = \
"# Sources of %sCore/Datatypes classes\n"
"\n"
"SET(Packages_%s_Core_Datatypes_SRCS\n"
")\n"
"\n"
"ADD_LIBRARY(Packages_%s_Core_Datatypes ${Packages_%s_Core_Datatypes_SRCS})\n"
"\n";

} // End namespace SCIRun

