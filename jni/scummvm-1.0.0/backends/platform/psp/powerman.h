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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/psp/powerman.h $
 * $Id: powerman.h 44277 2009-09-23 20:31:23Z joostp $
 *
 */

#ifndef POWERMAN_H
#define POWERMAN_H
 
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include "common/singleton.h"
#include "common/list.h"
 
 /*
  *  Implement this class (interface) if you want to use PowerManager's suspend callback functionality
  *
  */
 class Suspendable {
 public:
	virtual ~Suspendable() {}
	virtual int suspend() = 0;
	virtual int resume() = 0;
 };
 
 /****************************************************************************************************** 
 *
 *  This class will call a Suspendable when the PSP goes to suspend/resumes. It also provides the ability to block
 *  a thread when the PSP is going to suspend/suspending, and to wake it up when the PSP is resumed. 
 *	This ability is very useful for managing the PSPIoStream class, but may be found useful by other classes as well.
 *
 *******************************************************************************************************/
 class PowerManager: public Common::Singleton<PowerManager> {

public:
	int blockOnSuspend();								/* block if suspending */
	int beginCriticalSection(bool justBlock = false);	/* Use a critical section to block (if suspend was already pressed) */
	int endCriticalSection();							/* and to prevent the PSP from suspending in a particular section */
	int registerSuspend(Suspendable *item);			/* register to be called to suspend/resume */
	int unregisterSuspend(Suspendable *item);		/* remove from suspend/resume list */
	int suspend();									/* callback to have all items in list suspend */
	int resume();									/* callback to have all items in list resume */
	// Functions for pausing the engine
	void pollPauseEngine();							/* Poll whether the engine should be paused */
	
	enum {
		Error = -1,
		NotBlocked = 0,
		Blocked = 1
	};
	
	enum PauseState {
		Unpaused = 0,
		PauseEvent,
		UnpauseEvent,
		Pausing,
		Paused
	};
	
 private:
	friend class Common::Singleton<PowerManager>;
	PowerManager();
	~PowerManager();

	Common::List<Suspendable *> _suspendList;		/* list to register in */

	volatile bool _pauseFlag;						/* For pausing, which is before suspending */
	volatile bool _pauseFlagOld;					/* Save the last state of the flag while polling */
	volatile int _pauseClientState;					/* Pause state of the target */

	volatile bool _suspendFlag;						/* protected variable */
	SDL_mutex *_flagMutex;							/* mutex to access access flag */
	SDL_mutex *_listMutex;							/* mutex to access Suspendable list */
	SDL_cond *_condSuspendable;						/* signal to synchronize accessing threads */
	SDL_cond *_condPM;								/* signal to wake up the PM from a critical section */
	volatile int _criticalCounter;					/* Counter of how many threads are in a critical section */
	int _error;										/* error code - PM can't talk to us. For debugging */
	
	// States for PM to be in (used for debugging)
	enum PMState {
		kInitDone = 1 , 
		kDestroyPM,
		kWaitForClientPause,
		kWaitForClientToFinishPausing,
		kGettingFlagMutexSuspend,
		kGotFlagMutexSuspend,
		kWaitCritSectionSuspend,
		kDoneWaitingCritSectionSuspend,
		kGettingListMutexSuspend,
		kIteratingListSuspend,
		kDoneIteratingListSuspend,
		kDoneSuspend,
		kGettingListMutexResume,
		kIteratingListResume,
		kDoneIteratingListResume,
		kGettingFlagMutexResume,
		kGotFlagMutexResume,
		kSignalSuspendedThreadsResume,
		kDoneSignallingSuspendedThreadsResume,
		kDoneResume
	};	
#ifdef __PSP_DEBUG_SUSPEND__

	volatile int _listCounter;						/* How many people are in the list - just for debugging */
	
	void debugPM();									/* print info about the PM */
	void PMStatusSet(PMState s) { _PMStatus = s; }
	volatile int _PMStatus;							/* What the PM is doing */
	
 public:
 	int getPMStatus() { return _PMStatus; }
 
#endif /* __PSP_DEBUG_SUSPEND__ */		
			
 };
 
 // For easy access
#define PowerMan	PowerManager::instance()

 #endif /* POWERMAN_H */
