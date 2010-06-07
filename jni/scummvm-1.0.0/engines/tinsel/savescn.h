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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tinsel/savescn.h $
 * $Id: savescn.h 35453 2008-12-20 23:08:37Z lordhoto $
 *
 * Should really be called "moving actors.h"
 */

#ifndef TINSEL_SAVESCN_H
#define TINSEL_SAVESCN_H

#include <time.h>	// for time_t struct

#include "tinsel/actors.h"	// SAVED_ACTOR
#include "tinsel/dw.h"	// SCNHANDLE
#include "tinsel/rince.h"	// SAVED_MOVER
#include "tinsel/pcode.h"	// INT_CONTEXT
#include "tinsel/play.h"
#include "tinsel/polygons.h"
#include "tinsel/scroll.h"	// SCROLLDATA
#include "tinsel/sysvar.h"

namespace Tinsel {

enum {
	SG_DESC_LEN	= 40,	// Max. saved game description length
	MAX_SAVED_FILES	= 100,

	// FIXME: Save file names in ScummVM can be longer than 8.3, overflowing the
	// name field in savedFiles. Raising it to 256 as a preliminary fix.
	FNAMELEN	= 256 // 8.3
};

struct SFILES {
	char	name[FNAMELEN];
	char	desc[SG_DESC_LEN + 2];
	struct tm dateTime;
};

struct SAVED_DATA {
	SCNHANDLE	SavedSceneHandle;		// Scene handle
	SCNHANDLE	SavedBgroundHandle;		// Background handle
	SAVED_MOVER	SavedMoverInfo[MAX_MOVERS];	// Moving actors
	SAVED_ACTOR	SavedActorInfo[MAX_SAVED_ACTORS];	// } Actors
	int			NumSavedActors;				// }
	int			SavedLoffset, SavedToffset;	// Screen offsets
	INT_CONTEXT	SavedICInfo[MAX_INTERPRET];	// Interpret contexts
	bool		SavedDeadPolys[MAX_POLY];
	bool		SavedControl;
	SCNHANDLE	SavedMidi;			// }
	bool		SavedLoop;			// } Midi
	bool		SavedNoBlocking;
	SCROLLDATA	SavedNoScrollData;

	// Tinsel 2 fields
	Z_POSITIONS	zPositions[NUM_ZPOSITIONS];
	byte		savedActorZ[MAX_SAVED_ACTOR_Z];
	POLY_VOLATILE	SavedPolygonStuff[MAX_POLY];
	uint32		SavedTune[3];			// Music
	bool		bTinselDim;
	int			SavedScrollFocus;
	int			SavedSystemVars[SV_TOPVALID];
	SOUNDREELS	SavedSoundReels[MAX_SOUNDREELS];
};


enum SRSTATE {
	SR_IDLE, SR_DORESTORE, SR_DONERESTORE,
	SR_DOSAVE, SR_DONESAVE,	SR_ABORTED
};

void TinselRestoreScene(bool bFade);
void TinselSaveScene(CORO_PARAM);
void DoRestoreScene(SAVED_DATA *sd, bool bFadeOut);
void DoSaveScene(SAVED_DATA *sd);

bool IsRestoringScene();


enum letype{
	LE_NAME, LE_DESC
};

char *ListEntry(int i, letype which);
int getList(void);
void setNeedLoad(void);

void RestoreGame(int num);
void SaveGame(char *name, char *desc);

void ProcessSRQueue(void);

void RequestSaveGame(char *name, char *desc, SAVED_DATA *sd, int *ssCount, SAVED_DATA *ssData);
void RequestRestoreGame(int num, SAVED_DATA *sd, int *ssCount, SAVED_DATA *ssData);

void InitialiseSaveScenes(void);
void FreeSaveScenes(void);

} // end of namespace Tinsel

#endif	/* TINSEL_SAVESCN_H */
