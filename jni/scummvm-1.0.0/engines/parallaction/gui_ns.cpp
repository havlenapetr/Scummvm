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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/parallaction/gui_ns.cpp $
 * $Id: gui_ns.cpp 39813 2009-04-03 16:32:47Z peres001 $
 *
 */

#include "common/system.h"
#include "common/hashmap.h"

#include "parallaction/gui.h"
#include "parallaction/input.h"
#include "parallaction/parallaction.h"
#include "parallaction/saveload.h"
#include "parallaction/sound.h"


namespace Parallaction {

class SplashInputState_NS : public MenuInputState {
protected:
	Common::String _slideName;
	uint32 _timeOut;
	Common::String _nextState;
	uint32	_startTime;

	Parallaction *_vm;

public:
	SplashInputState_NS(Parallaction *vm, const Common::String &name, MenuInputHelper *helper) : MenuInputState(name, helper), _vm(vm)  {
	}

	virtual MenuInputState* run() {
		uint32 curTime = _vm->_system->getMillis();
		if (curTime - _startTime > _timeOut) {
			return _helper->getState(_nextState);
		}
		return this;
	}

	virtual void enter() {
		_vm->_input->setMouseState(MOUSE_DISABLED);
		_vm->showSlide(_slideName.c_str());
		_startTime = _vm->_system->getMillis();
	}
};

class SplashInputState0_NS : public SplashInputState_NS {

public:
	SplashInputState0_NS(Parallaction *vm, MenuInputHelper *helper) : SplashInputState_NS(vm, "intro0", helper)  {
		_slideName = "intro";
		_timeOut = 2000;
		_nextState = "intro1";
	}
};

class SplashInputState1_NS : public SplashInputState_NS {

public:
	SplashInputState1_NS(Parallaction *vm, MenuInputHelper *helper) : SplashInputState_NS(vm, "intro1", helper) {
		_slideName = "minintro";
		_timeOut = 2000;
		_nextState = "chooselanguage";
	}
};


class ChooseLanguageInputState_NS : public MenuInputState {
	#define BLOCK_WIDTH		16
	#define BLOCK_HEIGHT	24

	#define BLOCK_X			112
	#define BLOCK_Y			130

	#define BLOCK_SELECTION_X		  (BLOCK_X-1)
	#define BLOCK_SELECTION_Y		  (BLOCK_Y-1)

	#define BLOCK_X_OFFSET	(BLOCK_WIDTH+1)
	#define BLOCK_Y_OFFSET	9

	//	destination slots for code blocks
	//
	#define SLOT_X			61
	#define SLOT_Y			64
	#define SLOT_WIDTH		(BLOCK_WIDTH+2)

	int	_language;
	bool	_allowChoice;
	Common::String _nextState;

	static const Common::Rect _dosLanguageSelectBlocks[4];
	static const Common::Rect _amigaLanguageSelectBlocks[4];
	const Common::Rect *_blocks;

	Parallaction *_vm;

public:
	ChooseLanguageInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("chooselanguage", helper), _vm(vm) {
		_allowChoice = false;
		_nextState = "selectgame";

		if (_vm->getPlatform() == Common::kPlatformAmiga) {
			if (!(_vm->getFeatures() & GF_LANG_MULT)) {
				if (_vm->getFeatures() & GF_DEMO) {
					_language = 1;	// Amiga Demo supports English
					_nextState = "startdemo";
					return;
				} else {
					_language = 0;	// The only other non multi-lingual version just supports Italian
					return;
				}
			}

			_blocks = _amigaLanguageSelectBlocks;
		} else {
			_blocks = _dosLanguageSelectBlocks;
		}

		_language = -1;
		_allowChoice = true;
	}

	virtual MenuInputState* run() {
		if (!_allowChoice) {
			_vm->setInternLanguage(_language);
			return _helper->getState(_nextState);
		}

		int event = _vm->_input->getLastButtonEvent();
		if (event != kMouseLeftUp) {
			return this;
		}

		Common::Point p;
		_vm->_input->getCursorPos(p);

		for (uint16 i = 0; i < 4; i++) {
			if (_blocks[i].contains(p)) {
				_vm->setInternLanguage(i);
				_vm->beep();
				_vm->_gfx->freeLabels();
				return _helper->getState(_nextState);
			}
		}

		return this;
	}

	virtual void enter() {
		if (!_allowChoice) {
			return;
		}

		_vm->_input->setMouseState(MOUSE_ENABLED_SHOW);

		// user can choose language in this version
		_vm->showSlide("lingua");

		uint id = _vm->_gfx->createLabel(_vm->_introFont, "SELECT LANGUAGE", 1);
		_vm->_gfx->showLabel(id, 60, 30);

		_vm->_input->setArrowCursor();
	}
};

const Common::Rect ChooseLanguageInputState_NS::_dosLanguageSelectBlocks[4] = {
	Common::Rect(  80, 110, 128, 180 ),	// Italian
	Common::Rect( 129,  85, 177, 155 ),	// French
	Common::Rect( 178,  60, 226, 130 ),	// English
	Common::Rect( 227,  35, 275, 105 )	// German
};

const Common::Rect ChooseLanguageInputState_NS::_amigaLanguageSelectBlocks[4] = {
	Common::Rect(  -1,  -1,  -1,  -1 ),	// Italian: not supported by Amiga multi-lingual version
	Common::Rect( 129,  85, 177, 155 ),	// French
	Common::Rect( 178,  60, 226, 130 ),	// English
	Common::Rect( 227,  35, 275, 105 )	// German
};

class SelectGameInputState_NS : public MenuInputState {

	int _choice, _oldChoice;
	Common::String _nextState[2];

	uint	_labels[2];

	Parallaction *_vm;

	static const char *newGameMsg[4];
	static const char *loadGameMsg[4];

public:
	SelectGameInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("selectgame", helper), _vm(vm) {
		_choice = 0;
		_oldChoice = -1;

		_nextState[0] = "newgame";
		_nextState[1] = "loadgame";
	}


	virtual MenuInputState *run() {
		int event = _vm->_input->getLastButtonEvent();

		if (event == kMouseLeftUp) {
			_vm->_gfx->freeLabels();
			return _helper->getState(_nextState[_choice]);
		}

		Common::Point p;
		_vm->_input->getCursorPos(p);
		_choice = (p.x > 160) ? 1 : 0;

		if (_choice != _oldChoice) {
			if (_oldChoice != -1)
				_vm->_gfx->hideLabel(_labels[_oldChoice]);

			if (_choice != -1)
				_vm->_gfx->showLabel(_labels[_choice], 60, 30);

			_oldChoice = _choice;
		}

		return this;
	}

	virtual void enter() {
		_vm->showSlide("restore");
		_vm->_input->setMouseState(MOUSE_ENABLED_SHOW);

		_labels[0] = _vm->_gfx->createLabel(_vm->_introFont, newGameMsg[_vm->getInternLanguage()], 1);
		_labels[1] = _vm->_gfx->createLabel(_vm->_introFont, loadGameMsg[_vm->getInternLanguage()], 1);
	}

};

const char *SelectGameInputState_NS::newGameMsg[4] = {
	"NUOVO GIOCO",
	"NEUF JEU",
	"NEW GAME",
	"NEUES SPIEL"
};

const char *SelectGameInputState_NS::loadGameMsg[4] = {
	"GIOCO SALVATO",
	"JEU SAUVE'",
	"SAVED GAME",
	"SPIEL GESPEICHERT"
};



class LoadGameInputState_NS : public MenuInputState {
	bool _result;
	Parallaction *_vm;

public:
	LoadGameInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("loadgame", helper), _vm(vm) { }

	virtual MenuInputState* run() {
		if (!_result) {
			_vm->scheduleLocationSwitch("fogne.dough");
		}
		return 0;
	}

	virtual void enter() {
		_result = _vm->_saveLoad->loadGame();
	}
};



class NewGameInputState_NS : public MenuInputState {
	Parallaction_ns *_vm;

	static const char *introMsg3[4];

public:
	NewGameInputState_NS(Parallaction_ns *vm, MenuInputHelper *helper) : MenuInputState("newgame", helper), _vm(vm) {
	}

	virtual MenuInputState* run() {
		int event = _vm->_input->getLastButtonEvent();

		if (event == kMouseLeftUp || event == kMouseRightUp) {
			_vm->_input->setMouseState(MOUSE_ENABLED_SHOW);
			_vm->_gfx->freeLabels();

			if (event == kMouseLeftUp) {
				_vm->scheduleLocationSwitch("fogne.dough");
				return 0;
			}

			return _helper->getState("selectcharacter");
		}

		return this;
	}

	virtual void enter() {
		_vm->changeBackground("test");
		_vm->_input->setMouseState(MOUSE_ENABLED_HIDE);

		uint id[4];
		id[0] = _vm->_gfx->createLabel(_vm->_menuFont, introMsg3[0], 1);
		id[1] = _vm->_gfx->createLabel(_vm->_menuFont, introMsg3[1], 1);
		id[2] = _vm->_gfx->createLabel(_vm->_menuFont, introMsg3[2], 1);
		id[3] = _vm->_gfx->createLabel(_vm->_menuFont, introMsg3[3], 1);
		_vm->_gfx->showLabel(id[0], CENTER_LABEL_HORIZONTAL, 50);
		_vm->_gfx->showLabel(id[1], CENTER_LABEL_HORIZONTAL, 70);
		_vm->_gfx->showLabel(id[2], CENTER_LABEL_HORIZONTAL, 100);
		_vm->_gfx->showLabel(id[3], CENTER_LABEL_HORIZONTAL, 120);
	}
};

const char *NewGameInputState_NS::introMsg3[4] = {
	"PRESS LEFT MOUSE BUTTON",
	"TO SEE INTRO",
	"PRESS RIGHT MOUSE BUTTON",
	"TO START"
};



class StartDemoInputState_NS : public MenuInputState {
	Parallaction *_vm;

public:
	StartDemoInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("startdemo", helper), _vm(vm) {
	}

	virtual MenuInputState* run() {
		_vm->scheduleLocationSwitch("fognedemo.dough");
		_vm->_input->setMouseState(MOUSE_ENABLED_SHOW);
		return 0;
	}

	virtual void enter() {
		_vm->_input->setMouseState(MOUSE_DISABLED);
	}
};

class SelectCharacterInputState_NS : public MenuInputState {

	#define PASSWORD_LEN	6

	#define CHAR_DINO	0
	#define CHAR_DONNA	1
	#define CHAR_DOUGH	2

	static const Common::Rect codeSelectBlocks[9];
	static const Common::Rect codeTrueBlocks[9];

	Parallaction_ns *_vm;

	int guiGetSelectedBlock(const Common::Point &p) {

		int selection = -1;

		for (uint16 i = 0; i < 9; i++) {
			if (codeSelectBlocks[i].contains(p)) {
				selection = i;
				break;
			}
		}

		if ((selection != -1) && (_vm->getPlatform() == Common::kPlatformAmiga)) {
			_vm->_gfx->invertBackground(codeTrueBlocks[selection]);
			_vm->_gfx->updateScreen();
			_vm->beep();
			_vm->_system->delayMillis(100);
			_vm->_gfx->invertBackground(codeTrueBlocks[selection]);
			_vm->_gfx->updateScreen();
		}

		return selection;
	}

	byte	_points[3];
	bool	_fail;
	const uint16 (*_keys)[PASSWORD_LEN];
	Graphics::Surface _block;
	Graphics::Surface _emptySlots;

	uint	_labels[2];
	uint	_len;
	uint32	_startTime;

	enum {
		CHOICE,
		FAIL,
		SUCCESS,
		DELAY
	};

	uint	_state;

	static const char *introMsg1[4];
	static const char *introMsg2[4];

	static const uint16 _amigaKeys[3][PASSWORD_LEN];
	static const uint16 _pcKeys[3][PASSWORD_LEN];
	static const char *_charStartLocation[3];


public:
	SelectCharacterInputState_NS(Parallaction_ns *vm, MenuInputHelper *helper) : MenuInputState("selectcharacter", helper), _vm(vm) {
		_keys = (_vm->getPlatform() == Common::kPlatformAmiga && (_vm->getFeatures() & GF_LANG_MULT)) ? _amigaKeys : _pcKeys;
		_block.create(BLOCK_WIDTH, BLOCK_HEIGHT, 1);
	}

	~SelectCharacterInputState_NS() {
		_block.free();
		_emptySlots.free();
	}

	void cleanup() {
		_points[0] = _points[1] = _points[2] = 0;
		_vm->_gfx->hideLabel(_labels[1]);
		_vm->_gfx->showLabel(_labels[0], 60, 30);
		_fail = false;
		_len = 0;
	}

	void delay() {
		if (_vm->_system->getMillis() - _startTime < 2000) {
			return;
		}
		cleanup();
		_state = CHOICE;
	}

	void choice() {
		int event = _vm->_input->getLastButtonEvent();
		if (event != kMouseLeftUp) {
			return;
		}

		Common::Point p;
		_vm->_input->getCursorPos(p);
		int _si = guiGetSelectedBlock(p);

		if (_si != -1) {
			_vm->_gfx->grabBackground(codeTrueBlocks[_si], _block);
			_vm->_gfx->patchBackground(_block, _len * SLOT_WIDTH + SLOT_X, SLOT_Y, false);

			if (_keys[0][_len] != _si && _keys[1][_len] != _si && _keys[2][_len] != _si) {
				_fail = true;
			}

			// build user preference
			_points[0] += (_keys[0][_len] == _si);
			_points[1] += (_keys[1][_len] == _si);
			_points[2] += (_keys[2][_len] == _si);

			_len++;
		}

		if (_len == PASSWORD_LEN) {
			_state = _fail ? FAIL : SUCCESS;
		}
	}

	void fail() {
		_vm->_gfx->patchBackground(_emptySlots, SLOT_X, SLOT_Y, false);
		_vm->_gfx->hideLabel(_labels[0]);
		_vm->_gfx->showLabel(_labels[1], 60, 30);
		_startTime = _vm->_system->getMillis();
		_state = DELAY;
	}

	void success() {
		_vm->_gfx->freeLabels();
		_vm->_gfx->setBlackPalette();
		_emptySlots.free();

		// actually select character
		int character = -1;
		if (_points[0] >= _points[1] && _points[0] >= _points[2]) {
			character = CHAR_DINO;
		} else
		if (_points[1] >= _points[0] && _points[1] >= _points[2]) {
			character = CHAR_DONNA;
		} else
		if (_points[2] >= _points[0] && _points[2] >= _points[1]) {
			character = CHAR_DOUGH;
		} else {
			error("If you read this, either your CPU or transivity is broken (we believe the former).");
		}

		_vm->cleanupGame();
		_vm->scheduleLocationSwitch(_charStartLocation[character]);
	}

	virtual MenuInputState* run() {
		MenuInputState* nextState = this;

		switch (_state) {
		case DELAY:
			delay();
			break;

		case CHOICE:
			choice();
			break;

		case FAIL:
			fail();
			break;

		case SUCCESS:
			success();
			nextState = 0;
			break;

		default:
			error("unknown state in SelectCharacterInputState");
		}

		return nextState;
	}

	virtual void enter() {
		_vm->_soundManI->stopMusic();
		_vm->showSlide("password");

		_emptySlots.create(BLOCK_WIDTH * 8, BLOCK_HEIGHT, 1);
		Common::Rect rect(SLOT_X, SLOT_Y, SLOT_X + BLOCK_WIDTH * 8, SLOT_Y + BLOCK_HEIGHT);
		_vm->_gfx->grabBackground(rect, _emptySlots);

		_labels[0] = _vm->_gfx->createLabel(_vm->_introFont, introMsg1[_vm->getInternLanguage()], 1);
		_labels[1] = _vm->_gfx->createLabel(_vm->_introFont, introMsg2[_vm->getInternLanguage()], 1);

		cleanup();

		_vm->_input->setArrowCursor();
		_vm->_input->setMouseState(MOUSE_ENABLED_SHOW);
		_state = CHOICE;
	}
};

const char *SelectCharacterInputState_NS::introMsg1[4] = {
	"INSERISCI IL CODICE",
	"ENTREZ CODE",
	"ENTER CODE",
	"GIB DEN KODE EIN"
};

const char *SelectCharacterInputState_NS::introMsg2[4] = {
	"CODICE ERRATO",
	"CODE ERRONE",
	"WRONG CODE",
	"GIB DEN KODE EIN"
};

const uint16 SelectCharacterInputState_NS::_amigaKeys[][PASSWORD_LEN] = {
	{ 5, 3, 6, 2, 2, 7 },		// dino
	{ 0, 3, 6, 2, 2, 6 },		// donna
	{ 1, 3 ,7, 2, 4, 6 }		// dough
};

const uint16 SelectCharacterInputState_NS::_pcKeys[][PASSWORD_LEN] = {
	{ 5, 3, 6, 1, 4, 7 },		// dino
	{ 0, 2, 8, 5, 5, 1 },		// donna
	{ 1, 7 ,7, 2, 2, 6 }		// dough
};

const char *SelectCharacterInputState_NS::_charStartLocation[] = {
	"test.dino",
	"test.donna",
	"test.dough"
};


const Common::Rect SelectCharacterInputState_NS::codeSelectBlocks[9] = {
	Common::Rect( 111, 129, 127, 153 ),		// na
	Common::Rect( 128, 120, 144, 144 ),		// wa
	Common::Rect( 145, 111, 161, 135 ),		// ra
	Common::Rect( 162, 102, 178, 126 ),		// ri
	Common::Rect( 179, 93, 195, 117 ),		// i
	Common::Rect( 196, 84, 212, 108 ),		// ne
	Common::Rect( 213, 75, 229, 99 ),		// ho
	Common::Rect( 230, 66, 246, 90 ),		// ki
	Common::Rect( 247, 57, 263, 81 )		// ka
};

const Common::Rect SelectCharacterInputState_NS::codeTrueBlocks[9] = {
	Common::Rect( 112, 130, 128, 154 ),
	Common::Rect( 129, 121, 145, 145 ),
	Common::Rect( 146, 112, 162, 136 ),
	Common::Rect( 163, 103, 179, 127 ),
	Common::Rect( 180, 94, 196, 118 ),
	Common::Rect( 197, 85, 213, 109 ),
	Common::Rect( 214, 76, 230, 100 ),
	Common::Rect( 231, 67, 247, 91 ),
	Common::Rect( 248, 58, 264, 82 )
};


class ShowCreditsInputState_NS : public MenuInputState {
	Parallaction *_vm;
	int	_current;
	uint32 _startTime;

	struct Credit {
		const char *_role;
		const char *_name;
	};

	static const Credit _credits[6];

public:
	ShowCreditsInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("showcredits", helper), _vm(vm) {
	}

	void drawCurrentLabel() {
		uint id[2];
		id[0] = _vm->_gfx->createLabel(_vm->_menuFont, _credits[_current]._role, 1);
		id[1] = _vm->_gfx->createLabel(_vm->_menuFont, _credits[_current]._name, 1);
		_vm->_gfx->showLabel(id[0], CENTER_LABEL_HORIZONTAL, 80);
		_vm->_gfx->showLabel(id[1], CENTER_LABEL_HORIZONTAL, 100);
	}


	virtual MenuInputState* run() {
		if (_current == -1) {
			_startTime = _vm->_system->getMillis();
			_current = 0;
			drawCurrentLabel();
			return this;
		}

		int event = _vm->_input->getLastButtonEvent();
		uint32 curTime = _vm->_system->getMillis();
		if ((event == kMouseLeftUp) || (curTime - _startTime > 5500)) {
			_current++;
			_startTime = curTime;
			_vm->_gfx->freeLabels();

			if (_current == 6) {
				return _helper->getState("endintro");
			}

			drawCurrentLabel();
		}

		return this;
	}

	virtual void enter() {
		_current = -1;
		_vm->_input->setMouseState(MOUSE_DISABLED);
	}
};

const ShowCreditsInputState_NS::Credit ShowCreditsInputState_NS::_credits[6] = {
	{"Music and Sound Effects", "MARCO CAPRELLI"},
	{"PC Version", "RICCARDO BALLARINO"},
	{"Project Manager", "LOVRANO CANEPA"},
	{"Production", "BRUNO BOZ"},
	{"Special Thanks to", "LUIGI BENEDICENTI - GILDA and DANILO"},
	{"Copyright 1992 Euclidea s.r.l ITALY", "All rights reserved"}
};

class EndIntroInputState_NS : public MenuInputState {
	Parallaction_ns *_vm;
	bool _isDemo;

public:
	EndIntroInputState_NS(Parallaction_ns *vm, MenuInputHelper *helper) : MenuInputState("endintro", helper), _vm(vm) {
		_isDemo = (_vm->getFeatures() & GF_DEMO) != 0;
	}

	virtual MenuInputState* run() {

		int event = _vm->_input->getLastButtonEvent();
		if (event != kMouseLeftUp) {
			return this;
		}

		if (_isDemo) {
			_vm->quitGame();
			return 0;
		}

		_vm->_gfx->freeLabels();
		return _helper->getState("selectcharacter");
	}

	virtual void enter() {
		_vm->_input->setMouseState(MOUSE_DISABLED);

		if (!_isDemo) {
			_vm->_soundManI->stopMusic();
			int label = _vm->_gfx->createLabel(_vm->_menuFont, "CLICK MOUSE BUTTON TO START", 1);
			_vm->_gfx->showLabel(label, CENTER_LABEL_HORIZONTAL, 80);
		}
	}
};


class EndPartInputState_NS : public MenuInputState {
	Parallaction *_vm;
	bool _allPartsComplete;

	// part completion messages
	static const char *endMsg0[4];
	static const char *endMsg1[4];
	static const char *endMsg2[4];
	static const char *endMsg3[4];
	// game completion messages
	static const char *endMsg4[4];
	static const char *endMsg5[4];
	static const char *endMsg6[4];
	static const char *endMsg7[4];


public:
	EndPartInputState_NS(Parallaction *vm, MenuInputHelper *helper) : MenuInputState("endpart", helper), _vm(vm) {
	}

	virtual MenuInputState* run() {
		int event = _vm->_input->getLastButtonEvent();
		if (event != kMouseLeftUp) {
			return this;
		}

		_vm->_gfx->freeLabels();

		if (_allPartsComplete) {
			_vm->scheduleLocationSwitch("estgrotta.drki");
			return 0;
		}

		return _helper->getState("selectcharacter");
	}

	virtual void enter() {
		bool completed[3];
		_vm->_saveLoad->getGamePartProgress(completed, 3);
		_allPartsComplete = (completed[0] && completed[1] && completed[2]);
		_vm->_input->setMouseState(MOUSE_DISABLED);

		uint16 language = _vm->getInternLanguage();
		uint id[4];
		if (_allPartsComplete) {
			id[0] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg4[language], 1);
			id[1] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg5[language], 1);
			id[2] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg6[language], 1);
			id[3] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg7[language], 1);
		} else {
			id[0] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg0[language], 1);
			id[1] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg1[language], 1);
			id[2] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg2[language], 1);
			id[3] = _vm->_gfx->createLabel(_vm->_menuFont, endMsg3[language], 1);
		}

		_vm->_gfx->showLabel(id[0], CENTER_LABEL_HORIZONTAL, 70);
		_vm->_gfx->showLabel(id[1], CENTER_LABEL_HORIZONTAL, 100);
		_vm->_gfx->showLabel(id[2], CENTER_LABEL_HORIZONTAL, 130);
		_vm->_gfx->showLabel(id[3], CENTER_LABEL_HORIZONTAL, 160);
	}
};

// part completion messages
const char *EndPartInputState_NS::endMsg0[] = {"COMPLIMENTI!", "BRAVO!", "CONGRATULATIONS!", "PRIMA!"};
const char *EndPartInputState_NS::endMsg1[] = {"HAI FINITO QUESTA PARTE", "TU AS COMPLETE' CETTE AVENTURE", "YOU HAVE COMPLETED THIS PART", "DU HAST EIN ABENTEUER ERFOLGREICH"};
const char *EndPartInputState_NS::endMsg2[] = {"ORA COMPLETA IL RESTO ", "AVEC SUCCES.",  "NOW GO ON WITH THE REST OF", "ZU ENDE GEFUHRT"};
const char *EndPartInputState_NS::endMsg3[] = {"DELL' AVVENTURA",  "CONTINUE AVEC LES AUTRES", "THIS ADVENTURE", "MACH' MIT DEN ANDEREN WEITER"};
// game completion messages
const char *EndPartInputState_NS::endMsg4[] = {"COMPLIMENTI!", "BRAVO!", "CONGRATULATIONS!", "PRIMA!"};
const char *EndPartInputState_NS::endMsg5[] = {"HAI FINITO LE TRE PARTI", "TU AS COMPLETE' LES TROIS PARTIES", "YOU HAVE COMPLETED THE THREE PARTS", "DU HAST DREI ABENTEURE ERFOLGREICH"};
const char *EndPartInputState_NS::endMsg6[] = {"DELL' AVVENTURA", "DE L'AVENTURE", "OF THIS ADVENTURE", "ZU ENDE GEFUHRT"};
const char *EndPartInputState_NS::endMsg7[] = {"ED ORA IL GRAN FINALE ", "ET MAINTENANT LE GRAND FINAL", "NOW THE GREAT FINAL", "UND YETZT DER GROSSE SCHLUSS!"};

void Parallaction_ns::startGui() {
	_menuHelper = new MenuInputHelper;
	assert(_menuHelper);

	new SelectGameInputState_NS(this, _menuHelper);
	new LoadGameInputState_NS(this, _menuHelper);
	new NewGameInputState_NS(this, _menuHelper);
	new StartDemoInputState_NS(this, _menuHelper);
	new SelectCharacterInputState_NS(this, _menuHelper);
	new ChooseLanguageInputState_NS(this, _menuHelper);
	new SplashInputState1_NS(this, _menuHelper);
	new SplashInputState0_NS(this, _menuHelper);
	_menuHelper->setState("intro0");

	_input->_inputMode = Input::kInputModeMenu;
}

void Parallaction_ns::startCreditSequence() {
	_menuHelper = new MenuInputHelper;
	assert(_menuHelper);

	new ShowCreditsInputState_NS(this, _menuHelper);
	new EndIntroInputState_NS(this, _menuHelper);
	new SelectCharacterInputState_NS(this, _menuHelper);
	_menuHelper->setState("showcredits");

	_input->_inputMode = Input::kInputModeMenu;
}

void Parallaction_ns::startEndPartSequence() {
	_menuHelper = new MenuInputHelper;
	assert(_menuHelper);

	new EndPartInputState_NS(this, _menuHelper);
	new SelectCharacterInputState_NS(this, _menuHelper);
	_menuHelper->setState("endpart");

	_input->_inputMode = Input::kInputModeMenu;
}


} // namespace Parallaction
