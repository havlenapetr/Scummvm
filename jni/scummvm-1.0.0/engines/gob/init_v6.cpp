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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/init_v6.cpp $
 * $Id: init_v6.cpp 42130 2009-07-05 11:29:54Z drmccoy $
 *
 */

#include "common/endian.h"

#include "gob/gob.h"
#include "gob/init.h"
#include "gob/global.h"

namespace Gob {

Init_v6::Init_v6(GobEngine *vm) : Init_v3(vm) {
}

void Init_v6::initGame() {
	_vm->_global->_noCd = false;

	if (Common::File::exists("cd1.itk") && Common::File::exists("cd2.itk") &&
	    Common::File::exists("cd3.itk") && Common::File::exists("cd4.itk")) {
		_vm->_global->_noCd = true;
	}

	Init::initGame();
}

} // End of namespace Gob
