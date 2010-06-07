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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/music/musicbase.cpp $
 * $Id: musicbase.cpp 36304 2009-02-13 16:55:16Z joostp $
 *
 */

#include "sky/music/musicbase.h"
#include "sky/disk.h"
#include "common/util.h"
#include "common/endian.h"

namespace Sky {

MusicBase::MusicBase(Disk *pDisk) {
	_musicData = NULL;
	_skyDisk = pDisk;
	_currentMusic = 0;
	_musicVolume = 127;
	_numberOfChannels = _currentMusic = 0;
}

MusicBase::~MusicBase(void) {
	stopMusic();
	if (_musicData)
		free(_musicData);
}

void MusicBase::loadSection(uint8 pSection) {
	_mutex.lock();
	if (_currentMusic)
		stopMusicInternal();
	if (_musicData)
		free(_musicData);
	_currentSection = pSection;
	_musicData = _skyDisk->loadFile(_driverFileBase + FILES_PER_SECTION * pSection);

	_musicTempo0 = 0x78; // init constants taken from idb file, area ~0x1060
	_musicTempo1 = 0xC0;
	_onNextPoll.musicToProcess = 0;
	_tempo = _aktTime = 0x10001;
	_numberOfChannels = _currentMusic = 0;
	setupPointers();
	startDriver();
	_mutex.unlock();
}

bool MusicBase::musicIsPlaying(void) {
	for (uint8 cnt = 0; cnt < _numberOfChannels; cnt++)
		if (_channels[cnt]->isActive())
			return true;
	return false;
}

void MusicBase::stopMusic(void) {
	_mutex.lock();
	stopMusicInternal();
	_mutex.unlock();
}

void MusicBase::stopMusicInternal(void) {
	for (uint8 cnt = 0; cnt < _numberOfChannels; cnt++)
		delete _channels[cnt];
	_numberOfChannels = 0;
}

void MusicBase::updateTempo(void) {
	uint16 tempoMul = _musicTempo0 * _musicTempo1;
	uint16 divisor = 0x4446390/ 23864;
	_tempo = (tempoMul / divisor) << 16;
	_tempo |= (((tempoMul % divisor) << 16) | (tempoMul / divisor)) / divisor;
}

void MusicBase::loadNewMusic(void) {
	uint16 musicPos;
	if (_onNextPoll.musicToProcess > _musicData[_musicDataLoc]) {
		error("Music %d requested but doesn't exist in file.", _onNextPoll.musicToProcess);
		return;
	}
	if (_currentMusic != 0)
		stopMusicInternal();

	_currentMusic = _onNextPoll.musicToProcess;

	if (_currentMusic != 0) {
		musicPos = READ_LE_UINT16(_musicData + _musicDataLoc + 1);
		musicPos += _musicDataLoc + ((_currentMusic - 1) << 1);
		musicPos = READ_LE_UINT16(_musicData + musicPos) + _musicDataLoc;

		_musicTempo0 = _musicData[musicPos];
		_musicTempo1 = _musicData[musicPos+1];

		setupChannels(_musicData + musicPos + 2);

		updateTempo();
	}
}

void MusicBase::pollMusic(void) {
	_mutex.lock();
	uint8 newTempo;
	if (_onNextPoll.musicToProcess != _currentMusic)
		loadNewMusic();

	_aktTime += _tempo;

	for (uint8 cnt = 0; cnt < _numberOfChannels; cnt++) {
		newTempo = _channels[cnt]->process((uint16)(_aktTime >> 16));
		if (newTempo) {
			_musicTempo1 = newTempo;
			updateTempo();
		}
	}
	_mutex.unlock();
	_aktTime &= 0xFFFF;
}

void MusicBase::startMusic(uint16 param) {
	_onNextPoll.musicToProcess = param & 0xF;
}

uint8 MusicBase::giveVolume(void) {
	return (uint8)_musicVolume;
}

uint8 MusicBase::giveCurrentMusic(void) {
	return _currentMusic;
}

} // End of namespace Sky
