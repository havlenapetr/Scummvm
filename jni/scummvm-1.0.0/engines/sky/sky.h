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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/sky.h $
 * $Id: sky.h 39002 2009-03-01 04:30:55Z fingolfin $
 *
 */

#ifndef SKY_H
#define SKY_H


#include "common/events.h"
#include "engines/engine.h"

namespace Sky {

struct SystemVars {
	uint32 systemFlags;
	uint32 gameVersion;
	uint32 mouseFlag;
	uint16 language;
	uint32 currentPalette;
	uint16 gameSpeed;
	uint16 currentMusic;
	bool pastIntro;
	bool paused;
};

struct Compact;
class Sound;
class Disk;
class Text;
class Logic;
class Mouse;
class Screen;
class Control;
class MusicBase;
class Debugger;
class SkyCompact;

class SkyEngine : public Engine {
protected:
	Common::KeyState _keyPressed;

	Sound *_skySound;
	Disk *_skyDisk;
	Text *_skyText;
	Logic *_skyLogic;
	Mouse *_skyMouse;
	Screen *_skyScreen;
	Control *_skyControl;
	SkyCompact *_skyCompact;
	Debugger *_debugger;

	MusicBase *_skyMusic;

public:
	SkyEngine(OSystem *syst);
	virtual ~SkyEngine();

	static bool isDemo(void);
	static bool isCDVersion(void);

	Common::Error loadGameState(int slot);
	Common::Error saveGameState(int slot, const char *desc);
	bool canLoadGameStateCurrently();
	bool canSaveGameStateCurrently();

	static void *fetchItem(uint32 num);
	static void *_itemList[300];

	static SystemVars _systemVars;

protected:
	// Engine APIs
	Common::Error init();
	Common::Error go();
	virtual Common::Error run() {
		Common::Error err;
		err = init();
		if (err != Common::kNoError)
			return err;
		return go();
	}
	virtual GUI::Debugger *getDebugger();
	virtual bool hasFeature(EngineFeature f) const;

	byte _fastMode;

	void delay(int32 amount);
	void handleKey(void);

	uint32 _lastSaveTime;

	void initItemList();

	void initVirgin();
	static void timerHandler(void *ptr);
	void gotTimerTick();
	void loadFixedItems();
};

} // End of namespace Sky

#endif
