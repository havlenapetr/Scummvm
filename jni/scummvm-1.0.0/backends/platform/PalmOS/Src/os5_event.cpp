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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/os5_event.cpp $
 * $Id: os5_event.cpp 27024 2007-05-30 21:56:52Z fingolfin $
 *
 */

#include "be_os5.h"

void OSystem_PalmOS5::get_coordinates(EventPtr ev, Coord &x, Coord &y) {
	Boolean dummy;
	EvtGetPenNative(WinGetDisplayWindow(), &ev->screenX, &ev->screenY, &dummy);

	x = (ev->screenX - _screenOffset.x);
	y = (ev->screenY - _screenOffset.y);

	if (_stretched) {
		x = (x * _screenWidth) / _screenDest.w;
		y = (y * _screenHeight) / _screenDest.h;
	}
}

bool OSystem_PalmOS5::check_event(Common::Event &event, EventPtr ev) {
	if (ev->eType == keyUpEvent) {
		switch (ev->data.keyUp.chr) {
		case vchrHard3:
		case vchrHard4:
			// will be handled by hard keys
			return true;
		}

	} else if (ev->eType == keyDownEvent) {
		switch (ev->data.keyDown.chr) {

		// hot swap gfx
//	case 0x1B04:
		case vchrHard1:
			if (OPTIONS_TST(kOptCollapsible))
				hotswap_gfx_mode(_mode == GFX_WIDE ? GFX_NORMAL: GFX_WIDE);
			return false; // not a key

//	case 0x1B05:
		case vchrHard2:
			setFeatureState(kFeatureAspectRatioCorrection, 0);
			return false; // not a key

		case vchrHard3:
			_keyExtraPressed |= _keyExtra.bitActionA;
			event.type = Common::EVENT_LBUTTONDOWN;
			event.mouse.x = _mouseCurState.x;
			event.mouse.y = _mouseCurState.y;
			return true;

		case vchrHard4:
			_keyExtraPressed |= _keyExtra.bitActionB;
			event.type = Common::EVENT_RBUTTONDOWN;
			event.mouse.x = _mouseCurState.x;
			event.mouse.y = _mouseCurState.y;
			return true;
		}
	}

	return false;
}
