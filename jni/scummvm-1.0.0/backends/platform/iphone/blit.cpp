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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/platform/iphone/blit.cpp $
 * $Id: blit.cpp 40867 2009-05-24 15:17:42Z lordhoto $
 *
 */

#include "common/scummsys.h"
#include "blit_arm.h"

void blitLandscapeScreenRect16bpp(uint16 *dst, uint16 *src, int width, int height, int screenWidth, int screenHeight)
{
	for (int x = width; x > 0; x--) {
		for (int y = height; y > 0; y--) {
			*(dst++) = *src;
			src += screenWidth;
		}
		dst -= screenHeight + height;
		src += 1 - height * screenWidth;
	}
}

void blitLandscapeScreenRect8bpp(uint16 *dst, byte *src, int width, int height, uint16 *palette, int screenWidth, int screenHeight)
{
	for (int x = width; x > 0; x--) {
		for (int y = height; y > 0; y--) {
			*(dst++) = palette[*src];
			src += screenWidth;
		}
		dst -= screenHeight + height;
		src += 1 - height * screenWidth;
	}
}
