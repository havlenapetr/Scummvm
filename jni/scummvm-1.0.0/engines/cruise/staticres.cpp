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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/cruise/staticres.cpp $
 * $Id: staticres.cpp 44864 2009-10-10 05:33:45Z dreammaster $
 *
 */

#include "cruise/staticres.h"
#include "cruise/cruise.h"
#include "common/util.h"

namespace Cruise {

const int actor_move[][13] = {
	{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0},						// back
	{13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 0},			// right side
	{25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 0},			// front
	{ -13, -14, -15, -16, -17, -18, -19, -20, -21, -22, -23, -24, 0}// left side
};

const int actor_end[][13] = {
	{37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// stat back
	{38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// stat right-side
	{39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	// stat front
	{ -38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}	// stat left-side
};

const int actor_stat[][13] = {
	{53, 54, 55, 56, 57, 0, 0, 0, 0, 0, 0, 0, 0},
	{59, 60, 62, 63, 78, 0, 0, 0, 0, 0, 0, 0, 0},
	{ -78, -63, -62, -60, -59, 0, 0, 0, 0, 0, 0, 0, 0},
	{ -57, -56, -55, -54, -53, 0, 0, 0, 0, 0, 0, 0, 0}
};

const int actor_invstat[][13] = {
	{ -53, -54, -55, -56, -57, 0, 0, 0, 0, 0, 0, 0, 0},
	{57, 56, 55, 54, 53, 0, 0, 0, 0, 0, 0, 0, 0},
	{78, 63, 62, 60, 59, 0, 0, 0, 0, 0, 0, 0, 0},
	{ -59, -60, -62, -63, -78, 0, 0, 0, 0, 0, 0, 0, 0}
};

// font character lookup tables

const int16 english_fontCharacterTable[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
	44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
	58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
	86, 87, 88, 89, 90, 91, 92,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1,
	106, 105,
	-1, -1, -1, -1,
	107, 108,
	-1, -1,
	109, 110,
	-1, -1, -1, -1, -1, -1,
	111, -1,
	112,
	-1, -1,
	113,
	114,
	-1,
	-1,
	116, 93,
	-1,
	118,
	-1,
	94,
	-1,
	117,
	115,
	96,
	95,
	97,
	98,
	-1,
	-1,
	99,
	100,
	-1,
	-1,
	-1,
	-1,
	101,
	-1,
	102,
	-1,
	-1,
	103,
	-1,
	104,
	-1,
	-1,
	-1,
	-1,
};

const int16 german_fontCharacterTable[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
	44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
	58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
	86, 87, 88, 89, 90, 91, 92,
	-1, -1, -1,
	0x72,
	-1, -1,
	0x5e,
	-1, -1, -1, -1, -1, -1, -1, -1, -1,
	0x6a,
	-1, -1, -1, -1, -1,
	0x66,
	-1, -1, -1, -1,
	0x70, 0x78,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,
	0x6a, 0x69,
	-1, -1, -1, -1,
	0x6b, 0x6c,
	-1, -1,
	0x6d, 0x6e,
	-1, -1, -1, -1, -1, -1,
	0x6f,
	-1,
	0x70,
	-1, -1,
	0x71, 0x72,
	-1, -1,
	0x74, 0x5D, 0x74, 0x76,
	-1,
	0x5E,
	-1,
	0x75, 0x73, 0x60, 0x5F, 0x61, 0x62,
	-1, -1,
	0x63, 0x64,
	-1, -1, -1, -1
};

const int16 spanish_fontCharacterTable[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
	44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57,
	58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
	72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
	86, 87, 88, 89, 90, 91, 92,
	-1, -1, -1,
	0x72, 0x80
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	0x7f, 0x79, 0x7b, 0x81, 0x82, 0x83,
	-1, -1,
	0x7d,
	-1, -1, -1, -1,
	0x7E,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1,
	0x6A, 0x69,
	-1, -1, -1, -1,
	0x6B, 0x6C,
	-1, -1,
	0x6D, 0x6E,
	-1, -1, -1, -1, -1, -1,
	0x6F,
	-1,
	0x70,
	-1, -1,
	0x71, 0x72,
	-1, -1,
	0x74, 0x5D,
	-1,
	0x76,
	-1,
	0x5E,
	-1,
	0x75, 0x73, 0x60, 0x5F, 0x61, 0x62, 0x79, 0x78, 0x63, 0x64,
	-1, -1,
	0x7B, 0x7A,
	0x65,
	-1,
	0x66,
	-1, -1,
	0x67,
	-1,
	0x68,
	-1, -1, -1
};

//
// Mouse data
//

const byte mouseCursorNormal[] = {
	0x00, 0x00, 0x40, 0x00, 0x60, 0x00, 0x70, 0x00,
	0x78, 0x00, 0x7C, 0x00, 0x7E, 0x00, 0x7F, 0x00,
	0x7F, 0x80, 0x7C, 0x00, 0x6C, 0x00, 0x46, 0x00,
	0x06, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00,

	0xC0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xF8, 0x00,
	0xFC, 0x00, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x80,
	0xFF, 0xC0, 0xFF, 0xC0, 0xFE, 0x00, 0xFF, 0x00,
	0xCF, 0x00, 0x07, 0x80, 0x07, 0x80, 0x03, 0x80
};

const byte mouseCursorDisk[] = {
	0x7F, 0xFC, 0x9F, 0x12, 0x9F, 0x12, 0x9F, 0x12,
	0x9F, 0x12, 0x9F, 0xE2, 0x80, 0x02, 0x9F, 0xF2,
	0xA0, 0x0A, 0xA0, 0x0A, 0xA0, 0x0A, 0xA0, 0x0A,
	0xA0, 0x0A, 0xA0, 0x0A, 0x7F, 0xFC, 0x00, 0x00,

	0x7F, 0xFC, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE,
	0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE,
	0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE,
	0xFF, 0xFE, 0xFF, 0xFE, 0x7F, 0xFC, 0x00, 0x00
};

const byte mouseCursorCross[] = {
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x7C, 0x7C,
	0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80,
	0x03, 0x80, 0x03, 0x80, 0xFF, 0xFE, 0xFE, 0xFE,
	0xFF, 0xFE, 0x03, 0x80, 0x03, 0x80, 0x03, 0x80,
	0x03, 0x80, 0x03, 0x80, 0x03, 0x80, 0x00, 0x00
};

const byte mouseCursorNoMouse[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const byte mouseCursorWalk[] = {
	0x03, 0xC0, 0x03, 0xC0, 0x03, 0xC0, 0x01, 0xE0,
	0x03, 0xF0, 0x03, 0x38, 0x1B, 0xDC, 0x1B, 0xCC,
	0x03, 0xCC, 0x03, 0x80, 0x07, 0x00, 0x0E, 0xC0,
	0x1C, 0xE0, 0x18, 0x70, 0x18, 0x38, 0x18, 0x18,

	0x07, 0xE0, 0x07, 0xE0, 0x07, 0xE0, 0x03, 0xF0,
	0x07, 0xF8, 0x07, 0xFC, 0x3F, 0xFE, 0x3F, 0xFE,
	0x07, 0xFE, 0x07, 0xC0, 0x0F, 0x80, 0x1F, 0xE0,
	0x3F, 0xF0, 0x3C, 0xF8, 0x3C, 0x7C, 0x3C, 0x3C
};

const byte mouseCursorExit[] = {
	0x7f, 0xf8, 0x60, 0x18, 0x60, 0x18, 0x60, 0x18,
	0x60, 0x00, 0x60, 0x08, 0x60, 0x0c, 0x60, 0xfe,
	0x60, 0xfe, 0x60, 0x0c, 0x60, 0x08, 0x60, 0x00,
	0x60, 0x18, 0x60, 0x18, 0x60, 0x18, 0x7f, 0xf8,

	0xff, 0xfc, 0xf0, 0x3c, 0xf0, 0x3c, 0xf0, 0x3c,
	0xf0, 0x00, 0xf0, 0x1c, 0xf0, 0x1e, 0xf1, 0xff,
	0xf1, 0xff, 0xf0, 0x1e, 0xf0, 0x1c, 0xf0, 0x00,
	0xf0, 0x3c, 0xf0, 0x3c, 0xf0, 0x3c, 0xff, 0xfc
};

const byte mouseCursorMagnifyingGlass[] = {
	0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x02, 0x02,
	0x02, 0x02, 0x02, 0x02, 0x00, 0x22, 0x00, 0x02,
	0x07, 0x82, 0x03, 0x82, 0x07, 0x9e, 0xfe, 0x80,
	0xfc, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x07, 0xff, 0x07, 0xff, 0x06, 0x03,
	0x06, 0x03, 0x06, 0x03, 0x00, 0x23, 0x0f, 0xc3,
	0x0f, 0xc3, 0x07, 0xc3, 0xff, 0xdf, 0xff, 0xdf,
	0xfe, 0xc0, 0xfc, 0x00, 0xf8, 0x00, 0x00, 0x00
};

const char *englishLanguageStrings[13] = {
	"Pause", NULL, NULL, NULL, NULL, "Inventory", "Speak about...", "Player Menu", NULL,
	"Save", "Load", "Start Again", "Quit"
};
const char *frenchLanguageStrings[13] = {
	"", NULL, NULL, NULL, NULL, "Inventaire", "Parler de...", "Menu Joueur", NULL,
	"Sauvegarde", "Chargement", "Recommencer le jeu", "Quitter"
};
// The original Amiga version does use "Speilermen\xFC" instead of "Spielermen\xFC", if we want to correct
// this typo, we can easily do so.
const char *germanLanguageStrings[13] = {
	" ", NULL, NULL, NULL, NULL, "Inventar", "Sprechen ""\xFC""ber", "Speilermen\xFC", "Speicherlaufwerk",
	"Speichern", "Laden", "Neu beginnen", "Ende"
};

} // End of namespace Cruise
