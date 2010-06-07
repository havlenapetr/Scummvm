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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/tools.cpp $
 * $Id: tools.cpp 42167 2009-07-06 10:39:22Z thebluegr $
 *
 */

#include "sci/tools.h"
#include "sci/engine/state.h"

#include "sci/sci.h"	// For _console only
#include "sci/console.h"	// For _console only

namespace Sci {

int sci_ffs(int bits) {
	if (!bits)
		return 0;

	int retval = 1;

	while (!(bits & 1)) {
		retval++;
		bits >>= 1;
	}

	return retval;
}

} // End of namespace Sci
