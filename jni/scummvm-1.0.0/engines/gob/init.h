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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/init.h $
 * $Id: init.h 42130 2009-07-05 11:29:54Z drmccoy $
 *
 */

#ifndef GOB_INIT_H
#define GOB_INIT_H

#include "gob/video.h"

namespace Gob {

class Init {
public:
	virtual void initGame();

	virtual void initVideo() = 0;

	Init(GobEngine *vm);
	virtual ~Init() {}

protected:
	Video::PalDesc *_palDesc;
	static const char *_fontNames[4];
	GobEngine *_vm;

	void cleanup();
	void doDemo();
};

class Init_v1 : public Init {
public:
	virtual void initVideo();

	Init_v1(GobEngine *vm);
	virtual ~Init_v1() {}
};

class Init_v2 : public Init_v1 {
public:
	virtual void initVideo();

	Init_v2(GobEngine *vm);
	virtual ~Init_v2() {}
};

class Init_v3 : public Init_v2 {
public:
	virtual void initVideo();

	Init_v3(GobEngine *vm);
	virtual ~Init_v3() {}
};

class Init_v6 : public Init_v3 {
public:
	virtual void initGame();

	Init_v6(GobEngine *vm);
	virtual ~Init_v6() {}
};

} // End of namespace Gob

#endif // GOB_INIT_H
