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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/adpcm.h $
 * $Id: adpcm.h 39236 2009-03-08 15:57:59Z sev $
 *
 */

/**
 * Sound decoder used in engines:
 *  - agos
 *  - saga
 *  - scumm
 *  - tinsel
 */

#ifndef SOUND_ADPCM_H
#define SOUND_ADPCM_H

#include "common/scummsys.h"
#include "common/stream.h"


namespace Audio {

class AudioStream;

// There are several types of ADPCM encoding, only some are supported here
// For all the different encodings, refer to:
// http://wiki.multimedia.cx/index.php?title=Category:ADPCM_Audio_Codecs
// Usually, if the audio stream we're trying to play has the FourCC header
// string intact, it's easy to discern which encoding is used
enum typesADPCM {
	kADPCMOki,		// Dialogic/Oki ADPCM (aka VOX)
	kADPCMMSIma,	// Microsoft IMA ADPCM
	kADPCMMS,		// Microsoft ADPCM
	kADPCMTinsel4,	// 4-bit ADPCM used by the Tinsel engine
	kADPCMTinsel6,	// 6-bit ADPCM used by the Tinsel engine
	kADPCMTinsel8,	// 8-bit ADPCM used by the Tinsel engine
	kADPCMIma		// Standard IMA ADPCM
};

/**
 * Takes an input stream containing ADPCM compressed sound data and creates
 * an AudioStream from that.
 *
 * @param stream			the SeekableReadStream from which to read the ADPCM data
 * @param disposeAfterUse	whether to delete the stream after use
 * @param size				how many bytes to read from the stream (0 = all)
 * @param type				the compression type used
 * @param rate				the sampling rate (default = 22050)
 * @param channels			the number of channels (default = 2)
 * @param blockAlign		block alignment ??? (default = 0)
 * @param numLoop		how many types the sounds should loop, 0 for infinite loop (default = 1)
 * @return	a new AudioStream, or NULL, if an error occured
 */
AudioStream *makeADPCMStream(
	Common::SeekableReadStream *stream,
	bool disposeAfterUse,
	uint32 size,
	typesADPCM type,
	int rate = 22050,
	int channels = 2,
	uint32 blockAlign = 0,
	uint numLoops = 1);

} // End of namespace Audio

#endif
