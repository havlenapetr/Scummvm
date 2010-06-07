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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/cursor.h $
 * $Id: cursor.h 35316 2008-12-12 15:48:38Z thebluegr $
 *
 * Clipping rectangle defines
 */

#ifndef TINSEL_CURSOR_H	// prevent multiple includes
#define TINSEL_CURSOR_H

#include "tinsel/dw.h"	// for SCNHANDLE

namespace Tinsel {

void AdjustCursorXY(int deltaX, int deltaY);
void SetCursorXY(int x, int y);
void SetCursorScreenXY(int newx, int newy);
void GetCursorXY(int *x, int *y, bool absolute);
bool GetCursorXYNoWait(int *x, int *y, bool absolute);
bool isCursorShown();

void RestoreMainCursor(void);
void SetTempCursor(SCNHANDLE pScript);
void DwHideCursor(void);
void UnHideCursor(void);
void FreezeCursor(void);
void DoFreezeCursor(bool bFreeze);
void HideCursorTrails(void);
void UnHideCursorTrails(void);
void DelAuxCursor(void);
void SetAuxCursor(SCNHANDLE hFilm);
void DwInitCursor(SCNHANDLE bfilm);
void DropCursor(void);
void RestartCursor(void);
void RebootCursor(void);
void StartCursorFollowed(void);
void EndCursorFollowed(void);

} // end of namespace Tinsel

#endif	// TINSEL_CURSOR_H
