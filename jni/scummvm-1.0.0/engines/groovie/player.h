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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/groovie/player.h $
 * $Id: player.h 44010 2009-09-08 06:29:12Z scott_t $
 *
 */

#ifndef GROOVIE_PLAYER_H
#define GROOVIE_PLAYER_H

#include "common/system.h"
#include "sound/audiostream.h"

namespace Groovie {

class GroovieEngine;

class VideoPlayer {
public:
	VideoPlayer(GroovieEngine *vm);
	virtual ~VideoPlayer() {}

	bool load(Common::SeekableReadStream *file, uint16 flags);
	bool playFrame();
	virtual void resetFlags() {};
	virtual void setOrigin(int16 x, int16 y) {};

protected:
	// To be implemented by subclasses
	virtual uint16 loadInternal() = 0;
	virtual bool playFrameInternal() = 0;

	GroovieEngine *_vm;
	OSystem *_syst;
	Common::SeekableReadStream *_file;
	uint16 _flags;
	Audio::AppendableAudioStream *_audioStream;

private:
	// Synchronization stuff
	bool _begunPlaying;
	uint16 _millisBetweenFrames;
	uint32 _lastFrameTime;

protected:
	void waitFrame();
};

} // End of Groovie namespace

#endif // GROOVIE_PLAYER_H
