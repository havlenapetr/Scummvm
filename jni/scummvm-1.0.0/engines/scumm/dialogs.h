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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/scumm/dialogs.h $
 * $Id: dialogs.h 43439 2009-08-16 12:15:44Z lordhoto $
 */

#ifndef SCUMM_DIALOGS_H
#define SCUMM_DIALOGS_H

#include "common/str.h"
#include "gui/dialog.h"
#include "gui/options.h"
#include "gui/widget.h"

#include "scumm/detection.h"
#ifndef DISABLE_HELP
#include "scumm/help.h"
#endif

namespace GUI {
	class ListWidget;
	class CommandSender;
}


namespace Scumm {

class ScummEngine;

class ScummDialog : public GUI::Dialog {
public:
	ScummDialog(Common::String name);

protected:
	typedef Common::String String;
};

class SaveLoadChooser : public GUI::Dialog {
	typedef Common::String String;
	typedef Common::StringList StringList;
protected:
	bool _saveMode;
	GUI::ListWidget		*_list;
	GUI::ButtonWidget	*_chooseButton;
	GUI::GraphicsWidget	*_gfxWidget;
	GUI::StaticTextWidget	*_date;
	GUI::StaticTextWidget	*_time;
	GUI::StaticTextWidget	*_playtime;
	GUI::ContainerWidget	*_container;
	ScummEngine			*_vm;

	uint8 _fillR, _fillG, _fillB;

	void updateInfos(bool redraw);
public:
	SaveLoadChooser(const String &title, const String &buttonLabel, bool saveMode, ScummEngine *engine);
	~SaveLoadChooser();

	virtual void handleCommand(GUI::CommandSender *sender, uint32 cmd, uint32 data);
	const String &getResultString() const;
	void setList(const StringList& list);
	int runModal();

	virtual void reflowLayout();
};

class ScummMenuDialog : public ScummDialog {
public:
	ScummMenuDialog(ScummEngine *scumm);
	~ScummMenuDialog();
	virtual void handleCommand(GUI::CommandSender *sender, uint32 cmd, uint32 data);

	virtual void reflowLayout();

	int runModal();
protected:
	ScummEngine		*_vm;

	GUI::Dialog		*_aboutDialog;
	GUI::Dialog		*_optionsDialog;
#ifndef DISABLE_HELP
	GUI::Dialog		*_helpDialog;
#endif
	SaveLoadChooser	*_saveDialog;
	SaveLoadChooser	*_loadDialog;

	GUI::ButtonWidget *_saveButton;

	void save();
	void load();
};

#ifndef DISABLE_HELP

class HelpDialog : public ScummDialog {
public:
	HelpDialog(const GameSettings &game);
	virtual void handleCommand(GUI::CommandSender *sender, uint32 cmd, uint32 data);

	virtual void reflowLayout();

protected:
	typedef Common::String String;

	GUI::ButtonWidget *_nextButton;
	GUI::ButtonWidget *_prevButton;

	GUI::StaticTextWidget *_title;
	GUI::StaticTextWidget *_key[HELP_NUM_LINES];
	GUI::StaticTextWidget *_dsc[HELP_NUM_LINES];

	int _page;
	int _numPages;
	int _numLines;

	const GameSettings _game;

	void displayKeyBindings();
};

#endif

class ConfigDialog : public GUI::OptionsDialog {
protected:
#ifdef SMALL_SCREEN_DEVICE
	GUI::Dialog		*_keysDialog;
#endif

public:
	ConfigDialog();
	~ConfigDialog();

	virtual void handleCommand(GUI::CommandSender *sender, uint32 cmd, uint32 data);
};

/**
 * A dialog which displays an arbitrary message to the user and returns
 * ther users reply as its result value. More specifically, it returns
 * the ASCII code of the key used to close the dialog (0 if a mouse
 * click closed the dialog).
 */
class InfoDialog : public ScummDialog {
protected:
	ScummEngine		*_vm;
	String _message;
	GUI::StaticTextWidget *_text;

public:
	// arbitrary message
	InfoDialog(ScummEngine *scumm, const String& message);
	// from resources
	InfoDialog(ScummEngine *scumm, int res);

	void setInfoText(const String& message);

	virtual void handleMouseDown(int x, int y, int button, int clickCount) {
		setResult(0);
		close();
	}
	virtual void handleKeyDown(Common::KeyState state) {
		setResult(state.ascii);
		close();
	}

	virtual void reflowLayout();

protected:

	// Query a string from the resources
	const String queryResString(int stringno);
};

/**
 * The pause dialog, visible whenever the user activates pause mode. Goes
 * away uon any key or mouse button press.
 */
class PauseDialog : public InfoDialog {
public:
	PauseDialog(ScummEngine *scumm, int res);
	virtual void handleKeyDown(Common::KeyState state);
};

/**
 * A simple yes/no dialog, used to ask the user whether to really
 * quit/restart ScummVM.
 */
class ConfirmDialog : public InfoDialog {
public:
	ConfirmDialog(ScummEngine *scumm, int res);
	virtual void handleKeyDown(Common::KeyState state);
};

/**
 * A dialog used to display the music volume / text speed.
 * Automatically closes after a brief time passed.
 */
class ValueDisplayDialog : public GUI::Dialog {
public:
	ValueDisplayDialog(const Common::String& label, int minVal, int maxVal, int val, uint16 incKey, uint16 decKey);

	virtual void open();
	virtual void drawDialog();
	virtual void handleTickle();
	virtual void handleMouseDown(int x, int y, int button, int clickCount) {
		close();
	}
	virtual void handleKeyDown(Common::KeyState state);

	virtual void reflowLayout();

protected:
	enum {
		kDisplayDelay = 1500
	};
	Common::String _label;
	const int _min, _max;
	const uint16 _incKey, _decKey;
	int _percentBarWidth;
	int _value;
	uint32 _timer;
};

/**
 * A dialog used to display and cycle subtitle settings.
 * Automatically closes after a brief time has passed.
 */
class SubtitleSettingsDialog : public InfoDialog {
public:
	SubtitleSettingsDialog(ScummEngine *scumm, int value);

	virtual void open();
	virtual void handleTickle();
	virtual void handleMouseDown(int x, int y, int button, int clickCount) {
		close();
	}
	virtual void handleKeyDown(Common::KeyState state);
protected:
	int _value;
	uint32 _timer;

	void cycleValue();
};

//The Indy IQ dialog
class Indy3IQPointsDialog : public InfoDialog {
public:
	Indy3IQPointsDialog(ScummEngine *scumm, char* text);
	virtual void handleKeyDown(Common::KeyState state);
};

class DebugInputDialog : public InfoDialog {
public:
	DebugInputDialog(ScummEngine *scumm, char* text);
	virtual void handleKeyDown(Common::KeyState state);
	bool done;
	Common::String buffer;
	Common::String mainText;
};

} // End of namespace Scumm

#endif
