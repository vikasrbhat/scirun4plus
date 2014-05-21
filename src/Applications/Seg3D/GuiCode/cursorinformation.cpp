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
// Name:        cursorinformation.cpp
// Purpose:     
// Author:      
// Modified by: 
// Created:     21/08/2007 15:01:49
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 21/08/2007 15:01:49

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

#include "cursorinformation.h"
#include "seg3devents.h"

////@begin XPM images
////@end XPM images


/*!
 * CursorInformation type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CursorInformation, wxPanel )


/*!
 * CursorInformation event table definition
 */

BEGIN_EVENT_TABLE( CursorInformation, wxPanel )
EVT_COMMAND (wxID_ANY, wxEVT_CURSOR_INFORMATION_CHANGE, CursorInformation::SetMousePositionAndValue)

////@begin CursorInformation event table entries
////@end CursorInformation event table entries

END_EVENT_TABLE()


/*!
 * CursorInformation constructors
 */

CursorInformation::CursorInformation()
{
    Init();
}

CursorInformation::CursorInformation( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, pos, size, style);
}


/*!
 * CursorInformation creator
 */

bool CursorInformation::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  ////@begin CursorInformation creation
  SetParent(parent);
  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
  ////@end CursorInformation creation
  return true;
}


/*!
 * CursorInformation destructor
 */

CursorInformation::~CursorInformation()
{
  ////@begin CursorInformation destruction
  ////@end CursorInformation destruction
}


/*!
 * Member initialisation
 */

void CursorInformation::Init()
{
  ////@begin CursorInformation member initialisation
  mColor_ = NULL;
  mSpace_ = NULL;

  mValue_ = NULL;
  mPosX_ = NULL;
  mPosY_ = NULL;
  mPosZ_ = NULL;
  ////@end CursorInformation member initialisation
}


void CursorInformation::SetMousePositionAndValue(wxCommandEvent &event)
{
  CursorInformationStruct *CI =
    (CursorInformationStruct *)event.GetClientData();
	
  mValue_->Clear();

  mPosX_->Clear();
  mPosY_->Clear();
  mPosZ_->Clear();

  if (CI->value_is_valid)
  {
    if (mColor_->GetSelection() == 0)
    {
      *mValue_ << CI->value;
    }
    else
    {
      *mValue_ << CI->valuer;
    }

    if (mSpace_->GetSelection() == 0)
    {
      *mPosX_ << CI->x;
      *mPosY_ << CI->y;
      *mPosZ_ << CI->z;
    }
    else
    {
      *mPosX_ << CI->xi;
      *mPosY_ << CI->yi;
      *mPosZ_ << CI->zi;
    }      
  }
  else
  {
    *mValue_ << wxT("none");
    *mPosX_ << wxT("none");
    *mPosY_ << wxT("none");
    *mPosZ_ << wxT("none");
  }

  delete CI;
}

void CursorInformation::SetMousePosition(double x, double y, double z)
{
  mPosX_->Clear();
  mPosY_->Clear();
  mPosZ_->Clear();
  *mPosX_ << x;
  *mPosY_ << y;
  *mPosZ_ << z;
}

void CursorInformation::SetInfoValue(double value)
{	
  mValue_->Clear();
  *mValue_ << value;
}

/*!
 * Control creation for CursorInformation
 */
void CursorInformation::CreateControls()
{    
  ////@begin CursorInformation content construction
  if (!wxXmlResource::Get()->LoadPanel(this, GetParent(), _T("ID_CURSORINFORMATION")))
    wxLogError(wxT("Missing wxXmlResource::Get()->Load() in OnInit()?"));

  mColor_ = XRCCTRL(*this, "ID_IMAGE_VALUE_CHOICE", wxChoice);
  mSpace_ = XRCCTRL(*this, "ID_COORDINATE_CHOICE", wxChoice);
  mValue_ = XRCCTRL(*this, "ID_VALUE_TEXTCTRL", wxTextCtrl);
  mPosX_ = XRCCTRL(*this, "ID_X_POS_TEXTCTRL", wxTextCtrl);
  mPosY_ = XRCCTRL(*this, "ID_Y_POS_TEXTCTRL", wxTextCtrl);
  mPosZ_ = XRCCTRL(*this, "ID_Z_POS_TEXTCTRL", wxTextCtrl);

  // Set validators
  if (FindWindow(XRCID("ID_VALUE_TEXTCTRL")))
    FindWindow(XRCID("ID_VALUE_TEXTCTRL"))->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
  if (FindWindow(XRCID("ID_X_POS_TEXTCTRL")))
    FindWindow(XRCID("ID_X_POS_TEXTCTRL"))->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
  if (FindWindow(XRCID("ID_Y_POS_TEXTCTRL")))
    FindWindow(XRCID("ID_Y_POS_TEXTCTRL"))->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
  if (FindWindow(XRCID("ID_Z_POS_TEXTCTRL")))
    FindWindow(XRCID("ID_Z_POS_TEXTCTRL"))->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
  ////@end CursorInformation content construction
       
  // Create custom windows not generated automatically here.
  ////@begin CursorInformation content initialisation
  ////@end CursorInformation content initialisation
}


/*!
 * Should we show tooltips?
 */

bool CursorInformation::ShowToolTips()
{
  return true;
}


/*!
 * Get bitmap resources
 */

wxBitmap CursorInformation::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  ////@begin CursorInformation bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
  ////@end CursorInformation bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CursorInformation::GetIconResource( const wxString& name )
{
  // Icon retrieval
  ////@begin CursorInformation icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
  ////@end CursorInformation icon retrieval
}