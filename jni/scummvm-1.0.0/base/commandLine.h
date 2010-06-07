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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/base/commandLine.h $
 * $Id: commandLine.h 39591 2009-03-21 16:07:46Z marcus_c $
 *
 */

#ifndef BASE_OPTIONS_H
#define BASE_OPTIONS_H

#include "common/str.h"
#include "common/config-manager.h"

namespace Base {

void registerDefaults();
Common::String parseCommandLine(Common::StringMap &settings, int argc, const char * const *argv);
bool processSettings(Common::String &command, Common::StringMap &settings);

} // End of namespace Base

#endif
