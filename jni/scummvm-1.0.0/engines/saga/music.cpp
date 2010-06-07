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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/music.cpp $
 * $Id: music.cpp 39234 2009-03-08 15:39:19Z thebluegr $
 *
 */

// MIDI and digital music class

#include "saga/saga.h"

#include "saga/resource.h"
#include "saga/music.h"

#include "sound/audiostream.h"
#include "sound/mididrv.h"
#include "sound/midiparser.h"
#include "common/config-manager.h"
#include "common/file.h"

namespace Saga {

#define BUFFER_SIZE 4096
#define MUSIC_SUNSPOT 26

class DigitalMusicInputStream : public Audio::AudioStream {
private:
	Audio::AudioStream *_compressedStream;
	ResourceContext *_context;
	ResourceData * resourceData;
	GameSoundTypes soundType;
	Common::File *_file;
	uint32 _filePos;
	uint32 _startPos;
	uint32 _endPos;
	bool _finished;
	bool _looping;
	int16 _buf[BUFFER_SIZE];
	const int16 *_bufferEnd;
	const int16 *_pos;
	MemoryReadStream *_memoryStream;
	SagaEngine *_vm;

	void refill();
	bool eosIntern() const {
		return _pos >= _bufferEnd;
	}

public:
	DigitalMusicInputStream(SagaEngine *vm, ResourceContext *context, uint32 resourceId, bool looping, uint32 loopStart);
	~DigitalMusicInputStream();

	void createCompressedStream();

	int readBuffer(int16 *buffer, const int numSamples);

	bool endOfData() const	{ return eosIntern(); }
	bool isStereo() const	{
		// The digital music in the ITE Mac demo version is not stereo
		return _vm->getFeatures() & GF_MONO_MUSIC ? false : true;
	}
	int getRate() const	{ return 11025; }
};

DigitalMusicInputStream::DigitalMusicInputStream(SagaEngine *vm, ResourceContext *context, uint32 resourceId, bool looping, uint32 loopStart)
	: _vm(vm), _context(context), _finished(false), _looping(looping), _bufferEnd(_buf + BUFFER_SIZE) {

	byte compressedHeader[10];

	resourceData = context->getResourceData(resourceId);
	_file = context->getFile(resourceData);

	_compressedStream = NULL;

	if (context->isCompressed) {
		// Read compressed header to determine compression type
		_file->seek((long)resourceData->offset, SEEK_SET);
		_file->read(compressedHeader, 9);

		if (compressedHeader[0] == char(0)) {
			soundType = kSoundMP3;
		} else if (compressedHeader[0] == char(1)) {
			soundType = kSoundOGG;
		} else if (compressedHeader[0] == char(2)) {
			soundType = kSoundFLAC;
		}

		createCompressedStream();
	}

	// Determine the end position
	_filePos = resourceData->offset;
	_endPos = _filePos + resourceData->size;

	if (_compressedStream != NULL) {
		_filePos += 9;	// skip compressed header
		_endPos -= 9;	// decrease size by the size of the compressed header
	}

	_startPos = _filePos + loopStart;
	if (_startPos >= _endPos)
		_startPos = _filePos;

	// Read in initial data
	refill();
}

DigitalMusicInputStream::~DigitalMusicInputStream() {
	delete _compressedStream;
}

void DigitalMusicInputStream::createCompressedStream() {
#if defined(USE_MAD) || defined(USE_VORBIS) || defined(USE_FLAC)
	uint numLoops = _looping ? 0 : 1;
#endif
	_memoryStream = _file->readStream(resourceData->size - 9);

	switch (soundType) {
#ifdef USE_MAD
		case kSoundMP3:
			debug(1, "Playing MP3 compressed digital music");
			_compressedStream = Audio::makeMP3Stream(_memoryStream, true, 0, 0, numLoops);
			break;
#endif
#ifdef USE_VORBIS
		case kSoundOGG:
			debug(1, "Playing OGG compressed digital music");
			_compressedStream = Audio::makeVorbisStream(_memoryStream, true, 0, 0, numLoops);
			break;
#endif
#ifdef USE_FLAC
		case kSoundFLAC:
			debug(1, "Playing FLAC compressed digital music");
			_compressedStream = Audio::makeFlacStream(_memoryStream, true, 0, 0, numLoops);
			break;
#endif
		default:
			// Unknown compression
			error("Trying to play compressed digital music, but the compression is not known");
			break;
	}
}

int DigitalMusicInputStream::readBuffer(int16 *buffer, const int numSamples) {
	if (_compressedStream != NULL)
		return _compressedStream->readBuffer(buffer, numSamples);

	int samples = 0;
	int len = 0;

	while (samples < numSamples && !eosIntern()) {
		len = MIN(numSamples - samples, (int) (_bufferEnd - _pos));
		memcpy(buffer, _pos, len * 2);
		buffer += len;
		_pos += len;
		samples += len;
		if (_pos >= _bufferEnd)
			refill();
	}
	return samples;
}

void DigitalMusicInputStream::refill() {
	if (_finished)
		return;

	uint32 lengthLeft;
	byte *ptr = (byte *) _buf;

	_file->seek(_filePos, SEEK_SET);

	if (_looping)
		lengthLeft = 2 * BUFFER_SIZE;
	else
		lengthLeft = MIN((uint32) (2 * BUFFER_SIZE), _endPos - _filePos);

	while (lengthLeft > 0) {
		uint32 len = _file->read(ptr, MIN(lengthLeft, _endPos - _file->pos()));

		if (len & 1)
			len--;

#ifdef SCUMM_BIG_ENDIAN
		if (!_context->isBigEndian) {
#else
		if (_context->isBigEndian) {
#endif
			uint16 *ptr16 = (uint16 *)ptr;
			for (uint32 i = 0; i < (len / 2); i++)
				ptr16[i] = SWAP_BYTES_16(ptr16[i]);
		}

		lengthLeft -= len;
		ptr += len;

		if (lengthLeft > 0)
			_file->seek(_startPos);
	}

	_filePos = _file->pos();
	_pos = _buf;
	_bufferEnd = (int16 *)ptr;

	if (!_looping && _filePos >= _endPos) {
		_finished = true;
	}
}


MusicPlayer::MusicPlayer(MidiDriver *driver) : _parser(0), _driver(driver), _looping(false), _isPlaying(false), _passThrough(false), _isGM(false) {
	memset(_channel, 0, sizeof(_channel));
	_masterVolume = 0;
	this->open();
}

MusicPlayer::~MusicPlayer() {
	_driver->setTimerCallback(NULL, NULL);
	stopMusic();
	this->close();
}

void MusicPlayer::setVolume(int volume) {
	volume = CLIP(volume, 0, 255);

	if (_masterVolume == volume)
		return;

	_masterVolume = volume;

	Common::StackLock lock(_mutex);

	for (int i = 0; i < 16; ++i) {
		if (_channel[i]) {
			_channel[i]->volume(_channelVolume[i] * _masterVolume / 255);
		}
	}
}

int MusicPlayer::open() {
	// Don't ever call open without first setting the output driver!
	if (!_driver)
		return 255;

	int ret = _driver->open();
	if (ret)
		return ret;

	_driver->setTimerCallback(this, &onTimer);
	return 0;
}

void MusicPlayer::close() {
	stopMusic();
	if (_driver)
		_driver->close();
	_driver = 0;
}

void MusicPlayer::send(uint32 b) {
	if (_passThrough) {
		_driver->send(b);
		return;
	}

	byte channel = (byte)(b & 0x0F);
	if ((b & 0xFFF0) == 0x07B0) {
		// Adjust volume changes by master volume
		byte volume = (byte)((b >> 16) & 0x7F);
		_channelVolume[channel] = volume;
		volume = volume * _masterVolume / 255;
		b = (b & 0xFF00FFFF) | (volume << 16);
	} else if ((b & 0xF0) == 0xC0 && !_isGM && !_nativeMT32) {
		b = (b & 0xFFFF00FF) | MidiDriver::_mt32ToGm[(b >> 8) & 0xFF] << 8;
	}
	else if ((b & 0xFFF0) == 0x007BB0) {
		//Only respond to All Notes Off if this channel
		//has currently been allocated
		if (_channel[b & 0x0F])
			return;
	}

	if (!_channel[channel])
		_channel[channel] = (channel == 9) ? _driver->getPercussionChannel() : _driver->allocateChannel();

	if (_channel[channel])
		_channel[channel]->send(b);
}

void MusicPlayer::metaEvent(byte type, byte *data, uint16 length) {
	// FIXME: The "elkfanfare" is played much too quickly. There are some
	//        meta events that we don't handle. Perhaps there is a
	//        connection...?

	switch (type) {
	case 0x2F:	// End of Track
		if (_looping)
			_parser->jumpToTick(0);
		else
			stopMusic();
		break;
	default:
		//warning("Unhandled meta event: %02x", type);
		break;
	}
}

void MusicPlayer::onTimer(void *refCon) {
	MusicPlayer *music = (MusicPlayer *)refCon;
	Common::StackLock lock(music->_mutex);

	if (music->_isPlaying)
		music->_parser->onTimer();
}

void MusicPlayer::playMusic() {
	_isPlaying = true;
}

void MusicPlayer::stopMusic() {
	Common::StackLock lock(_mutex);

	_isPlaying = false;
	if (_parser) {
		_parser->unloadMusic();
		_parser = NULL;
	}
}

Music::Music(SagaEngine *vm, Audio::Mixer *mixer, MidiDriver *driver) : _vm(vm), _mixer(mixer), _adlib(false) {
	_player = new MusicPlayer(driver);
	_currentVolume = 0;

	xmidiParser = MidiParser::createParser_XMIDI();
	smfParser = MidiParser::createParser_SMF();

	_digitalMusicContext = _vm->_resource->getContext(GAME_DIGITALMUSICFILE);

	_songTableLen = 0;
	_songTable = 0;

	_midiMusicData = NULL;
	_digitalMusic = false;
}

Music::~Music() {
	_vm->getTimerManager()->removeTimerProc(&musicVolumeGaugeCallback);
	_mixer->stopHandle(_musicHandle);
	delete _player;
	xmidiParser->setMidiDriver(NULL);
	smfParser->setMidiDriver(NULL);
	delete xmidiParser;
	delete smfParser;

	free(_songTable);
	free(_midiMusicData);
}

void Music::musicVolumeGaugeCallback(void *refCon) {
	((Music *)refCon)->musicVolumeGauge();
}

void Music::musicVolumeGauge() {
	int volume;

	_currentVolumePercent += 10;

	if (_currentVolume - _targetVolume > 0) { // Volume decrease
		volume = _targetVolume + (_currentVolume - _targetVolume) * (100 - _currentVolumePercent) / 100;
	} else {
		volume = _currentVolume + (_targetVolume - _currentVolume) * _currentVolumePercent / 100;
	}

	if (volume < 0)
		volume = 1;

	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, volume);
	_player->setVolume(volume);

	if (_currentVolumePercent == 100) {
		_vm->getTimerManager()->removeTimerProc(&musicVolumeGaugeCallback);
		_currentVolume = _targetVolume;
	}
}

void Music::setVolume(int volume, int time) {
	_targetVolume = volume;
	_currentVolumePercent = 0;

	if (volume == -1) // Set Full volume
		volume = 255;

	if (time == 1) {
		_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, volume);
		_player->setVolume(volume);
		_vm->getTimerManager()->removeTimerProc(&musicVolumeGaugeCallback);
		_currentVolume = volume;
		return;
	}

	_vm->getTimerManager()->installTimerProc(&musicVolumeGaugeCallback, time * 100L, this);
}

bool Music::isPlaying() {
	return _mixer->isSoundHandleActive(_musicHandle) || _player->isPlaying();
}

void Music::play(uint32 resourceId, MusicFlags flags) {
	Audio::AudioStream *audioStream = NULL;
	MidiParser *parser;
	ResourceContext *context = NULL;
	byte *resourceData;
	size_t resourceSize;
	uint32 loopStart;

	debug(2, "Music::play %d, %d", resourceId, flags);

	if (isPlaying() && _trackNumber == resourceId) {
		return;
	}

	_trackNumber = resourceId;
	_player->stopMusic();
	_mixer->stopHandle(_musicHandle);

	int realTrackNumber;

	if (_vm->getGameId() == GID_ITE) {
		if (flags == MUSIC_DEFAULT) {
			if (resourceId == 13 || resourceId == 19) {
				flags = MUSIC_NORMAL;
			} else {
				flags = MUSIC_LOOP;
			}
		}
		realTrackNumber = resourceId - 8;
	} else {
		realTrackNumber = resourceId + 1;
	}

	// Try to open standalone digital track
	char trackName[2][16];
	sprintf(trackName[0], "track%d", realTrackNumber);
	sprintf(trackName[1], "track%02d", realTrackNumber);
	Audio::AudioStream *stream = 0;
	for (int i = 0; i < 2; ++i) {
		// We multiply by 40 / 3 = 1000 / 75 to convert frames to milliseconds
		// FIXME: Do we really want a duration of 10000 frames = 133 seconds, or is that just a random value?
		stream = Audio::AudioStream::openStreamFile(trackName[i], 0, 10000 * 40 / 3, (flags == MUSIC_LOOP) ? 0 : 1);
		if (stream) {
			_mixer->playInputStream(Audio::Mixer::kMusicSoundType, &_musicHandle, stream);
			_digitalMusic = true;
			return;
		}
	}

	if (_vm->getGameId() == GID_ITE) {
		if (resourceId >= 9 && resourceId <= 34) {
			if (_digitalMusicContext != NULL) {
				//TODO: check resource size
				loopStart = 0;
				// fix ITE sunstatm/sunspot score
				if ((_vm->getGameId() == GID_ITE) && (resourceId == MUSIC_SUNSPOT)) {
					loopStart = 4 * 18727;
				}

				// digital music
				audioStream = new DigitalMusicInputStream(_vm, _digitalMusicContext, resourceId - 9, flags == MUSIC_LOOP, loopStart);
			}
		}
	}

	if (audioStream) {
		debug(2, "Playing digitized music");
		_mixer->playInputStream(Audio::Mixer::kMusicSoundType, &_musicHandle, audioStream);
		_digitalMusic = true;
		return;
	}

	if (flags == MUSIC_DEFAULT) {
		flags = MUSIC_NORMAL;
	}

	// Load MIDI/XMI resource data

	if (_vm->getGameId() == GID_ITE) {
		context = _vm->_resource->getContext(GAME_MUSICFILE_GM);
		if (context == NULL) {
			context = _vm->_resource->getContext(GAME_RESOURCEFILE);
		}
	} else if (_vm->getGameId() == GID_IHNM && _vm->isMacResources()) {
		// The music of the Mac version of IHNM is loaded from its
		// associated external file later on
	} else {
		// I've listened to music from both the FM and the GM
		// file, and I've tentatively reached the conclusion
		// that they are both General MIDI. My guess is that
		// the FM file has been reorchestrated to sound better
		// on Adlib and other FM synths.
		//
		// Sev says the Adlib music does not sound like in the
		// original, but I still think assuming General MIDI is
		// the right thing to do. Some music, like the End
		// Title (song 0) sound absolutely atrocious when piped
		// through our MT-32 to GM mapping.
		//
		// It is, however, quite possible that the original
		// used a different GM to FM mapping. If the original
		// sounded markedly better, perhaps we should add some
		// way of replacing our stock mapping in adlib.cpp?
		//
		// For the composer's own recording of the End Title,
		// see http://www.johnottman.com/

		// Oddly enough, the intro music (song 1) is very
		// different in the two files. I have no idea why.
		// Note that the IHNM demo has only got one music file
		// (music.rsc). It is assumed that it contains FM music

		if (hasAdlib() || _vm->getFeatures() & GF_IHNM_DEMO) {
			context = _vm->_resource->getContext(GAME_MUSICFILE_FM);
		} else {
			context = _vm->_resource->getContext(GAME_MUSICFILE_GM);
		}
	}

	_player->setGM(true);

	if (_vm->getGameId() == GID_IHNM && _vm->isMacResources()) {
		// Load the external music file for Mac IHNM
		Common::File musicFile;
		char musicFileName[40];
		sprintf(musicFileName, "Music/Music%02x", resourceId);
		musicFile.open(musicFileName);
		resourceSize = musicFile.size();
		resourceData = new byte[resourceSize];
		musicFile.read(resourceData, resourceSize);
		musicFile.close();

		// TODO: The Mac music format is unsupported (QuickTime MIDI)
		// so stop here
		return;
	} else {
		_vm->_resource->loadResource(context, resourceId, resourceData, resourceSize);
	}

	if (resourceSize < 4) {
		error("Music::play() wrong music resource size");
	}

	if (xmidiParser->loadMusic(resourceData, resourceSize)) {
		if (_vm->getGameId() == GID_ITE)
			_player->setGM(false);

		parser = xmidiParser;
	} else {
		if (smfParser->loadMusic(resourceData, resourceSize)) {
			parser = smfParser;
		} else {
			error("Music::play() wrong music resource");
		}
	}

	parser->setTrack(0);
	parser->setMidiDriver(_player);
	parser->setTimerRate(_player->getBaseTempo());
	parser->property(MidiParser::mpCenterPitchWheelOnUnload, 1);

	_player->_parser = parser;
	setVolume(_vm->_musicVolume);

	if (flags & MUSIC_LOOP)
		_player->setLoop(true);
	else
		_player->setLoop(false);

	_player->playMusic();
	free(_midiMusicData);
	_midiMusicData = resourceData;
}

void Music::pause(void) {
	_player->setVolume(-1);
	_player->setPlaying(false);
}

void Music::resume(void) {
	_player->setVolume(_vm->_musicVolume);
	_player->setPlaying(true);
}

void Music::stop(void) {
	_player->stopMusic();
}

} // End of namespace Saga

