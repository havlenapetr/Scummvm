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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/made/pmvplayer.h $
 * $Id: pmvplayer.h 35997 2009-01-22 13:04:29Z john_doe $
 *
 */

#ifndef MADE_PMVPLAYER_H
#define MADE_PMVPLAYER_H

#include "common/system.h"
#include "common/events.h"
#include "common/file.h"
#include "common/endian.h"
#include "graphics/surface.h"
#include "sound/mixer.h"
#include "sound/audiostream.h"

#include "made/graphics.h"
#include "made/sound.h"
#include "made/made.h"

namespace Made {

class PmvPlayer {
public:
	PmvPlayer(MadeEngine *vm, Audio::Mixer *mixer);
	~PmvPlayer();
	// Returns true if the movie was played till the end
	bool play(const char *filename);
protected:
	MadeEngine *_vm;
	Audio::Mixer *_mixer;
	Common::File *_fd;
	Audio::AppendableAudioStream *_audioStream;
	Audio::SoundHandle _audioStreamHandle;
	byte _paletteRGB[768];
	Graphics::Surface *_surface;
	bool _aborted;
	void readChunk(uint32 &chunkType, uint32 &chunkSize);
	void handleEvents();
	void updateScreen();
	void decompressPalette(byte *palData, byte *outPal, uint32 palDataSize);
};

}

#endif
