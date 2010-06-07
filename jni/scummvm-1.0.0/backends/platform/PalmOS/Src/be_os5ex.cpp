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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/be_os5ex.cpp $
 * $Id: be_os5ex.cpp 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

// for DAL.h
#define ALLOW_ACCESS_TO_INTERNALS_OF_BITMAPS

#include "be_os5ex.h"
#include "palmdefs.h"

#include <System/WIP.h>
#include <System/DAL.h>

static TimerExType _timerEx;

OSystem_PalmOS5Ex::OSystem_PalmOS5Ex() : OSystem_PalmOS5() {
	_timerEx.timerID = 0;
	_timerEx.timer = &_timer;
	_timerEx.ticks = SysTicksPerSecond();
}

static SYSTEM_CALLBACK void timer_handler(void *userDataP) {
	CALLBACK_PROLOGUE
	TimerExPtr _timerEx = (TimerExPtr)userDataP;
	TimerPtr _timer = _timerEx->timer;
	_timer->duration = _timer->callback(_timer->duration);
	KALTimerSet(_timerEx->timerID, (_timer->duration * _timerEx->ticks / 1000));
	CALLBACK_EPILOGUE
}

void OSystem_PalmOS5Ex::setTimerCallback(TimerProc callback, int timer) {
	if (_timer.active && _timerEx.timerID)
		KALTimerDelete(_timerEx.timerID);

	if (callback != NULL) {
		Err e;
		CALLBACK_INIT(_timerEx);
		_timer.duration = timer;
		_timer.callback = callback;

		// create the timer
		e = KALTimerCreate(&_timerEx.timerID, appFileCreator, &::timer_handler, &_timerEx);
		if (!e) {
			e = KALTimerSet(_timerEx.timerID, (timer * _timerEx.ticks / 1000));
			if (e) KALTimerDelete(_timerEx.timerID);
		}
		_timer.active = (!e);

	} else {
		_timer.active = false;
	}

	if (!_timer.active)
		_timerEx.timerID = 0;
}

OSystem::MutexRef OSystem_PalmOS5Ex::createMutex() {
	UInt32 mutexID;
	Err e = KALMutexCreate(&mutexID, appFileCreator);
	return (MutexRef)(e ? NULL : mutexID);

}

void OSystem_PalmOS5Ex::lockMutex(MutexRef mutex) {
	if (mutex)
		KALMutexReserve((UInt32)mutex);
}

void OSystem_PalmOS5Ex::unlockMutex(MutexRef mutex) {
	if (mutex)
		KALMutexRelease((UInt32)mutex, 0);
}

void OSystem_PalmOS5Ex::deleteMutex(MutexRef mutex) {
	if (mutex)
		KALMutexDelete((UInt32)mutex);
}

void OSystem_PalmOS5Ex::int_quit() {
	if (_timerEx.timerID)
		KALTimerDelete(_timerEx.timerID);
}
