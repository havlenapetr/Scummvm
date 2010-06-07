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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/native/zodiacARM.cpp $
 * $Id: zodiacARM.cpp 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#include "PalmVersion.h"
#include <MSL_PalmOS.h>

#include "globals.h"
#include "args.h"
#include "palmdefs.h"

#include "rumble.h"


//#include <AdnDebugMgr.h>
//#define DEBUG_ARM

#include "base/main.h"
#ifdef COMPILE_ZODIAC
#	include "be_zodiac.h"
#endif
#include "be_os5ex.h"

GlobalsDataType g_vars;
GlobalsDataPtr gVars = &g_vars;
UInt32 g_stackSize;

extern "C" void __destroy_global_chain(void);
extern void DrawStatus(Boolean show);

static void palm_main(int argc, char **argvP)  {
#ifdef COMPILE_OS5
	if (gVars->advancedMode)
		g_system = new OSystem_PalmOS5Ex();
	else
		g_system = new OSystem_PalmOS5();
#elif defined(COMPILE_ZODIAC)
	g_system = new OSystem_PalmZodiac();
#else
	#error "No target defined."
#endif

	assert(g_system);

	scummvm_main(argc, argvP);

	g_system->quit();	// TODO: Consider removing / replacing this!
}

static void Go() {
	void *tmp;
	char **argvP;
	int argc;

#ifdef DEBUG_ARM
// Tell the debugger we want to enable full debugging
	UInt32 flags = AdnDebugEnableGet();
	flags |= kAdnEnableMasterSwitch | kAdnEnableFullDebugging;
	AdnDebugEnableSet(flags);
// Tell the debugger where our code lives in memory:
	AdnDebugNativeRegister(sysFileTApplication, appFileCreator, 'ARMC', 1);
#endif

	// get global struct
	FtrGet(appFileCreator, ftrVars, (UInt32 *)&tmp);
	MemMove(gVars, tmp, sizeof(GlobalsDataType));

	// init STDIO
	stdio_set_cache(0);
	stdio_init(gVars->VFS.volRefNum, "/PALM/Programs/ScummVM/scumm.log");
	if (gVars->indicator.showLED)
		stdio_set_led(DrawStatus);
	stdio_set_cache(gVars->VFS.cacheSize);

	// get args
	FtrGet(appFileCreator, ftrArgsData, (UInt32 *)&argvP);
	FtrGet(appFileCreator, ftrArgsCount, (UInt32 *)&argc);

	// init system
	WinSetDrawWindow(WinGetDisplayWindow());
	if (HWR_INIT(INIT_VIBRATOR))	gVars->vibrator =	RumbleInit();

	// run ...
	DO_EXIT ( palm_main(argc, argvP); )

	// release
	if (HWR_INIT(INIT_VIBRATOR))	RumbleRelease();
	stdio_release();

#ifdef DEBUG_ARM
	AdnDebugNativeUnregister();
#endif
}

/* Stack size */

Int8 *g_newStack, *g_newStackPos;
void *g_oldStack;

static asm void *StkSwap(void *newStack ,void *dummy) {
	mov  r1, r13
	mov  r13, r0
	mov	 r0, r1
	bx	 lr
}

extern UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags) {
	if (cmd == sysAppLaunchCmdNormalLaunch) {
		FtrGet(appFileCreator, ftrStack, &g_stackSize);
		if (!g_stackSize)
			g_stackSize = 32 * 1024;

		g_newStack = (Int8 *)malloc(g_stackSize + 8);
		g_newStackPos = (g_newStack + g_stackSize);
		g_newStackPos -= ((UInt32)g_newStackPos & 7);

		g_oldStack = StkSwap(g_newStackPos, 0);

		Go();

		StkSwap(g_oldStack, 0);
		free(g_newStack);
	}

	// Destroy all constructed global objects
	__destroy_global_chain();

	return 0;
}
