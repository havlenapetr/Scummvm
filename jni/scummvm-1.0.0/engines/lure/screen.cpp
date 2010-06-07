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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/lure/screen.cpp $
 * $Id: screen.cpp 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#include "lure/screen.h"
#include "lure/luredefs.h"
#include "lure/memory.h"
#include "lure/disk.h"
#include "lure/decode.h"
#include "lure/events.h"

namespace Lure {

static Screen *int_disk = NULL;

Screen &Screen::getReference() {
	return *int_disk;
}

Screen::Screen(OSystem &system): _system(system),
		_screen(new Surface(FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT)),
		_disk(Disk::getReference()),
		_palette(new Palette(GAME_PALETTE_RESOURCE_ID, RGB64)) {
	int_disk = this;
	_screen->empty();
	_system.setPalette(_palette->data(), 0, GAME_COLOURS);
}

Screen::~Screen() {
	delete _screen;
	delete _palette;
}

// setPaletteEmpty
// Defaults the palette to an empty set

void Screen::setPaletteEmpty(int numEntries) {
	Palette emptyPalette(numEntries, NULL, RGB64);
	_system.setPalette(emptyPalette.data(), 0, numEntries);
	_palette->copyFrom(&emptyPalette);
/*
	delete _palette;
	_palette = new Palette();

	_system.setPalette(_palette->data(), 0, numEntries);
*/
	_system.updateScreen();
}

// setPalette
// Sets the current palette to the passed palette

void Screen::setPalette(Palette *p) {
	_palette->copyFrom(p);
	_system.setPalette(_palette->data(), 0, GAME_COLOURS);
	_system.updateScreen();
}

// setPalette
// Variation that allows the specification of a subset of a palette passed in to be copied

void Screen::setPalette(Palette *p, uint16 start, uint16 num) {
	_palette->palette()->copyFrom(p->palette(), start * 4, start * 4, num * 4);
	_system.setPalette(_palette->data(), start, num);
	_system.updateScreen();
}

// paletteFadeIn
// Fades in the palette. For proper operation, the palette should have been
// previously set to empty

void Screen::paletteFadeIn(Palette *p) {
	assert(p->numEntries() <= _palette->numEntries());
	Events &events = Events::getReference();
	bool changed;

	do {
		changed = false;
		byte *pFinal = p->data();
		byte *pCurrent = _palette->data();

		for (int palCtr = 0; palCtr < p->numEntries() * PALETTE_FADE_INC_SIZE; ++palCtr, ++pCurrent, ++pFinal) {
			if (palCtr % PALETTE_FADE_INC_SIZE == (PALETTE_FADE_INC_SIZE - 1)) continue;
			bool isDifferent = *pCurrent < *pFinal;

			if (isDifferent) {
				if ((*pFinal - *pCurrent) < PALETTE_FADE_INC_SIZE)
					*pCurrent = *pFinal;
				else
					*pCurrent += PALETTE_FADE_INC_SIZE;
				changed = true;
			}
		}

		if (changed) {
			_system.setPalette(_palette->data(), 0, p->numEntries());
			_system.updateScreen();
			_system.delayMillis(20);
			while (events.pollEvent())
				;
		}
	} while (changed);
}

// paletteFadeOut
// Fades the screen to black by gradually decreasing the palette colours

void Screen::paletteFadeOut(int numEntries) {
	assert((uint32)numEntries <= _palette->palette()->size());
	Events &events = Events::getReference();
	bool changed;

	do {
		byte *pTemp = _palette->data();
		changed = false;

		for (uint32 palCtr = 0; palCtr < (uint32)(numEntries * PALETTE_FADE_INC_SIZE); ++palCtr, ++pTemp) {
			if (palCtr % PALETTE_FADE_INC_SIZE == (PALETTE_FADE_INC_SIZE - 1))
				continue;
			bool isDifferent = *pTemp > 0;
			if (isDifferent) {
				if (*pTemp < PALETTE_FADE_INC_SIZE) *pTemp = 0;
				else *pTemp -= PALETTE_FADE_INC_SIZE;
				changed = true;
			}
		}

		if (changed) {
			_system.setPalette(_palette->data(), 0, numEntries);
			_system.updateScreen();
			_system.delayMillis(20);
			while (events.pollEvent())
				;
		}
	} while (changed);
}

void Screen::resetPalette() {
	Palette p(GAME_PALETTE_RESOURCE_ID);
	setPalette(&p);
}

void Screen::empty() {
	_screen->empty();
	update();
}

void Screen::update() {
	_system.copyRectToScreen(screen_raw(), FULL_SCREEN_WIDTH, 0, 0, FULL_SCREEN_WIDTH, FULL_SCREEN_HEIGHT);
	_system.updateScreen();
}

void Screen::updateArea(uint16 x, uint16 y, uint16 w, uint16 h) {
	_system.copyRectToScreen(screen_raw(), FULL_SCREEN_WIDTH, x, y, w, h);
	_system.updateScreen();
}

} // end of namespace Lure
