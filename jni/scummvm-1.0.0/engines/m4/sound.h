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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/m4/sound.h $
 * $Id: sound.h 31600 2008-04-20 14:47:37Z thebluegr $
 *
 */

// Sound class

#ifndef M4_SOUND_H
#define M4_SOUND_H

#include "common/file.h"
#include "common/array.h"
#include "sound/mixer.h"
#include "sound/mp3.h"
#include "sound/vorbis.h"
#include "sound/flac.h"

namespace M4 {

#define SOUND_HANDLES 10

enum SOUND_FLAGS {
	SOUND_LOOP = 1
};

enum sndHandleType {
	kFreeHandle,
	kEffectHandle,
	kVoiceHandle
};

struct SndHandle {
	Audio::SoundHandle handle;
	sndHandleType type;
};

struct DSREntry {
	int16 frequency;
	int channels;
	int32 compSize;
	int32 uncompSize;
	int32 offset;
};

struct DSRFile {
	char fileName[20];
	int entryCount;
	Common::Array<DSREntry *> dsrEntries;
};

class M4Engine;

class Sound {
public:

	Sound(M4Engine *vm, Audio::Mixer *mixer, int volume);
	~Sound();

	void playSound(const char *soundName, int volume, bool loop, int channel = -1);
	void pauseSound();
	void resumeSound();
	void stopSound(int channel = -1);

	void playVoice(const char *soundName, int volume);
	void pauseVoice();
	void resumeVoice();
	void stopVoice();

	void stopAll();

	void setVolume(int volume);

	bool isHandleActive(SndHandle *handle);
	SndHandle *getHandle();

	void loadDSRFile(const char *fileName);
	void unloadDSRFile();
	void playDSRSound(int soundIndex, int volume, bool loop);

 private:

	M4Engine *_vm;
	Audio::Mixer *_mixer;
	SndHandle _handles[SOUND_HANDLES];

	DSRFile _dsrFile;
	bool _dsrFileLoaded;
};

} // End of namespace M4

#endif
