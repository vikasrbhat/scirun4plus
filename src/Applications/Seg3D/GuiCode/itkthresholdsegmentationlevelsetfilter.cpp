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
// Name:        itkthresholdsegmentationlevelsetfilter.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     Thu 18 Feb 10:43:35 2010
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Thu 18 Feb 10:43:35 2010

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <Applications/Seg3D/Painter.h>
#include <Applications/Seg3D/Seg3DFrame.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "itkthresholdsegmentationlevelsetfilter.h"
#include "wx/progdlg.h"
#include "seg3devents.h"

////@begin XPM images
////@end XPM images


/*!
 * ITKThresholdSegmentationLevelSetFilter type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ITKThresholdSegmentationLevelSetFilter, wxPanel )


/*!
 * ITKThresholdSegmentationLevelSetFilter event table definition
 */

BEGIN_EVENT_TABLE( ITKThresholdSegmentationLevelSetFilter, wxPanel )

////@begin ITKThresholdSegmentationLevelSetFilter event table entries
////@end ITKThresholdSegmentationLevelSetFilter event table entries
  EVT_BUTTON( XRCID("SET_DATALAYER_BUTTON"), ITKThresholdSegmentationLevelSetFilter::OnSetDataLayerButtonClick )
  EVT_BUTTON( XRCID("SET_THRESHOLDLAYER_BUTTON"), ITKThresholdSegmentationLevelSetFilter::OnSetThresholdLayerButtonClick )
  EVT_BUTTON( XRCID("START_BUTTON"), ITKThresholdSegmentationLevelSetFilter::OnStartButtonClick )
  EVT_BUTTON( XRCID("STOP_BUTTON"), ITKThresholdSegmentationLevelSetFilter::OnStopButtonClick )
  EVT_BUTTON( XRCID("CLOSE_BUTTON"), ITKThresholdSegmentationLevelSetFilter::OnCloseButtonClick )

  // Custom entries
  EVT_SPINCTRL( XRCID("SPIN_BRUSH_RADIUS"), ITKThresholdSegmentationLevelSetFilter::SpinRadius)
  EVT_COMMAND( wxID_ANY, wxEVT_BRUSH_RADIUS_CHANGE, ITKThresholdSegmentationLevelSetFilter::UpdateRadius)

  EVT_COMMAND( wxID_ANY, wxEVT_SET_PROGRESS, ITKThresholdSegmentationLevelSetFilter::OnSetProgress)

END_EVENT_TABLE()


/*!
 * ITKThresholdSegmentationLevelSetFilter constructors
 */

ITKThresholdSegmentationLevelSetFilter::ITKThresholdSegmentationLevelSetFilter()
{
    Init();
}

ITKThresholdSegmentationLevelSetFilter::ITKThresholdSegmentationLevelSetFilter( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, pos, size, style);
}


/*!
 * ITKThresholdSegmentationLevelSetFilter creator
 */

bool ITKThresholdSegmentationLevelSetFilter::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ITKThresholdSegmentationLevelSetFilter creation
    SetParent(parent);
    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
////@end ITKThresholdSegmentationLevelSetFilter creation
    return true;
}


/*!
 * ITKThresholdSegmentationLevelSetFilter destructor
 */

ITKThresholdSegmentationLevelSetFilter::~ITKThresholdSegmentationLevelSetFilter()
{
////@begin ITKThresholdSegmentationLevelSetFilter destruction
////@end ITKThresholdSegmentationLevelSetFilter destruction
}


/*!
 * Member initialisation
 */

void ITKThresholdSegmentationLevelSetFilter::Init()
{
////@begin ITKThresholdSegmentationLevelSetFilter member initialisation
    mRadius = NULL;
    mMaxIterations = NULL;
    mCurvatureScale = NULL;
    mEdgeWeight = NULL;
    mSmoothIterations = NULL;
    mSmoothConductance = NULL;
    mSmoothTimeStep = NULL;
    mReverseExpansion = NULL;
////@end ITKThresholdSegmentationLevelSetFilter member initialisation

    progress_dialog_ = NULL;
}


/*!
 * Control creation for ITKThresholdSegmentationLevelSetFilter
 */

void ITKThresholdSegmentationLevelSetFilter::CreateControls()
{    
////@begin ITKThresholdSegmentationLevelSetFilter content construction
    if (!wxXmlResource::Get()->LoadPanel(this, GetParent(), _T("ID_ITKTHRESHOLDSEGMENTATIONLEVELSETFILTER")))
        wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));
    mRadius = XRCCTRL(*this, "SPIN_BRUSH_RADIUS", wxSpinCtrl);
    mMaxIterations = XRCCTRL(*this, "SPIN_MAX_ITERATIONS", wxSpinCtrl);
    mCurvatureScale = XRCCTRL(*this, "SPIN_CURVATURE_SCALING", wxSpinCtrl);
    mEdgeWeight = XRCCTRL(*this, "SPIN_EDGE_WEIGHT", wxSpinCtrl);
    mSmoothIterations = XRCCTRL(*this, "SPIN_SMOOTHING_ITERATIONS", wxSpinCtrl);
    mSmoothConductance = XRCCTRL(*this, "SPIN_SMOOTHING_CONDUCTANCE", wxSpinCtrl);
    mSmoothTimeStep = XRCCTRL(*this, "TEXT_SMOOTHING_TIME_STEP", wxTextCtrl);
    mReverseExpansion = XRCCTRL(*this, "CHECK_REVERSE_EXPANSION", wxCheckBox);
////@end ITKThresholdSegmentationLevelSetFilter content construction

    // Set validators
    if (FindWindow(XRCID("TEXT_SMOOTHING_TIME_STEP")))
      FindWindow(XRCID("TEXT_SMOOTHING_TIME_STEP"))->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
    // Create custom windows not generated automatically here.
////@begin ITKThresholdSegmentationLevelSetFilter content initialisation
////@end ITKThresholdSegmentationLevelSetFilter content initialisation
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for SET_SEEDLAYER_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnSetSeedLayerButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::ThrowSkinnerSignal("Painter::SetLayer");
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for SET_DATALAYER_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnSetDataLayerButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::ThrowSkinnerSignal("Painter::SetDataLayer");
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for SET_THRESHOLDLAYER_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnSetThresholdLayerButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::ThrowSkinnerSignal("Painter::SetThresholdLayer");
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for CLOSE_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnCloseButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::global_seg3dframe_pointer_->HideTool();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for START_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnStartButtonClick( wxCommandEvent& event )
{
  aborted_ = false;
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::FinishTool");
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::NumberOfIterations",
		SCIRun::to_string(mMaxIterations->GetValue()));
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::CurvatureScaling",
                SCIRun::to_string(mCurvatureScale->GetValue()));
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::EdgeWeight",
                SCIRun::to_string(mEdgeWeight->GetValue()));
  if (mEdgeWeight->GetValue() > 0.0)
  {
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingIterations",
		  SCIRun::to_string(mSmoothIterations->GetValue()));
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingConductance",
		  SCIRun::to_string(mSmoothConductance->GetValue()));
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingTimeStep",
		  SCIRun::to_string(mSmoothTimeStep->GetValue()));
  }
  else 
  {
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingIterations",
		  SCIRun::to_string(0));
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingConductance",
		  SCIRun::to_string(0));
    tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::SmoothingTimeStep",
		  SCIRun::to_string(1));
  }
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::ReverseExpansionDirection",
                SCIRun::to_string(mReverseExpansion->GetValue()));
  
  // provide good defaults for the values not exposed to the GUI
  // Setting PropagationScaling to -1.0 here because a positive value was causing the surface to shrink
  // instead of grow with ITK 3.16.0.  ITK documentation suggests the opposite should happen.
  // So ideally this would be set to +1 instead of -1.
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::PropagationScaling",
               SCIRun::to_string(-1.0));
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::AdvectionScaling",
		SCIRun::to_string(0.0));
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::MaximumRMSError",
                SCIRun::to_string(0.0));
  tsse->add_var("ITKThresholdSegmentationLevelSetImageFilterTool::IsoSurfaceValue",
                SCIRun::to_string(0.5));

  SCIRun::Painter::ThrowSkinnerSignal(tsse, false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for STOP_BUTTON
 */
void ITKThresholdSegmentationLevelSetFilter::OnStopButtonClick( wxCommandEvent& event )
{
  SCIRun::Painter::ThrowSkinnerSignal("Painter::AbortFilterOn");
}


void
ITKThresholdSegmentationLevelSetFilter::SpinRadius( wxSpinEvent &event)
{
  SCIRun::ThrowSkinnerSignalEvent *tsse =
    new SCIRun::ThrowSkinnerSignalEvent("Painter::UpdateBrushRadius");
  tsse->add_var("Painter::brush_radius",
                SCIRun::to_string(event.GetPosition()));
  SCIRun::Painter::ThrowSkinnerSignal(tsse);
}


void
ITKThresholdSegmentationLevelSetFilter::UpdateRadius( wxCommandEvent &event)
{
  mRadius->SetValue(event.GetInt());
}


void
ITKThresholdSegmentationLevelSetFilter::OnSetProgress( wxCommandEvent &event)
{	
  int progress = event.GetInt();

  // start_progress() sends -1
  if (progress < 0)
  {
#ifdef __WX_GTK__
    progress_dialog_ =
      new wxProgressDialog(_T("Seg3D Filter Progress"),
                           _T(""), 100, NULL,
                           wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT );
    progress_dialog_->Update(0, _T("Initializing level set filter."));
#else
    progress_dialog_ =
      new wxProgressDialog(_T("Seg3D Filter Progress"),
                           _T("Initializing level set filter.          "), 100, NULL,
                           wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT );
#endif
  }
  // finish_progress() sends 101
  else if (progress > 100)
  {
    if (progress_dialog_) { delete progress_dialog_; progress_dialog_ = 0; }
  }
  else
  {
    if (progress_dialog_ && !aborted_)
    {
      if (!progress_dialog_->Pulse(_T("Running level set filter.")))
      {
        if (!aborted_)
        {
          SCIRun::Painter::ThrowSkinnerSignal("Painter::AbortFilterOn");
          aborted_ = true;
        }
      }
    }
  }
}


/*!
 * Should we show tooltips?
 */

bool ITKThresholdSegmentationLevelSetFilter::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ITKThresholdSegmentationLevelSetFilter::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ITKThresholdSegmentationLevelSetFilter bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ITKThresholdSegmentationLevelSetFilter bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ITKThresholdSegmentationLevelSetFilter::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ITKThresholdSegmentationLevelSetFilter icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ITKThresholdSegmentationLevelSetFilter icon retrieval
}