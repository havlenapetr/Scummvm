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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/timers.h $
 * $Id: timers.h 39428 2009-03-15 20:30:57Z fingolfin $
 *
 * Handles timers.
 */

#ifndef TINSEL_TIMERS_H	// prevent multiple includes
#define TINSEL_TIMERS_H

#include "common/scummsys.h"
#include "tinsel/dw.h"

namespace Common {
	class Serializer;
}

namespace Tinsel {

#define ONE_SECOND 24

uint32 DwGetCurrentTime(void);

void RebootTimers(void);

void syncTimerInfo(Common::Serializer &s);

void FettleTimers(void);

void StartTimer(int num, int sval, bool up, bool frame);

int Timer(int num);

} // end of namespace Tinsel

#endif
