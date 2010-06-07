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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/agos/midi.h $
 * $Id: midi.h 34243 2008-09-01 17:46:53Z fingolfin $
 *
 */

#ifndef AGOS_MIDI_H
#define AGOS_MIDI_H

#include "sound/mididrv.h"
#include "sound/midiparser.h"
#include "common/mutex.h"

namespace Common {
	class File;
}

namespace AGOS {

struct MusicInfo {
	MidiParser *parser;
	byte *data;
	byte num_songs;           // For Type 1 SMF resources
	byte *songs[16];          // For Type 1 SMF resources
	uint32 song_sizes[16];    // For Type 1 SMF resources

	MidiChannel *channel[16]; // Dynamic remapping of channels to resolve conflicts
	byte volume[16];          // Current channel volume

	MusicInfo() { clear(); }
	void clear() {
		parser = 0; data = 0; num_songs = 0;
		memset(songs, 0, sizeof(songs));
		memset(song_sizes, 0, sizeof(song_sizes));
		memset(channel, 0, sizeof(channel));
	}
};

class MidiPlayer : public MidiDriver {
protected:
	Common::Mutex _mutex;
	MidiDriver *_driver;
	bool _map_mt32_to_gm;
	bool _passThrough;

	MusicInfo _music;
	MusicInfo _sfx;
	MusicInfo *_current; // Allows us to establish current context for operations.

	// These are maintained for both music and SFX
	byte _masterVolume;    // 0-255
	byte _musicVolume;
	byte _sfxVolume;
	bool _paused;

	// These are only used for music.
	byte _currentTrack;
	bool _loopTrack;
	byte _queuedTrack;
	bool _loopQueuedTrack;

protected:
	static void onTimer(void *data);
	void clearConstructs();
	void clearConstructs(MusicInfo &info);
	void resetVolumeTable();

public:
	bool _enable_sfx;

public:
	MidiPlayer();
	virtual ~MidiPlayer();

	void loadSMF(Common::File *in, int song, bool sfx = false);
	void loadMultipleSMF(Common::File *in, bool sfx = false);
	void loadXMIDI(Common::File *in, bool sfx = false);
	void loadS1D(Common::File *in, bool sfx = false);

	void mapMT32toGM(bool map);
	void setLoop(bool loop);
	void startTrack(int track);
	void queueTrack(int track, bool loop);
	bool isPlaying(bool check_queued = false) { return (_currentTrack != 255 && (_queuedTrack != 255 || !check_queued)); }

	void stop();
	void pause(bool b);

	int  getMusicVolume() { return _musicVolume; }
	int  getSFXVolume() { return _sfxVolume; }
	void setVolume(int musicVol, int sfxVol);
	void setDriver(MidiDriver *md);

public:
	// MidiDriver interface implementation
	int open();
	void close();
	void send(uint32 b);

	void metaEvent(byte type, byte *data, uint16 length);
	void setPassThrough(bool b)		{ _passThrough = b; }

	// Timing functions - MidiDriver now operates timers
	void setTimerCallback(void *timer_param, void (*timer_proc) (void *)) { }
	uint32 getBaseTempo(void) { return _driver ? _driver->getBaseTempo() : 0; }

	// Channel allocation functions
	MidiChannel *allocateChannel() { return 0; }
	MidiChannel *getPercussionChannel() { return 0; }
};

} // End of namespace AGOS

#endif
