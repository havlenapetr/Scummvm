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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/globals.h $
 * $Id: globals.h 30667 2008-01-28 00:14:17Z jvprat $
 *
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <VFSMgr.h>
#include "stuffs.h"

enum {
	INIT_VIBRATOR	= 1 <<	0x00,
	INIT_PA1LIB		= 1 <<	0x01,
	INIT_ARM		= 1 <<	0x02,
	INIT_AUTOOFF	= 1 <<	0x03,
	INIT_GOLCD		= 1 <<	0x04
};

enum {
	FM_QUALITY_LOW = 0,
	FM_QUALITY_MED,
	FM_QUALITY_HI,
	FM_QUALITY_INI
};

typedef struct {
	// common parts
	UInt32 _4B, _2B;

	// 4 bytes part
	UInt32 startupMemory;
	UInt32 slkVersion;
	UInt32 options;
	UInt32 screenPitch;

	struct {
		FileRef	logFile;
		UInt32 cacheSize;
		UInt16 volRefNum;
		UInt16 dummy;
	} VFS;

	// 2 bytes part
	UInt16 HRrefNum;
	UInt16 slkRefNum;
	Coord screenWidth, screenHeight;			// silkarea shown
	Coord screenFullWidth, screenFullHeight;	// silkarea hidden
	Int16 autoSave;
	struct {
		Int16 on;
		Int16 off;
		Int16 showLED;
	} indicator;

	// 1 byte part
	Boolean vibrator;
	Boolean stdPalette;
	Boolean filter;
	Boolean stylusClick;
	Boolean arrowKeys;
	UInt8 init;
	UInt8 palmVolume;
	UInt8 fmQuality;
	UInt8 advancedMode;

} GlobalsDataType, *GlobalsDataPtr;

extern GlobalsDataPtr gVars;

#define VARS_EXPORT()		gVars->_4B = 6; \
							gVars->_2B = 12;

#define DO_VARS(z, t, o) \
	{	Int8 *tmp = (Int8 *)gVars + o + 8; \
		for (Int8 cnt = 0; cnt < gVars->z; cnt++) \
			{	UInt##t val = *((UInt##t *)tmp);	\
				val = ByteSwap##t(val);	\
				*((UInt##t *)tmp) = val;	\
				tmp += (t / 8);	\
			}	\
	}

#define OPTIONS_DEF()		gVars->options

#define HWR_INIT(x)			(gVars->init & (x))
#define HWR_SET(x)			gVars->init |= (x)
#define HWR_RST(x)			gVars->init &= ~(x)
#define HWR_RSTALL()		gVars->init = 0
#define HWR_GET()			(gVars->init)

#define ARM(x)	gVars->arm[x]

#endif
