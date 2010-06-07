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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/maemo/main.cpp $
 * $Id: main.cpp 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#include <common/scummsys.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xsp.h>

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>

#include "backends/platform/maemo/maemo-sdl.h"
#include "base/main.h"
#include "base/internal_version.h"
#include <hildon-widgets/hildon-app.h>
#include <gtk/gtk.h>
#include <libosso.h>

#include <sys/time.h>
#include <sys/resource.h>

#define OSSO_APP_NAME    "scummvm"
#define OSSO_APP_VERSION SCUMMVM_VERSION

void set_doubling(unsigned char enable) {
  return;

  SDL_SysWMinfo wminfo;
  SDL_VERSION(&wminfo.version);
  SDL_GetWMInfo(&wminfo);
  XSPSetPixelDoubling(wminfo.info.x11.display, 0, enable);
}

int main(int argc, char *argv[]) {
    osso_context_t *osso_context;

    // Initialize maemo application
    //osso_context = osso_initialize(OSSO_APP_NAME, OSSO_APP_VERSION, TRUE, NULL);

    // Check that initialization was ok
    //if (osso_context == NULL) {
	//   return OSSO_ERROR;
    //}

	// Maemo task navigator priority inheritance fix
	setpriority(PRIO_PROCESS, 0, 0);

	set_doubling(0);

	g_system = new OSystem_MAEMO();
	assert(g_system);

	// Invoke the actual ScummVM main entry point:
	int res = scummvm_main(argc, argv);
	g_system->quit();	// TODO: Consider removing / replacing this!

    /* Deinitialize OSSO */
    //osso_deinitialize(osso_context);

	set_doubling(0);

	return res;
}
