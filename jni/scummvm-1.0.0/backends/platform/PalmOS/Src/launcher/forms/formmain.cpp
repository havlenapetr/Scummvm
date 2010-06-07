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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/launcher/forms/formmain.cpp $
 * $Id: formmain.cpp 30667 2008-01-28 00:14:17Z jvprat $
 *
 */

#include <PalmOS.h>

#include "StarterRsc.h"
#include "start.h"
#include "skin.h"
#include "games.h"
#include "globals.h"

#include "base/version.h"

#include "formEditGame.h"
#include "formUtil.h"

static UInt16 sknLastOn = skinButtonNone;

static Err BeamMe() {
	UInt16 cardNo;
	LocalID dbID;
	Err err;

	err = SysCurAppDatabase(&cardNo, &dbID);
	if (dbID)
		err = SendDatabase(0, dbID, "ScummVM.prc", "\nPlay your favorite LucasArts games");
	else
		err = DmGetLastErr();

	return err;
}

/***********************************************************************
 *
 * FUNCTION:    MainFormInit
 *
 * DESCRIPTION: This routine initializes the MainForm form.
 *
 * PARAMETERS:  frm - pointer to the MainForm form.
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/
static void MainFormInit()
{
	SknApplySkin();
}

/***********************************************************************
 *
 * FUNCTION:    MainFormDoCommand
 *
 * DESCRIPTION: This routine performs the menu command specified.
 *
 * PARAMETERS:  command  - menu item id
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean MainFormDoCommand(UInt16 command)
{
	Boolean handled = false;

	switch (command) {
		case MainGamesMemoryCard:
			FrmPopupForm(CardSlotForm);
			handled = true;
			break;

		case MainGamesNew:
			gFormEditMode = edtModeAdd;
			FrmPopupForm(GameEditForm);
			handled = true;
			break;

		case MainGamesEdit:
			gFormEditMode = edtModeEdit;
			FrmPopupForm(GameEditForm);
			handled = true;
			break;

		case MainGamesDelete:
			EditGameFormDelete(true);
			handled = true;
			break;

		case MainOptionsBeam:
			BeamMe();
			//if (BeamMe())
				//FrmCustomAlert(FrmErrorAlert,"Unable to beam ScummVM for PalmOS.",0,0);
			handled = true;
			break;

		case MainOptionsAbout:
			FrmPopupForm(InfoForm);
			handled = true;
			break;

		case MainGamesMusicSound:
			FrmPopupForm(MusicForm);
			handled = true;
			break;

		case MainOptionsSkins:
			FrmPopupForm(SkinsForm);
			handled = true;
			break;

		case MainOptionsMisc:
			FrmPopupForm(MiscForm);
			handled = true;
			break;
		}

	MenuEraseStatus(0);
	return handled;
}

/***********************************************************************
 *
 * FUNCTION:    MainFormHandleEvent
 *
 * DESCRIPTION: This routine is the event handler for the
 *              "MainForm" of this application.
 *
 * PARAMETERS:  eventP  - a pointer to an EventType structure
 *
 * RETURNED:    true if the event has handle and should not be passed
 *              to a higher level handler.
 *
 * REVISION HISTORY:
 *
 *
 ***********************************************************************/

static Boolean PenDownRepeat() {
	Coord x,y;
	Boolean penDown, handled = false;
	EvtGetPen(&x, &y, &penDown);

	if (penDown && sknLastOn) {
		RectangleType r;
		DmOpenRef skinDBP;

		skinDBP = SknOpenSkin();
		SknGetObjectBounds(skinDBP, sknLastOn, &r);

		if (RctPtInRectangle(x*2, y*2, &r)) {
			if (SknGetState(skinDBP, sknLastOn) != sknStateSelected) {
				SknSetState(skinDBP, sknLastOn, sknStateSelected);
				SknShowObject(skinDBP, sknLastOn);
			}

			switch (sknLastOn) {
				case skinSliderUpArrow:
				case skinSliderDownArrow:
					handled = SknProcessArrowAction(sknLastOn);
					break;
			}

		} else {
			if (SknGetState(skinDBP, sknLastOn) != sknStateNormal) {
				SknSetState(skinDBP, sknLastOn, sknStateNormal);
				SknShowObject(skinDBP, sknLastOn);
			}
		}

		SknCloseSkin(skinDBP);
	}

	return handled;
}


Boolean MainFormHandleEvent(EventPtr eventP)
{
	Boolean handled = false;
	FormPtr frmP;
	Coord x,y;
	DmOpenRef skinDBP;

	switch (eventP->eType) {
		case frmUpdateEvent:
		{
			RectangleType r;
			UInt16 idx;
			IndexedColorType bgColor = UIColorGetTableEntryIndex(UIFormFill);
			frmP = FrmGetFormPtr(MainForm);

			if (gPrefs->card.volRefNum != vfsInvalidVolRef)
				idx = FrmGetObjectIndex (frmP, MainMSBitMap);
			else
				idx = FrmGetObjectIndex (frmP, MainMSNoneBitMap);

			WinSetDrawMode(winPaint);
			WinSetBackColor(bgColor);
			FrmGetObjectBounds(frmP, idx, &r);
			WinEraseRectangle(&r, 0);
			FrmShowObject(frmP, idx);

			if (eventP->data.frmUpdate.updateCode == frmRedrawUpdateMSImport) {
				GamImportDatabase();
				SknUpdateList();
			}
			handled = true;
			break;
		}
		case menuEvent:
			handled = MainFormDoCommand(eventP->data.menu.itemID);
			break;

		case frmOpenEvent:
			MainFormInit();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case MainCardsButton:
					//gPrefs->card.volRefNum = parseCards(true);
					FrmPopupForm(CardSlotForm);
					break;

				case MainAboutButton:
					FrmPopupForm(InfoForm);
					break;

//				case MainListTypeSelTrigger:
//					FrmList(eventP, MainListTypeList);
//					break;
			}
			handled = true;
			break;

		case penUpEvent:
			x = eventP->screenX;
			y = eventP->screenY;
			lastIndex = dmMaxRecordIndex;	// enable select/unselect list item

			if (sknLastOn != skinButtonNone) {
				RectangleType r;
				skinDBP = SknOpenSkin();
				SknGetObjectBounds(skinDBP, sknLastOn, &r);
				SknSetState(skinDBP, sknLastOn, sknStateNormal);
				SknShowObject(skinDBP, sknLastOn);
				SknCloseSkin(skinDBP);

				if (RctPtInRectangle(x*2, y*2, &r)) {
					switch (sknLastOn) {
						case skinButtonGameAdd:
							gFormEditMode = edtModeAdd;
							FrmPopupForm(GameEditForm);
							handled = true;
							break;

						case skinButtonGameAudio:
							FrmPopupForm(MusicForm);
							handled = true;
							break;

						case skinButtonGameEdit:
						case skinButtonGameParams:
							gFormEditMode = edtModeParams;
							FrmPopupForm(GameEditForm);
							handled = true;
							break;

						case skinButtonGameStart:
							if (gPrefs->card.volRefNum == vfsInvalidVolRef)
								FrmCustomAlert(FrmWarnAlert,"Please select/insert a memory card.", 0, 0);
							else if (GamGetSelected() != dmMaxRecordIndex)
								StartScummVM();
							else
								FrmPopupForm(EngineForm);
							handled = true;
							break;

						case skinButtonGameDelete:
							EditGameFormDelete(true);
							break;
					}
				}
				sknLastOn = skinButtonNone;
			}
			break;

		case penDownEvent:
		case penMoveEvent:
			if (sknLastOn == skinButtonNone) {
				x = eventP->screenX;
				y = eventP->screenY;
				skinDBP = SknOpenSkin();

				switch (sknLastOn = SknCheckClick(skinDBP, x,y)) {
					case skinButtonNone:
						break;
					case skinSliderUpArrow:
					case skinSliderDownArrow:
					case skinButtonGameAdd:
					case skinButtonGameEdit:
					case skinButtonGameParams:
					case skinButtonGameStart:
					case skinButtonGameDelete:
					case skinButtonGameAudio:
						SknSetState(skinDBP, sknLastOn, sknStateSelected);
						SknShowObject(skinDBP, sknLastOn);
						if (gPrefs->soundClick)
							SndPlaySystemSound(sndClick);
						handled = true;
						break;
					default:
						FrmCustomAlert(FrmWarnAlert,"Unknown button !",0,0);
				}
				SknCloseSkin(skinDBP);
				SknSelect(x, y);
			}
			break;

		case keyDownEvent:
			if (	(eventP->data.keyDown.chr >= 'a' && eventP->data.keyDown.chr <= 'z') ||
					(eventP->data.keyDown.chr >= 'A' && eventP->data.keyDown.chr <= 'Z')) {
				if (GamJumpTo(eventP->data.keyDown.chr)) {
					SknUpdateList();
					handled = true;
				}
			}
			break;

		default:
			handled = PenDownRepeat();
	}

	return handled;
}
