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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/common/system.cpp $
 * $Id: system.cpp 36135 2009-01-30 03:35:47Z fingolfin $
 *
 */

#include "common/system.h"

OSystem *g_system = 0;

OSystem::OSystem() {
}

OSystem::~OSystem() {
}

bool OSystem::setGraphicsMode(const char *name) {
	if (!name)
		return false;

	// Special case for the 'default' filter
	if (!scumm_stricmp(name, "normal") || !scumm_stricmp(name, "default")) {
		return setGraphicsMode(getDefaultGraphicsMode());
	}

	const GraphicsMode *gm = getSupportedGraphicsModes();

	while (gm->name) {
		if (!scumm_stricmp(gm->name, name)) {
			return setGraphicsMode(gm->id);
		}
		gm++;
	}

	return false;
}

bool OSystem::openCD(int drive) {
	return false;
}

bool OSystem::pollCD() {
	return false;
}
