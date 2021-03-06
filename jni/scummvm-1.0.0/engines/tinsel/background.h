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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/background.h $
 * $Id: background.h 39203 2009-03-07 19:24:31Z fingolfin $
 *
 * Data structures used for handling backgrounds
 */

#ifndef TINSEL_BACKGND_H     // prevent multiple includes
#define TINSEL_BACKGND_H

#include "common/frac.h"
#include "common/rect.h"
#include "tinsel/coroutine.h"
#include "tinsel/dw.h"	// for SCNHANDLE
#include "tinsel/palette.h"	// palette definitions

namespace Tinsel {

struct OBJECT;


/** Scrolling padding. Needed because scroll process does not normally run on every frame */
enum {
	SCROLLX_PAD	= 64,
	SCROLLY_PAD	= 64
};

/** When module BLK_INFO list is this long, switch from a binary to linear search */
#define	LINEAR_SEARCH	5

/** background playfield structure - a playfield is a container for modules */
struct PLAYFIELD {
	OBJECT *pDispList;	//!< object display list for this playfield
	frac_t fieldX;		//!< current world x position of playfield
	frac_t fieldY;		//!< current world y position of playfield
	frac_t fieldXvel;	//!< current x velocity of playfield
	frac_t fieldYvel;	//!< current y velocity of playfield
	Common::Rect rcClip;	//!< clip rectangle for this playfield
	bool bMoved;		//!< set when playfield has moved
};

/** multi-playfield background structure - a backgnd is a container of playfields */
struct BACKGND {
	COLORREF rgbSkyColour;	//!< background sky colour
	Common::Point ptInitWorld;		//!< initial world position
	Common::Rect rcScrollLimits;	//!< scroll limits
	int refreshRate;		//!< background update process refresh rate
	frac_t *pXscrollTable;	//!< pointer to x direction scroll table for this background
	frac_t *pYscrollTable;	//!< pointer to y direction scroll table for this background
	int numPlayfields;		//!< number of playfields for this background
	PLAYFIELD *fieldArray;	//!< pointer to array of all playfields for this background
	bool bAutoErase;		//!< when set - screen is cleared before anything is plotted (unused)
};


/*----------------------------------------------------------------------*\
|*			Background Function Prototypes			*|
\*----------------------------------------------------------------------*/

void InitBackground(		// called to initialise a background
	BACKGND *pBgnd);	// pointer to data struct for current background

void StartupBackground(CORO_PARAM, SCNHANDLE hFilm);

void StopBgndScrolling(void);	// Stops all background playfields from scrolling

void PlayfieldSetPos(		// Sets the xy position of the specified playfield in the current background
	int which,		// which playfield
	int newXpos,		// new x position
	int newYpos);		// new y position

void PlayfieldGetPos(		// Returns the xy position of the specified playfield in the current background
	int which,		// which playfield
	int *pXpos,		// returns current x position
	int *pYpos);		// returns current y position

int PlayfieldGetCentreX(	// Returns the xy position of the specified playfield in the current background
	int which);		// which playfield

OBJECT *GetPlayfieldList(	// Returns the display list for the specified playfield
	int which);		// which playfield

void KillPlayfieldList(		// Kills all the objects on the display list for the specified playfield
	int which);		// which playfield

void DrawBackgnd(void);		// Draws all playfields for the current background

void RedrawBackgnd(void);	// Completely redraws all the playfield object lists for the current background

OBJECT *GetBgObject();

SCNHANDLE BgPal(void);

void ForceEntireRedraw(void);

int BgWidth(void);

int BgHeight(void);

} // end of namespace Tinsel

#endif	// TINSEL_BACKGND_H
