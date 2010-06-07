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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/softsynth/emumidi.h $
 * $Id: emumidi.h 40932 2009-05-27 00:34:31Z lordhoto $
 */

#ifndef SOUND_SOFTSYNTH_EMUMIDI_H
#define SOUND_SOFTSYNTH_EMUMIDI_H

#include "sound/audiostream.h"
#include "sound/mididrv.h"
#include "sound/mixer.h"

#define FIXP_SHIFT 16

class MidiDriver_Emulated : public Audio::AudioStream, public MidiDriver {
protected:
	bool _isOpen;
	Audio::Mixer *_mixer;
	Audio::SoundHandle _mixerSoundHandle;

private:
	Common::TimerManager::TimerProc _timerProc;
	void *_timerParam;

	int _nextTick;
	int _samplesPerTick;

protected:
	virtual void generateSamples(int16 *buf, int len) = 0;
	virtual void onTimer() {}

	int _baseFreq;

public:
	MidiDriver_Emulated(Audio::Mixer *mixer) : _mixer(mixer) {
		_isOpen = false;

		_timerProc = 0;
		_timerParam = 0;

		_nextTick = 0;
		_samplesPerTick = 0;

		_baseFreq = 250;
	}

	int open() {
		_isOpen = true;

		int d = getRate() / _baseFreq;
		int r = getRate() % _baseFreq;

		// This is equivalent to (getRate() << FIXP_SHIFT) / BASE_FREQ
		// but less prone to arithmetic overflow.

		_samplesPerTick = (d << FIXP_SHIFT) + (r << FIXP_SHIFT) / _baseFreq;
		return 0;
	}

	void setTimerCallback(void *timer_param, Common::TimerManager::TimerProc timer_proc) {
		_timerProc = timer_proc;
		_timerParam = timer_param;
	}

	uint32 getBaseTempo() { return 1000000 / _baseFreq; }


	// AudioStream API
	int readBuffer(int16 *data, const int numSamples) {
		const int stereoFactor = isStereo() ? 2 : 1;
		int len = numSamples / stereoFactor;
		int step;

		do {
			step = len;
			if (step > (_nextTick >> FIXP_SHIFT))
				step = (_nextTick >> FIXP_SHIFT);

			generateSamples(data, step);

			_nextTick -= step << FIXP_SHIFT;
			if (!(_nextTick >> FIXP_SHIFT)) {
				if (_timerProc)
					(*_timerProc)(_timerParam);
				onTimer();
				_nextTick += _samplesPerTick;
			}
			data += step * stereoFactor;
			len -= step;
		} while (len);

		return numSamples;
	}
	bool endOfData() const { return false; }
};

#endif
