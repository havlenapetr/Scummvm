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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/kyra/text.h $
 * $Id: text.h 32044 2008-05-11 23:16:50Z lordhoto $
 *
 */

#ifndef KYRA_TEXT_H
#define KYRA_TEXT_H

#include "common/scummsys.h"

#include "kyra/screen.h"

namespace Kyra {
class KyraEngine_v1;

class TextDisplayer {
public:
	TextDisplayer(KyraEngine_v1 *vm, Screen *screen);
	virtual ~TextDisplayer() {}

	int maxSubstringLen() const { return TALK_SUBSTRING_LEN; }

	void setTalkCoords(uint16 y);
	int getCenterStringX(const char *str, int x1, int x2);
	int getCharLength(const char *str, int len);
	int dropCRIntoString(char *str, int offs);
	virtual char *preprocessString(const char *str);
	int buildMessageSubstrings(const char *str);
	int getWidestLineWidth(int linesCount);
	virtual void calcWidestLineBounds(int &x1, int &x2, int w, int cx);
	virtual void restoreTalkTextMessageBkgd(int srcPage, int dstPage);
	void printTalkTextMessage(const char *text, int x, int y, uint8 color, int srcPage, int dstPage);
	void printIntroTextMessage(const char *text, int x, int y, uint8 col1, uint8 col2, uint8 col3,
			int dstPage, Screen::FontId font=Screen::FID_8_FNT);
	virtual void printText(const char *str, int x, int y, uint8 c0, uint8 c1, uint8 c2, Screen::FontId font=Screen::FID_8_FNT);
	void printCharacterText(const char *text, int8 charNum, int charX);

	uint16 _talkMessageY;
	uint16 _talkMessageH;
	bool printed() const { return _talkMessagePrinted; }
protected:
	Screen *_screen;
	KyraEngine_v1 *_vm;

	struct TalkCoords {
		uint16 y, x, w;
	};

	enum {
		TALK_SUBSTRING_LEN = 80,
		TALK_SUBSTRING_NUM = 6
	};

	char _talkBuffer[1040];
	char _talkSubstrings[TALK_SUBSTRING_LEN * TALK_SUBSTRING_NUM];
	TalkCoords _talkCoords;
	bool _talkMessagePrinted;
};
} // end of namespace Kyra

#endif

