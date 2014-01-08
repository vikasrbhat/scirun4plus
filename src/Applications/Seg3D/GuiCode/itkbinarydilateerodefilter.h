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

/////////////////////////////////////////////////////////////////////////////
// Name:        itkbinarydilateerodefilter.h
// Purpose:     
// Author:      
// Modified by: 
// Created:     14/08/2007 13:07:39
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 14/08/2007 13:07:39

#ifndef _ITKBINARYDILATEERODEFILTER_H_
#define _ITKBINARYDILATEERODEFILTER_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/xrc/xmlres.h"
#include "wx/spinctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_ITKBINARYDILATEERODEFILTER 10012
#define SYMBOL_ITKBINARYDILATEERODEFILTER_STYLE 0
#define SYMBOL_ITKBINARYDILATEERODEFILTER_TITLE _("ITKBinaryDilateErodeFilter")
#define SYMBOL_ITKBINARYDILATEERODEFILTER_IDNAME ID_ITKBINARYDILATEERODEFILTER
#define SYMBOL_ITKBINARYDILATEERODEFILTER_SIZE wxSize(200, -1)
#define SYMBOL_ITKBINARYDILATEERODEFILTER_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * ITKBinaryDilateErodeFilter class declaration
 */

class ITKBinaryDilateErodeFilter: public wxPanel
{    
  DECLARE_DYNAMIC_CLASS( ITKBinaryDilateErodeFilter )
    DECLARE_EVENT_TABLE()

    public:
  /// Constructors
  ITKBinaryDilateErodeFilter();
  ITKBinaryDilateErodeFilter( wxWindow* parent, wxWindowID id = SYMBOL_ITKBINARYDILATEERODEFILTER_IDNAME, const wxPoint& pos = SYMBOL_ITKBINARYDILATEERODEFILTER_POSITION, const wxSize& size = SYMBOL_ITKBINARYDILATEERODEFILTER_SIZE, long style = SYMBOL_ITKBINARYDILATEERODEFILTER_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ITKBINARYDILATEERODEFILTER_IDNAME, const wxPoint& pos = SYMBOL_ITKBINARYDILATEERODEFILTER_POSITION, const wxSize& size = SYMBOL_ITKBINARYDILATEERODEFILTER_SIZE, long style = SYMBOL_ITKBINARYDILATEERODEFILTER_STYLE );

  /// Destructor
  ~ITKBinaryDilateErodeFilter();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();
  void OnDilateButtonClick( wxCommandEvent& event );
  void OneErodeButtonClick( wxCommandEvent& event );
  void OnDilateErodeButtonClick( wxCommandEvent& event );
  void OnCloseButtonClick( wxCommandEvent& event );
  void OnSetProgress( wxCommandEvent& event );
  void OnSet2D( wxCommandEvent& event ) { mDimension_ = 2; };
  void OnSet3D( wxCommandEvent& event ) { mDimension_ = 3; };

  ////@begin ITKBinaryDilateErodeFilter member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );
  ////@end ITKBinaryDilateErodeFilter member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  ////@begin ITKBinaryDilateErodeFilter member variables
  wxSpinCtrl* mDilateRadius;
  wxSpinCtrl* mErodeRadius;

  wxGauge* mPercentage;
  ////@end ITKBinaryDilateErodeFilter member variables

  wxWindowDisabler *disabler_;

  unsigned int mDimension_;
};

#endif
// _ITKBINARYDILATEERODEFILTER_H_
