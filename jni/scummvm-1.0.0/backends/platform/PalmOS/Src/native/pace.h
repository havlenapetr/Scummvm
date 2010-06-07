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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/native/pace.h $
 * $Id: pace.h 30667 2008-01-28 00:14:17Z jvprat $
 *
 */

#ifndef PACE_H
#define PACE_H

#include <PceNativeCall.h>
//#include "endianutils.h"

// local definition of the emulation state structure
typedef struct {
	UInt32 instr;
	UInt32 regData[8];
	UInt32 regAddress[8];
	UInt32 regPC;
} EmulStateType;

typedef struct {
	EmulStateType *emulStateP;
	Call68KFuncType *call68KFuncP;
} GlobalsType;

extern GlobalsType global;

// TODO : check this, already defined in ARMlet_Runtime
//extern EmulStateType	*g_emulStateP;
//extern Call68KFuncType	*g_call68KFuncP;
#define g_emulStateP	global.emulStateP
#define g_call68KFuncP	global.call68KFuncP


#ifdef __cplusplus
#	define PACE_CLASS_WRAPPER(rv)		extern "C" rv
#else
#	define PACE_CLASS_WRAPPER(rv)		rv
#endif
#define PACE_CALLBACK_PTR			g_call68KFuncP
#define PACE_EMULSTATE				g_emulStateP

#define ALIGN_4BYTE(addr) (((UInt32)(addr) + 3) & 0xFFFFFFFC)

/****** TAKEN FROM PACEInteface.cpp (ARMlet_Runtime) ******/
// local definition of the emulation state structure

#define PACE_PARAMS_INIT()						\
	UInt8 params[] = {

#define PACE_PARAMS_ADD8(param)					\
	(UInt8)(param),								\
	0,

#define PACE_PARAMS_ADD16(param)				\
	(UInt8)((UInt16)(param) >> 8),				\
	(UInt8)(param),

#define PACE_PARAMS_ADD32(param)				\
	(UInt8)((UInt32)(param) >> 24),				\
	(UInt8)((UInt32)(param) >> 16),				\
	(UInt8)((UInt32)(param) >> 8),				\
	(UInt8)(param),

#define PACE_PARAMS_END()						\
	};

// PIN
#define PACE_PIN_EXEC_NP(pinTrap, returnType)	\
	PACE_EMULSTATE->regData[2] = pinTrap;		\
	return ((returnType)((PACE_CALLBACK_PTR)(	\
		static_cast<void *>(PACE_EMULSTATE),	\
		PceNativeTrapNo(sysTrapPinsDispatch),	\
		NULL, 0)));

#define PACE_PIN_EXEC(pinTrap, returnType)		\
	PACE_EMULSTATE->regData[2] = pinTrap;		\
	return ((returnType)((PACE_CALLBACK_PTR)(	\
		static_cast<void *>(PACE_EMULSTATE),	\
		PceNativeTrapNo(sysTrapPinsDispatch),	\
		&params,								\
		sizeof(params))));

#endif
