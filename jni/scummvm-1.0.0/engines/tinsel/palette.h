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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/palette.h $
 * $Id: palette.h 41142 2009-06-03 16:15:17Z Hkz $
 *
 * Palette Allocator Definitions
 */

#ifndef TINSEL_PALETTE_H	// prevent multiple includes
#define TINSEL_PALETTE_H

#include "tinsel/dw.h"

namespace Tinsel {

typedef	uint32	COLORREF;

#define TINSEL_RGB(r,g,b)	((COLORREF)TO_LE_32(((uint8)(r)|((uint16)(g)<<8))|(((uint32)(uint8)(b))<<16)))

#define TINSEL_GetRValue(rgb)	((uint8)(FROM_LE_32(rgb)))
#define TINSEL_GetGValue(rgb)	((uint8)(((uint16)(FROM_LE_32(rgb)))>>8))
#define TINSEL_GetBValue(rgb)	((uint8)((FROM_LE_32(rgb))>>16))

#define TINSEL_PSX_RGB(r,g,b) ((uint16)(((uint8)(r))|((uint16)(g)<<5)|(((uint16)(b))<<10)))

enum {
	MAX_COLOURS		= 256,	//!< maximum number of colours - for VGA 256
	BITS_PER_PIXEL	= 8,	//!< number of bits per pixel for VGA 256
	MAX_INTENSITY	= 255,	//!< the biggest value R, G or B can have
	NUM_PALETTES	= 32,	//!< number of palettes

	// Discworld has some fixed apportioned bits in the palette.
	BGND_DAC_INDEX	= 0,	//!< index of background colour in Video DAC
	FGND_DAC_INDEX	= 1,	//!< index of first foreground colour in Video DAC
	TBLUE1			= 228,	//!< Blue used in translucent rectangles
	TBLUE2			= 229,	//!< Blue used in translucent rectangles
	TBLUE3			= 230,	//!< Blue used in translucent rectangles
	TBLUE4			= 231,	//!< Blue used in translucent rectangles
	TALKFONT_COL	= 233
};

// some common colours

#define	BLACK	(TINSEL_RGB(0, 0, 0))
#define	WHITE	(TINSEL_RGB(MAX_INTENSITY, MAX_INTENSITY, MAX_INTENSITY))
#define	RED		(TINSEL_RGB(MAX_INTENSITY, 0, 0))
#define	GREEN	(TINSEL_RGB(0, MAX_INTENSITY, 0))
#define	BLUE	(TINSEL_RGB(0, 0, MAX_INTENSITY))
#define	YELLOW	(TINSEL_RGB(MAX_INTENSITY, MAX_INTENSITY, 0))
#define	MAGENTA	(TINSEL_RGB(MAX_INTENSITY, 0, MAX_INTENSITY))
#define	CYAN	(TINSEL_RGB(0, MAX_INTENSITY, MAX_INTENSITY))


#include "common/pack-start.h"	// START STRUCT PACKING

/** hardware palette structure */
struct PALETTE {
	int32 numColours;		//!< number of colours in the palette
	COLORREF palRGB[MAX_COLOURS];	//!< actual palette colours
} PACKED_STRUCT;

#include "common/pack-end.h"	// END STRUCT PACKING


/** palette queue structure */
struct PALQ {
	SCNHANDLE hPal;		//!< handle to palette data struct
	int objCount;		//!< number of objects using this palette
	int posInDAC;		//!< palette position in the video DAC
	int numColours;		//!< number of colours in the palette
	// Discworld 2 fields
	bool bFading;		// Whether or not fading
	COLORREF palRGB[MAX_COLOURS];	// actual palette colours
};
typedef PALQ *PPALQ;

#define	PALETTE_MOVED	0x8000	// when this bit is set in the "posInDAC"
				// field - the palette entry has moved

// Translucent objects have NULL pPal
#define	HasPalMoved(pPal) (((pPal) != NULL) && ((pPal)->posInDAC & PALETTE_MOVED))


/*----------------------------------------------------------------------*\
|*			Palette Manager Function Prototypes		*|
\*----------------------------------------------------------------------*/

void ResetPalAllocator(void);	// wipe out all palettes

#ifdef	DEBUG
void PaletteStats(void);	// Shows the maximum number of palettes used at once
#endif

void psxPaletteMapper(PALQ *originalPal, uint8 *psxClut, byte *mapperTable); // Maps PSX CLUTs to original palette in resource file

void PalettesToVideoDAC(void);	// Update the video DAC with palettes currently the the DAC queue

void UpdateDACqueueHandle(
	int posInDAC,		// position in video DAC
	int numColours,		// number of colours in palette
	SCNHANDLE hPalette);	// handle to palette

void UpdateDACqueue(		// places a palette in the video DAC queue
	int posInDAC,		// position in video DAC
	int numColours,		// number of colours in palette
	COLORREF *pColours);	// list of RGB tripples

PALQ *AllocPalette(		// allocate a new palette
	SCNHANDLE hNewPal);	// palette to allocate

void FreePalette(		// free a palette allocated with "AllocPalette"
	PALQ *pFreePal);	// palette queue entry to free

PALQ *FindPalette(		// find a palette in the palette queue
	SCNHANDLE hSrchPal);	// palette to search for

void SwapPalette(		// swaps palettes at the specified palette queue position
	PALQ *pPalQ,		// palette queue position
	SCNHANDLE hNewPal);	// new palette

PALQ *GetNextPalette(		// returns the next palette in the queue
	PALQ *pStrtPal);	// queue position to start from - when NULL will start from beginning of queue

COLORREF GetBgndColour(void);	// returns current background colour

void SetBgndColour(		// sets current background colour
	COLORREF colour);	// colour to set the background to

void FadingPalette(PPALQ pPalQ, bool bFading);

void CreateTranslucentPalette(SCNHANDLE BackPal);

void CreateGhostPalette(SCNHANDLE hPalette);

void NoFadingPalettes(void);	// All fading processes have just been killed

void DimPartPalette(
	SCNHANDLE hPal,
	int startColour,
	int length,
	int brightness);	// 0 = black, 10 == 100%


int TranslucentColour(void);

#define BoxColour TranslucentColour

int HighlightColour(void);

int TalkColour(void);

void SetTalkColourRef(COLORREF colRef);

COLORREF GetTalkColourRef(void);

void SetTagColorRef(COLORREF colRef);

COLORREF GetTagColorRef(void);

void SetTalkTextOffset(int offset);

void SetTranslucencyOffset(int offset);

} // end of namespace Tinsel

#endif	// TINSEL_PALETTE_H
