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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/sound.cpp $
 * $Id: sound.cpp 35724 2009-01-04 14:38:03Z thebluegr $
 *
 */

#include "common/config-manager.h"

#include "saga/saga.h"
#include "saga/sound.h"

#include "sound/audiostream.h"
#include "sound/mixer.h"
#include "sound/adpcm.h"

namespace Saga {

Sound::Sound(SagaEngine *vm, Audio::Mixer *mixer) :
	_vm(vm), _mixer(mixer), _voxStream(0) {

	for (int i = 0; i < SOUND_HANDLES; i++)
		_handles[i].type = kFreeHandle;

	setVolume();
}

Sound::~Sound() {
	delete _voxStream;
}

SndHandle *Sound::getHandle() {
	for (int i = 0; i < SOUND_HANDLES; i++) {
		if (_handles[i].type == kFreeHandle)
			return &_handles[i];

		if (!_mixer->isSoundHandleActive(_handles[i].handle)) {
			_handles[i].type = kFreeHandle;
			return &_handles[i];
		}
	}

	error("Sound::getHandle(): Too many sound handles");

	return NULL;
}

void Sound::playSoundBuffer(Audio::SoundHandle *handle, SoundBuffer &buffer, int volume,
				sndHandleType handleType, bool loop) {
	byte flags;

	flags = Audio::Mixer::FLAG_AUTOFREE;

	if (loop)
		flags |= Audio::Mixer::FLAG_LOOP;

	if (buffer.sampleBits == 16) {
		flags |= Audio::Mixer::FLAG_16BITS;

		if (!buffer.isBigEndian)
			flags |= Audio::Mixer::FLAG_LITTLE_ENDIAN;
	}
	if (buffer.stereo)
		flags |= Audio::Mixer::FLAG_STEREO;
	if (!buffer.isSigned)
		flags |= Audio::Mixer::FLAG_UNSIGNED;

	if (!buffer.isCompressed) {
		if (handleType == kVoiceHandle)
			_mixer->playRaw(Audio::Mixer::kSpeechSoundType, handle, buffer.buffer,
					buffer.size, buffer.frequency, flags, -1, volume);
		else
			_mixer->playRaw(Audio::Mixer::kSFXSoundType, handle, buffer.buffer,
					buffer.size, buffer.frequency, flags, -1, volume);
	} else {
		Audio::AudioStream *stream = NULL;
#if defined(USE_MAD) || defined(USE_VORBIS) || defined(USE_FLAC)
		MemoryReadStream *tmp = NULL;
#endif

		switch (buffer.soundType) {
#ifdef USE_MAD
			case kSoundMP3:
				debug(1, "Playing MP3 compressed sound");
				buffer.soundFile->seek((long)buffer.fileOffset, SEEK_SET);
				tmp = buffer.soundFile->readStream(buffer.size);
				assert(tmp);
				stream = Audio::makeMP3Stream(tmp, true);
				break;
#endif
#ifdef USE_VORBIS
			case kSoundOGG:
				debug(1, "Playing OGG compressed sound");
				buffer.soundFile->seek((long)buffer.fileOffset, SEEK_SET);
				tmp = buffer.soundFile->readStream(buffer.size);
				assert(tmp);
				stream = Audio::makeVorbisStream(tmp, true);
				break;
#endif
#ifdef USE_FLAC
			case kSoundFLAC:
				debug(1, "Playing FLAC compressed sound");
				buffer.soundFile->seek((long)buffer.fileOffset, SEEK_SET);
				tmp = buffer.soundFile->readStream(buffer.size);
				assert(tmp);
				stream = Audio::makeFlacStream(tmp, true);
				break;
#endif
			default:
				// No compression, play it as raw sound
				if (handleType == kVoiceHandle)
					_mixer->playRaw(Audio::Mixer::kSpeechSoundType, handle, buffer.buffer,
							buffer.size, buffer.frequency, flags, -1, volume);
				else
					_mixer->playRaw(Audio::Mixer::kSFXSoundType, handle, buffer.buffer,
							buffer.size, buffer.frequency, flags, -1, volume);
				break;
		}

		if (stream != NULL) {
			if (handleType == kVoiceHandle)
				_mixer->playInputStream(Audio::Mixer::kSpeechSoundType, handle, stream, -1,
							volume, 0, true, false);
			else
				_mixer->playInputStream(Audio::Mixer::kSFXSoundType, handle, stream, -1,
							volume, 0, true, false);
		}
	}
}

void Sound::playSound(SoundBuffer &buffer, int volume, bool loop) {
	SndHandle *handle = getHandle();

	handle->type = kEffectHandle;
	playSoundBuffer(&handle->handle, buffer, 2 * volume, handle->type, loop);
}

void Sound::pauseSound() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kEffectHandle)
			_mixer->pauseHandle(_handles[i].handle, true);
}

void Sound::resumeSound() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kEffectHandle)
			_mixer->pauseHandle(_handles[i].handle, false);
}

void Sound::stopSound() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kEffectHandle) {
			_mixer->stopHandle(_handles[i].handle);
			_handles[i].type = kFreeHandle;
		}
}

void Sound::playVoice(SoundBuffer &buffer) {
	SndHandle *handle = getHandle();

	handle->type = kVoiceHandle;
	playSoundBuffer(&handle->handle, buffer, 255, handle->type, false);
}

void Sound::pauseVoice() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kVoiceHandle)
			_mixer->pauseHandle(_handles[i].handle, true);
}

void Sound::resumeVoice() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kVoiceHandle)
			_mixer->pauseHandle(_handles[i].handle, false);
}

void Sound::stopVoice() {
	for (int i = 0; i < SOUND_HANDLES; i++)
		if (_handles[i].type == kVoiceHandle) {
			_mixer->stopHandle(_handles[i].handle);
			_handles[i].type = kFreeHandle;
		}
}

void Sound::stopAll() {
	stopVoice();
	stopSound();
}

void Sound::setVolume() {
	_vm->_soundVolume = ConfMan.getInt("sfx_volume");
	_vm->_speechVolume = ConfMan.getInt("speech_volume");
	_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType, _vm->_soundVolume);
	_mixer->setVolumeForSoundType(Audio::Mixer::kSpeechSoundType, _vm->_speechVolume);
}

} // End of namespace Saga
