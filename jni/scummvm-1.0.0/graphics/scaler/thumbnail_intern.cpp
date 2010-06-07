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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/graphics/scaler/thumbnail_intern.cpp $
 * $Id: thumbnail_intern.cpp 41131 2009-06-03 00:31:50Z Kirben $
 *
 */

#include "common/scummsys.h"
#include "common/system.h"

#include "graphics/colormasks.h"
#include "graphics/scaler.h"
#include "graphics/scaler/intern.h"

template<int bitFormat>
uint16 quadBlockInterpolate(const uint8* src, uint32 srcPitch) {
	uint16 colorx1y1 = *(((const uint16*)src));
	uint16 colorx2y1 = *(((const uint16*)src) + 1);

	uint16 colorx1y2 = *(((const uint16*)(src + srcPitch)));
	uint16 colorx2y2 = *(((const uint16*)(src + srcPitch)) + 1);

	return interpolate16_1_1_1_1<Graphics::ColorMasks<bitFormat> >(colorx1y1, colorx2y1, colorx1y2, colorx2y2);
}

template<int bitFormat>
void createThumbnail_2(const uint8* src, uint32 srcPitch, uint8* dstPtr, uint32 dstPitch, int width, int height) {
	assert(width % 2 == 0);
	assert(height % 2 == 0);
	for (int y = 0; y < height; y += 2) {
		for (int x = 0; x < width; x += 2, dstPtr += 2) {
			*((uint16*)dstPtr) = quadBlockInterpolate<bitFormat>(src + 2 * x, srcPitch);
		}
		dstPtr += (dstPitch - 2 * width / 2);
		src += 2 * srcPitch;
	}
}

template<int bitFormat>
void createThumbnail_4(const uint8* src, uint32 srcPitch, uint8* dstPtr, uint32 dstPitch, int width, int height) {
	assert(width % 4 == 0);
	assert(height % 4 == 0);
	for (int y = 0; y < height; y += 4) {
		for (int x = 0; x < width; x += 4, dstPtr += 2) {
			uint16 upleft = quadBlockInterpolate<bitFormat>(src + 2 * x, srcPitch);
			uint16 upright = quadBlockInterpolate<bitFormat>(src + 2 * (x + 2), srcPitch);
			uint16 downleft = quadBlockInterpolate<bitFormat>(src + srcPitch * 2 + 2 * x, srcPitch);
			uint16 downright = quadBlockInterpolate<bitFormat>(src + srcPitch * 2 + 2 * (x + 2), srcPitch);

			*((uint16*)dstPtr) = interpolate16_1_1_1_1<Graphics::ColorMasks<bitFormat> >(upleft, upright, downleft, downright);
		}
		dstPtr += (dstPitch - 2 * width / 4);
		src += 4 * srcPitch;
	}
}

static void createThumbnail(const uint8* src, uint32 srcPitch, uint8* dstPtr, uint32 dstPitch, int width, int height) {
	// only 1/2 and 1/4 downscale supported
	if (width != 320 && width != 640)
		return;

	int downScaleMode = (width == 320) ? 2 : 4;

	if (downScaleMode == 2) {
		createThumbnail_2<565>(src, srcPitch, dstPtr, dstPitch, width, height);
	} else if (downScaleMode == 4) {
		createThumbnail_4<565>(src, srcPitch, dstPtr, dstPitch, width, height);
	}
}


/**
 * Copies the current screen contents to a new surface, using RGB565 format.
 * WARNING: surf->free() must be called by the user to avoid leaking.
 *
 * @param surf		the surface to store the data in it
 */
static bool grabScreen565(Graphics::Surface *surf) {
	Graphics::Surface *screen = g_system->lockScreen();
	if (!screen)
		return false;

	assert(screen->bytesPerPixel == 1 && screen->pixels != 0);

	byte palette[256 * 4];
	g_system->grabPalette(&palette[0], 0, 256);

	surf->create(screen->w, screen->h, 2);

	for (uint y = 0; y < screen->h; ++y) {
		for (uint x = 0; x < screen->w; ++x) {
			byte r, g, b;
			r = palette[((uint8*)screen->pixels)[y * screen->pitch + x] * 4];
			g = palette[((uint8*)screen->pixels)[y * screen->pitch + x] * 4 + 1];
			b = palette[((uint8*)screen->pixels)[y * screen->pitch + x] * 4 + 2];

			((uint16*)surf->pixels)[y * surf->w + x] = Graphics::RGBToColor<Graphics::ColorMasks<565> >(r, g, b);
		}
	}

	g_system->unlockScreen();
	return true;
}

static bool createThumbnail(Graphics::Surface &out, Graphics::Surface &in) {
	uint16 width = in.w;
	uint16 inHeight = in.h;

	if (width < 320) {
		// Special case to handle MM NES (uses a screen width of 256)
		width = 320;

		// center MM NES screen
		Graphics::Surface newscreen;
		newscreen.create(width, in.h, in.bytesPerPixel);

		uint8 *dst = (uint8*)newscreen.getBasePtr((320 - in.w) / 2, 0);
		const uint8 *src = (uint8*)in.getBasePtr(0, 0);
		uint16 height = in.h;

		while (height--) {
			memcpy(dst, src, in.pitch);
			dst += newscreen.pitch;
			src += in.pitch;
		}

		in.free();
		in = newscreen;
	} else if (width == 720) {
		// Special case to handle Hercules mode
		width = 640;
		inHeight = 400;

		// cut off menu and so on..
		Graphics::Surface newscreen;
		newscreen.create(width, 400, in.bytesPerPixel);

		uint8 *dst = (uint8*)in.getBasePtr(0, (400 - 240) / 2);
		const uint8 *src = (uint8*)in.getBasePtr(41, 28);

		for (int y = 0; y < 240; ++y) {
			memcpy(dst, src, 640 * in.bytesPerPixel);
			dst += newscreen.pitch;
			src += in.pitch;
		}

		in.free();
		in = newscreen;
	}

	uint16 newHeight = !(inHeight % 240) ? kThumbnailHeight2 : kThumbnailHeight1;

	out.create(kThumbnailWidth, newHeight, sizeof(uint16));
	createThumbnail((const uint8 *)in.pixels, width * sizeof(uint16), (uint8 *)out.pixels, out.pitch, width, inHeight);

	in.free();

	return true;
}

bool createThumbnailFromScreen(Graphics::Surface* surf) {
	assert(surf);

	Graphics::Surface screen;

	if (!grabScreen565(&screen))
		return false;

	return createThumbnail(*surf, screen);
}

bool createThumbnail(Graphics::Surface *surf, const uint8 *pixels, int w, int h, const uint8 *palette) {
	assert(surf);

	Graphics::Surface screen;
	screen.create(w, h, 2);

	for (uint y = 0; y < screen.h; ++y) {
		for (uint x = 0; x < screen.w; ++x) {
			byte r, g, b;
			r = palette[pixels[y * w + x] * 3];
			g = palette[pixels[y * w + x] * 3 + 1];
			b = palette[pixels[y * w + x] * 3 + 2];

			((uint16 *)screen.pixels)[y * screen.w + x] = Graphics::RGBToColor<Graphics::ColorMasks<565> >(r, g, b);
		}
	}

	return createThumbnail(*surf, screen);
}

