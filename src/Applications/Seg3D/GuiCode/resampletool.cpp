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
// Name:        resampletool.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     28/08/2007 06:41:39
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 28/08/2007 06:41:39

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <Applications/Seg3D/Painter.h>
#include <Applications/Seg3D/Seg3DwxGuiUtils.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "resampletool.h"

////@begin XPM images
////@end XPM images


/*!
 * ResampleTool type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ResampleTool, wxPanel )


/*!
 * ResampleTool event table definition
 */

BEGIN_EVENT_TABLE( ResampleTool, wxPanel )

////@begin ResampleTool event table entries
////@end ResampleTool event table entries
EVT_BUTTON( XRCID("START_BUTTON"), ResampleTool::OnStartButtonClick )
EVT_BUTTON( XRCID("CLOSE_BUTTON"), ResampleTool::OnCloseButtonClick )

END_EVENT_TABLE()


/*!
 * ResampleTool constructors
 */

  ResampleTool::ResampleTool()
{
  Init();
}

ResampleTool::ResampleTool( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, pos, size, style);
}


/*!
 * ResampleTool creator
 */

bool ResampleTool::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  ////@begin ResampleTool creation
  SetParent(parent);
  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
  ////@end ResampleTool creation
  return true;
}


/*!
 * ResampleTool destructor
 */

ResampleTool::~ResampleTool()
{
  ////@begin ResampleTool destruction
  ////@end ResampleTool destruction
}


/*!
 * Member initialisation
 */

void ResampleTool::Init()
{
  ////@begin ResampleTool member initialisation
  mX = NULL;
  mY = NULL;
  mZ = NULL;
  mFilter = NULL;
  ////@end ResampleTool member initialisation
}


/*!
 * Control creation for ResampleTool
 */

void ResampleTool::CreateControls()
{    
  ////@begin ResampleTool content construction
  if (!wxXmlResource::Get()->LoadPanel(this, GetParent(), _T("ID_RESAMPLETOOL")))
    wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
  mX = XRCCTRL(*this, "ID_TEXTCTRL5", wxSpinCtrl);
  mY = XRCCTRL(*this, "ID_TEXTCTRL6", wxSpinCtrl);
  mZ = XRCCTRL(*this, "ID_TEXTCTRL7", wxSpinCtrl);
  mFilter = XRCCTRL(*this, "ID_CHOICE1", wxChoice);
  ////@end ResampleTool content construction

  // Create custom windows not generated automatically here.

  // Set the default value and range for the spinners.
  mX->SetRange(1, 0x7fffffff);
  mY->SetRange(1, 0x7fffffff);
  mZ->SetRange(1, 0x7fffffff);
  mX->SetValue(128);
  mY->SetValue(128);
  mZ->SetValue(128);

  ////@begin ResampleTool content initialisation
////@end ResampleTool content initialisation
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for START_BUTTON
 */
void ResampleTool::OnStartButtonClick( wxCommandEvent& event )
{
  wxBeginBusyCursor();
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::FinishTool");
  tsse->add_var( "Painter::Resample::x", SCIRun::to_string( mX->GetValue() ) );
  tsse->add_var( "Painter::Resample::y", SCIRun::to_string( mY->GetValue() ) );
  tsse->add_var( "Painter::Resample::z", SCIRun::to_string( mZ->GetValue() ) );
  tsse->add_var( "Painter::Resample::filter", wx2std( mFilter->GetStringSelection() ) );
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
  wxEndBusyCursor();

  SCIRun::Painter::global_seg3dframe_pointer_->HideTool();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for CLOSE_BUTTON
 */
void ResampleTool::OnCloseButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::global_seg3dframe_pointer_->HideTool();
}

/*!
 * Should we show tooltips?
 */

bool ResampleTool::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ResampleTool::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ResampleTool bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ResampleTool bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ResampleTool::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ResampleTool icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ResampleTool icon retrieval
}