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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/timer/default/default-timer.h $
 * $Id: default-timer.h 32827 2008-06-28 15:27:40Z fingolfin $
 */

#ifndef BACKENDS_TIMER_DEFAULT_H
#define BACKENDS_TIMER_DEFAULT_H

#include "common/timer.h"
#include "common/mutex.h"

class OSystem;

struct TimerSlot;

class DefaultTimerManager : public Common::TimerManager {
private:
	Common::Mutex _mutex;
	void *_timerHandler;
	TimerSlot *_head;

public:
	DefaultTimerManager();
	~DefaultTimerManager();
	bool installTimerProc(TimerProc proc, int32 interval, void *refCon);
	void removeTimerProc(TimerProc proc);

	/**
	 * Timer callback, to be invoked at regular time intervals by the backend.
	 */
	void handler();
};

#endif
