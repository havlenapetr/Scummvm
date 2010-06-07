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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/init_v1.cpp $
 * $Id: init_v1.cpp 39596 2009-03-21 18:17:24Z drmccoy $
 *
 */

#include "common/endian.h"

#include "gob/gob.h"
#include "gob/init.h"
#include "gob/global.h"
#include "gob/draw.h"

namespace Gob {

Init_v1::Init_v1(GobEngine *vm) : Init(vm) {
}

void Init_v1::initVideo() {
	if (_vm->_global->_videoMode)
		_vm->validateVideoMode(_vm->_global->_videoMode);

	_vm->_global->_mousePresent = 1;

	_vm->_global->_inVM = 0;

	if ((_vm->_global->_videoMode == 0x13) && !_vm->isEGA())
		_vm->_global->_colorCount = 256;

	_vm->_global->_pPaletteDesc = &_vm->_global->_paletteStruct;
	_vm->_global->_pPaletteDesc->vgaPal = _vm->_draw->_vgaPalette;
	_vm->_global->_pPaletteDesc->unused1 = _vm->_global->_unusedPalette1;
	_vm->_global->_pPaletteDesc->unused2 = _vm->_global->_unusedPalette2;

	if (_vm->_global->_videoMode != 0)
		_vm->_video->initSurfDesc(_vm->_global->_videoMode, 320, 200, PRIMARY_SURFACE);
}

} // End of namespace Gob
