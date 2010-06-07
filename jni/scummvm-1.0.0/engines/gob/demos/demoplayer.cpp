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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/demos/demoplayer.cpp $
 * $Id: demoplayer.cpp 43666 2009-08-23 09:58:18Z drmccoy $
 *
 */

#include "common/endian.h"
#include "common/file.h"

#include "gob/gob.h"
#include "gob/demos/demoplayer.h"
#include "gob/helper.h"
#include "gob/global.h"
#include "gob/util.h"
#include "gob/draw.h"
#include "gob/inter.h"
#include "gob/videoplayer.h"

namespace Gob {

DemoPlayer::Script DemoPlayer::_scripts[] = {
	{kScriptSourceFile, "demo.scn"},
	{kScriptSourceFile, "wdemo.s24"},
	{kScriptSourceFile, "play123.scn"},
	{kScriptSourceFile, "e.scn"},
	{kScriptSourceFile, "i.scn"},
	{kScriptSourceFile, "s.scn"},
	{kScriptSourceDirect,
		"slide machu.imd 20\nslide conseil.imd 20\nslide cons.imd 20\n" \
		"slide tumia.imd 1\nslide tumib.imd 1\nslide tumic.imd 1\n"     \
		"slide tumid.imd 1\nslide post.imd 1\nslide posta.imd 1\n"      \
		"slide postb.imd 1\nslide postc.imd 1\nslide xdome.imd 20\n"    \
		"slide xant.imd 20\nslide tum.imd 20\nslide voile.imd 20\n"     \
		"slide int.imd 20\nslide voila.imd 1\nslide voilb.imd 1\n"},
	{kScriptSourceFile, "coktelplayer.scn"},
};

DemoPlayer::DemoPlayer(GobEngine *vm) : _vm(vm) {
	_autoDouble = false;
	_doubleMode = false;
	_rebase0 = false;
}

DemoPlayer::~DemoPlayer() {
}

bool DemoPlayer::play(const char *fileName) {
	if (!fileName)
		return false;

	debugC(1, kDebugDemo, "Playing \"%s\"", fileName);

	init();

	Common::File bat;

	if (!bat.open(fileName))
		return false;

	return playStream(bat);
}

bool DemoPlayer::play(uint32 index) {
	if (index >= ARRAYSIZE(_scripts))
		return false;

	Script &script = _scripts[index];

	debugC(1, kDebugDemo, "Playing demoIndex %d: %d", index, script.source);

	switch (script.source) {
	case kScriptSourceFile:
		return play(script.script);

	case kScriptSourceDirect:
		{
			Common::MemoryReadStream stream((const byte *) script.script, strlen(script.script));

			init();
			return playStream(stream);
		}

	default:
		return false;
	}

	return false;
}

bool DemoPlayer::lineStartsWith(const Common::String &line, const char *start) {
	return (strstr(line.c_str(), start) == line.c_str());
}

void DemoPlayer::init() {
	// The video player needs some fake variables
	_vm->_inter->allocateVars(32);

	// Init the screen
	_vm->_draw->initScreen();
	_vm->_draw->_cursorIndex = -1;

	_vm->_util->longDelay(200); // Letting everything settle

}

void DemoPlayer::clearScreen() {
	debugC(1, kDebugDemo, "Clearing the screen");
	_vm->_video->clearSurf(*_vm->_draw->_backSurface);
	_vm->_draw->forceBlit();
	_vm->_video->retrace();
}

void DemoPlayer::playVideo(const char *fileName) {
	uint32 waitTime = 0;
	char *file, *filePtr;

	file = filePtr = strdup(fileName);

	// Trimming spaces front
	while (*file == ' ')
		file++;

	char *spaceBack = strchr(file, ' ');
	if (spaceBack) {
		char *nextSpace = strchr(spaceBack, ' ');

		if (nextSpace)
			*nextSpace = '\0';

		*spaceBack++ = '\0';

		waitTime = atoi(spaceBack) * 100;
	}

	debugC(1, kDebugDemo, "Playing video \"%s\"", file);

	int16 x = _rebase0 ? 0 : -1;
	int16 y = _rebase0 ? 0 : -1;
	if (_vm->_vidPlayer->primaryOpen(file, x, y)) {
		bool videoSupportsDouble =
			((_vm->_vidPlayer->getFeatures() & Graphics::CoktelVideo::kFeaturesSupportsDouble) != 0);

		if (_autoDouble) {
			int16 defX = _rebase0 ? 0 : _vm->_vidPlayer->getDefaultX();
			int16 defY = _rebase0 ? 0 : _vm->_vidPlayer->getDefaultY();
			int16 right  = defX + _vm->_vidPlayer->getWidth()  - 1;
			int16 bottom = defY + _vm->_vidPlayer->getHeight() - 1;

			_doubleMode = ((right < 320) && (bottom < 200));
		}

		if (_doubleMode) {
			if (videoSupportsDouble) {
				_vm->_vidPlayer->slotSetDoubleMode(-1, true);
				playVideoNormal();
			} else
				playVideoDoubled();
		} else
			playVideoNormal();

		_vm->_vidPlayer->primaryClose();

		if (waitTime > 0)
			_vm->_util->longDelay(waitTime);
	}


	free(filePtr);
}

void DemoPlayer::playVideoNormal() {
	_vm->_vidPlayer->primaryPlay();
}

void DemoPlayer::playVideoDoubled() {
	Common::String fileNameOpened = _vm->_vidPlayer->getFileName();
	_vm->_vidPlayer->primaryClose();

	int16 x = _rebase0 ? 0 : -1;
	int16 y = _rebase0 ? 0 : -1;
	if (_vm->_vidPlayer->primaryOpen(fileNameOpened.c_str(), x, y,
				VideoPlayer::kFlagScreenSurface)) {

		for (int i = 0; i < _vm->_vidPlayer->getFramesCount(); i++) {
			_vm->_vidPlayer->playFrame(i);

			Graphics::CoktelVideo::State state = _vm->_vidPlayer->getState();

			int16 w = state.right - state.left + 1;
			int16 h = state.bottom - state.top + 1;
			int16 wD = (state.left * 2) + (w * 2);
			int16 hD = (state.top * 2) + (h * 2);

			_vm->_video->drawSpriteDouble(*_vm->_draw->_spritesArray[0], *_vm->_draw->_frontSurface,
					state.left, state.top, state.right, state.bottom, state.left, state.top, 0);
			_vm->_draw->dirtiedRect(_vm->_draw->_frontSurface,
					state.left * 2, state.top * 2, wD, hD);
			_vm->_video->retrace();

			_vm->_util->processInput();
			if (_vm->shouldQuit())
				break;

			int16 key;
			bool end = false;
			while (_vm->_util->checkKey(key))
				if (key == kKeyEscape)
					end = true;
			if (end)
				break;

			_vm->_vidPlayer->slotWaitEndFrame();

		}
	}
}

void DemoPlayer::evaluateVideoMode(const char *mode) {
	debugC(2, kDebugDemo, "Video mode \"%s\"", mode);

	_autoDouble = false;
	_doubleMode = false;

	// Only applicable when we actually can double
	if (_vm->is640()) {
		if (!scumm_strnicmp(mode, "AUTO", 4))
			_autoDouble = true;
		else if (!scumm_strnicmp(mode, "VGA", 3) && _vm->is640())
			_doubleMode = true;
	}
}

} // End of namespace Gob
