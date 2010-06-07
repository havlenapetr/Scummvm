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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/sound/protracker.h $
 * $Id: protracker.h 41632 2009-06-18 13:27:14Z drmccoy $
 *
 */

#ifndef GOB_SOUND_PROTRACKER_H
#define GOB_SOUND_PROTRACKER_H

#include "sound/mixer.h"
#include "sound/audiostream.h"

namespace Gob {

class Protracker {
public:
	Protracker(Audio::Mixer &mixer);
	~Protracker();

	bool play(const char *fileName);
	void stop();

private:
	Audio::Mixer *_mixer;

	Audio::AudioStream *_protrackerStream;
	Audio::SoundHandle _handle;
};

} // End of namespace Gob

#endif // GOB_SOUND_PROTRACKER_H
