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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/PalmOS/Src/launcher/forms/formCards.cpp $
 * $Id: formCards.cpp 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#include <PalmOS.h>
#include <VFSMgr.h>
#include <PmPalmOSNVFS.h>

#include "start.h"
#include "formTabs.h"
#include "forms.h"
#include "globals.h"

typedef struct {
	UInt16 volRefNum;
	Char nameP[expCardInfoStringMaxLen+1];

} CardInfoType;

static TabType *myTabP;
static UInt16 lastTab = 0;

static void CardSlotFormExit(Boolean bSave);

static void CardSlotFreeList() {
	MemHandle cards = NULL;
	MemHandle items = NULL;

	if (itemsText && itemsList) {
		cards = MemPtrRecoverHandle(itemsList);
		items = MemPtrRecoverHandle(itemsText);

		itemsText = NULL;
		itemsList = NULL;
	}
	itemsType = ITEM_TYPE_UNKNOWN;

	if (items && cards) {
		MemHandleUnlock(items);
		MemHandleUnlock(cards);
		MemHandleFree(items);
		MemHandleFree(cards);
	}
}

static UInt16 CardSlotFillList(Boolean getRefNum = false) {
	Err err;
	UInt16 index;
	UInt16 volRefNum;
	UInt32 volIterator = vfsIteratorStart|vfsIncludePrivateVolumes;
	UInt8 counter = 0;
	UInt32 other = 1;

	MemHandle items = NULL;
	MemHandle cards = NULL;
	CardInfoType *cardsInfo;

	// retreive card infos
	while (volIterator != vfsIteratorStop) {
		err = VFSVolumeEnumerate(&volRefNum, &volIterator);

		if (!err) {
			Char labelP[expCardInfoStringMaxLen+1];
			MemSet(labelP, expCardInfoStringMaxLen+1, 0);
			err = VFSVolumeGetLabel(volRefNum, labelP, expCardInfoStringMaxLen+1);

			if (err || StrLen(labelP) == 0) {	// if no label try to retreive card type
				VolumeInfoType volInfo;
				err = VFSVolumeInfo(volRefNum, &volInfo);

				if (!err) {
					ExpCardInfoType info;
					err = ExpCardInfo(volInfo.slotRefNum, &info);
					StrCopy(labelP, info.deviceClassStr);
				}

				if (err)	// if err default name
					StrPrintF(labelP,"Other Card %ld", other++);
			}

			if (!cards)
				cards = MemHandleNew(sizeof(CardInfoType));
			else
				MemHandleResize(cards, MemHandleSize(cards) + sizeof(CardInfoType));

			cardsInfo = (CardInfoType *)MemHandleLock(cards);
			cardsInfo[counter].volRefNum = volRefNum;
			StrCopy(cardsInfo[counter].nameP, labelP);
			MemHandleUnlock(cards);
			counter++;
		}
	}

	if (counter > 0) {
		// set the list items ...
		if (!getRefNum) {
			for (index = 0; index < counter; index++) {
				if (!items)
					items = MemHandleNew(sizeof(Char *));
				else
					MemHandleResize(items, MemHandleSize(items) + sizeof(Char *));

				itemsText = (Char **)MemHandleLock(items);
				itemsText[index] = cardsInfo[index].nameP;
				MemHandleUnlock(items);
			}

			// save globals
			itemsText = (Char **)MemHandleLock(items);
			itemsList = (void *)MemHandleLock(cards);
			itemsType = ITEM_TYPE_CARD;

		// ... or just return a default volRefNum
		} else {
			UInt16 volRefNum;

			cardsInfo = (CardInfoType *)MemHandleLock(cards);
			volRefNum =  cardsInfo[0].volRefNum;	// return the first volref
			MemHandleUnlock(cards);
			MemHandleFree(cards);

			return volRefNum;
		}

	// no card found ? free old list in any or return invalid volref
	} else {
		if (!getRefNum)
			CardSlotFreeList();
		else
			return vfsInvalidVolRef;
	}

	return counter;
}

static void ConfigTabInit(Boolean update = false) {
	ListPtr listP;

	UInt16 index;
	Int16 selected = -1;

	UInt16 counter = CardSlotFillList();
	listP = (ListType *)GetObjectPtr(TabCardConfigSlotList);

	// itemsText can be NULL if counter = 0
	LstSetListChoices (listP, itemsText, counter);
	if (counter > 0) {
		CardInfoType *cardsInfo = (CardInfoType *)itemsList;

		for (index = 0; index < counter; index++) {
			if (cardsInfo[index].volRefNum == gPrefs->card.volRefNum) {
				selected = index;
				break;
			}
		}

		LstSetSelection(listP, selected);
	}

	if (!update) {
		FieldType *fld1P;
		Char *cacheP;
		MemHandle cacheH;

		fld1P = (FieldType *)GetObjectPtr(TabCardConfigCacheSizeField);
		cacheH = MemHandleNew(FldGetMaxChars(fld1P)+1);
		cacheP = (Char *)MemHandleLock(cacheH);
		StrIToA(cacheP, gPrefs->card.cacheSize / 1024);
		MemHandleUnlock(cacheH);

		FldSetTextHandle(fld1P, cacheH);
		CtlSetValue((ControlType *)GetObjectPtr(TabCardConfigCacheCheckbox), gPrefs->card.useCache);
		CtlSetValue((ControlType *)GetObjectPtr(TabCardConfigLedCheckbox), gPrefs->card.showLED);
		CtlSetValue((ControlType *)GetObjectPtr(TabCardConfigDetectCheckbox), gPrefs->card.autoDetect);
	// update ? redraw the list
	} else {
		WinScreenLock(winLockCopy);
		LstDrawList(listP);
		WinScreenUnlock();
	}
}

static UInt16 ConfigTabSave() {
	ControlType *cckP[3];
	FieldType *fld1P;
	ListPtr listP;
	FormPtr frmP;
	UInt16 updateCode = frmRedrawUpdateMS;

	cckP[0] = (ControlType *)GetObjectPtr(TabCardConfigCacheCheckbox);
	cckP[1] = (ControlType *)GetObjectPtr(TabCardConfigLedCheckbox);
	cckP[2] = (ControlType *)GetObjectPtr(TabCardConfigDetectCheckbox);

	gPrefs->card.useCache = CtlGetValue(cckP[0]);
	gPrefs->card.showLED = CtlGetValue(cckP[1]);
	gPrefs->card.autoDetect = CtlGetValue(cckP[2]);

	fld1P = (FieldType *)GetObjectPtr(TabCardConfigCacheSizeField);
	frmP = FrmGetActiveForm();
	if (FldGetTextLength(fld1P) == 0 && CtlGetValue(cckP[0]) == 1) {
		TabSetActive(frmP, myTabP, 0);
		FrmSetFocus(frmP, FrmGetObjectIndex(frmP, TabCardConfigCacheSizeField));
		FrmCustomAlert(FrmWarnAlert,"You must specified a cache size.",0,0);
		return 0;
	}
	gPrefs->card.cacheSize = StrAToI(FldGetTextPtr(fld1P)) * 1024;

	Int16 selected;
	CardInfoType *cardsInfo = (CardInfoType *)itemsList;

	listP = (ListType *)GetObjectPtr(TabCardConfigSlotList);
	selected = LstGetSelection(listP);
	if (selected == -1) {
		gPrefs->card.volRefNum = vfsInvalidVolRef;
	} else if (gPrefs->card.volRefNum != cardsInfo[selected].volRefNum) {
		updateCode = frmRedrawUpdateMSImport;
		gPrefs->card.volRefNum = cardsInfo[selected].volRefNum;
	}

	CardSlotCreateDirs();
	CardSlotFreeList();

	return updateCode;
}

static void GameListTabInit() {
	CtlSetValue((ControlType *)GetObjectPtr(TabCardGameListMoveCheckbox), gPrefs->card.moveDB);
	CtlSetValue((ControlType *)GetObjectPtr(TabCardGameListDeleteCheckbox), gPrefs->card.deleteDB);
	CtlSetValue((ControlType *)GetObjectPtr(TabCardGameListConfirmCheckbox), gPrefs->card.confirmMoveDB);
}

static void GameListTabSave() {
	ControlType *cckP[3];

	cckP[0] = (ControlType *)GetObjectPtr(TabCardGameListMoveCheckbox);
	cckP[1] = (ControlType *)GetObjectPtr(TabCardGameListDeleteCheckbox);
	cckP[2] = (ControlType *)GetObjectPtr(TabCardGameListConfirmCheckbox);

	gPrefs->card.moveDB = CtlGetValue(cckP[0]);
	gPrefs->card.deleteDB = CtlGetValue(cckP[1]);
	gPrefs->card.confirmMoveDB = CtlGetValue(cckP[2]);
}

static void GameListTabDraw() {
	ControlType *cck1P;
	FormPtr frmP = FrmGetActiveForm();

	cck1P = (ControlType *)GetObjectPtr(TabCardGameListMoveCheckbox);
	if (CtlGetValue(cck1P)) {
		FrmShowObject(frmP, FrmGetObjectIndex (frmP, TabCardGameListDeleteCheckbox));
		FrmShowObject(frmP, FrmGetObjectIndex (frmP, TabCardGameListConfirmCheckbox));
	} else {
		FrmHideObject(frmP, FrmGetObjectIndex (frmP, TabCardGameListDeleteCheckbox));
		FrmHideObject(frmP, FrmGetObjectIndex (frmP, TabCardGameListConfirmCheckbox));
	}
}

static void CardSlotFormInit() {
	TabType *tabP;
	FormType *frmP = FrmGetActiveForm();

	tabP = TabNewTabs(2);
	TabAddContent(&frmP, tabP, "Cards", TabCardConfigForm);
	TabAddContent(&frmP, tabP, "Game List", TabCardGameListForm, GameListTabDraw);

	ConfigTabInit();
	GameListTabInit();

	FrmDrawForm(frmP);
	TabSetActive(frmP, tabP, lastTab);

	myTabP = tabP;
}

static void CardSlotFormSave() {
	UInt16 updateCode;
	updateCode = ConfigTabSave();
	if (!updateCode) return;
	GameListTabSave();
	CardSlotCreateDirs();

	TabDeleteTabs(myTabP);
	FrmReturnToMain(updateCode);
}

static void CardSlotFormCancel() {
	CardSlotFreeList();
	TabDeleteTabs(myTabP);
	FrmReturnToMain();
}

Boolean CardSlotFormHandleEvent(EventPtr eventP) {
	FormPtr frmP = FrmGetActiveForm();
	Boolean handled = false;

	switch (eventP->eType) {
		case frmOpenEvent:
			CardSlotFormInit();
			handled = true;
			break;

		case frmCloseEvent:
			CardSlotFormCancel();
			handled = true;
			break;

		case ctlSelectEvent:
			switch (eventP->data.ctlSelect.controlID)
			{
				case (CardSlotForm + 1) :
				case (CardSlotForm + 2) :
					lastTab = (eventP->data.ctlSelect.controlID - CardSlotForm - 1);
					TabSetActive(frmP, myTabP, lastTab);
					break;

				case CardSlotOkButton:
					CardSlotFormSave();
					break;

				case CardSlotCancelButton:
					CardSlotFormCancel();
					break;

				case TabCardGameListMoveCheckbox:
					GameListTabDraw();
					break;
			}
			handled = true;
			break;

		default:
			break;
	}

	return handled;
}

void CardSlotCreateDirs() {
	if (gPrefs->card.volRefNum != vfsInvalidVolRef) {
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Games");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Saved");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Audio");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Mods");
		VFSDirCreate(gPrefs->card.volRefNum, "/PALM/Programs/ScummVM/Themes");
	}
}

void CardSlotFormUpdate() {
	if (itemsType == ITEM_TYPE_CARD) {
		CardSlotFreeList();
		ConfigTabInit(true);
	}
}

UInt16 parseCards() {
	UInt16 volRefNum = CardSlotFillList(true);
	CardSlotFreeList();
	return volRefNum;
}
