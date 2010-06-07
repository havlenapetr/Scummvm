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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/queen/credits.h $
 * $Id: credits.h 27024 2007-05-30 21:56:52Z fingolfin $
 *
 */

#ifndef QUEEN_CREDITS_H
#define QUEEN_CREDITS_H

#include "common/util.h"
#include "queen/defs.h"

namespace Queen {

class QueenEngine;
class LineReader;

class Credits {
public:

	Credits(QueenEngine *vm, const char* filename);

	//! update/display credits for current room
	void update();

	//! handles room switching
	void nextRoom();

	//! returns true if the credits are running
	bool running() const { return _running; }

private:

	struct Line {
		short x, y, color, fontSize;
		const char *text;
	};

	//! contains the formatted lines of texts to display
	Line _list[19];

	//! true if end of credits description hasn't been reached
	bool _running;

	//! number of elements in _list array
	int _count;

	//! pause counts for next room
	int _pause;

	//! current text justification mode
	int _justify;

	//! current font size (unused ?)
	int _fontSize;

	//! current text color
	int _color;

	//! current text position
	int _zone;

	uint _lineNum;

	//! contains the credits description
	Common::StringList _credits;

	QueenEngine *_vm;
};

} // End of namespace Queen

#endif
