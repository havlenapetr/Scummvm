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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/iff_sound.h $
 * $Id: iff_sound.h 42287 2009-07-09 13:17:46Z thebluegr $
 *
 */

/**
 * Sound decoder used in engines:
 *  - parallaction
 */

#ifndef SOUND_IFF_H
#define SOUND_IFF_H

#include "common/iff_container.h"
#include "sound/audiostream.h"

namespace Audio {

struct Voice8Header {
	uint32	oneShotHiSamples;
	uint32	repeatHiSamples;
	uint32	samplesPerHiCycle;
	uint16	samplesPerSec;
	byte	octaves;
	byte	compression;
	uint32	volume;

	Voice8Header() {
		memset(this, 0, sizeof(Voice8Header));
	}

	void load(Common::ReadStream &stream);
};

AudioStream *make8SVXStream(Common::ReadStream &stream, bool loop);


}

#endif
