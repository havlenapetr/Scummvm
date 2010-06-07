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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/game.h $
 * $Id: game.h 43148 2009-08-08 21:43:43Z drmccoy $
 *
 */

#ifndef GOB_GAME_H
#define GOB_GAME_H

#include "gob/util.h"

namespace Gob {

class Script;
class Resources;
class Variables;
class Hotspots;

class Environments {
public:
	static const uint8 kEnvironmentCount = 5;

	Environments(GobEngine *vm);
	~Environments();

	void set(uint8 env);
	void get(uint8 env) const;

	const char *getTotFile(uint8 env) const;

	bool has(Variables *variables, uint8 startEnv = 0, int16 except = -1) const;
	bool has(Script *script      , uint8 startEnv = 0, int16 except = -1) const;
	bool has(Resources *resources, uint8 startEnv = 0, int16 except = -1) const;

	void clear();

private:
	struct Environment {
		int16      cursorHotspotX;
		int16      cursorHotspotY;
		char       curTotFile[14];
		Variables *variables;
		Script    *script;
		Resources *resources;
	};

	GobEngine *_vm;

	Environment *_environments;
};

class Game {
public:
	Script    *_script;
	Resources *_resources;
	Hotspots  *_hotspots;

	char _curTotFile[14];
	char _totToLoad[20];

	int32 _startTimeKey;
	MouseButtons _mouseButtons;

	bool _noScroll;
	bool _preventScroll;

	bool  _wantScroll;
	int16 _wantScrollX;
	int16 _wantScrollY;

	byte _handleMouse;
	char _forceHandleMouse;

	Game(GobEngine *vm);
	virtual ~Game();

	void prepareStart();

	void playTot(int16 skipPlay);

	void capturePush(int16 left, int16 top, int16 width, int16 height);
	void capturePop(char doDraw);

	void freeSoundSlot(int16 slot);

	void wantScroll(int16 x, int16 y);
	void evaluateScroll();

	int16 checkKeys(int16 *pMousex = 0, int16 *pMouseY = 0,
			MouseButtons *pButtons = 0, char handleMouse = 0);
	void start();

	void totSub(int8 flags, const char *newTotFile);
	void switchTotSub(int16 index, int16 skipPlay);

protected:
	char _tempStr[256];

	// Capture
	Common::Rect _captureStack[20];
	int16 _captureCount;

	// For totSub()
	int8 _curEnvironment;
	int8 _numEnvironments;
	Environments *_environments;

	GobEngine *_vm;

	void clearUnusedEnvironment();
};

} // End of namespace Gob

#endif // GOB_GAME_H
