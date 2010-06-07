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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/groovie/vdx.h $
 * $Id: vdx.h 44010 2009-09-08 06:29:12Z scott_t $
 *
 */

#ifndef GROOVIE_VDX_H
#define GROOVIE_VDX_H

#include "groovie/player.h"

namespace Groovie {

class VDXPlayer : public VideoPlayer {
public:
	VDXPlayer(GroovieEngine *vm);
	~VDXPlayer();
	void resetFlags();
	void setOrigin(int16 x, int16 y);

protected:
	uint16 loadInternal();
	bool playFrameInternal();

private:
	Graphics::Surface *_fg, *_bg;
	uint8 _palBuf[3 * 256];

	// Origin
	int16 _origX, _origY;

	// Video flags
	bool _flagZero;
	bool _flagOne;
	bool _flagOnePrev;
	byte _flag2Byte;
	bool _flagThree;
	bool _flagFour;
	bool _flagFive;
	bool _flagSix;
	bool _flagSeven;
	bool _flagEight;
	bool _flagNine;

	bool _flagSkipStill;
	bool _flagSkipPalette;
	bool _flagFirstFrame;
	bool _flagTransparent;
	bool _flagUpdateStill;

	void getStill(Common::ReadStream *in);
	void getDelta(Common::ReadStream *in);
	void expandColourMap(byte *out, uint16 colourMap, uint8 colour1, uint8 colour0);
	void decodeBlockStill(byte *buf, byte *colours, uint16 imageWidth, uint8 mask);
	void decodeBlockDelta(uint32 offset, byte *colours, uint16 imageWidth);
	void chunkSound(Common::ReadStream *in);
	void setPalette(uint8 *palette);
	void fadeIn(uint8 *palette);
};

} // End of Groovie namespace

#endif // GROOVIE_VDX_H
