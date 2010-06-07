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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/introproc_saga2.cpp $
 * $Id: introproc_saga2.cpp 40759 2009-05-21 13:02:56Z thebluegr $
 *
 */

#ifdef ENABLE_SAGA2

// "Dinotopia" and "Faery Tale Adventure II: Halls of the Dead" Intro sequence scene procedures

#include "saga/saga.h"
#include "saga/scene.h"
#include "saga/gfx.h"

#include "sound/mixer.h"
#include "graphics/surface.h"
#include "graphics/video/smk_decoder.h"

#include "common/events.h"
#include "common/system.h"
#include "common/list.h"

namespace Saga {

Common::List<Common::Event> stopEvents;

int Scene::DinoStartProc() {
	_vm->_gfx->showCursor(false);

	Common::Event stopEvent;
	stopEvents.clear();
	stopEvent.type = Common::EVENT_KEYDOWN;
	stopEvent.kbd = Common::KEYCODE_ESCAPE;
	stopEvents.push_back(stopEvent);

	Graphics::SmackerDecoder *smkDecoder = new Graphics::SmackerDecoder(_vm->_mixer);
	Graphics::VideoPlayer *player = new Graphics::VideoPlayer(smkDecoder);
	if (smkDecoder->loadFile("testvid.smk"))
		player->playVideo(stopEvents);        // Play introduction
	smkDecoder->closeFile();
	delete player;
	delete smkDecoder;

	// HACK: Forcibly quit here
	_vm->quitGame();

	return SUCCESS;
}

int Scene::FTA2StartProc() {
	_vm->_gfx->showCursor(false);

	Common::Event stopEvent;
	stopEvents.clear();
	stopEvent.type = Common::EVENT_KEYDOWN;
	stopEvent.kbd = Common::KEYCODE_ESCAPE;
	stopEvents.push_back(stopEvent);

	Graphics::SmackerDecoder *smkDecoder = new Graphics::SmackerDecoder(_vm->_mixer);
	Graphics::VideoPlayer *player = new Graphics::VideoPlayer(smkDecoder);
	if (smkDecoder->loadFile("trimark.smk"))
		player->playVideo(stopEvents);      // Show Ignite logo
	smkDecoder->closeFile();
	if (smkDecoder->loadFile("intro.smk"))
		player->playVideo(stopEvents);        // Play introduction
	smkDecoder->closeFile();
	delete player;
	delete smkDecoder;

	// HACK: Forcibly quit here
	_vm->quitGame();

	return SUCCESS;
}

int Scene::FTA2EndProc(FTA2Endings whichEnding) {
	char videoName[20];

	switch (whichEnding) {
	case kFta2BadEndingLaw:
		strcpy(videoName, "end_1.smk");
		break;
	case kFta2BadEndingChaos:
		strcpy(videoName, "end_2.smk");
		break;
	case kFta2GoodEnding1:
		strcpy(videoName, "end_3a.smk");
		break;
	case kFta2GoodEnding2:
		strcpy(videoName, "end_3b.smk");
		break;
	case kFta2BadEndingDeath:
		strcpy(videoName, "end_4.smk");
		break;
	default:
		error("Unknown FTA2 ending");
	}

	_vm->_gfx->showCursor(false);


	Common::Event stopEvent;
	stopEvents.clear();
	stopEvent.type = Common::EVENT_KEYDOWN;
	stopEvent.kbd = Common::KEYCODE_ESCAPE;
	stopEvents.push_back(stopEvent);

	// Play ending
	Graphics::SmackerDecoder *smkDecoder = new Graphics::SmackerDecoder(_vm->_mixer);
	Graphics::VideoPlayer *player = new Graphics::VideoPlayer(smkDecoder);
	if (smkDecoder->loadFile(videoName)) {
		player->playVideo(stopEvents);
		smkDecoder->closeFile();
	}
	delete player;
	delete smkDecoder;

	return SUCCESS;
}

} // End of namespace Saga

#endif
