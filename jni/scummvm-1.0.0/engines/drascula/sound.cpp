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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/drascula/sound.cpp $
 * $Id: sound.cpp 40476 2009-05-12 10:02:18Z sev $
 *
 */

#include "sound/mixer.h"
#include "sound/voc.h"
#include "sound/audiocd.h"
#include "common/config-manager.h"

#include "drascula/drascula.h"

namespace Drascula {

void DrasculaEngine::updateVolume(Audio::Mixer::SoundType soundType, int prevVolume) {
	int vol = _mixer->getVolumeForSoundType(soundType) / 16;
	if (mouseY < prevVolume && vol < 15)
		vol++;
	if (mouseY > prevVolume && vol > 0)
		vol--;
	_mixer->setVolumeForSoundType(soundType, vol * 16);
}

void DrasculaEngine::volumeControls() {
	if (_lang == kSpanish)
		loadPic(95, tableSurface);

	copyRect(1, 56, 73, 63, 177, 97, tableSurface, screenSurface);
	updateScreen(73, 63, 73, 63, 177, 97, screenSurface);

	setCursor(kCursorCrosshair);
	showCursor();

	for (;;) {
		int masterVolume = CLIP((_mixer->getVolumeForSoundType(Audio::Mixer::kPlainSoundType) / 16), 0, 15);
		int voiceVolume = CLIP((_mixer->getVolumeForSoundType(Audio::Mixer::kSpeechSoundType) / 16), 0, 15);
		int musicVolume = CLIP((_mixer->getVolumeForSoundType(Audio::Mixer::kMusicSoundType) / 16), 0, 15);

		int masterVolumeY = 72 + 61 - masterVolume * 4;
		int voiceVolumeY = 72 + 61 - voiceVolume * 4;
		int musicVolumeY = 72 + 61 - musicVolume * 4;

		updateRoom();

		copyRect(1, 56, 73, 63, 177, 97, tableSurface, screenSurface);

		copyBackground(183, 56, 82, masterVolumeY, 39, 2 + masterVolume * 4, tableSurface, screenSurface);
		copyBackground(183, 56, 138, voiceVolumeY, 39, 2 + voiceVolume * 4, tableSurface, screenSurface);
		copyBackground(183, 56, 194, musicVolumeY, 39, 2 + musicVolume * 4, tableSurface, screenSurface);

		updateScreen();

		updateEvents();

		// we're ignoring keypresses, so just empty the keyboard buffer
		while (getScan())
			;

		if (rightMouseButton == 1) {
			delay(100);
			break;
		}
		if (leftMouseButton == 1) {
			delay(100);
			if (mouseX > 80 && mouseX < 121) {
				updateVolume(Audio::Mixer::kPlainSoundType, masterVolumeY);
			}

			if (mouseX > 136 && mouseX < 178) {
				updateVolume(Audio::Mixer::kSpeechSoundType, voiceVolumeY);
			}

			if (mouseX > 192 && mouseX < 233) {
				updateVolume(Audio::Mixer::kMusicSoundType, musicVolumeY);
			}
		}

	}

	if (_lang == kSpanish)
		loadPic(974, tableSurface);

	selectVerb(kVerbNone);

	updateEvents();
}

void DrasculaEngine::playSound(int soundNum) {
	char file[20];
	sprintf(file, "s%i.als", soundNum);

	playFile(file);
}

void DrasculaEngine::finishSound() {
	delay(1);

	while (soundIsActive())
		_system->delayMillis(10);
}

void DrasculaEngine::playMusic(int p) {
	AudioCD.stop();
	AudioCD.play(p - 1, 1, 0, 0);
}

void DrasculaEngine::stopMusic() {
	AudioCD.stop();
}

void DrasculaEngine::updateMusic() {
	AudioCD.updateCD();
}

int DrasculaEngine::musicStatus() {
	return AudioCD.isPlaying();
}

void DrasculaEngine::stopSound() {
	_mixer->stopHandle(_soundHandle);
}

void DrasculaEngine::MusicFadeout() {
	int org_vol = _mixer->getVolumeForSoundType(Audio::Mixer::kMusicSoundType);
	for (;;) {
		int vol = _mixer->getVolumeForSoundType(Audio::Mixer::kMusicSoundType);
		vol -= 10;
			if (vol < 0)
				vol = 0;
		_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, vol);
		if (vol == 0)
			break;
		updateEvents();
		_system->updateScreen();
		_system->delayMillis(50);
	}
	AudioCD.stop();
	_system->delayMillis(100);
	_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, org_vol);
}

void DrasculaEngine::playFile(const char *fname) {
	if (_arj.open(fname)) {
		int soundSize = _arj.size();
		byte *soundData = (byte *)malloc(soundSize);

		if (!(!strcmp(fname, "3.als") && soundSize == 145166 && _lang != kSpanish)) {
			_arj.seek(32);
		} else {
			// WORKAROUND: File 3.als with English speech files has a big silence at
			// its beginning and end. We seek past the silence at the beginning,
			// and ignore the silence at the end
			// Fixes bug #2111815 - "DRASCULA: Voice delayed"
			_arj.seek(73959, SEEK_SET);
			soundSize = 117158 - 73959;
		}

		_arj.read(soundData, soundSize);
		_arj.close();

		_subtitlesDisabled = !ConfMan.getBool("subtitles");
		if (ConfMan.getBool("speech_mute"))
			memset(soundData, 0x80, soundSize); // Mute speech but keep the pause

		_mixer->playRaw(Audio::Mixer::kSpeechSoundType, &_soundHandle, soundData, soundSize - 64,
						11025, Audio::Mixer::FLAG_AUTOFREE | Audio::Mixer::FLAG_UNSIGNED);
	} else
		warning("playFile: Could not open %s", fname);
}

bool DrasculaEngine::soundIsActive() {
	return _mixer->isSoundHandleActive(_soundHandle);
}

} // End of namespace Drascula
