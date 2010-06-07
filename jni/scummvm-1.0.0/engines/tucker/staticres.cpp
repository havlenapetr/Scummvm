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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/tucker/staticres.cpp $
 * $Id: staticres.cpp 41520 2009-06-14 15:07:34Z cyx $
 *
 */

#include "tucker/tucker.h"
#include "tucker/graphics.h"

namespace Tucker {

const uint8 TuckerEngine::_locationWidthTableGame[85] = {
	1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 4, 2, 1,
	1, 2, 1, 2, 4, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1,
	1, 1, 1, 1, 2, 1, 2, 2, 2, 4, 4, 2, 2, 1, 1, 1, 4, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0
};

const uint8 TuckerEngine::_locationWidthTableDemo[70] = {
	1, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 1, 2, 4, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1,
	1, 1, 1, 1, 2, 1, 2, 2, 2, 4, 4, 2, 2, 1, 1, 0
};

const uint8 TuckerEngine::_sprA02LookupTable[88] = {
	 0,  6,  2,  8,  1,  0,  6,  0,  2,  2,  2,  1,  2,  0,  1,  1,
	 6,  0,  1,  2,  1,  2,  3,  0,  6, 12,  7,  7,  1,  8,  1,  0,
	 3,  0,  4,  5,  0,  0,  3,  3,  2,  7,  7,  0,  4,  1,  5,  2,
	 4,  1,  1,  2,  4,  3,  1,  0,  2,  3,  4,  1,  1,  5,  3,  3,
	 1,  5,  3,  0,  1,  0,  0,  2,  2,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0
};

const uint8 TuckerEngine::_sprC02LookupTable[100] = {
	 0,  0,  6, 20,  3,  3, 15,  5,  9,  6,  7,  8,  8,  6,  3,  6,
	13,  3,  4, 10,  0,  7,  2, 34, 14,  0,  2,  3,  8,  3,  3,  3,
	19, 13,  1,  0,  2,  3,  0,  0,  0,  5,  5, 12,  0,  1,  0,  1,
	 3,  6,  7,  6,  0,  7,  5,  1,  2,  6,  3,  4,  9, 18,  0, 12,
	 0,  2, 10,  0,  0, 19,  0,  2,  2,  1, 22,  0,  0,  0,  0,  3,
	 0,  3,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  1,  0
};

const uint8 TuckerEngine::_sprC02LookupTable2[100] = {
	0, 0, 1, 3, 1, 2, 3, 2, 2, 1, 1, 5, 2, 1, 1, 3, 3, 1,
	1, 4, 0, 1, 1, 4, 4, 0, 2, 1, 3, 3, 3, 2, 4, 4, 1, 0,
	1, 3, 0, 0, 0, 1, 1, 7, 0, 1, 0, 1, 1, 1, 7, 3, 0, 2,
	1, 1, 1, 2, 1, 1, 2, 3, 0, 5, 0, 1, 5, 0, 1, 4, 0, 1,
	1, 1, 1, 0, 0, 0, 0, 1, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0
};

const int TuckerEngine::_staticData3Table[1600] = {
	0x014, 0x014, 0x015, 0x016, 0x017, 0x017, 0x016, 0x015, 0x3E7, 0x0A9, 0x0A9, 0x0AA, 0x0AA, 0x0AB, 0x0AB, 0x0AC,
	0x0AC, 0x3E7, 0x05E, 0x05F, 0x060, 0x061, 0x3E7, 0x0AD, 0x0AE, 0x0AF, 0x0B0, 0x3E7, 0x052, 0x053, 0x054, 0x055,
	0x3E7, 0x056, 0x057, 0x058, 0x059, 0x3E7, 0x05A, 0x05B, 0x05C, 0x05D, 0x3E7, 0x062, 0x063, 0x064, 0x065, 0x3E7,
	0x066, 0x067, 0x068, 0x069, 0x3E7, 0x00C, 0x00D, 0x00E, 0x00F, 0x3E7, 0x01A, 0x01B, 0x01C, 0x01D, 0x3E7, 0x01E,
	0x01F, 0x020, 0x021, 0x3E7, 0x024, 0x025, 0x026, 0x027, 0x3E7, 0x0B1, 0x0B2, 0x0B3, 0x0B4, 0x3E7, 0x0CB, 0x0CC,
	0x0CD, 0x0CE, 0x3E7, 0x0CF, 0x0D0, 0x0D1, 0x0D2, 0x3E7, 0x0D3, 0x0D4, 0x0D5, 0x0D6, 0x3E7, 0x0D7, 0x0D8, 0x0D9,
	0x0DA, 0x3E7, 0x0B5, 0x0B6, 0x0B7, 0x0B8, 0x3E7, 0x04A, 0x04B, 0x04C, 0x04D, 0x3E7, 0x04E, 0x04F, 0x050, 0x051,
	0x3E7, 0x02A, 0x02B, 0x02C, 0x02D, 0x3E7, 0x02E, 0x02F, 0x030, 0x031, 0x3E6, 0x0E6, 0x001, 0x3E7, 0x0B9, 0x0BA,
	0x0BB, 0x0BC, 0x3E7, 0x06A, 0x06B, 0x06C, 0x06D, 0x3E7, 0x032, 0x033, 0x034, 0x035, 0x3E7, 0x036, 0x037, 0x038,
	0x039, 0x3E7, 0x03A, 0x03B, 0x03C, 0x03D, 0x3E7, 0x03E, 0x03F, 0x040, 0x041, 0x3E7, 0x042, 0x043, 0x044, 0x045,
	0x3E7, 0x046, 0x047, 0x048, 0x049, 0x3E7, 0x06E, 0x06F, 0x070, 0x071, 0x3E6, 0x045, 0x000, 0x3E7, 0x072, 0x073,
	0x074, 0x075, 0x3E7, 0x076, 0x077, 0x078, 0x079, 0x3E7, 0x07A, 0x07B, 0x07C, 0x07D, 0x3E7, 0x07F, 0x080, 0x081,
	0x082, 0x3E7, 0x085, 0x086, 0x087, 0x088, 0x3E7, 0x089, 0x08A, 0x08B, 0x08C, 0x3E7, 0x08F, 0x090, 0x091, 0x092,
	0x3E7, 0x0BD, 0x0BE, 0x0BF, 0x0C0, 0x3E7, 0x0C1, 0x0C2, 0x0C3, 0x0C4, 0x3E7, 0x0C5, 0x0C6, 0x0C7, 0x0C8, 0x3E7,
	0x093, 0x094, 0x095, 0x096, 0x3E7, 0x099, 0x099, 0x099, 0x09A, 0x09A, 0x09A, 0x09B, 0x09B, 0x09B, 0x09C, 0x09C,
	0x09C, 0x3E7, 0x09D, 0x09D, 0x09E, 0x09E, 0x09F, 0x09F, 0x0A0, 0x0A0, 0x3E7, 0x0A1, 0x0A1, 0x0A2, 0x0A2, 0x0A3,
	0x0A3, 0x0A4, 0x0A4, 0x3E7, 0x0A5, 0x0A5, 0x0A6, 0x0A6, 0x0A7, 0x0A7, 0x0A8, 0x0A8, 0x3E7, 0x0DE, 0x0DF, 0x0E0,
	0x0E1, 0x3E7, 0x010, 0x011, 0x012, 0x013, 0x3E7, 0x0E3, 0x0E4, 0x0E5, 0x0E6, 0x3E6, 0x03F, 0x000, 0x3E7, 0x000,
	0x001, 0x002, 0x003, 0x3E7, 0x004, 0x005, 0x006, 0x007, 0x3E7, 0x008, 0x009, 0x00A, 0x00B, 0x3E7, 0x00C, 0x00D,
	0x00E, 0x00F, 0x3E7, 0x010, 0x011, 0x012, 0x013, 0x3E7, 0x014, 0x015, 0x016, 0x017, 0x3E7, 0x018, 0x019, 0x01A,
	0x01B, 0x3E7, 0x01D, 0x01E, 0x01F, 0x020, 0x3E7, 0x021, 0x022, 0x023, 0x024, 0x3E7, 0x025, 0x026, 0x027, 0x028,
	0x3E7, 0x029, 0x02A, 0x02B, 0x02C, 0x3E7, 0x02D, 0x02E, 0x02F, 0x030, 0x3E7, 0x031, 0x032, 0x033, 0x034, 0x3E7,
	0x035, 0x036, 0x037, 0x038, 0x3E7, 0x039, 0x03A, 0x03B, 0x03C, 0x3E7, 0x03D, 0x03E, 0x03F, 0x040, 0x3E7, 0x041,
	0x042, 0x043, 0x044, 0x3E7, 0x049, 0x04A, 0x04B, 0x04C, 0x3E7, 0x04D, 0x04E, 0x04F, 0x050, 0x3E7, 0x051, 0x052,
	0x053, 0x054, 0x3E7, 0x055, 0x056, 0x057, 0x058, 0x3E7, 0x059, 0x05A, 0x05B, 0x05C, 0x3E7, 0x05D, 0x05E, 0x05F,
	0x060, 0x3E7, 0x061, 0x062, 0x063, 0x064, 0x3E7, 0x068, 0x069, 0x06A, 0x06B, 0x3E7, 0x06C, 0x06D, 0x06E, 0x06F,
	0x3E7, 0x070, 0x071, 0x072, 0x073, 0x3E7, 0x074, 0x075, 0x076, 0x077, 0x3E7, 0x07A, 0x07B, 0x07C, 0x07D, 0x3E7,
	0x07E, 0x07F, 0x080, 0x081, 0x3E7, 0x082, 0x083, 0x084, 0x085, 0x3E7, 0x086, 0x087, 0x088, 0x089, 0x3E7, 0x08A,
	0x08B, 0x08C, 0x08D, 0x3E7, 0x08E, 0x08E, 0x08F, 0x08F, 0x090, 0x090, 0x091, 0x091, 0x3E7, 0x000, 0x001, 0x002,
	0x003, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x3E7,
	0x004, 0x005, 0x006, 0x007, 0x3E7, 0x008, 0x009, 0x00A, 0x00B, 0x3E7, 0x00C, 0x00D, 0x00E, 0x00F, 0x3E7, 0x010,
	0x011, 0x012, 0x013, 0x3E7, 0x014, 0x015, 0x016, 0x017, 0x3E7, 0x018, 0x019, 0x01A, 0x01B, 0x01C, 0x01D, 0x01E,
	0x01F, 0x020, 0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027, 0x028, 0x029, 0x02A, 0x02B, 0x02C, 0x02D, 0x02E,
	0x02F, 0x030, 0x031, 0x032, 0x3E7, 0x033, 0x034, 0x035, 0x3E7, 0x036, 0x037, 0x038, 0x3E7, 0x039, 0x03A, 0x03B,
	0x3E6, 0x091, 0x003, 0x3E7, 0x03B, 0x03A, 0x039, 0x3E6, 0x091, 0x000, 0x3E7, 0x03C, 0x03D, 0x03E, 0x03F, 0x040,
	0x041, 0x042, 0x043, 0x044, 0x3E6, 0x06E, 0x000, 0x3E7, 0x045, 0x046, 0x047, 0x048, 0x049, 0x04A, 0x04B, 0x04C,
	0x04D, 0x04E, 0x3E6, 0x068, 0x002, 0x3E7, 0x04F, 0x050, 0x051, 0x052, 0x3E7, 0x052, 0x051, 0x050, 0x04F, 0x3E7,
	0x053, 0x054, 0x055, 0x056, 0x3E7, 0x057, 0x058, 0x059, 0x05A, 0x05B, 0x05C, 0x05D, 0x05E, 0x05F, 0x060, 0x061,
	0x062, 0x063, 0x064, 0x065, 0x3E7, 0x066, 0x067, 0x068, 0x069, 0x06A, 0x06B, 0x06C, 0x06D, 0x06E, 0x06F, 0x070,
	0x071, 0x072, 0x073, 0x074, 0x075, 0x076, 0x3E7, 0x077, 0x078, 0x079, 0x07A, 0x07B, 0x07C, 0x07D, 0x3E6, 0x069,
	0x001, 0x3E7, 0x07D, 0x07C, 0x07B, 0x07A, 0x079, 0x078, 0x077, 0x3E6, 0x069, 0x000, 0x3E7, 0x07E, 0x07F, 0x080,
	0x081, 0x082, 0x083, 0x084, 0x3E6, 0x06A, 0x001, 0x3E7, 0x084, 0x083, 0x082, 0x081, 0x080, 0x07F, 0x07E, 0x3E6,
	0x06A, 0x000, 0x3E7, 0x085, 0x086, 0x087, 0x088, 0x089, 0x08A, 0x08B, 0x08C, 0x08D, 0x08E, 0x08F, 0x090, 0x091,
	0x092, 0x093, 0x094, 0x095, 0x096, 0x097, 0x098, 0x099, 0x09A, 0x09B, 0x09C, 0x09D, 0x09E, 0x09F, 0x0A0, 0x0A1,
	0x0A2, 0x0A3, 0x0A4, 0x0A5, 0x0A6, 0x0A7, 0x0A8, 0x0A9, 0x3E7, 0x0AA, 0x0AB, 0x0AC, 0x0AD, 0x0AE, 0x0AF, 0x0B0,
	0x0B1, 0x0B2, 0x0B3, 0x3E6, 0x06B, 0x000, 0x3E7, 0x0B3, 0x3E7, 0x0B4, 0x0B4, 0x0B5, 0x0B5, 0x0B6, 0x0B6, 0x0B7,
	0x0B7, 0x3E7, 0x0B8, 0x0B9, 0x0B9, 0x0BB, 0x0BC, 0x0BD, 0x0BE, 0x0BF, 0x3E7, 0x0C0, 0x0C1, 0x0C2, 0x0C3, 0x0C4,
	0x0C5, 0x0C6, 0x0C7, 0x0C8, 0x0C9, 0x0CA, 0x0CB, 0x0CC, 0x0CD, 0x0CE, 0x0CF, 0x0D0, 0x0D1, 0x0D2, 0x0D3, 0x0D4,
	0x0D5, 0x0D6, 0x0D7, 0x0D8, 0x3E7, 0x0D9, 0x0DA, 0x0DB, 0x0DC, 0x0DD, 0x0DE, 0x0DF, 0x0E0, 0x0E1, 0x0E2, 0x0E3,
	0x0E4, 0x0E5, 0x0E6, 0x0E7, 0x0E8, 0x0E9, 0x3E7, 0x1C5, 0x0EA, 0x0EB, 0x0EC, 0x0ED, 0x0EE, 0x0EF, 0x0F0, 0x0F1,
	0x0F2, 0x3E7, 0x0F3, 0x0F4, 0x0F5, 0x0F6, 0x0F7, 0x0F8, 0x0F9, 0x0FA, 0x3E6, 0x074, 0x001, 0x3E7, 0x0FB, 0x0FC,
	0x0FD, 0x0FE, 0x3E7, 0x0FF, 0x100, 0x101, 0x102, 0x3E7, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A,
	0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x3E7, 0x116, 0x115, 0x114,
	0x113, 0x112, 0x111, 0x110, 0x10F, 0x10E, 0x3E7, 0x117, 0x118, 0x119, 0x11A, 0x3E7, 0x11B, 0x11C, 0x11D, 0x11E,
	0x3E7, 0x11F, 0x120, 0x121, 0x122, 0x3E7, 0x123, 0x124, 0x125, 0x3E6, 0x091, 0x003, 0x3E7, 0x125, 0x124, 0x123,
	0x3E7, 0x126, 0x127, 0x128, 0x3E7, 0x128, 0x127, 0x126, 0x3E7, 0x129, 0x12A, 0x12B, 0x12C, 0x12D, 0x12E, 0x12F,
	0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x136, 0x137, 0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D, 0x13E, 0x13F,
	0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147, 0x148, 0x149, 0x14A, 0x14B, 0x14C, 0x14D, 0x14E, 0x14F,
	0x150, 0x151, 0x152, 0x3E7, 0x153, 0x154, 0x155, 0x156, 0x157, 0x159, 0x15A, 0x15B, 0x15C, 0x3E7, 0x15E, 0x15F,
	0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167, 0x168, 0x169, 0x16A, 0x16B, 0x16C, 0x16D, 0x16E, 0x16F,
	0x170, 0x171, 0x172, 0x3E7, 0x173, 0x173, 0x173, 0x173, 0x173, 0x173, 0x173, 0x174, 0x174, 0x174, 0x174, 0x174,
	0x174, 0x3E7, 0x175, 0x175, 0x175, 0x175, 0x175, 0x175, 0x176, 0x3E7, 0x177, 0x178, 0x179, 0x17A, 0x17B, 0x17C,
	0x17D, 0x17E, 0x17F, 0x180, 0x181, 0x182, 0x183, 0x184, 0x185, 0x186, 0x187, 0x3E7, 0x188, 0x189, 0x18A, 0x18B,
	0x18C, 0x3E7, 0x198, 0x199, 0x19A, 0x19B, 0x19C, 0x19D, 0x19E, 0x19F, 0x1A0, 0x1A1, 0x1A2, 0x1A3, 0x1A4, 0x1A5,
	0x1A6, 0x1A7, 0x1A8, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1AE, 0x1AF, 0x3E7, 0x1B0, 0x1B1, 0x1B2, 0x3E7, 0x1B3,
	0x1B4, 0x1B5, 0x1B6, 0x1B7, 0x1B8, 0x1B9, 0x1BA, 0x3E7, 0x1BB, 0x1BC, 0x1BD, 0x1BE, 0x1BF, 0x1C0, 0x1C1, 0x1C2,
	0x1C3, 0x1C4, 0x3E6, 0x06F, 0x000, 0x3E7, 0x098, 0x099, 0x09A, 0x3E7, 0x09A, 0x099, 0x098, 0x3E7, 0x09D, 0x09E,
	0x09F, 0x3E7, 0x09F, 0x09E, 0x09D, 0x3E7, 0x0A1, 0x0A2, 0x0A3, 0x3E7, 0x0A6, 0x0A7, 0x0A8, 0x0A9, 0x0AA, 0x0AB,
	0x0AC, 0x0AD, 0x3E6, 0x08E, 0x002, 0x3E7, 0x0AE, 0x0AF, 0x0B0, 0x0B1, 0x0B2, 0x0B3, 0x0B4, 0x0B5, 0x0B6, 0x0B7,
	0x0B8, 0x0B9, 0x0BA, 0x0BB, 0x0BC, 0x0BD, 0x0BE, 0x3E7, 0x0F2, 0x0F2, 0x0F3, 0x0F3, 0x0F4, 0x0F4, 0x3E6, 0x030,
	0x002, 0x3E7, 0x1D3, 0x1D4, 0x1D5, 0x1D6, 0x1D7, 0x1D8, 0x3E7, 0x0F6, 0x0F7, 0x0F8, 0x0F8, 0x3E7, 0x0FB, 0x0FC,
	0x0FD, 0x0FE, 0x0FF, 0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x3E6, 0x07F, 0x002, 0x3E7, 0x106, 0x107, 0x108,
	0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x3E7, 0x1E8, 0x1E7, 0x1E6, 0x1E5, 0x1E4, 0x1E3,
	0x1E2, 0x3E6, 0x095, 0x002, 0x3E7, 0x1A7, 0x1A8, 0x3E7, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1A9, 0x1AA, 0x1AB,
	0x1AC, 0x3E6, 0x09D, 0x003, 0x3E7, 0x1A0, 0x1A1, 0x3E7, 0x0EA, 0x0EB, 0x0EC, 0x0ED, 0x0EE, 0x0EF, 0x0F0, 0x0F1,
	0x0F2, 0x0F3, 0x0F4, 0x0F5, 0x3E7, 0x0F6, 0x0F7, 0x0F8, 0x0F9, 0x0FA, 0x0FB, 0x0FC, 0x0FD, 0x0FE, 0x0FF, 0x100,
	0x101, 0x3E7, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x3E7, 0x10E,
	0x10F, 0x110, 0x111, 0x3E7, 0x112, 0x113, 0x114, 0x115, 0x3E7, 0x116, 0x117, 0x3E7, 0x118, 0x119, 0x11A, 0x11B,
	0x3E7, 0x11C, 0x11D, 0x3E7, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128, 0x129,
	0x3E7, 0x12A, 0x12B, 0x12C, 0x12D, 0x12E, 0x12F, 0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x3E7, 0x136, 0x137,
	0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D, 0x13E, 0x13F, 0x140, 0x141, 0x3E7, 0x106, 0x107, 0x108, 0x109, 0x10A,
	0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x3E7, 0x1A2, 0x1A3, 0x1A4, 0x1A2, 0x1A3, 0x1A4, 0x1A2, 0x1A3,
	0x1A4, 0x1A2, 0x1A3, 0x1A4, 0x3E6, 0x09D, 0x005, 0x3E7, 0x3E7, 0x3E7, 0x3E7, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000
};

uint8 TuckerEngine::_locationHeightTableGame[80] = {
	0x00, 0x1C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3C, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

uint8 TuckerEngine::_locationHeightTableDemo[70] = {
	0x00, 0x1C, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x3C, 0x00, 0x3C, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int TuckerEngine::_objectKeysPosXTable[80] = {
	0x000, 0x0A0, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x12B, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x140, 0x000, 0x000, 0x000, 0x000, 0x09E, 0x060, 0x0C0, 0x040, 0x0A0, 0x12C, 0x068, 0x098,
	0x08E, 0x09A, 0x0A0, 0x098, 0x092, 0x096, 0x09A, 0x09A, 0x08C, 0x000, 0x000, 0x000, 0x000, 0x000, 0x0A0, 0x000,
	0x086, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x0A0, 0x185, 0x000, 0x0A0, 0x140, 0x140, 0x000,
	0x000, 0x124, 0x140, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
};

int TuckerEngine::_objectKeysPosYTable[80] = {
	0x000, 0x06B, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x080, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x075, 0x000, 0x000, 0x000, 0x000, 0x086, 0x02B, 0x079, 0x07C, 0x07C, 0x07B, 0x073, 0x07B,
	0x06C, 0x08A, 0x086, 0x086, 0x086, 0x086, 0x083, 0x083, 0x07B, 0x000, 0x000, 0x000, 0x000, 0x000, 0x078, 0x000,
	0x082, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x089, 0x08A, 0x000, 0x088, 0x082, 0x076, 0x000,
	0x000, 0x07F, 0x083, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
};

int TuckerEngine::_objectKeysLocationTable[80] = {
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x001, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x001, 0x000, 0x000, 0x000, 0x000, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001,
	0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x001, 0x000, 0x000, 0x000, 0x000, 0x000, 0x001, 0x000,
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x001, 0x000, 0x000, 0x001, 0x001, 0x001, 0x000,
	0x000, 0x001, 0x001, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
};

int TuckerEngine::_mapSequenceFlagsLocationTable[70] = {
	0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 1, 0, 0,
};

const uint8 TuckerEngine::_charWidthCharset1[224] = {
	0x06, 0x06, 0x04, 0x06, 0x07, 0x08, 0x08, 0x02, 0x04, 0x04, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0x07, 0x07, 0x08, 0x04, 0x04, 0x04, 0x04, 0x07,
	0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x05, 0x07, 0x07, 0x03, 0x05, 0x07, 0x04, 0x08, 0x07, 0x07,
	0x07, 0x08, 0x07, 0x07, 0x04, 0x07, 0x07, 0x08, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x06, 0x07, 0x04, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08,
	0x08, 0x04, 0x08, 0x08, 0x07, 0x07, 0x06, 0x05, 0x07, 0x08, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07
};

const uint8 TuckerEngine::_charWidthCharset2[58] = {
	0x13, 0x0F, 0x10, 0x10, 0x10, 0x0E, 0x11, 0x10, 0x0D, 0x0A, 0x11, 0x0D, 0x14, 0x13, 0x13, 0x11,
	0x13, 0x12, 0x10, 0x11, 0x13, 0x14, 0x14, 0x10, 0x13, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x13, 0x0F, 0x10, 0x10, 0x10, 0x0E, 0x11, 0x10, 0x0D, 0x0A, 0x11, 0x0D, 0x14, 0x13, 0x13, 0x11,
	0x13, 0x12, 0x10, 0x11, 0x13, 0x14, 0x14, 0x10, 0x13, 0x10,
};

static const SoundSequenceData _soundDataSeq3_4[] = {
	{   16,  3,  33, 100, 1 }, {   18,  5,  34, 100, 1 }, {   26,  9,  16, 100, 5 }, {   74,  9,  35, 100, 0 },
	{  118, 10,  36, 100, 0 }, {  139,  9,  35, 100, 0 }, {  140, 10,   0, 100, 2 }, {  144,  5,   0, 100, 2 },
	{  146,  3,   0, 100, 2 }, {  147,  9,   0, 100, 2 }, {  148,  2,  30, 100, 1 }, {  156,  0, 103,  50, 1 },
	{  158,  4,  31, 100, 5 }, {  188,  4,  31, 100, 5 }, {  212,  2,   0, 100, 2 }, {  223,  1,  20, 100, 0 },
	{  230,  1,   0, 100, 2 }, {  234,  1,  20, 100, 0 }, {  251,  1,  20, 100, 0 }, {  265, 11,  69,  80, 6 },
	{  293,  5,  34,  80, 1 }, {  295, 12,  76,  80, 6 }, {  343, 13,  87, 110, 6 }, {  373, 14,  97,  80, 6 },
	{  429,  5,   0, 100, 2 }, {  431, 15, 100,  80, 6 }, {  456,  2,  30, 100, 1 }, {  457,  1,  19, 100, 5 },
	{  463,  0,  18, 100, 5 }, {  482,  2,   0, 100, 2 }, {  483,  8,  40, 100, 1 }, {  494,  8,  37, 100, 5 },
	{  507,  7,  17, 100, 5 }, {  511,  7,  17, 100, 5 }, {  522, 13,  21, 100, 1 }, {  547,  6,  49, 100, 5 },
	{  548, 11,  48, 100, 0 }, {  556, 12,  38, 100, 0 }, {  557,  2,  23, 100, 5 }, {  599,  7,  39, 100, 1 },
	{  639,  7,   0, 100, 2 }, {  641, 12,  38, 100, 0 }, {  642,  7,  17, 100, 5 }, {  718, 14,   0, 100, 2 },
	{  719,  1,   0, 100, 2 }, {  720,  2,   0, 100, 2 }, {  721,  3,   0, 100, 2 }, {  722,  4,   0, 100, 2 },
	{  723,  5,   0, 100, 2 }, {  724,  6,   0, 100, 2 }, {  725,  7,   0, 100, 2 }, {  726,  8,   0, 100, 2 },
	{  727,  9,   0, 100, 2 }, {  728, 10,   0, 100, 2 }, {  729, 11,   0, 100, 2 }, {  730, 12,   0, 100, 2 },
	{  731,  0,   0, 100, 2 }, {  732, 13,   0, 100, 2 },
};

static const SoundSequenceData _soundDataSeq9_10[] = {
	{    2,  0,   0, 100, 3 }, {    3,  2,  32, 100, 0 }, {    3,  5,  54,  20, 1 }, {    6, 25,  71,  80, 6 },
	{   22,  1,  18, 100, 5 }, {   37, 29,  75,  80, 6 }, {   59, 30,  77,  80, 6 }, {   69, 26,  72,  80, 6 },
	{  124, 27,  73,  80, 6 }, {  163, 28,  74,  80, 6 }, {  197, 31,  78,  80, 6 }, {  224, 32,  79,  80, 6 },
	{  306, 33,  80,  80, 6 }, {  350, 34,  81,  80, 6 }, {  388,  8,  57, 100, 0 }, {  389,  6,  55, 100, 1 },
	{  390,  9,  58, 100, 1 }, {  395, 10,  59, 100, 0 }, {  398,  3,  52, 100, 0 }, {  408,  3,  52, 100, 0 },
	{  413,  4,  53, 100, 0 }, {  418,  3,  52, 100, 0 }, {  426,  6,   0, 100, 2 }, {  427,  9,   0, 100, 2 },
	{  430, 35,  82,  80, 6 }, {  472, 36,  83,  80, 6 }, {  514,  9,  58, 100, 1 }, {  515,  7,  56, 100, 0 },
	{  516,  6,  55, 100, 1 }, {  517,  4,  53, 100, 0 }, {  520, 11,  60, 100, 0 }, {  523,  3,  52, 100, 0 },
	{  531,  7,   0, 100, 2 }, {  532,  9,   0, 100, 2 }, {  533,  6,   0, 100, 2 }, {  534,  3,  52, 100, 0 },
	{  535, 37,  84,  80, 6 }, {  553, 38,  85,  80, 6 }, {  569, 39,  86,  80, 6 }, {  588, 40,  88,  80, 6 },
	{  662, 41,  89,  80, 6 }, {  777, 42,  90,  80, 6 }, {  978,  1,  22, 100, 0 }, { 1008, 12,  61, 100, 0 },
	{ 1011,  4,  15, 100, 5 }, { 1022, 52,  99,  80, 6 }, { 1039,  3,  63, 100, 5 }, { 1045,  3,  63, 100, 5 },
	{ 1051,  3,  63, 100, 5 }, { 1057,  3,  63, 100, 5 }, { 1063,  3,  63, 100, 5 }, { 1066,  3,  63,  40, 5 },
	{ 1074, 43,  91,  80, 6 }, { 1256, 44,  92,  80, 6 }, { 1322, 45,  93,  80, 6 }, { 1373, 46,  94,  80, 6 },
	{ 1419, 47,  95,  80, 6 }, { 1792, 13,  62, 100, 0 }, { 1793,  0,  50, 100, 5 }, { 1895, 51,  98,  80, 6 },
};

static const SoundSequenceData _soundDataSeq21_20[] = {
	{    2,  0,   0, 100, 3 }, {    4,  3,  34,  40, 1 }, {    5,  7,  54,  20, 1 }, {    7,  3,  51, 100, 5 },
	{   62,  3,   0, 100, 2 }, {   63, 48,  96,  80, 6 }, {  198,  7,   0, 100, 2 }, {  202,  5,  46, 100, 1 },
	{  203, 16, 101,  80, 6 }, {  211,  1,  41, 100, 5 }, {  216,  0, 103,  40, 1 }, {  217,  5,   0, 100, 2 },
	{  218,  6,  47, 100, 1 }, {  224,  2,  43, 100, 5 }, {  229, 17, 102,  80, 6 }, {  273,  6,   0, 100, 2 },
	{  274,  4,  40, 100, 1 }, {  275,  4,  44, 100, 5 }, {  335,  1,  21, 100, 1 }, {  336, 13,  42, 100, 0 },
	{  339, 11,  28, 100, 0 }, {  358,  0,  25, 100, 5 }, {  364, 24,  70, 100, 4 }, {  397,  7,  49, 100, 5 },
	{  399,  6,  48, 100, 5 }, {  433,  5,  45, 100, 5 }, {  451, 13,  42, 100, 0 }, {  459,  1,   0, 100, 2 },
	{  508,  4,   0, 100, 2 }, {  509,  1,  21, 100, 1 }, {  521, 11,  28, 100, 0 }, {  530,  8, 118, 100, 0 },
	{  537,  9,  26, 100, 0 }, {  557,  8, 106, 100, 5 }, {  578,  5,  45, 100, 5 }, {  602, 13,  42, 100, 0 },
	{  609,  1,   0, 100, 2 }, {  626, 12,  29, 100, 0 }, {  628, 10,  27, 100, 0 }, {  670,  6,  48, 100, 5 },
	{  690, 10,  27, 100, 0 }, {  692,  3,  51, 100, 5 }, {  705, 11,  28, 100, 0 }, {  710,  2,  43, 100, 5 },
	{  714,  9,  26, 100, 0 }, {  720,  9,   0, 100, 2 }, {  721,  7,  49, 100, 5 }, {  725,  0,  25, 100, 5 },
};

static const SoundSequenceData _soundDataSeq13_14[] = {
	{    3,  0, 128, 100, 1 }, {   34,  0,   0, 100, 2 }, {   57,  0, 123, 100, 5 }, {   77,  0, 123, 100, 5 },
	{   84,  9,  24, 100, 0 }, {  100,  9,  24, 100, 0 }, {  140,  3,  19, 100, 5 }, {  157,  2, 109, 100, 5 },
	{  158,  1, 129, 100, 0 }, {  204,  1, 114, 100, 5 }, {  220,  2, 107, 100, 1 }, {  235,  7, 113, 100, 0 },
	{  236,  2,   0, 100, 2 }, {  248, 12, 118, 100, 0 }, {  251, 13, 119, 100, 0 }, {  265, 11, 117, 100, 0 },
	{  276, 11, 117, 100, 0 }, {  310,  5, 110, 100, 0 }, {  335,  6, 112, 100, 0 }, {  370,  4, 124, 100, 0 },
	{  373,  8, 127, 100, 0 }, {  378,  8, 127, 100, 0 }, {  434, 10, 120, 100, 0 }, {  453,  3, 108, 100, 0 },
	{  484,  3,   0, 100, 2 },
};

static const SoundSequenceData _soundDataSeq15_16[] = {
	{    3,  4,   3, 100, 5 }, {   10,  5,   4, 100, 5 }, {   22,  6,  64, 100, 5 }, {   32,  7,  65, 100, 5 },
	{   39,  6,  64, 100, 5 }, {   48,  7,  65, 100, 5 }, {   57,  6,  64, 100, 5 }, {   65,  3,  67,  50, 0 },
	{  102,  4,  68,  50, 0 }, {  151,  2,  12,  50, 5 }, {  205,  3,   5, 100, 5 }, {  218,  0,  63, 100, 5 },
	{  227,  0,  63, 100, 5 }, {  236,  0,  63, 100, 5 }, {  245,  0,  63, 100, 5 }, {  254,  0,  63, 100, 5 },
	{  263,  0,  63, 100, 5 }, {  272,  0,  63, 100, 5 }, {  291,  1,  54,  40, 1 }, {  309,  8,   6, 100, 5 },
	{  360,  8,   6, 100, 5 }, {  384,  2,  55,  60, 1 }, {  398, 10,  66, 100, 0 }, {  440,  2,   0, 100, 2 },
	{  441,  1,   0, 100, 2 }, {  452, 11,  62, 100, 0 }, {  466,  1,   2, 100, 5 }, {  470,  8,   9, 100, 0 },
	{  503,  6,   7, 100, 0 }, {  519,  7,   8, 100, 0 }, {  536,  8,   9, 100, 0 }, {  538,  4,   3, 100, 5 },
	{  549,  5,   4, 100, 5 }, {  558,  4,   3, 100, 5 }, {  568,  5,   4, 100, 5 }, {  578,  4,   3, 100, 5 },
	{  586,  5,   4, 100, 5 }, {  593,  4,   3, 100, 5 }, {  600,  5,   4, 100, 5 }, {  606,  4,   3, 100, 5 },
	{  613,  5,   4, 100, 5 }, {  620,  9,  10, 100, 0 }, {  630, 12,  11, 100, 0 },
};

static const SoundSequenceData _soundDataSeq27_28[] = {
	{   17,  2, 111, 100, 0 }, {   57,  8, 105, 100, 0 }, {   62, 10, 126, 100, 0 }, {   67,  9, 110, 100, 0 },
	{   74,  3, 115, 100, 1 }, {   87,  3,   0, 100, 2 }, {   85,  0, 121, 100, 0 }, {  138,  5, 116, 100, 0 },
	{  196,  7, 125, 100, 0 }, {  222,  6, 122, 100, 0 }, {  228,  1, 129, 100, 0 },
};

static const SoundSequenceData _soundDataSeq19_20[] = {
	{    2,  0,   0, 100, 3 }, {    3,  0, 115, 100, 1 }, {   33,  0,   0, 100, 2 }, {   33,  1,  13, 100, 0 },
	{   53,  2,  14, 100, 1 }, {   78,  2,   0, 100, 2 }, {   80,  0,   0, 100, 4 },
};

const SoundSequenceDataList AnimationSequencePlayer::_soundSeqDataList[] = {
	{   0,   0, 14, 10, 58, _soundDataSeq3_4 },
	{   0,   0, 14,  5, 60, _soundDataSeq9_10 },
	{   0,   0, 14,  9, 48, _soundDataSeq21_20 },
	{   1,  80, 14,  4, 25, _soundDataSeq13_14 },
	{   1,  80, 13,  9, 43, _soundDataSeq15_16 },
	{   1,  80, 11,  0, 11, _soundDataSeq27_28 },
	{ 104, 100,  0,  0,  0, 0 },
	{   0, 100,  4,  0,  7, _soundDataSeq19_20 }
};

const char *AnimationSequencePlayer::_audioFileNamesTable[] = {
	"demomenu.raw",
	"demorolc.raw",
	"fx101.wav",
	"fx102.wav",
	"fx103.wav",
	"fx104.wav",
	"fx105.wav",
	"fx107.wav",
	"fx108.wav",
	"fx109.wav",
	"fx110.wav",
	"fx111.wav",
	"fx112.wav",
	"fx113.wav",
	"fx114.wav",
	"fx116.wav",
	"fx117.wav",
	"fx32.wav",
	"fx33.wav",
	"fx34.wav",
	"fx35.wav",
	"fx36.wav",
	"fx37.wav",
	"fx38.wav",
	"fx39.wav",
	"fx40.wav",
	"fx42.wav",
	"fx43.wav",
	"fx44.wav",
	"fx45.wav",
	"fx47.wav",
	"fx48.wav",
	"fx49.wav",
	"fx50.wav",
	"fx52.wav",
	"fx53.wav",
	"fx55.wav",
	"fx56.wav",
	"fx57.wav",
	"fx58.wav",
	"fx59.wav",
	"fx60.wav",
	"fx61.wav",
	"fx62.wav",
	"fx63.wav",
	"fx64.wav",
	"fx66.wav",
	"fx67.wav",
	"fx68.wav",
	"fx69.wav",
	"fx70.wav",
	"fx71.wav",
	"fx72.wav",
	"fx73.wav",
	"fx74.wav",
	"fx75.wav",
	"fx76.wav",
	"fx77.wav",
	"fx78.wav",
	"fx79.wav",
	"fx80.wav",
	"fx81.wav",
	"fx83.wav",
	"fx86.wav",
	"fx91.wav",
	"fx92.wav",
	"fx93.wav",
	"fx97.wav",
	"fx98.wav",
	"int1.raw",
	"int14.raw",
	"int15.raw",
	"int16.raw",
	"int17.raw",
	"int18.raw",
	"int19.raw",
	"int2.raw",
	"int20.raw",
	"int21.raw",
	"int22.raw",
	"int23.raw",
	"int24.raw",
	"int25.raw",
	"int26.raw",
	"int27.raw",
	"int28.raw",
	"int29.raw",
	"int3.raw",
	"int30.raw",
	"int31.raw",
	"int32.raw",
	"int33.raw",
	"int34.raw",
	"int35.raw",
	"int36.raw",
	"int37.raw",
	"int38.raw",
	"int4.raw",
	"int41.raw",
	"int42.raw",
	"int5.raw",
	"int6.raw",
	"int7.raw",
	"introdua.wav",
	"merilogo.raw",
	"rdfx1.wav",
	"rdfx12.wav",
	"rdfx13.wav",
	"rdfx14.wav",
	"rdfx15.wav",
	"rdfx16.wav",
	"rdfx17.wav",
	"rdfx18.wav",
	"rdfx20.wav",
	"rdfx21.wav",
	"rdfx22.wav",
	"rdfx24.wav",
	"rdfx25.wav",
	"rdfx26.wav",
	"rdfx27.wav",
	"rdfx28.wav",
	"rdfx3.wav",
	"rdfx30.wav",
	"rdfx31.wav",
	"rdfx33.wav",
	"rdfx36.wav",
	"rdfx37.wav",
	"rdfx38.wav",
	"rdfx8.wav",
	"rdfx9.wav",
};

} // namespace Tucker
