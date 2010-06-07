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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/gp2xwiz/gp2xwiz-main.cpp $
 * $Id: gp2xwiz-main.cpp 43030 2009-08-03 19:04:07Z djwillis $
 *
 */

#include "common/scummsys.h"
#include <SDL/SDL.h>

#include "backends/platform/gp2xwiz/gp2xwiz-sdl.h"
#include "backends/platform/gp2xwiz/gp2xwiz-hw.h"
#include "backends/plugins/posix/posix-provider.h"
#include "base/main.h"
#include "base/internal_version.h"

#include "common/archive.h"
#include "common/config-manager.h"
#include "common/debug.h"
#include "common/events.h"
#include "common/util.h"

#include "common/file.h"
#include "base/main.h"

#include "backends/saves/default/default-saves.h"

#include "backends/timer/default/default-timer.h"
#include "sound/mixer_intern.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>	// for getTimeAndDate()

/* Dump console info to files. */
// #define DUMP_STDOUT

int main(int argc, char *argv[]) {

	g_system = new OSystem_GP2XWIZ();
	assert(g_system);

#ifdef DYNAMIC_MODULES
	PluginManager::instance().addPluginProvider(new POSIXPluginProvider());
#endif

	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);
	g_system->quit();

	return res;
}

void OSystem_GP2XWIZ::initBackend() {

	/* Setup default save path to be workingdir/saves */

	char savePath[PATH_MAX+1];
	char workDirName[PATH_MAX+1];

	if (getcwd(workDirName, PATH_MAX) == NULL) {
		error("Could not obtain current working directory.");
	} else {
		printf("Current working directory: %s\n", workDirName);
	}

	strcpy(savePath, workDirName);
	strcat(savePath, "/saves");
	printf("Current save directory: %s\n", savePath);
	struct stat sb;
	if (stat(savePath, &sb) == -1)
		if (errno == ENOENT) // Create the dir if it does not exist
			if (mkdir(savePath, 0755) != 0)
				warning("mkdir for '%s' failed!", savePath);

	_savefile = new DefaultSaveFileManager(savePath);

	#ifdef DUMP_STDOUT
		// The GP2X Wiz has a serial console on the breakout board but most users do not use this so we
		// output all our STDOUT and STDERR to files for debug purposes.
		char STDOUT_FILE[PATH_MAX+1];
		char STDERR_FILE[PATH_MAX+1];

		strcpy(STDOUT_FILE, workDirName);
		strcpy(STDERR_FILE, workDirName);
		strcat(STDOUT_FILE, "/scummvm.stdout.txt");
		strcat(STDERR_FILE, "/scummvm.stderr.txt");

		// Flush the output in case anything is queued
		fclose(stdout);
		fclose(stderr);

		// Redirect standard input and standard output
		FILE *newfp = freopen(STDOUT_FILE, "w", stdout);
		if (newfp == NULL) {
		#if !defined(stdout)
			stdout = fopen(STDOUT_FILE, "w");
		#else
			newfp = fopen(STDOUT_FILE, "w");
			if (newfp) {
				*stdout = *newfp;
			}
		#endif
		}

		newfp = freopen(STDERR_FILE, "w", stderr);
		if (newfp == NULL) {
		#if !defined(stderr)
			stderr = fopen(STDERR_FILE, "w");
		#else
			newfp = fopen(STDERR_FILE, "w");
			if (newfp) {
				*stderr = *newfp;
			}
		#endif
		}

		setbuf(stderr, NULL);
		printf("%s\n", "Debug: STDOUT and STDERR redirected to text files.");
	#endif /* DUMP_STDOUT */

	/* Initialise any GP2X Wiz specific stuff we may want (Batt Status, scaler etc.) */
	WIZ_HW::deviceInit();

	/* Set Default hardware mixer volume to a preset level (VOLUME_INITIAL). This is done to 'reset' volume level if set by other apps. */
	WIZ_HW::mixerMoveVolume(0);

	/* Up default volume values as we use a seperate system level volume anyway. */
	ConfMan.registerDefault("music_volume", 192);
	ConfMan.registerDefault("sfx_volume", 192);
	ConfMan.registerDefault("speech_volume", 192);

	/* Trigger autosave every 4 minutes - On low batts 5 mins is about your warning time. */
	ConfMan.registerDefault("autosave_period", 4 * 60);

	/* Make sure that aspect ratio correction is enabled on the 1st run to stop users asking me what the 'wasted space' is ;-). */
	ConfMan.registerDefault("aspect_ratio", true);

	/* Make sure SDL knows that we have a joystick we want to use. */
	ConfMan.setInt("joystick_num", 0);

	printf("%s\n", "Passing to OSystem::SDL initBackend.");

	/* Pass to SDL backend to do the heavy lifting */
	OSystem_SDL::initBackend();
}

void OSystem_GP2XWIZ::addSysArchivesToSearchSet(Common::SearchSet &s, int priority) {

	/* Setup default extra data paths for engine data files and plugins */
	char workDirName[PATH_MAX+1];

	if (getcwd(workDirName, PATH_MAX) == NULL) {
		error("Error: Could not obtain current working directory.");
	}

	Common::FSNode workdirNode(workDirName);
	if (workdirNode.exists() && workdirNode.isDirectory()) {
		s.add("__GP2XWIZ_WORKDIR__", new Common::FSDirectory(workDirName), priority);
	}

	char enginedataPath[PATH_MAX+1];

	strcpy(enginedataPath, workDirName);
	strcat(enginedataPath, "/engine-data");

	Common::FSNode engineNode(enginedataPath);
	if (engineNode.exists() && engineNode.isDirectory()) {
		s.add("__GP2XWIZ_ENGDATA__", new Common::FSDirectory(enginedataPath), priority);
	}

	char pluginsPath[PATH_MAX+1];

	strcpy(pluginsPath, workDirName);
	strcat(pluginsPath, "/plugins");

	Common::FSNode pluginsNode(pluginsPath);
	if (pluginsNode.exists() && pluginsNode.isDirectory()) {
		s.add("__GP2XWIZ_PLUGINS__", new Common::FSDirectory(pluginsPath), priority);
	}
}

void OSystem_GP2XWIZ::quit() {

	WIZ_HW::deviceDeinit();

	#ifdef DUMP_STDOUT
		printf("%s\n", "Debug: STDOUT and STDERR text files closed.");
		fclose(stdout);
		fclose(stderr);
	#endif /* DUMP_STDOUT */

	OSystem_SDL::quit();
}
