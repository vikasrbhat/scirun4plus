#
#  For more information, please see: http://software.sci.utah.edu
# 
#  The MIT License
# 
#  Copyright (c) 2009 Scientific Computing and Imaging Institute,
#  University of Utah.
# 
#  
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#  and/or sell copies of the Software, and to permit persons to whom the
#  Software is furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included
#  in all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
#  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
#  DEALINGS IN THE SOFTWARE.
#

# Sources of Core/Skinner classes

SET(Core_Skinner_SRCS
  Animation.cc
  Arithmetic.cc
  Arrow.cc
  Arc.cc
  Box.cc
  Checkerboard.cc
  Collection.cc
  Color.cc
  ColorMap1D.cc
  ColorPicker.cc
  Drawable.cc
  EventProcessor.cc
  FileBrowser.cc
  FocusGrab.cc
  FocusRegion.cc
  Frame.cc
  Gradient.cc
  Graph2D.cc
  GeomSkinnerVarSwitch.cc
  Grid.cc
  Histogram.cc
  Layout.cc
  MenuButton.cc
  MenuManager.cc
  MenuList.cc
  Parent.cc
  Progress.cc
  RectRegion.cc
  RenderBuffer.cc
  SceneGraph.cc
  Signals.cc
  Skinner.cc
  Slider.cc
  SliderButton.cc
  Text.cc
  TextEntry.cc
  Texture.cc
  TransferFunction2D.cc
  Window.cc
  Variables.cc
  VisibilityGroup.cc
  ViewSubRegion.cc
  Volume.cc
  WindowRedraw.cc
  XMLIO.cc    
)

SCIRUN_ADD_LIBRARY(Core_Skinner ${Core_Skinner_SRCS})

TARGET_LINK_LIBRARIES(Core_Skinner 
  Core_Datatypes
  Core_Events
  Core_Events_OGLView
  Core_Exceptions
  Core_Geom  
  Core_Geometry  
  Core_Math  
  Core_Persistent  
  Core_Thread 
  Core_Util 
  Core_XMLUtil
  ${DL_LIBRARY} 
  ${THREAD_LIBRARY} 
  ${GL_LIBRARY}
  ${SCI_TEEM_LIBRARY} 
  ${SCI_LIBXML2_LIBRARY}
  ${SCI_BOOST_LIBRARY}
)

IF(APPLE_LEOPARD)
  SET_TARGET_PROPERTIES(Core_Skinner PROPERTIES LINK_FLAGS  "${GL_LINK}")
ENDIF(APPLE_LEOPARD)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DBUILD_Core_Skinner)
ENDIF(BUILD_SHARED_LIBS)

