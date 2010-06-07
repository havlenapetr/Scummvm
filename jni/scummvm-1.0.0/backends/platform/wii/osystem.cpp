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
 */

#include "backends/fs/wii/wii-fs-factory.h"
#include "common/config-manager.h"

#include "osystem.h"

#include <unistd.h>

#include <ogc/mutex.h>
#include <ogc/lwp_watchdog.h>

OSystem_Wii::OSystem_Wii() :
	_startup_time(0),

	_palette(NULL),
	_cursorPalette(NULL),
	_cursorPaletteDisabled(true),

	_gameWidth(0),
	_gameHeight(0),
	_gamePixels(NULL),

	_overlayVisible(false),
	_overlayWidth(0),
	_overlayHeight(0),
	_overlaySize(0),
	_overlayPixels(NULL),

	_lastScreenUpdate(0),
	_texture(NULL),
	_currentWidth(0),
	_currentHeight(0),

	_activeGraphicsMode(0),

	_fullscreen(false),

	_mouseVisible(false),
	_mouseX(0),
	_mouseY(0),
	_mouseWidth(0),
	_mouseHeight(0),
	_mouseHotspotX(0),
	_mouseHotspotY(0),
	_mouseKeyColor(0),
	_mouseCursor(NULL),

	_kbd_active(false),

	_event_quit(false),

	_savefile(NULL),
	_mixer(NULL),
	_timer(NULL) {
}

OSystem_Wii::~OSystem_Wii() {
	if (_savefile) {
		delete _savefile;
		_savefile = NULL;
	}

	if (_mixer) {
		delete _mixer;
		_mixer = NULL;
	}

	if (_timer) {
		delete _timer;
		_timer = NULL;
	}
}

void OSystem_Wii::initBackend() {
	_startup_time = gettime();

	char buf[MAXPATHLEN];
	if (!getcwd(buf, MAXPATHLEN))
		strcpy(buf, "/");

	_savefile = new DefaultSaveFileManager(buf);
	_mixer = new Audio::MixerImpl(this);
	_timer = new DefaultTimerManager();

	_fullscreen = ConfMan.getBool("fullscreen");

	initGfx();
	initSfx();
	initEvents();

	OSystem::initBackend();
}

void OSystem_Wii::quit() {
	deinitEvents();
	deinitSfx();
	deinitGfx();

	// umount all async filesystems
	WiiFilesystemFactory::asyncHandler(false, NULL);
}

bool OSystem_Wii::hasFeature(Feature f) {
	return (f == kFeatureFullscreenMode) ||
			(f == kFeatureCursorHasPalette);
}

void OSystem_Wii::setFeatureState(Feature f, bool enable) {
	switch (f) {
	case kFeatureFullscreenMode:
		_fullscreen = enable;
		setGraphicsMode(_activeGraphicsMode);
		break;
	default:
		break;
	}
}

bool OSystem_Wii::getFeatureState(Feature f) {
	switch (f) {
	case kFeatureFullscreenMode:
		return _fullscreen;
	default:
		return false;
	}
}

uint32 OSystem_Wii::getMillis() {
	return ticks_to_millisecs(diff_ticks(_startup_time, gettime()));
}

void OSystem_Wii::delayMillis(uint msecs) {
	usleep(msecs * 1000);
}

OSystem::MutexRef OSystem_Wii::createMutex() {
	mutex_t *mutex = (mutex_t *) malloc(sizeof(mutex_t));
	s32 res = LWP_MutexInit(mutex, true);

	if (res) {
		printf("ERROR creating mutex\n");
		free(mutex);
		return NULL;
	}

	return (MutexRef) mutex;
}

void OSystem_Wii::lockMutex(MutexRef mutex) {
	s32 res = LWP_MutexLock(*(mutex_t *) mutex);

	if (res)
		printf("ERROR locking mutex %p (%d)\n", mutex, res);
}

void OSystem_Wii::unlockMutex(MutexRef mutex) {
	s32 res = LWP_MutexUnlock(*(mutex_t *) mutex);

	if (res)
		printf("ERROR unlocking mutex %p (%d)\n", mutex, res);
}

void OSystem_Wii::deleteMutex(MutexRef mutex) {
	s32 res = LWP_MutexDestroy(*(mutex_t *) mutex);

	if (res)
		printf("ERROR destroying mutex %p (%d)\n", mutex, res);

	free(mutex);
}

void OSystem_Wii::setWindowCaption(const char *caption) {
	printf("window caption: %s\n", caption);
}

Common::SaveFileManager *OSystem_Wii::getSavefileManager() {
	assert(_savefile);
	return _savefile;
}

Audio::Mixer *OSystem_Wii::getMixer() {
	assert(_mixer);
	return _mixer;
}

Common::TimerManager *OSystem_Wii::getTimerManager() {
	assert(_timer);
	return _timer;
}

FilesystemFactory *OSystem_Wii::getFilesystemFactory() {
	return &WiiFilesystemFactory::instance();
}

void OSystem_Wii::getTimeAndDate(struct tm &t) const {
	time_t curTime = time(0);
	t = *localtime(&curTime);
}

void OSystem_Wii::engineInit() {
	// umount not required filesystems for this game
	WiiFilesystemFactory::asyncHandler(false, &ConfMan.get("path"));
}

