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
* $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/softsynth/pcspk.cpp $
* $Id: pcspk.cpp 29856 2007-12-13 23:27:36Z drmccoy $
*
*/

#include "sound/softsynth/pcspk.h"

namespace Audio {

const PCSpeaker::generatorFunc PCSpeaker::generateWave[] =
	{&PCSpeaker::generateSquare, &PCSpeaker::generateSine,
	 &PCSpeaker::generateSaw,    &PCSpeaker::generateTriangle};

PCSpeaker::PCSpeaker(int rate) {
	_rate = rate;
	_wave = kWaveFormSquare;
	_playForever = false;
	_oscLength = 0;
	_oscSamples = 0;
	_remainingSamples = 0;
	_mixedSamples = 0;
	_volume = 255;
}

PCSpeaker::~PCSpeaker() {
}

void PCSpeaker::play(WaveForm wave, int freq, int32 length) {
	Common::StackLock lock(_mutex);

	assert((wave >= kWaveFormSquare) && (wave <= kWaveFormTriangle));

	_wave = wave;
	_oscLength = _rate / freq;
	_oscSamples = 0;
	if (length == -1) {
		_remainingSamples = 1;
		_playForever = true;
	} else {
		_remainingSamples = (_rate * length) / 1000;
		_playForever = false;
	}
	_mixedSamples = 0;
}

void PCSpeaker::stop(int32 delay) {
	Common::StackLock lock(_mutex);

	_remainingSamples = (_rate * delay) / 1000;
	_playForever = false;
}

void PCSpeaker::setVolume(byte volume) {
	_volume = volume;
}

bool PCSpeaker::isPlaying() const {
	return _remainingSamples != 0;
}

int PCSpeaker::readBuffer(int16 *buffer, const int numSamples) {
	Common::StackLock lock(_mutex);

	int i;

	for (i = 0; _remainingSamples && (i < numSamples); i++) {
		buffer[i] = generateWave[_wave](_oscSamples, _oscLength) * _volume;
		if (_oscSamples++ >= _oscLength)
			_oscSamples = 0;
		if (!_playForever)
			_remainingSamples--;
		_mixedSamples++;
	}

	// Clear the rest of the buffer
	if (i < numSamples)
		memset(buffer + i, 0, (numSamples - i) * sizeof(int16));

	return numSamples;
}

int8 PCSpeaker::generateSquare(uint32 x, uint32 oscLength) {
	return (x < (oscLength / 2)) ? 127 : -128;
}

int8 PCSpeaker::generateSine(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	// TODO: Maybe using a look-up-table would be better?
	return CLIP<int16>((int16) (128 * sin(2.0 * PI * x / oscLength)), -128, 127);
}

int8 PCSpeaker::generateSaw(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	return ((x * (65536 / oscLength)) >> 8) - 128;
}

int8 PCSpeaker::generateTriangle(uint32 x, uint32 oscLength) {
	if (oscLength == 0)
		return 0;

	int y = ((x * (65536 / (oscLength / 2))) >> 8) - 128;

	return (x <= (oscLength / 2)) ? y : (256 - y);
}

} // End of namespace Audio
