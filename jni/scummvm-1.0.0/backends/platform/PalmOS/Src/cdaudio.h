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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/cdaudio.h $
 * $Id: cdaudio.h 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#ifndef CDAUDIO_H
#define CDAUDIO_H

#include "common/system.h"

// CD frames are 1/75 sec
#define CD_FPS			75
#define TO_MSECS(frame)	((UInt32)((frame) * 1000 / CD_FPS))

// consider frame at 1/1000 sec
#define FROM_MIN(mins)	((UInt32)((mins) * 60 * 1000))
#define FROM_SEC(secs)	((UInt32)((secs) * 1000))

class CDAudio {
public:
	CDAudio() {
		_isInitialized = false;
		_volumeLevel = 100;
	};

	virtual bool init() = 0;
	virtual void release() = 0;

	// OSystem functions
	virtual bool poll() = 0;
	virtual void play(int track, int num_loops, int start_frame, int duration) = 0;
	virtual void stop() = 0;
	virtual void update() = 0;

	// volume in percent
	virtual void setVolume(int volume) { _volumeLevel = volume; }
	virtual int getVolume() const { return _volumeLevel; }

	virtual void upVolume(int value) {}
	virtual void downVolume(int value) {}

protected:
	int _volumeLevel;
	bool _isInitialized;
};

#endif
