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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/cruise/debugger.h $
 * $Id: debugger.h 36259 2009-02-11 10:03:01Z dreammaster $
 *
 */

#ifndef CRUISE_DEBUGGER_H
#define CRUISE_DEBUGGER_H

#include "gui/debugger.h"

namespace Cruise {

class Debugger : public GUI::Debugger {
public:
	Debugger();
	virtual ~Debugger() {}  // we need this for __SYMBIAN32__ archaic gcc/UIQ

protected:
	bool cmd_hotspots(int argc, const char **argv);
	bool cmd_items(int argc, const char **argv);
};

} // End of namespace Cruise

#endif
