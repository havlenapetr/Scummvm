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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/gfx/seq_decoder.h $
 * $Id: seq_decoder.h 41785 2009-06-22 21:48:47Z fingolfin $
 *
 */

#include "common/file.h"
#include "sci/gfx/gfx_system.h"

namespace Sci {

/**
 * Decoder for image sequences
 */
class SeqDecoder {
public:
	SeqDecoder() : _fileStream(0), _palette(0) { }
	~SeqDecoder();
	bool loadFile(Common::String fileName);
	void closeFile();
	gfx_pixmap_t *getFrame(bool &hasNext);

private:
	bool decodeFrame(byte *runlength_data, int runlength_size,
		byte *literal_data, int literal_size, byte *dest, int xl, int yl,
		int color_key);

	Common::SeekableReadStream *_fileStream;
	Palette *_palette;
	int _frameCount;
	int _currentFrame;
};

} // End of namespace Sci
