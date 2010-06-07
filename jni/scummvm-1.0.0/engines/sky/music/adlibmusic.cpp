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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/music/adlibmusic.cpp $
 * $Id: adlibmusic.cpp 36304 2009-02-13 16:55:16Z joostp $
 *
 */


#include "common/endian.h"

#include "sky/music/adlibmusic.h"
#include "sky/music/adlibchannel.h"
#include "sound/mixer.h"
#include "sky/sky.h"

namespace Sky {

AdlibMusic::AdlibMusic(Audio::Mixer *pMixer, Disk *pDisk) : MusicBase(pDisk) {
	_driverFileBase = 60202;
	_mixer = pMixer;
	_sampleRate = pMixer->getOutputRate();

	_opl = makeAdlibOPL(_sampleRate);

	_mixer->playInputStream(Audio::Mixer::kMusicSoundType, &_soundHandle, this, -1, Audio::Mixer::kMaxChannelVolume, 0, false, true);
}

AdlibMusic::~AdlibMusic(void) {
	OPLDestroy(_opl);
	_mixer->stopHandle(_soundHandle);
}

int AdlibMusic::readBuffer(int16 *data, const int numSamples) {
	if (_musicData == NULL) {
		// no music loaded
		memset(data, 0, numSamples * sizeof(int16));
	} else if ((_currentMusic == 0) || (_numberOfChannels == 0)) {
		// music loaded but not played as of yet
		memset(data, 0, numSamples * sizeof(int16));
		// poll anyways as pollMusic() can activate the music
		pollMusic();
		_nextMusicPoll = _sampleRate / 50;
	} else {
		uint32 render;
		uint remaining = numSamples;
		while (remaining) {
			render = (remaining > _nextMusicPoll) ? _nextMusicPoll : remaining;
			remaining -= render;
			_nextMusicPoll -= render;
			YM3812UpdateOne(_opl, data, render);
			data += render;
			if (_nextMusicPoll == 0) {
				pollMusic();
				_nextMusicPoll = _sampleRate / 50;
			}
		}
	}
	return numSamples;
}

void AdlibMusic::setupPointers(void) {
	if (SkyEngine::_systemVars.gameVersion == 109) {
		// disk demo uses a different adlib driver version, some offsets have changed
		//_musicDataLoc = (_musicData[0x11CC] << 8) | _musicData[0x11CB];
		//_initSequence = _musicData + 0xEC8;

		_musicDataLoc = READ_LE_UINT16(_musicData + 0x1200);
		_initSequence = _musicData + 0xEFB;
	} else if (SkyEngine::_systemVars.gameVersion == 267) {
		_musicDataLoc = READ_LE_UINT16(_musicData + 0x11F7);
		_initSequence = _musicData + 0xE87;
	} else {
		_musicDataLoc = READ_LE_UINT16(_musicData + 0x1201);
		_initSequence = _musicData + 0xE91;
	}
	_nextMusicPoll = 0;
}

void AdlibMusic::setupChannels(uint8 *channelData) {
	_numberOfChannels = channelData[0];
	channelData++;
	for (uint8 cnt = 0; cnt < _numberOfChannels; cnt++) {
		uint16 chDataStart = READ_LE_UINT16((uint16 *)channelData + cnt) + _musicDataLoc;
		_channels[cnt] = new AdlibChannel(_opl, _musicData, chDataStart);
	}
}

void AdlibMusic::startDriver(void) {
	uint16 cnt = 0;
	while (_initSequence[cnt] || _initSequence[cnt + 1]) {
		OPLWriteReg (_opl, _initSequence[cnt], _initSequence[cnt + 1]);
		cnt += 2;
	}
}

void AdlibMusic::setVolume(uint16 param) {
	_musicVolume = param;
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, 2 * param);
}

bool AdlibMusic::isStereo(void) const {
	return false;
}

bool AdlibMusic::endOfData(void) const {
	return false;
}

int AdlibMusic::getRate(void) const {
	return _sampleRate;
}

} // End of namespace Sky
