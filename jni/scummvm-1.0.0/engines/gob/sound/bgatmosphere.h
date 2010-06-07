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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/sound/bgatmosphere.h $
 * $Id: bgatmosphere.h 41632 2009-06-18 13:27:14Z drmccoy $
 *
 */

#ifndef GOB_SOUND_BGATMOSPHERE_H
#define GOB_SOUND_BGATMOSPHERE_H

#include "sound/mixer.h"
#include "common/mutex.h"

#include "gob/sound/soundmixer.h"

namespace Gob {

class SoundDesc;

class BackgroundAtmosphere : private SoundMixer {
public:
	enum PlayMode {
		kPlayModeLinear,
		kPlayModeRandom
	};

	BackgroundAtmosphere(Audio::Mixer &mixer);
	~BackgroundAtmosphere();

	void play();
	void stop();

	void setPlayMode(PlayMode mode);

	void queueSample(SoundDesc &sndDesc);
	void queueClear();

	void setShadable(bool shadable);
	void shade();
	void unshade();

private:
	PlayMode _playMode;

	Common::Array<SoundDesc *> _queue;
	int _queuePos;
	bool _shaded;
	bool _shadable;

	Common::Mutex _mutex;

	Common::RandomSource _rnd;

	void checkEndSample();
	void getNextQueuePos();
};

} // End of namespace Gob

#endif // GOB_SOUND_BGATMOSPHERE_H
