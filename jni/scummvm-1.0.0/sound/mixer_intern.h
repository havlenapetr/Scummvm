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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/mixer_intern.h $
 * $Id: mixer_intern.h 35481 2008-12-22 11:22:15Z jvprat $
 *
 */

#ifndef SOUND_MIXER_INTERN_H
#define SOUND_MIXER_INTERN_H

#include "common/scummsys.h"
#include "common/mutex.h"
#include "sound/mixer.h"

namespace Audio {

/**
 * The (default) implementation of the ScummVM audio mixing subsystem.
 *
 * Backends are responsible for allocating (and later releasing) an instance
 * of this class, which engines can access via OSystem::getMixer().
 *
 * Initialisation of instances of this class usually happens as follows:
 * 1) Creat a new Audio::MixerImpl instance.
 * 2) Set the hardware output sample rate via the setSampleRate() method.
 * 3) Hook up the mixCallback() in a suitable audio processing thread/callback.
 * 4) Change the mixer into ready mode via setReady(true).
 * 5) Start audio processing (e.g. by resuming the audio thread, if applicable).
 *
 * In the future, we might make it possible for backends to provide
 * (partial) alternative implementations of the mixer, e.g. to make
 * better use of native sound mixing support on low-end devices.
 *
 * @see OSystem::getMixer()
 */
class MixerImpl : public Mixer {
private:
	enum {
		NUM_CHANNELS = 16
	};

	OSystem *_syst;
	Common::Mutex _mutex;

	uint _sampleRate;
	bool _mixerReady;
	uint32 _handleSeed;

	int _volumeForSoundType[4];
	Channel *_channels[NUM_CHANNELS];


public:
	MixerImpl(OSystem *system);
	~MixerImpl();

	virtual bool isReady() const { return _mixerReady; }

	virtual void playRaw(
		SoundType type,
		SoundHandle *handle,
		void *sound, uint32 size, uint rate, byte flags,
		int id = -1, byte volume = 255, int8 balance = 0,
		uint32 loopStart = 0, uint32 loopEnd = 0);

	virtual void playInputStream(
		SoundType type,
		SoundHandle *handle,
		AudioStream *input,
		int id = -1, byte volume = 255, int8 balance = 0,
		bool autofreeStream = true,
		bool permanent = false,
		bool reverseStereo = false);



	virtual void stopAll();
	virtual void stopID(int id);
	virtual void stopHandle(SoundHandle handle);

	virtual void pauseAll(bool paused);
	virtual void pauseID(int id, bool paused);
	virtual void pauseHandle(SoundHandle handle, bool paused);

	virtual bool isSoundIDActive(int id);
	virtual int getSoundID(SoundHandle handle);

	virtual bool isSoundHandleActive(SoundHandle handle);

	virtual void setChannelVolume(SoundHandle handle, byte volume);
	virtual void setChannelBalance(SoundHandle handle, int8 balance);

	virtual uint32 getSoundElapsedTime(SoundHandle handle);

	virtual bool hasActiveChannelOfType(SoundType type);

	virtual void setVolumeForSoundType(SoundType type, int volume);
	virtual int getVolumeForSoundType(SoundType type) const;

	virtual uint getOutputRate() const;

protected:
	void insertChannel(SoundHandle *handle, Channel *chan);

public:
	/**
	 * The mixer callback function, to be called at regular intervals by
	 * the backend (e.g. from an audio mixing thread). All the actual mixing
	 * work is done from here.
	 */
	void mixCallback(byte *samples, uint len);

	/**
	 * Set the internal 'is ready' flag of the mixer.
	 * Backends should invoke Mixer::setReady(true) once initialisation of
	 * their audio system has been completed (and in particular, *after*
	 * setOutputRate() has been called).
	 */
	void setReady(bool ready);

	/**
	 * Set the output sample rate.
	 *
	 * @param sampleRate	the new output sample rate
	 *
	 * @note Right now, this can be done exactly ONCE. That is, the mixer
	 * currently does not support changing the output sample rate after it
	 * has been set for the first time.  This may change in the future.
	 */
	void setOutputRate(uint sampleRate);
};


} // End of namespace Audio

#endif
