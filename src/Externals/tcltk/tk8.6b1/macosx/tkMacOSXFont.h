/*
 * tkMacOSXFont.h --
 *
 *	Contains the Macintosh implementation of the platform-independant
 *	font package interface.
 *
 * Copyright (c) 1990-1994 The Regents of the University of California.
 * Copyright (c) 1994-1997 Sun Microsystems, Inc.
 * Copyright 2001, Apple Computer, Inc.
 * Copyright (c) 2006-2007 Daniel A. Steffen <das@users.sourceforge.net>
 *
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 * RCS: @(#) $Id: tkMacOSXFont.h,v 1.6 2008/12/10 05:02:52 das Exp $
 */

#ifndef TKMACOSXFONT_H
#define TKMACOSXFONT_H 1

#include "tkFont.h"

#ifndef _TKMACINT
#include "tkMacOSXInt.h"
#endif

/*
 * Function prototypes
 */

MODULE_SCOPE void TkMacOSXInitControlFontStyle(Tk_Font tkfont,
	ControlFontStylePtr fsPtr);
MODULE_SCOPE void TkMacOSXFMFontInfoForFont(Tk_Font tkfont,
	FMFontFamily *fontFamilyPtr, FMFontStyle *fontStylePtr,
	FMFontSize *fontSizePtr, ATSUStyle *fontATSUStylePtr);
MODULE_SCOPE Tcl_Obj * TkMacOSXFontDescriptionForFMFontInfo(
	FMFontFamily fontFamily, FMFontStyle fontStyle, FMFontSize fontSize,
	FMFont fontID);

#endif /*TKMACOSXFONT_H*/
