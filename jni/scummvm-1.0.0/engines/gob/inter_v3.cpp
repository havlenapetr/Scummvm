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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/inter_v3.cpp $
 * $Id: inter_v3.cpp 41859 2009-06-25 10:54:35Z drmccoy $
 *
 */

#include "common/endian.h"
#include "common/file.h"

#include "gob/gob.h"
#include "gob/inter.h"
#include "gob/global.h"
#include "gob/dataio.h"
#include "gob/draw.h"
#include "gob/game.h"
#include "gob/script.h"
#include "gob/resources.h"

namespace Gob {

#define OPCODEVER Inter_v3
#define OPCODEDRAW(i, x)  _opcodesDraw[i]._OPCODEDRAW(OPCODEVER, x)
#define OPCODEFUNC(i, x)  _opcodesFunc[i]._OPCODEFUNC(OPCODEVER, x)
#define OPCODEGOB(i, x)   _opcodesGob[i]._OPCODEGOB(OPCODEVER, x)

Inter_v3::Inter_v3(GobEngine *vm) : Inter_v2(vm) {
}

void Inter_v3::setupOpcodesDraw() {
	Inter_v2::setupOpcodesDraw();
}

void Inter_v3::setupOpcodesFunc() {
	Inter_v2::setupOpcodesFunc();

	OPCODEFUNC(0x1A, o3_getTotTextItemPart);
	OPCODEFUNC(0x32, o3_copySprite);
}

void Inter_v3::setupOpcodesGob() {
	OPCODEGOB(  0, o2_loadInfogramesIns);
	OPCODEGOB(  1, o2_startInfogrames);
	OPCODEGOB(  2, o2_stopInfogrames);

	OPCODEGOB( 10, o2_playInfogrames);

	OPCODEGOB(100, o3_wobble);
}

bool Inter_v3::o3_getTotTextItemPart(OpFuncParams &params) {
	byte *totData;
	int16 totTextItem;
	int16 part, curPart = 0;
	int16 offX = 0, offY = 0;
	int16 collId = 0, collCmd;
	uint32 stringStartVar, stringVar;
	bool end;

	totTextItem = _vm->_game->_script->readInt16();
	stringStartVar = _vm->_game->_script->readVarIndex();
	part = _vm->_game->_script->readValExpr();

	stringVar = stringStartVar;
	WRITE_VARO_UINT8(stringVar, 0);

	TextItem *textItem = _vm->_game->_resources->getTextItem(totTextItem);
	if (!textItem)
		return false;

	totData = textItem->getData();

	// Skip background rectangles
	while (((int16) READ_LE_UINT16(totData)) != -1)
		totData += 9;
	totData += 2;

	while (*totData != 1) {
		switch (*totData) {
		case 2:
		case 5:
			totData++;
			offX = READ_LE_UINT16(totData);
			offY = READ_LE_UINT16(totData + 2);
			totData += 4;
			break;

		case 3:
		case 4:
			totData += 2;
			break;

		case 6:
			totData++;

			collCmd = *totData++;
			if (collCmd & 0x80) {
				collId = READ_LE_UINT16(totData);
				totData += 2;
			}

			// Skip collision coordinates
			if (collCmd & 0x40)
				totData += 8;

			if ((collCmd & 0x8F) && ((-collId - 1) == part)) {
				int n = 0;

				while (1) {
					if ((*totData < 1) || (*totData > 7)) {
						if (*totData >= 32) {
							WRITE_VARO_UINT8(stringVar++, *totData++);
							n++;
						} else
							totData++;
						continue;
					}

					if ((n != 0) || (*totData == 1) ||
							(*totData == 6) || (*totData == 7)) {
						WRITE_VARO_UINT8(stringVar, 0);
						delete textItem;
						return false;
					}

					switch (*totData) {
					case 2:
					case 5:
						totData += 5;
						break;

					case 3:
					case 4:
						totData += 2;
						break;
					}
				}

			}
			break;

		case 7:
		case 8:
		case 9:
			totData++;
			break;

		case 10:
			if (curPart == part) {
				WRITE_VARO_UINT8(stringVar++, 0xFF);
				WRITE_VARO_UINT16(stringVar, offX);
				WRITE_VARO_UINT16(stringVar + 2, offY);
				WRITE_VARO_UINT16(stringVar + 4,
						totData - _vm->_game->_resources->getTexts());
				WRITE_VARO_UINT8(stringVar + 6, 0);
				delete textItem;
				return false;
			}

			end = false;
			while (!end) {
				switch (*totData) {
				case 2:
				case 5:
					if (ABS(offY - READ_LE_UINT16(totData + 3)) > 1)
						end = true;
					else
						totData += 5;
					break;

				case 3:
					totData += 2;
					break;

				case 10:
					totData += totData[1] * 2 + 2;
					break;

				default:
					if (*totData < 32)
						end = true;
					while (*totData >= 32)
						totData++;
					break;
				}
			}

			if (part >= 0)
				curPart++;
			break;

		default:
			while (1) {

				while (*totData >= 32)
					WRITE_VARO_UINT8(stringVar++, *totData++);
				WRITE_VARO_UINT8(stringVar, 0);

				if (((*totData != 2) && (*totData != 5)) ||
						(ABS(offY - READ_LE_UINT16(totData + 3)) > 1)) {

					if (curPart == part) {
						delete textItem;
						return false;
					}

					stringVar = stringStartVar;
					WRITE_VARO_UINT8(stringVar, 0);

					while (*totData >= 32)
						totData++;

					if (part >= 0)
						curPart++;
					break;

				} else
					totData += 5;

			}
			break;
		}
	}

	delete textItem;
	return false;
}

bool Inter_v3::o3_copySprite(OpFuncParams &params) {
	o1_copySprite(params);

	// For the close-up "fading" in the CD version
	if (_vm->_draw->_destSurface == 20)
		_vm->_video->sparseRetrace(20);
	return false;
}

void Inter_v3::o3_wobble(OpGobParams &params) {
	_vm->_draw->wobble(*_vm->_draw->_backSurface);
}

} // End of namespace Gob
