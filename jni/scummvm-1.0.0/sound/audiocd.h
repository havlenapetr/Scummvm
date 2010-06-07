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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/audiocd.h $
 * $Id: audiocd.h 36030 2009-01-24 01:29:52Z wjpalenstijn $
 *
 */

#ifndef SOUND_AUDIOCD_H
#define SOUND_AUDIOCD_H

#include "common/scummsys.h"
#include "common/singleton.h"
#include "sound/mixer.h"


namespace Audio {


class AudioCDManager : public Common::Singleton<AudioCDManager> {
public:
	struct Status {
		bool playing;
		int track;
		int start;
		int duration;
		int numLoops;
	};

	/**
	 * Start playback of the specified "CD" track. This method mimics
	 * the interface of OSystem::playCD (which it in fact may call, if an Audio CD is
	 * present), but also can play digital audio tracks in various formats.
	 *
	 * @param track			the track to play.
	 * @param num_loops		how often playback should be repeated (-1 = infinitely often).
	 * @param start_frame	the frame at which playback should start (75 frames = 1 second).
	 * @param duration		the number of frames to play (0: play until end)
	 * @param only_emulate  if true, don't try to play from a real CD
	 */
	void play(int track, int numLoops, int startFrame, int duration, bool only_emulate = false);
	void stop();
	bool isPlaying() const;

	void updateCD();

	Status getStatus() const;

private:
	friend class Common::Singleton<SingletonBaseType>;
	AudioCDManager();

	/* used for emulated CD music */
	SoundHandle _handle;
	bool _emulating;

	Status _cd;
	Mixer	*_mixer;
};

/** Shortcut for accessing the audio CD manager. */
#define AudioCD		Audio::AudioCDManager::instance()

} // End of namespace Audio

#endif
