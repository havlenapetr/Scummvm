/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/font.h $
 * $Id: font.h 35196 2008-12-01 20:35:36Z thebluegr $
 *
 */

#ifndef TINSEL_FONT_H	// prevent multiple includes
#define TINSEL_FONT_H

#include "tinsel/dw.h"

namespace Tinsel {

// A temporary buffer for extracting text into is defined in font.c
// Accessed using TextBufferAddr(), this is how big it is:
#define TBUFSZ	512


char *TextBufferAddr(void);

SCNHANDLE GetTagFontHandle(void);

SCNHANDLE GetTalkFontHandle(void);

void SetTagFontHandle(SCNHANDLE hFont);

void SetTalkFontHandle(SCNHANDLE hFont);

void SetTempTagFontHandle(SCNHANDLE hFont);

void SetTempTalkFontHandle(SCNHANDLE hFont);

void ResetFontHandles(void);

void FettleFontPal(SCNHANDLE fontPal);

} // end of namespace Tinsel

#endif		// TINSEL_FONT_H
