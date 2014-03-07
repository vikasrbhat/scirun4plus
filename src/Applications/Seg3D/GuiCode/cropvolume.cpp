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
// Name:        cropvolume.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     20/08/2007 08:51:42
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 20/08/2007 08:51:42

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <Applications/Seg3D/Painter.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "cropvolume.h"
#include "seg3devents.h"

////@begin XPM images
////@end XPM images


/*!
 * CropVolume type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CropVolume, wxPanel )


/*!
 * CropVolume event table definition
 */

BEGIN_EVENT_TABLE( CropVolume, wxPanel )

////@begin CropVolume event table entries
////@end CropVolume event table entries
    EVT_BUTTON( XRCID("START_BUTTON"), CropVolume::OnStartButtonClick )
    EVT_BUTTON( XRCID("CLOSE_BUTTON"), CropVolume::OnCloseButtonClick )
    EVT_COMMAND (wxID_ANY, wxEVT_CROP_SET_MINMAX, CropVolume::OnCropSetMinMax )
    EVT_COMMAND (wxID_ANY, wxEVT_CROP_SET_RANGE, CropVolume::OnCropSetRange )
    EVT_SPINCTRL( XRCID("SPIN_MIN_X"), CropVolume::OnSpinMinX)
    EVT_SPINCTRL( XRCID("SPIN_MAX_X"), CropVolume::OnSpinMaxX)
    EVT_SPINCTRL( XRCID("SPIN_MIN_Y"), CropVolume::OnSpinMinY)
    EVT_SPINCTRL( XRCID("SPIN_MAX_Y"), CropVolume::OnSpinMaxY)
    EVT_SPINCTRL( XRCID("SPIN_MIN_Z"), CropVolume::OnSpinMinZ)
    EVT_SPINCTRL( XRCID("SPIN_MAX_Z"), CropVolume::OnSpinMaxZ)

END_EVENT_TABLE()


/*!
 * CropVolume constructors
 */

CropVolume::CropVolume()
{
    Init();
}

CropVolume::CropVolume( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * CropVolume creator
 */

bool CropVolume::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CropVolume creation
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
////@end CropVolume creation
    return true;
}


/*!
 * CropVolume destructor
 */

CropVolume::~CropVolume()
{
////@begin CropVolume destruction
////@end CropVolume destruction
}


/*!
 * Member initialisation
 */

void CropVolume::Init()
{
////@begin CropVolume member initialisation
    mSpinMinX = NULL;
    mSpinMaxX = NULL;
    mSpinMinY = NULL;
    mSpinMaxY = NULL;
    mSpinMinZ = NULL;
    mSpinMaxZ = NULL;
////@end CropVolume member initialisation
}


/*!
 * Control creation for CropVolume
 */

void CropVolume::CreateControls()
{    
////@begin CropVolume content construction
    if (!wxXmlResource::Get()->LoadPanel(this, GetParent(), _T("ID_CROPVOLUME")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    mSpinMinX = XRCCTRL(*this, "SPIN_MIN_X", wxSpinCtrl);
    mSpinMaxX = XRCCTRL(*this, "SPIN_MAX_X", wxSpinCtrl);
    mSpinMinY = XRCCTRL(*this, "SPIN_MIN_Y", wxSpinCtrl);
    mSpinMaxY = XRCCTRL(*this, "SPIN_MAX_Y", wxSpinCtrl);
    mSpinMinZ = XRCCTRL(*this, "SPIN_MIN_Z", wxSpinCtrl);
    mSpinMaxZ = XRCCTRL(*this, "SPIN_MAX_Z", wxSpinCtrl);
////@end CropVolume content construction

    // Set up a reasonable default range.
    mSpinMinX->SetRange(0, 0x7fffffff);
    mSpinMinY->SetRange(0, 0x7fffffff);
    mSpinMinZ->SetRange(0, 0x7fffffff);
    mSpinMaxX->SetRange(1, 0x7fffffff);
    mSpinMaxY->SetRange(1, 0x7fffffff);
    mSpinMaxZ->SetRange(1, 0x7fffffff);

    // Create custom windows not generated automatically here.
////@begin CropVolume content initialisation
////@end CropVolume content initialisation
}

void CropVolume::OnCropSetMinMax( wxCommandEvent &event )
{
    CropSetRangeStruct *csrs = (CropSetRangeStruct *)event.GetClientData();

    mSpinMinX->SetValue(csrs->min_x);
    mSpinMinY->SetValue(csrs->min_y);
    mSpinMinZ->SetValue(csrs->min_z);
    mSpinMaxX->SetValue(csrs->max_x);
    mSpinMaxY->SetValue(csrs->max_y);
    mSpinMaxZ->SetValue(csrs->max_z);
}


void CropVolume::OnCropSetRange( wxCommandEvent &event )
{
  CropSetRangeStruct *csrs = (CropSetRangeStruct *)event.GetClientData();

  mSpinMinX->SetRange(0, csrs->min_x);
  mSpinMinY->SetRange(0, csrs->min_y);
  mSpinMinZ->SetRange(0, csrs->min_z);
  mSpinMaxX->SetRange(1, csrs->max_x);
  mSpinMaxY->SetRange(1, csrs->max_y);
  mSpinMaxZ->SetRange(1, csrs->max_z);
}

void CropVolume::OnSpinMinX( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::min::x",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

void CropVolume::OnSpinMaxX( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::max::x",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

void CropVolume::OnSpinMinY( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::min::y",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

void CropVolume::OnSpinMaxY( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::max::y",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

void CropVolume::OnSpinMinZ( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::min::z",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

void CropVolume::OnSpinMaxZ( wxSpinEvent &event )
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::RedrawAll");
  tsse->add_var("Painter::crop::max::z",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for START_BUTTON
 */
void CropVolume::OnStartButtonClick( wxCommandEvent& event )
{
  wxBeginBusyCursor();
  SCIRun::Painter::ThrowSkinnerSignal("Painter::FinishTool");
  wxEndBusyCursor();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for CLOSE_BUTTON
 */
void CropVolume::OnCloseButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::global_seg3dframe_pointer_->HideTool();
}

/*!
 * Should we show tooltips?
 */

bool CropVolume::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CropVolume::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CropVolume bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CropVolume bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CropVolume::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CropVolume icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CropVolume icon retrieval
}
