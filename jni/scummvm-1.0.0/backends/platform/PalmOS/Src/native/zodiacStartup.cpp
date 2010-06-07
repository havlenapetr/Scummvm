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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/native/zodiacStartup.cpp $
 * $Id: zodiacStartup.cpp 27024 2007-05-30 21:56:52Z fingolfin $
 *
 */

#ifdef COMPILE_ZODIAC

#include <TapWave.h>

#ifndef __PALMOS_ARMLET__
#error "__PALMOS_ARMLET__ is not defined!!!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This variable holds the Tapwave Native Application interface
 * dispatch table. General ARM API calls will go through this table.
 */
struct TwGlue*   twGlue;
const void*      twEmulState;
Call68KFuncType* twCall68KFunc;

#ifdef __MWERKS__

#if __PALMOS_ARMLET__ < 0x120
#error "You must use CodeWarrior for Palm OS 9.3 or later!!!"
#endif


UInt32 __ARMlet_Startup__(const void*, void*, Call68KFuncType*);

/*
 * These symbols aren't real, but are linker-generated symbols that
 * mark the start and end of the various data sections.
 */
extern long __DataStart__[];
extern long __sinit__[];
extern long __RODataStart__[];
extern long __BSSStart__[];
extern long __BSSEnd__[];
extern long __CodeRelocStart__[];
extern long __CodeRelocEnd__[];
extern long __DataRelocStart__[];
extern long __DataRelocEnd__[];

/*	simple function pointer	*/
typedef void (*StaticInitializer)(void);

/*
 * This function performs relocation for Tapwave Native Application.
 */
static void relocate(void)
{
    // this symbol points to the very beginning of current application
    long base = (long) __ARMlet_Startup__;
    long *cur, *end;

	// handle static initializers
	if (__sinit__) {
		long s, *p;

		for (p = __sinit__; p && (s = *p) != 0; p++)
			((StaticInitializer)(s + base))();
	}

    // handle code-to-data relocation
    cur = __CodeRelocStart__;
    end = __CodeRelocEnd__;
    for (; cur < end; cur++) {
        *(long*)(base + *cur) += base;
    }

    // handle data-to-data relocation
    cur = __DataRelocStart__;
    end = __DataRelocEnd__;
    for (; cur < end; cur++) {
        *(long*)(base + *cur) += base;
    }
}

/*
 * This is the real entrypoint for Tapwave Native Application. It
 * depends on various CodeWarrior 9.2 compiler/linker/runtime features.
 */
static SYSTEM_CALLBACK UInt32
Startup(const void *emulStateP, void *userData68KP, Call68KFuncType *call68KFuncP)
{
    if (emulStateP) {
        twEmulState = emulStateP;
        twCall68KFunc = call68KFuncP;
        // COMMENT: normal pace native object launch
        return PilotMain(sysAppLaunchCmdNormalLaunch, userData68KP, 0);
    } else {
        // Setup TNA interface dispatch table
        twGlue = (struct TwGlue*) userData68KP;

        // OPTIONAL: relocate data segment
        relocate();

        // OPTIONAL: initialize the floating-point library
 //       _fp_init();

        return 0;
    }
}

#pragma PIC off
asm UInt32
__ARMlet_Startup__(const void *emulStateP, void *userData68KP, Call68KFuncType *call68KFuncP)
{
    b        Startup             //   0 forwarding call
    nop                          //   4
    nop                          //   8
    nop                          //  12
    nop                          //  16
    nop                          //  20
    nop                          //  24
    dcd      'TWNA'              //  28 Tapwave Native Application
    dcd      1                   //  32 version 1
    dcd      'cdwr'              //  36 make this section the same as the __ARMlet_Startup__
    dcd      __DataStart__       //  40 used by CodeWarrior 9.2
    dcd      __RODataStart__     //  44
    dcd      __BSSStart__        //  48
    dcd      __BSSEnd__          //  52
    dcd      __CodeRelocStart__  //  56
    dcd      __CodeRelocEnd__    //  60
    dcd      __DataRelocStart__  //  64
    dcd      __DataRelocEnd__    //  68
}
#pragma PIC reset

#endif // __MWERKS__

#ifdef __cplusplus
} // extern "C"
#endif

#endif
