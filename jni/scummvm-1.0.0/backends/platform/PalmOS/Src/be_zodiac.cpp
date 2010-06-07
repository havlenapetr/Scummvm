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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/be_zodiac.cpp $
 * $Id: be_zodiac.cpp 39028 2009-03-01 10:25:33Z chrilith $
 *
 */

#include "be_zodiac.h"

OSystem_PalmZodiac::OSystem_PalmZodiac() : OSystem_PalmOS5Ex() {
	_gfxH = NULL;
	_overlayP = NULL;
	_palmScreenP = NULL;
	_tmpScreenP = NULL;
	_stretched = false;
}

void OSystem_PalmZodiac::int_initBackend() {
	_keyExtra.bitUp			= keyBitRockerUp;
	_keyExtra.bitDown		= keyBitRockerDown;
	_keyExtra.bitLeft		= keyBitRockerLeft;
	_keyExtra.bitRight		= keyBitRockerRight;

//	_keyExtra.bitActionA	= keyBitActionD;
//	_keyExtra.bitActionB	= keyBitActionB;
}

void OSystem_PalmZodiac::calc_rect(Boolean fullscreen) {
	OSystem_PalmOS5::calc_rect(fullscreen);

	_dstRect.x = _screenOffset.x;
	_dstRect.y = _screenOffset.y;
	_dstRect.w = _screenDest.w;
	_dstRect.h = _screenDest.h;
}

void OSystem_PalmZodiac::setFeatureState(Feature f, bool enable) {
	switch (f) {
/*		case kFeatureFullscreenMode:
			if (_gfxLoaded)
				if (OPTIONS_TST(kOptModeWide) && _initMode != GFX_WIDE) {
					_fullscreen = enable;
					hotswap_gfx_mode(_mode);
				}
			break;
*/
		case kFeatureAspectRatioCorrection:
			if (_mode == GFX_WIDE) {
				_ratio.adjustAspect = (_ratio.adjustAspect + 1) % 3;
				calc_rect(true);
				TwGfxSetClip(_palmScreenP, &_dstRect);
				clear_screen();
			}
			break;
	}
}
