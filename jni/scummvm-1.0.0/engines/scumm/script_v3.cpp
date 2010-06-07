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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/scumm/script_v3.cpp $
 * $Id: script_v3.cpp 40010 2009-04-19 01:00:31Z fingolfin $
 *
 */

#include "scumm/scumm_v3.h"

namespace Scumm {

#define OPCODE(i, x)	_opcodes[i]._OPCODE(ScummEngine_v3, x)

void ScummEngine_v3::setupOpcodes() {
	ScummEngine_v4::setupOpcodes();

	OPCODE(0x30, o3_setBoxFlags);
	OPCODE(0xb0, o3_setBoxFlags);
}

void ScummEngine_v3::o3_setBoxFlags() {
	int a, b;

	a = getVarOrDirectByte(PARAM_1);
	b = fetchScriptByte();
	setBoxFlags(a, b);
}

} // End of namespace Scumm
