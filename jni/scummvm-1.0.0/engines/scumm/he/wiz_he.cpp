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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/scumm/he/wiz_he.cpp $
 * $Id: wiz_he.cpp 43296 2009-08-12 01:01:36Z Kirben $
 *
 */

#ifdef ENABLE_HE

#include "common/system.h"
#include "graphics/cursorman.h"
#include "graphics/primitives.h"
#include "scumm/he/intern_he.h"
#include "scumm/resource.h"
#include "scumm/scumm.h"
#include "scumm/util.h"
#include "scumm/he/wiz_he.h"

namespace Scumm {

Wiz::Wiz(ScummEngine_v71he *vm) : _vm(vm) {
	_imagesNum = 0;
	memset(&_images, 0, sizeof(_images));
	memset(&_polygons, 0, sizeof(_polygons));
	_rectOverrideEnabled = false;
}

void Wiz::clearWizBuffer() {
	_imagesNum = 0;
}

void Wiz::polygonClear() {
	for (int i = 0; i < ARRAYSIZE(_polygons); i++) {
		if (_polygons[i].flag == 1)
			memset(&_polygons[i], 0, sizeof(WizPolygon));
	}
}

void Wiz::polygonLoad(const uint8 *polData) {
	int slots = READ_LE_UINT32(polData);
	polData += 4;

	bool flag = 1;
	int id, points, vert1x, vert1y, vert2x, vert2y, vert3x, vert3y, vert4x, vert4y;
	while (slots--) {
		id = READ_LE_UINT32(polData);
		points = READ_LE_UINT32(polData + 4);
		if (points != 4)
			error("Illegal polygon with %d points", points);
		vert1x = READ_LE_UINT32(polData + 8);
		vert1y = READ_LE_UINT32(polData + 12);
		vert2x = READ_LE_UINT32(polData + 16);
		vert2y = READ_LE_UINT32(polData + 20);
		vert3x = READ_LE_UINT32(polData + 24);
		vert3y = READ_LE_UINT32(polData + 28);
		vert4x = READ_LE_UINT32(polData + 32);
		vert4y = READ_LE_UINT32(polData + 36);

		polData += 40;
		polygonStore(id, flag, vert1x, vert1y, vert2x, vert2y, vert3x, vert3y, vert4x, vert4y);
	}
}

void Wiz::polygonStore(int id, bool flag, int vert1x, int vert1y, int vert2x, int vert2y, int vert3x, int vert3y, int vert4x, int vert4y) {
	WizPolygon *wp = NULL;
	for (int i = 0; i < ARRAYSIZE(_polygons); ++i) {
		if (_polygons[i].id == 0) {
			wp = &_polygons[i];
			break;
		}
	}
	if (!wp) {
		error("Wiz::polygonStore: out of polygon slot, max = %d", ARRAYSIZE(_polygons));
	}

	wp->vert[0].x = vert1x;
	wp->vert[0].y = vert1y;
	wp->vert[1].x = vert2x;
	wp->vert[1].y = vert2y;
	wp->vert[2].x = vert3x;
	wp->vert[2].y = vert3y;
	wp->vert[3].x = vert4x;
	wp->vert[3].y = vert4y;
	wp->vert[4].x = vert1x;
	wp->vert[4].y = vert1y;
	wp->id = id;
	wp->numVerts = 5;
	wp->flag = flag;

	polygonCalcBoundBox(wp->vert, wp->numVerts, wp->bound);
}

void Wiz::polygonRotatePoints(Common::Point *pts, int num, int angle) {
	double alpha = angle * PI / 180.;
	double cos_alpha = cos(alpha);
	double sin_alpha = sin(alpha);

	for (int i = 0; i < num; ++i) {
		int16 x = pts[i].x;
		int16 y = pts[i].y;
		pts[i].x = (int16)(x * cos_alpha - y * sin_alpha);
		pts[i].y = (int16)(y * cos_alpha + x * sin_alpha);
	}
}

void Wiz::polygonTransform(int resNum, int state, int po_x, int po_y, int angle, int scale, Common::Point *pts) {
	int32 w, h;

	getWizImageDim(resNum, state, w, h);

	// set the transformation origin to the center of the image
	if (_vm->_game.heversion >= 99) {
		pts[0].x = pts[3].x = -(w / 2);
		pts[1].x = pts[2].x = w / 2 - 1;
		pts[0].y = pts[1].y = -(h / 2);
		pts[2].y = pts[3].y = h / 2 - 1;
	} else {
		pts[1].x = pts[2].x = w / 2 - 1;
		pts[0].x = pts[0].y = pts[1].y = pts[3].x = -(w / 2);
		pts[2].y = pts[3].y = h / 2 - 1;
	}

	// scale
	if (scale != 0 && scale != 256) {
		for (int i = 0; i < 4; ++i) {
			pts[i].x = pts[i].x * scale / 256;
			pts[i].y = pts[i].y * scale / 256;
		}
	}

	// rotate
	if (angle != 0)
		polygonRotatePoints(pts, 4, angle);

	// translate
	for (int i = 0; i < 4; ++i) {
		pts[i].x += po_x;
		pts[i].y += po_y;
	}
}

void Wiz::polygonCalcBoundBox(Common::Point *vert, int numVerts, Common::Rect &bound) {
	bound.left = 10000;
	bound.top = 10000;
	bound.right = -10000;
	bound.bottom = -10000;

	// compute bounding box
	for (int j = 0; j < numVerts; j++) {
		Common::Rect r(vert[j].x, vert[j].y, vert[j].x + 1, vert[j].y + 1);
		bound.extend(r);
	}
}

void Wiz::polygonErase(int fromId, int toId) {
	for (int i = 0; i < ARRAYSIZE(_polygons); i++) {
		if (_polygons[i].id >= fromId && _polygons[i].id <= toId)
			memset(&_polygons[i], 0, sizeof(WizPolygon));
	}
}

int Wiz::polygonHit(int id, int x, int y) {
	for (int i = 0; i < ARRAYSIZE(_polygons); i++) {
		if ((id == 0 || _polygons[i].id == id) && _polygons[i].bound.contains(x, y)) {
			if (polygonContains(_polygons[i], x, y)) {
				return _polygons[i].id;
			}
		}
	}
	return 0;
}

bool Wiz::polygonDefined(int id) {
	for (int i = 0; i < ARRAYSIZE(_polygons); i++)
		if (_polygons[i].id == id)
			return true;
	return false;
}

bool Wiz::polygonContains(const WizPolygon &pol, int x, int y) {
	int pi = pol.numVerts - 1;
	bool diry = (y < pol.vert[pi].y);
	bool curdir;
	bool r = false;

	for (int i = 0; i < pol.numVerts; i++) {
		curdir = (y < pol.vert[i].y);

		if (curdir != diry) {
			if (((pol.vert[pi].y - pol.vert[i].y) * (pol.vert[i].x - x) <
				 (pol.vert[pi].x - pol.vert[i].x) * (pol.vert[i].y - y)) == diry)
				r = !r;
		}

		pi = i;
		diry = curdir;
	}

	// HE80+
	int a, b;
	pi = pol.numVerts - 1;
	if (r == 0) {
		for (int i = 0; i < pol.numVerts; i++) {
			if (pol.vert[i].y == y && pol.vert[i].y == pol.vert[pi].y) {

				a = pol.vert[i].x;
				b = pol.vert[pi].x;

				if (pol.vert[i].x >= pol.vert[pi].x)
					a = pol.vert[pi].x;

				if (pol.vert[i].x > pol.vert[pi].x)
					b = pol.vert[i].x;

				if (x >= a && x <= b)
					return 1;

			} else if (pol.vert[i].x == x && pol.vert[i].x == pol.vert[pi].x) {

				a = pol.vert[i].y;
				b = pol.vert[i].y;

				if (pol.vert[i].y >= pol.vert[pi].y)
					a = pol.vert[pi].y;

				if (pol.vert[i].y <= pol.vert[pi].y)
					b = pol.vert[pi].y;

				if (y >= a && y <= b)
					return 1;
			}
			pi = i;
		}
	}

	return r;
}

void Wiz::copyAuxImage(uint8 *dst1, uint8 *dst2, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch) {
	Common::Rect dstRect(srcx, srcy, srcx + srcw, srcy + srch);
	dstRect.clip(dstw, dsth);

	int rw = dstRect.width();
	int rh = dstRect.height();
	if (rh <= 0 || rw <= 0)
		return;

	uint8 *dst1Ptr = dst1 + dstRect.left + dstRect.top * dstw;
	uint8 *dst2Ptr = dst2 + dstRect.left + dstRect.top * dstw;
	const uint8 *dataPtr = src;

	while (rh--) {
		uint16 off = READ_LE_UINT16(dataPtr); dataPtr += 2;
		const uint8 *dataPtrNext = off + dataPtr;
		uint8 *dst1PtrNext = dst1Ptr + dstw;
		uint8 *dst2PtrNext = dst2Ptr + dstw;
		if (off != 0) {
			int w = rw;
			while (w > 0) {
				uint8 code = *dataPtr++;
				if (code & 1) {
					code >>= 1;
					dst1Ptr += code;
					dst2Ptr += code;
					w -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
					w -= code;
					if (w >= 0) {
						memset(dst1Ptr, *dataPtr++, code);
						dst1Ptr += code;
						dst2Ptr += code;
					} else {
						code += w;
						memset(dst1Ptr, *dataPtr, code);
					}
				} else {
					code = (code >> 2) + 1;
					w -= code;
					if (w >= 0) {
						memcpy(dst1Ptr, dst2Ptr, code);
						dst1Ptr += code;
						dst2Ptr += code;
					} else {
						code += w;
						memcpy(dst1Ptr, dst2Ptr, code);
					}
				}
			}
		}
		dataPtr = dataPtrNext;
		dst1Ptr = dst1PtrNext;
		dst2Ptr = dst2PtrNext;
	}
}

static bool calcClipRects(int dst_w, int dst_h, int src_x, int src_y, int src_w, int src_h, const Common::Rect *rect, Common::Rect &srcRect, Common::Rect &dstRect) {
	srcRect = Common::Rect(src_w, src_h);
	dstRect = Common::Rect(src_x, src_y, src_x + src_w, src_y + src_h);
	Common::Rect r3;
	int diff;

	if (rect) {
		r3 = *rect;
		Common::Rect r4(dst_w, dst_h);
		if (r3.intersects(r4)) {
			r3.clip(r4);
		} else {
			return false;
		}
	} else {
		r3 = Common::Rect(dst_w, dst_h);
	}
	diff = dstRect.left - r3.left;
	if (diff < 0) {
		srcRect.left -= diff;
		dstRect.left -= diff;
	}
	diff = dstRect.right - r3.right;
	if (diff > 0) {
		srcRect.right -= diff;
		dstRect.right -= diff;
	}
	diff = dstRect.top - r3.top;
	if (diff < 0) {
		srcRect.top -= diff;
		dstRect.top -= diff;
	}
	diff = dstRect.bottom - r3.bottom;
	if (diff > 0) {
		srcRect.bottom -= diff;
		dstRect.bottom -= diff;
	}

	return srcRect.isValidRect() && dstRect.isValidRect();
}

void Wiz::copy16BitWizImage(uint8 *dst, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch, const Common::Rect *rect, int flags, const uint8 *palPtr, const uint8 *xmapPtr) {
	// TODO: Compressed 16 bits in 555 format

	Common::Rect r1, r2;
	if (calcClipRects(dstw, dsth, srcx, srcy, srcw, srch, rect, r1, r2)) {
		dst += r2.top * dstw + r2.left;
		if (flags & kWIFFlipY) {
			const int dy = (srcy < 0) ? srcy : (srch - r1.height());
			r1.translate(0, dy);
		}
		if (flags & kWIFFlipX) {
			const int dx = (srcx < 0) ? srcx : (srcw - r1.width());
			r1.translate(dx, 0);
		}
		if (xmapPtr) {
			decompress16BitWizImage<kWizXMap>(dst, dstw, src, r1, flags, palPtr, xmapPtr);
		} else {
			decompress16BitWizImage<kWizCopy>(dst, dstw, src, r1, flags);
		}
	}
}

void Wiz::copyWizImage(uint8 *dst, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch, const Common::Rect *rect, int flags, const uint8 *palPtr, const uint8 *xmapPtr) {
	Common::Rect r1, r2;
	if (calcClipRects(dstw, dsth, srcx, srcy, srcw, srch, rect, r1, r2)) {
		dst += r2.left + r2.top * dstw;
		if (flags & kWIFFlipY) {
			const int dy = (srcy < 0) ? srcy : (srch - r1.height());
			r1.translate(0, dy);
		}
		if (flags & kWIFFlipX) {
			const int dx = (srcx < 0) ? srcx : (srcw - r1.width());
			r1.translate(dx, 0);
		}
		if (xmapPtr) {
			decompressWizImage<kWizXMap>(dst, dstw, src, r1, flags, palPtr, xmapPtr);
		} else if (palPtr) {
			decompressWizImage<kWizRMap>(dst, dstw, src, r1, flags, palPtr);
		} else {
			decompressWizImage<kWizCopy>(dst, dstw, src, r1, flags);
		}
	}
}

static void decodeWizMask(uint8 *&dst, uint8 &mask, int w, int maskType) {
	switch (maskType) {
	case 0:
		while (w--) {
			mask >>= 1;
			if (mask == 0) {
				mask = 0x80;
				++dst;
			}
		}
		break;
	case 1:
		while (w--) {
			*dst &= ~mask;
			mask >>= 1;
			if (mask == 0) {
				mask = 0x80;
				++dst;
			}
		}
		break;
	case 2:
		while (w--) {
			*dst |= mask;
			mask >>= 1;
			if (mask == 0) {
				mask = 0x80;
				++dst;
			}
		}
		break;
	}
}

void Wiz::copyWizImageWithMask(uint8 *dst, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch, const Common::Rect *rect, int maskT, int maskP) {
	Common::Rect srcRect, dstRect;
	if (!calcClipRects(dstw, dsth, srcx, srcy, srcw, srch, rect, srcRect, dstRect)) {
		return;
	}
	dstw = dstw / 8;
	dst += dstRect.top * dstw + dstRect.left / 8;

	const uint8 *dataPtr, *dataPtrNext;
	uint8 code, mask, *dstPtr, *dstPtrNext;
	int h, w, xoff;
	uint16 off;

	dstPtr = dst;
	dataPtr = src;

	// Skip over the first 'srcRect->top' lines in the data
	h = srcRect.top;
	while (h--) {
		dataPtr += READ_LE_UINT16(dataPtr) + 2;
	}
	h = srcRect.height();
	w = srcRect.width();
	if (h <= 0 || w <= 0)
		return;

	while (h--) {
		xoff = srcRect.left;
		w = srcRect.width();
		mask = revBitMask(dstRect.left & 7);
		off = READ_LE_UINT16(dataPtr); dataPtr += 2;
		dstPtrNext = dstPtr + dstw;
		dataPtrNext = dataPtr + off;
		if (off != 0) {
			while (w > 0) {
				code = *dataPtr++;
				if (code & 1) {
					code >>= 1;
					if (xoff > 0) {
						xoff -= code;
						if (xoff >= 0)
							continue;

						code = -xoff;
					}
					decodeWizMask(dstPtr, mask, code, maskT);
					w -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						++dataPtr;
						if (xoff >= 0)
							continue;

						code = -xoff;
						--dataPtr;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					decodeWizMask(dstPtr, mask, code, maskP);
					dataPtr++;
				} else {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						dataPtr += code;
						if (xoff >= 0)
							continue;

						code = -xoff;
						dataPtr += xoff;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					decodeWizMask(dstPtr, mask, code, maskP);
					dataPtr += code;
				}
			}
		}
		dataPtr = dataPtrNext;
		dstPtr = dstPtrNext;
	}
}

void Wiz::copyRawWizImage(uint8 *dst, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch, const Common::Rect *rect, int flags, const uint8 *palPtr, int transColor) {
	Common::Rect r1, r2;
	if (calcClipRects(dstw, dsth, srcx, srcy, srcw, srch, rect, r1, r2)) {
		if (flags & kWIFFlipX) {
			int l = r1.left;
			int r = r1.right;
			r1.left = srcw - r;
			r1.right = srcw - l;
		}
		if (flags & kWIFFlipY) {
			int t = r1.top;
			int b = r1.bottom;
			r1.top = srch - b;
			r1.bottom = srch - t;
		}
		int h = r1.height();
		int w = r1.width();
		src += r1.left + r1.top * srcw;
		dst += r2.left + r2.top * dstw;
		if (palPtr) {
			decompressRawWizImage<kWizRMap>(dst, dstw, src, srcw, w, h, transColor, palPtr);
		} else {
			decompressRawWizImage<kWizCopy>(dst, dstw, src, srcw, w, h, transColor);
		}
	}
}

void Wiz::copyRaw16BitWizImage(uint8 *dst, const uint8 *src, int dstw, int dsth, int srcx, int srcy, int srcw, int srch, const Common::Rect *rect, int flags, const uint8 *palPtr, int transColor) {
	// TODO: RAW 16 bits in 555 format

	Common::Rect r1, r2;
	if (calcClipRects(dstw, dsth, srcx, srcy, srcw, srch, rect, r1, r2)) {
		if (flags & kWIFFlipX) {
			int l = r1.left;
			int r = r1.right;
			r1.left = srcw - r;
			r1.right = srcw - l;
		}
		if (flags & kWIFFlipY) {
			int t = r1.top;
			int b = r1.bottom;
			r1.top = srch - b;
			r1.bottom = srch - t;
		}
		int h = r1.height();
		int w = r1.width();
		src += (r1.top * srcw + r1.left) * 2;
		dst += r2.top * dstw + r2.left;
		while (h--) {
			for (int i = 0; i < w; ++i) {
				uint16 col = READ_LE_UINT16(src + 2 * i);
				uint8 r = ((col >> 10) & 0x1F) << 3;
				uint8 g = ((col >>  5) & 0x1F) << 3;
				uint8 b = ((col >>  0) & 0x1F) << 3;
				uint8 color = _vm->convert16BitColor(col, r, g, b);

				if (transColor == -1 || transColor != col) {
					dst[i] = color;
				}
			}
			src += srcw * 2;
			dst += dstw;
		}
	}
}

template <int type>
void Wiz::decompress16BitWizImage(uint8 *dst, int dstPitch, const uint8 *src, const Common::Rect &srcRect, int flags, const uint8 *palPtr, const uint8 *xmapPtr) {
	const uint8 *dataPtr, *dataPtrNext;
	uint8 code, *dstPtr, *dstPtrNext;
	int h, w, xoff, dstInc;

	if (type == kWizXMap) {
		assert(xmapPtr != 0);
	}
	if (type == kWizRMap) {
		assert(palPtr != 0);
	}

	dstPtr = dst;
	dataPtr = src;

	// Skip over the first 'srcRect->top' lines in the data
	h = srcRect.top;
	while (h--) {
		dataPtr += READ_LE_UINT16(dataPtr) + 2;
	}
	h = srcRect.height();
	w = srcRect.width();
	if (h <= 0 || w <= 0)
		return;

	if (flags & kWIFFlipY) {
		dstPtr += (h - 1) * dstPitch;
		dstPitch = -dstPitch;
	}
	dstInc = 1;
	if (flags & kWIFFlipX) {
		dstPtr += w - 1;
		dstInc = -1;
	}

	while (h--) {
		xoff = srcRect.left;
		w = srcRect.width();
		uint16 lineSize = READ_LE_UINT16(dataPtr); dataPtr += 2;
		dstPtrNext = dstPtr + dstPitch;
		dataPtrNext = dataPtr + lineSize;
		if (lineSize != 0) {
			while (w > 0) {
				code = *dataPtr++;
				if (code & 1) {
					code >>= 1;
					if (xoff > 0) {
						xoff -= code;
						if (xoff >= 0)
							continue;

						code = -xoff;
					}
					dstPtr += dstInc * code;
					w -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						dataPtr += 2;
						if (xoff >= 0)
							continue;

						code = -xoff;
						dataPtr -= 2;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					while (code--) {
						uint16 col = READ_LE_UINT16(dataPtr);
						uint8 r = ((col >> 10) & 0x1F) << 3;
						uint8 g = ((col >>  5) & 0x1F) << 3;
						uint8 b = ((col >>  0) & 0x1F) << 3;
						col = _vm->convert16BitColor(col, r, g, b);

						if (type == kWizXMap) {
							*dstPtr = xmapPtr[col * 256 + *dstPtr];
						}
						if (type == kWizCopy) {
							*dstPtr = col;
						}
						dstPtr += dstInc;
					}
					dataPtr+= 2;
				} else {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						dataPtr += code * 2;
						if (xoff >= 0)
							continue;

						code = -xoff;
						dataPtr += xoff * 2;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					while (code--) {
						uint16 col = READ_LE_UINT16(dataPtr);
						uint8 r = ((col >> 10) & 0x1F) << 3;
						uint8 g = ((col >>  5) & 0x1F) << 3;
						uint8 b = ((col >>  0) & 0x1F) << 3;
						col = _vm->convert16BitColor(col, r, g, b);

						if (type == kWizXMap) {
							*dstPtr = xmapPtr[col * 256 + *dstPtr];
						}
						if (type == kWizCopy) {
							*dstPtr = col;
						}
						dataPtr += 2;
						dstPtr += dstInc;
					}
				}
			}
		}
		dataPtr = dataPtrNext;
		dstPtr = dstPtrNext;
	}
}

template <int type>
void Wiz::decompressWizImage(uint8 *dst, int dstPitch, const uint8 *src, const Common::Rect &srcRect, int flags, const uint8 *palPtr, const uint8 *xmapPtr) {
	const uint8 *dataPtr, *dataPtrNext;
	uint8 code, *dstPtr, *dstPtrNext;
	int h, w, xoff, dstInc;

	if (type == kWizXMap) {
		assert(xmapPtr != 0);
	}
	if (type == kWizRMap) {
		assert(palPtr != 0);
	}

	dstPtr = dst;
	dataPtr = src;

	// Skip over the first 'srcRect->top' lines in the data
	h = srcRect.top;
	while (h--) {
		dataPtr += READ_LE_UINT16(dataPtr) + 2;
	}
	h = srcRect.height();
	w = srcRect.width();
	if (h <= 0 || w <= 0)
		return;

	if (flags & kWIFFlipY) {
		dstPtr += (h - 1) * dstPitch;
		dstPitch = -dstPitch;
	}
	dstInc = 1;
	if (flags & kWIFFlipX) {
		dstPtr += w - 1;
		dstInc = -1;
	}

	while (h--) {
		xoff = srcRect.left;
		w = srcRect.width();
		uint16 lineSize = READ_LE_UINT16(dataPtr); dataPtr += 2;
		dstPtrNext = dstPtr + dstPitch;
		dataPtrNext = dataPtr + lineSize;
		if (lineSize != 0) {
			while (w > 0) {
				code = *dataPtr++;
				if (code & 1) {
					code >>= 1;
					if (xoff > 0) {
						xoff -= code;
						if (xoff >= 0)
							continue;

						code = -xoff;
					}
					dstPtr += dstInc * code;
					w -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						++dataPtr;
						if (xoff >= 0)
							continue;

						code = -xoff;
						--dataPtr;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					while (code--) {
						if (type == kWizXMap) {
							*dstPtr = xmapPtr[*dataPtr * 256 + *dstPtr];
						}
						if (type == kWizRMap) {
							*dstPtr = palPtr[*dataPtr];
						}
						if (type == kWizCopy) {
							*dstPtr = *dataPtr;
						}
						dstPtr += dstInc;
					}
					dataPtr++;
				} else {
					code = (code >> 2) + 1;
					if (xoff > 0) {
						xoff -= code;
						dataPtr += code;
						if (xoff >= 0)
							continue;

						code = -xoff;
						dataPtr += xoff;
					}
					w -= code;
					if (w < 0) {
						code += w;
					}
					while (code--) {
						if (type == kWizXMap) {
							*dstPtr = xmapPtr[*dataPtr++ * 256 + *dstPtr];
						}
						if (type == kWizRMap) {
							*dstPtr = palPtr[*dataPtr++];
						}
						if (type == kWizCopy) {
							*dstPtr = *dataPtr++;
						}
						dstPtr += dstInc;
					}
				}
			}
		}
		dataPtr = dataPtrNext;
		dstPtr = dstPtrNext;
	}
}

// NOTE: These templates are used outside this file. We don't want the compiler to optimize them away, so we need to explicitely instantiate them.
template void Wiz::decompressWizImage<kWizXMap>(uint8 *dst, int dstPitch, const uint8 *src, const Common::Rect &srcRect, int flags, const uint8 *palPtr, const uint8 *xmapPtr);
template void Wiz::decompressWizImage<kWizRMap>(uint8 *dst, int dstPitch, const uint8 *src, const Common::Rect &srcRect, int flags, const uint8 *palPtr, const uint8 *xmapPtr);
template void Wiz::decompressWizImage<kWizCopy>(uint8 *dst, int dstPitch, const uint8 *src, const Common::Rect &srcRect, int flags, const uint8 *palPtr, const uint8 *xmapPtr);

template <int type>
void Wiz::decompressRawWizImage(uint8 *dst, int dstPitch, const uint8 *src, int srcPitch, int w, int h, int transColor, const uint8 *palPtr) {
	if (type == kWizRMap) {
		assert(palPtr != 0);
	}

	if (w <= 0 || h <= 0) {
		return;
	}
	while (h--) {
		for (int i = 0; i < w; ++i) {
			uint8 col = src[i];
			if (transColor == -1 || transColor != col) {
				if (type == kWizRMap) {
					dst[i] = palPtr[col];
				}
				if (type == kWizCopy) {
					dst[i] = col;
				}
			}
		}
		src += srcPitch;
		dst += dstPitch;
	}
}

int Wiz::isWizPixelNonTransparent(const uint8 *data, int x, int y, int w, int h, uint8 bitDepth) {
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return 0;
	}
	while (y != 0) {
		data += READ_LE_UINT16(data) + 2;
		--y;
	}
	uint16 off = READ_LE_UINT16(data); data += 2;
	if (off == 0) {
		return 0;
	}
	while (x > 0) {
		uint8 code = *data++;
		if (code & 1) {
			code >>= 1;
			if (code > x) {
				return 0;
			}
			x -= code;
		} else if (code & 2) {
			code = (code >> 2) + 1;
			if (code > x) {
				return 1;
			}
			x -= code;
			data += bitDepth;
		} else {
			code = (code >> 2) + 1;
			if (code > x) {
				return 1;
			}
			x -= code;
			data += code * bitDepth;
		}
	}

	if (bitDepth == 2)
		return (~READ_LE_UINT16(data)) & 1;
	else
		return (~data[0]) & 1;
}

uint16 Wiz::getWizPixelColor(const uint8 *data, int x, int y, int w, int h, uint8 bitDepth, uint16 color) {
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return color;
	}
	while (y != 0) {
		data += READ_LE_UINT16(data) + 2;
		--y;
	}
	uint16 off = READ_LE_UINT16(data); data += 2;
	if (off == 0) {
		return color;
	}
	while (x > 0) {
		uint8 code = *data++;
		if (code & 1) {
			code >>= 1;
			if (code > x) {
				return color;
			}
			x -= code;
		} else if (code & 2) {
			code = (code >> 2) + 1;
			if (code > x) {
				return (bitDepth == 2) ? READ_LE_UINT16(data) : data[0];
			}
			x -= code;
			data += bitDepth;
		} else {
			code = (code >> 2) + 1;
			if (code > x) {
				return (bitDepth == 2) ? READ_LE_UINT16(data + x) : data[x];
			}
			x -= code;
			data += code * bitDepth;
		}
	}

	if (bitDepth == 2)
		return (READ_LE_UINT16(data) & 1) ? color : READ_LE_UINT16(data + 1);
	else
		return (data[0] & 1) ? color : data[1];

}

uint16 Wiz::getRawWizPixelColor(const uint8 *data, int x, int y, int w, int h, uint8 bitDepth, uint16 color) {
	if (x < 0 || x >= w || y < 0 || y >= h) {
		return color;
	}
	if (bitDepth == 2)
		return READ_LE_UINT16(data + y * w + x * 2);
	else
		return data[y * w + x];
}

void Wiz::computeWizHistogram(uint32 *histogram, const uint8 *data, const Common::Rect &rCapt) {
	int h = rCapt.top;
	while (h--) {
		data += READ_LE_UINT16(data) + 2;
	}

	h = rCapt.height();
	while (h--) {
		uint16 off = READ_LE_UINT16(data); data += 2;
		if (off != 0) {
			const uint8 *p = data;
			int xoffs = rCapt.left;
			int w = rCapt.width();
			uint8 code;
			while (xoffs > 0) {
				code = *p++;
				if (code & 1) {
					code >>= 1;
					if (code > xoffs) {
						code -= xoffs;
						w -= code;
						break;
					}
					xoffs -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
					if (code > xoffs) {
						code -= xoffs;
						goto dec_sub2;
					}
					xoffs -= code;
					p++;
				} else {
					code = (code >> 2) + 1;
					if (code > xoffs) {
						code -= xoffs;
						p += xoffs;
						goto dec_sub3;
					}
					xoffs -= code;
					p += code;
				}
			}
			while (w > 0) {
				code = *p++;
				if (code & 1) {
					code >>= 1;
					w -= code;
				} else if (code & 2) {
					code = (code >> 2) + 1;
dec_sub2:				w -= code;
					if (w < 0) {
						code += w;
					}
					histogram[*p++] += code;
				} else {
					code = (code >> 2) + 1;
dec_sub3:				w -= code;
					if (w < 0) {
						code += w;
					}
					int n = code;
					while (n--) {
						++histogram[*p++];
					}
				}
			}
			data += off;
		}
	}
}

void Wiz::computeRawWizHistogram(uint32 *histogram, const uint8 *data, int srcPitch, const Common::Rect &rCapt) {
	data += rCapt.top * srcPitch + rCapt.left;
	int iw = rCapt.width();
	int ih = rCapt.height();
	while (ih--) {
		for (int i = 0; i < iw; ++i) {
			++histogram[data[i]];
		}
		data += srcPitch;
	}
}

static int wizPackType1(uint8 *dst, const uint8 *src, int srcPitch, const Common::Rect& rCapt, uint8 transColor) {
	debug(9, "wizPackType1(%d, [%d,%d,%d,%d])", transColor, rCapt.left, rCapt.top, rCapt.right, rCapt.bottom);
	src += rCapt.top * srcPitch + rCapt.left;
	int w = rCapt.width();
	int h = rCapt.height();
	int dataSize = 0;
	while (h--) {
		uint8 *dstLine = dst;
		if (dst) {
			dst += 2;
		}
		uint8 diffBuffer[0x40];
		int runCountSame = 0;
		int runCountDiff = 0;
		uint8 prevColor = src[0];
		for (int i = 1; i < w; ) {
			uint8 color = src[i++];
			if (i == 2) {
				if (prevColor == color) {
					runCountSame = 1;
				} else {
					diffBuffer[0] = prevColor;
					runCountDiff = 1;
				}
			}
			if (prevColor == color) {
				if (runCountDiff != 0) {
					runCountSame = 1;
					if (runCountDiff > 1) {
						--runCountDiff;
						if (dst) {
							*dst++ = ((runCountDiff - 1) << 2) | 0;
							memcpy(dst, diffBuffer, runCountDiff);
							dst += runCountDiff;
						}
						dataSize += runCountDiff + 1;
					}
					runCountDiff = 0;
				}
				++runCountSame;
				if (prevColor == transColor) {
					if (runCountSame == 0x7F) {
						if (dst) {
							*dst++ = (runCountSame << 1) | 1;
						}
						++dataSize;
						runCountSame = 0;
					}
				} else {
					if (runCountSame == 0x40) {
						if (dst) {
							*dst++ = ((runCountSame - 1) << 2) | 2;
							*dst++ = prevColor;
						}
						dataSize += 2;
						runCountSame = 0;
					}
				}
			} else {
				if (runCountSame != 0) {
					if (prevColor == transColor) {
						if (dst) {
							*dst++ = (runCountSame << 1) | 1;
						}
						++dataSize;
					} else {
						if (dst) {
							*dst++ = ((runCountSame - 1) << 2) | 2;
							*dst++ = prevColor;
						}
						dataSize += 2;
					}
					runCountSame = 0;
				}
				assert(runCountDiff < ARRAYSIZE(diffBuffer));
				diffBuffer[runCountDiff++] = color;
				if (runCountDiff == 0x40) {
					if (dst) {
						*dst++ = ((runCountDiff - 1) << 2) | 0;
						memcpy(dst, diffBuffer, runCountDiff);
						dst += runCountDiff;
					}
					dataSize += runCountDiff + 1;
					runCountDiff = 0;
				}
			}
			prevColor = color;
		}
		if (runCountSame != 0) {
			if (prevColor == transColor) {
				if (dst) {
					*dst++ = (runCountSame << 1) | 1;
				}
				++dataSize;
			} else {
				if (dst) {
					*dst++ = ((runCountSame - 1) << 2) | 2;
					*dst++ = prevColor;
				}
				dataSize += 2;
			}
		}
		if (runCountDiff != 0) {
			if (dst) {
				*dst++ = ((runCountDiff - 1) << 2) | 0;
				memcpy(dst, diffBuffer, runCountDiff);
				dst += runCountDiff;
			}
			dataSize += runCountDiff + 1;
		}
		if (dst) {
			WRITE_LE_UINT16(dstLine, dst - dstLine - 2);
		}
		dataSize += 2;
		src += srcPitch;
	}
	return dataSize;
}

static int wizPackType0(uint8 *dst, const uint8 *src, int srcPitch, const Common::Rect& rCapt) {
	debug(9, "wizPackType0([%d,%d,%d,%d])", rCapt.left, rCapt.top, rCapt.right, rCapt.bottom);
	int w = rCapt.width();
	int h = rCapt.height();
	int size = w * h;
	if (dst) {
		src += rCapt.top * srcPitch + rCapt.left;
		while (h--) {
			memcpy(dst, src, w);
			dst += w;
			src += srcPitch;
		}
	}
	return size;
}

void Wiz::captureWizImage(int resNum, const Common::Rect& r, bool backBuffer, int compType) {
	debug(5, "ScummEngine_v72he::captureWizImage(%d, %d, [%d,%d,%d,%d])", resNum, compType, r.left, r.top, r.right, r.bottom);
	uint8 *src = NULL;
	VirtScreen *pvs = &_vm->_virtscr[kMainVirtScreen];
	if (backBuffer) {
		src = pvs->getBackPixels(0, 0);
	} else {
		src = pvs->getPixels(0, 0);
	}
	Common::Rect rCapt(pvs->w, pvs->h);
	if (rCapt.intersects(r)) {
		rCapt.clip(r);
		const uint8 *palPtr;
		if (_vm->_game.heversion >= 99) {
			palPtr = _vm->_hePalettes + 1024;
		} else {
			palPtr = _vm->_currentPalette;
		}

		int w = rCapt.width();
		int h = rCapt.height();
		int transColor = (_vm->VAR_WIZ_TCOLOR != 0xFF) ? _vm->VAR(_vm->VAR_WIZ_TCOLOR) : 5;

		// compute compressed size
		int dataSize = 0;
		int headerSize = palPtr ? 1080 : 36;
		switch (compType) {
		case 0:
			dataSize = wizPackType0(0, src, pvs->pitch, rCapt);
			break;
		case 1:
			dataSize = wizPackType1(0, src, pvs->pitch, rCapt, transColor);
			break;
		default:
			error("unhandled compression type %d", compType);
			break;
		}

		// alignment
		dataSize = (dataSize + 1) & ~1;
		int wizSize = headerSize + dataSize;
		// write header
		uint8 *wizImg = _vm->_res->createResource(rtImage, resNum, dataSize + headerSize);
		WRITE_BE_UINT32(wizImg + 0x00, 'AWIZ');
		WRITE_BE_UINT32(wizImg + 0x04, wizSize);
		WRITE_BE_UINT32(wizImg + 0x08, 'WIZH');
		WRITE_BE_UINT32(wizImg + 0x0C, 0x14);
		WRITE_LE_UINT32(wizImg + 0x10, compType);
		WRITE_LE_UINT32(wizImg + 0x14, w);
		WRITE_LE_UINT32(wizImg + 0x18, h);
		int curSize = 0x1C;
		if (palPtr) {
			WRITE_BE_UINT32(wizImg + 0x1C, 'RGBS');
			WRITE_BE_UINT32(wizImg + 0x20, 0x308);
			memcpy(wizImg + 0x24, palPtr, 0x300);
			WRITE_BE_UINT32(wizImg + 0x324, 'RMAP');
			WRITE_BE_UINT32(wizImg + 0x328, 0x10C);
			WRITE_BE_UINT32(wizImg + 0x32C, 0);
			curSize = 0x330;
			for (int i = 0; i < 256; ++i) {
				wizImg[curSize] = i;
				++curSize;
			}
		}
		WRITE_BE_UINT32(wizImg + curSize + 0x0, 'WIZD');
		WRITE_BE_UINT32(wizImg + curSize + 0x4, dataSize + 8);
		curSize += 8;

		// write compressed data
		switch (compType) {
		case 0:
			wizPackType0(wizImg + headerSize, src, pvs->pitch, rCapt);
			break;
		case 1:
			wizPackType1(wizImg + headerSize, src, pvs->pitch, rCapt, transColor);
			break;
		default:
			break;
		}
	}
	_vm->_res->setModified(rtImage, resNum);
}

void Wiz::displayWizImage(WizImage *pwi) {
	if (_vm->_fullRedraw) {
		assert(_imagesNum < ARRAYSIZE(_images));
		WizImage *wi = &_images[_imagesNum];
		wi->resNum = pwi->resNum;
		wi->x1 = pwi->x1;
		wi->y1 = pwi->y1;
		wi->zorder = 0;
		wi->state = pwi->state;
		wi->flags = pwi->flags;
		wi->shadow = 0;
		wi->field_390 = 0;
		wi->palette = 0;
		++_imagesNum;
	} else if (pwi->flags & kWIFIsPolygon) {
		drawWizPolygon(pwi->resNum, pwi->state, pwi->x1, pwi->flags, 0, 0, 0);
	} else {
		const Common::Rect *r = NULL;
		drawWizImage(pwi->resNum, pwi->state, pwi->x1, pwi->y1, 0, 0, 0, r, pwi->flags, 0, 0);
	}
}

uint8 *Wiz::drawWizImage(int resNum, int state, int x1, int y1, int zorder, int shadow, int field_390, const Common::Rect *clipBox, int flags, int dstResNum, int palette) {
	debug(3, "drawWizImage(resNum %d, x1 %d y1 %d flags 0x%X zorder %d shadow %d field_390 %d dstResNum %d palette %d)", resNum, x1, y1, flags, zorder, shadow, field_390, dstResNum, palette);
	uint8 *dataPtr;
	uint8 *dst = NULL;

	const uint8 *palPtr = NULL;
	if (_vm->_game.heversion >= 99) {
		if (palette) {
			palPtr = _vm->_hePalettes + palette * 1024 + 768;
		} else {
			palPtr = _vm->_hePalettes + 1792;
		}
	}

	const uint8 *xmapPtr = NULL;
	if (shadow) {
		dataPtr = _vm->getResourceAddress(rtImage, shadow);
		assert(dataPtr);
		xmapPtr = _vm->findResourceData(MKID_BE('XMAP'), dataPtr);
		assert(xmapPtr);
	}

	dataPtr = _vm->getResourceAddress(rtImage, resNum);
	assert(dataPtr);

	uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
	assert(wizh);
	uint32 comp   = READ_LE_UINT32(wizh + 0x0);
	uint32 width  = READ_LE_UINT32(wizh + 0x4);
	uint32 height = READ_LE_UINT32(wizh + 0x8);
	debug(3, "wiz_header.comp = %d wiz_header.w = %d wiz_header.h = %d", comp, width, height);

	uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
	assert(wizd);

	if (flags & kWIFHasPalette) {
		uint8 *pal = _vm->findWrappedBlock(MKID_BE('RGBS'), dataPtr, state, 0);
		assert(pal);
		_vm->setPaletteFromPtr(pal, 256);
	}

	uint8 *rmap = NULL;
	if (flags & kWIFRemapPalette) {
		rmap = _vm->findWrappedBlock(MKID_BE('RMAP'), dataPtr, state, 0);
		assert(rmap);
		if (_vm->_game.heversion <= 80 || READ_BE_UINT32(rmap) != 0x01234567) {
			uint8 *rgbs = _vm->findWrappedBlock(MKID_BE('RGBS'), dataPtr, state, 0);
			assert(rgbs);
			_vm->remapHEPalette(rgbs, rmap + 4);
		}
	}

	if (flags & kWIFPrint) {
		error("WizImage printing is unimplemented");
	}

	int32 cw, ch;
	if (flags & kWIFBlitToMemBuffer) {
		dst = (uint8 *)malloc(width * height);
		int transColor = (_vm->VAR_WIZ_TCOLOR != 0xFF) ? (_vm->VAR(_vm->VAR_WIZ_TCOLOR)) : 5;
		memset(dst, transColor, width * height);
		cw = width;
		ch = height;
	} else {
		if (dstResNum) {
			uint8 *dstPtr = _vm->getResourceAddress(rtImage, dstResNum);
			assert(dstPtr);
			dst = _vm->findWrappedBlock(MKID_BE('WIZD'), dstPtr, 0, 0);
			assert(dst);
			getWizImageDim(dstResNum, 0, cw, ch);
		} else {
			VirtScreen *pvs = &_vm->_virtscr[kMainVirtScreen];
			if (flags & kWIFMarkBufferDirty) {
				dst = pvs->getPixels(0, pvs->topline);
			} else {
				dst = pvs->getBackPixels(0, pvs->topline);
			}
			cw = pvs->w;
			ch = pvs->h;
		}
	}

	Common::Rect rScreen(cw, ch);
	if (clipBox) {
		Common::Rect clip(clipBox->left, clipBox->top, clipBox->right, clipBox->bottom);
		if (rScreen.intersects(clip)) {
			rScreen.clip(clip);
		} else {
			return 0;
		}
	} else if (_rectOverrideEnabled) {
		if (rScreen.intersects(_rectOverride)) {
			rScreen.clip(_rectOverride);
		} else {
			return 0;
		}
	}

	if (flags & kWIFRemapPalette) {
		palPtr = rmap + 4;
	}

	int transColor = -1;
	if (_vm->VAR_WIZ_TCOLOR != 0xFF) {
		uint8 *trns = _vm->findWrappedBlock(MKID_BE('TRNS'), dataPtr, state, 0);
		transColor = (trns == NULL) ? _vm->VAR(_vm->VAR_WIZ_TCOLOR) : -1;
	}

	switch (comp) {
	case 0:
		copyRawWizImage(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, flags, palPtr, transColor);
		break;
	case 1:
		if (flags & 0x80) {
			dst = _vm->getMaskBuffer(0, 0, 1);
			copyWizImageWithMask(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, 0, 2);
		} else if (flags & 0x100) {
			dst = _vm->getMaskBuffer(0, 0, 1);
			copyWizImageWithMask(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, 0, 1);
		} else {
			copyWizImage(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, flags, palPtr, xmapPtr);
		}
		break;
	case 2:
		copyRaw16BitWizImage(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, flags, palPtr, transColor);
		break;
	case 4:
		// TODO: Unknown image type
		break;
	case 5:
		// TODO: 16bit color compressed image
		copy16BitWizImage(dst, wizd, cw, ch, x1, y1, width, height, &rScreen, flags, palPtr, xmapPtr);
		break;
	default:
		error("drawWizImage: Unhandled wiz compression type %d", comp);
	}

	if (!(flags & kWIFBlitToMemBuffer) && dstResNum == 0) {
		Common::Rect rImage(x1, y1, x1 + width, y1 + height);
		if (rImage.intersects(rScreen)) {
			rImage.clip(rScreen);
			if (!(flags & kWIFBlitToFrontVideoBuffer) && (flags & (kWIFBlitToFrontVideoBuffer | kWIFMarkBufferDirty))) {
				++rImage.bottom;
				_vm->markRectAsDirty(kMainVirtScreen, rImage);
			} else {
				_vm->restoreBackgroundHE(rImage);
			}
		}
	}

	return dst;
}

struct PolygonDrawData {
	struct PolygonArea {
		int32 xmin;
		int32 xmax;
		int32 x1;
		int32 y1;
		int32 x2;
		int32 y2;
	};
	struct ResultArea {
		int32 dst_offs;
		int32 x_step;
		int32 y_step;
		int32 x_s;
		int32 y_s;
		int32 w;
	};
	Common::Point mat[4];
	PolygonArea *pa;
	ResultArea *ra;
	int rAreasNum;
	int pAreasNum;

	PolygonDrawData(int n) {
		memset(mat, 0, sizeof(mat));
		pa = new PolygonArea[n];
		for (int i = 0; i < n; ++i) {
			pa[i].xmin = 0x7FFFFFFF;
			pa[i].xmax = 0x80000000;
		}
		ra = new ResultArea[n];
		rAreasNum = 0;
		pAreasNum = n;
	}

	~PolygonDrawData() {
		delete[] pa;
		delete[] ra;
	}

	void transform(const Common::Point *tp1, const Common::Point *tp2, const Common::Point *sp1, const Common::Point *sp2) {
		int32 tx_acc = tp1->x << 16;
		int32 sx_acc = sp1->x << 16;
		int32 sy_acc = sp1->y << 16;
		uint16 dy = ABS(tp2->y - tp1->y) + 1;
		int32 tx_step = ((tp2->x - tp1->x) << 16) / dy;
		int32 sx_step = ((sp2->x - sp1->x) << 16) / dy;
		int32 sy_step = ((sp2->y - sp1->y) << 16) / dy;

		int y = tp1->y - mat[0].y;
		while (dy--) {
			assert(y >= 0 && y < pAreasNum);
			PolygonArea *ppa = &pa[y];
			int32 ttx = tx_acc >> 16;
			int32 tsx = sx_acc >> 16;
			int32 tsy = sy_acc >> 16;

			if (ppa->xmin > ttx) {
				ppa->xmin = ttx;
				ppa->x1 = tsx;
				ppa->y1 = tsy;
			}
			if (ppa->xmax < ttx) {
				ppa->xmax = ttx;
				ppa->x2 = tsx;
				ppa->y2 = tsy;
			}

			tx_acc += tx_step;
			sx_acc += sx_step;
			sy_acc += sy_step;

			if (tp2->y <= tp1->y) {
				--y;
			} else {
				++y;
			}
		}
	}
};

void Wiz::drawWizComplexPolygon(int resNum, int state, int po_x, int po_y, int shadow, int angle, int scale, const Common::Rect *r, int flags, int dstResNum, int palette) {
	Common::Point pts[4];

	polygonTransform(resNum, state, po_x, po_y, angle, scale, pts);
	drawWizPolygonTransform(resNum, state, pts, flags, shadow, dstResNum, palette);
}

void Wiz::drawWizPolygon(int resNum, int state, int id, int flags, int shadow, int dstResNum, int palette) {
	int i;
	WizPolygon *wp = NULL;
	for (i = 0; i < ARRAYSIZE(_polygons); ++i) {
		if (_polygons[i].id == id) {
			wp = &_polygons[i];
			break;
		}
	}
	if (!wp) {
		error("Polygon %d is not defined", id);
	}
	if (wp->numVerts != 5) {
		error("Invalid point count %d for Polygon %d", wp->numVerts, id);
	}

	drawWizPolygonTransform(resNum, state, wp->vert, flags, shadow, dstResNum, palette);
}

void Wiz::drawWizPolygonTransform(int resNum, int state, Common::Point *wp, int flags, int shadow, int dstResNum, int palette) {
	debug(3, "drawWizPolygonTransform(resNum %d, flags 0x%X, shadow %d dstResNum %d palette %d)", resNum, flags, shadow, dstResNum, palette);
	const Common::Rect *r = NULL;
	uint8 *srcWizBuf = NULL;
	bool freeBuffer = true;
	int i;

	if (_vm->_game.heversion >= 99) {
		if (getWizImageData(resNum, state, 0) != 0 || (flags & (kWIFRemapPalette | kWIFFlipX | kWIFFlipY)) || palette != 0) {
			flags |= kWIFBlitToMemBuffer;

			if (flags & 0x800000) {
				debug(0, "drawWizPolygonTransform() unhandled flag 0x800000");
			}
			srcWizBuf = drawWizImage(resNum, state, 0, 0, 0, shadow, 0, r, flags, 0, palette);
		} else {
			uint8 *dataPtr = _vm->getResourceAddress(rtImage, resNum);
			assert(dataPtr);
			srcWizBuf = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
			assert(srcWizBuf);
			freeBuffer = false;
		}
	} else {
		if (getWizImageData(resNum, state, 0) != 0) {
			srcWizBuf = drawWizImage(resNum, state, 0, 0, 0, shadow, 0, r, kWIFBlitToMemBuffer, 0, palette);
		} else {
			uint8 *dataPtr = _vm->getResourceAddress(rtImage, resNum);
			assert(dataPtr);
			srcWizBuf = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
			assert(srcWizBuf);
			freeBuffer = false;
		}
	}

	if (srcWizBuf) {
		uint8 *dst;
		int32 dstw, dsth, dstpitch, wizW, wizH;
		VirtScreen *pvs = &_vm->_virtscr[kMainVirtScreen];
		int transColor = (_vm->VAR_WIZ_TCOLOR != 0xFF) ? _vm->VAR(_vm->VAR_WIZ_TCOLOR) : 5;

		if (dstResNum) {
			uint8 *dstPtr = _vm->getResourceAddress(rtImage, dstResNum);
			assert(dstPtr);
			dst = _vm->findWrappedBlock(MKID_BE('WIZD'), dstPtr, 0, 0);
			assert(dst);
			getWizImageDim(dstResNum, 0, dstw, dsth);
			dstpitch = dstw;
		} else {
			if (flags & kWIFMarkBufferDirty) {
				dst = pvs->getPixels(0, 0);
			} else {
				dst = pvs->getBackPixels(0, 0);
			}
			dstw = pvs->w;
			dsth = pvs->h;
			dstpitch = pvs->pitch;
		}

		getWizImageDim(resNum, state, wizW, wizH);

		Common::Point bbox[4];
		bbox[0].x = 0;
		bbox[0].y = 0;
		bbox[1].x = wizW - 1;
		bbox[1].y = 0;
		bbox[2].x = wizW - 1;
		bbox[2].y = wizH - 1;
		bbox[3].x = 0;
		bbox[3].y = wizH - 1;

		int16 xmin_p, xmax_p, ymin_p, ymax_p;
		xmin_p = ymin_p = (int16)0x7FFF;
		xmax_p = ymax_p = (int16)0x8000;

		for (i = 0; i < 4; ++i) {
			xmin_p = MIN(wp[i].x, xmin_p);
			xmax_p = MAX(wp[i].x, xmax_p);
			ymin_p = MIN(wp[i].y, ymin_p);
			ymax_p = MAX(wp[i].y, ymax_p);
		}

		int16 xmin_b, xmax_b, ymin_b, ymax_b;
		xmin_b = ymin_b = (int16)0x7FFF;
		xmax_b = ymax_b = (int16)0x8000;

		for (i = 0; i < 4; ++i) {
			xmin_b = MIN(bbox[i].x, xmin_b);
			xmax_b = MAX(bbox[i].x, xmax_b);
			ymin_b = MIN(bbox[i].y, ymin_b);
			ymax_b = MAX(bbox[i].y, ymax_b);
		}

		PolygonDrawData pdd(ymax_p - ymin_p + 1);
		pdd.mat[0].x = xmin_p;
		pdd.mat[0].y = ymin_p;
		pdd.mat[1].x = xmax_p;
		pdd.mat[1].y = ymax_p;
		pdd.mat[2].x = xmin_b;
		pdd.mat[2].y = ymin_b;
		pdd.mat[3].x = xmax_b;
		pdd.mat[3].y = ymax_b;

		// precompute the transformation which remaps 'bbox' pixels to 'wp'
		for (i = 0; i < 3; ++i) {
			pdd.transform(&wp[i], &wp[i + 1], &bbox[i], &bbox[i + 1]);
		}
		pdd.transform(&wp[3], &wp[0], &bbox[3], &bbox[0]);

		pdd.rAreasNum = 0;
		PolygonDrawData::ResultArea *pra = &pdd.ra[0];
		int32 yoff = pdd.mat[0].y * dstpitch;
		int16 y_start = pdd.mat[0].y;
		for (i = 0; i < pdd.pAreasNum; ++i) {
			PolygonDrawData::PolygonArea *ppa = &pdd.pa[i];
			if (y_start >= 0 && y_start < dsth) {
				int16 x1 = ppa->xmin;
				if (x1 < 0) {
					x1 = 0;
				}
				int16 x2 = ppa->xmax;
				if (x2 >= dstw) {
					x2 = dstw - 1;
				}
				int16 w = x2 - x1 + 1;
				if (w > 0) {
					int16 width = ppa->xmax - ppa->xmin + 1;
					pra->x_step = ((ppa->x2 - ppa->x1) << 16) / width;
					pra->y_step = ((ppa->y2 - ppa->y1) << 16) / width;
					pra->dst_offs = yoff + x1;
					pra->w = w;
					pra->x_s = ppa->x1 << 16;
					pra->y_s = ppa->y1 << 16;
					int16 tmp = x1 - ppa->xmin;
					if (tmp != 0) {
						pra->x_s += pra->x_step * tmp;
						pra->y_s += pra->y_step * tmp;
					}
					++pra;
					++pdd.rAreasNum;
				}
			}
			++ppa;
			yoff += dstpitch;
			++y_start;
		}

		pra = &pdd.ra[0];
		for (i = 0; i < pdd.rAreasNum; ++i, ++pra) {
			uint8 *dstPtr = dst + pra->dst_offs;
			int32 w = pra->w;
			int32 x_acc = pra->x_s;
			int32 y_acc = pra->y_s;
			while (--w) {
				int32 src_offs = (y_acc >> 16) * wizW + (x_acc >> 16);
				assert(src_offs < wizW * wizH);
				x_acc += pra->x_step;
				y_acc += pra->y_step;
				if (transColor == -1 || transColor != srcWizBuf[src_offs]) {
					*dstPtr = srcWizBuf[src_offs];
				}
				dstPtr++;
			}
		}

		Common::Rect bound(xmin_p, ymin_p, xmax_p + 1, ymax_p + 1);
		if (flags & kWIFMarkBufferDirty) {
			_vm->markRectAsDirty(kMainVirtScreen, bound);
		} else {
			_vm->restoreBackgroundHE(bound);
		}

		if (freeBuffer)
			free(srcWizBuf);
	}
}

void Wiz::flushWizBuffer() {
	for (int i = 0; i < _imagesNum; ++i) {
		WizImage *pwi = &_images[i];
		if (pwi->flags & kWIFIsPolygon) {
			drawWizPolygon(pwi->resNum, pwi->state, pwi->x1, pwi->flags, pwi->shadow, 0, pwi->palette);
		} else {
			const Common::Rect *r = NULL;
			drawWizImage(pwi->resNum, pwi->state, pwi->x1, pwi->y1, pwi->zorder, pwi->shadow, pwi->field_390, r, pwi->flags, 0, pwi->palette);
		}
	}
	_imagesNum = 0;
}

void Wiz::loadWizCursor(int resId) {
	int32 x, y;
	getWizImageSpot(resId, 0, x, y);
	if (x < 0) {
		x = 0;
	} else if (x > 32) {
		x = 32;
	}
	if (y < 0) {
		y = 0;
	} else if (y > 32) {
		y = 32;
	}

	const Common::Rect *r = NULL;
	uint8 *cursor = drawWizImage(resId, 0, 0, 0, 0, 0, 0, r, kWIFBlitToMemBuffer, 0, 0);
	int32 cw, ch;
	getWizImageDim(resId, 0, cw, ch);
	_vm->setCursorHotspot(x, y);
	_vm->setCursorFromBuffer(cursor, cw, ch, cw);

	// Since we set up cursor palette for default cursor, disable it now
	CursorMan.disableCursorPalette(true);

	free(cursor);
}

void Wiz::displayWizComplexImage(const WizParameters *params) {
	int sourceImage = 0;
	if (params->processFlags & kWPFMaskImg) {
		sourceImage = params->sourceImage;
		debug(0, "displayWizComplexImage() unhandled flag kWPFMaskImg");
	}
	int palette = 0;
	if (params->processFlags & kWPFPaletteNum) {
		palette = params->img.palette;
	}
	int scale = 256;
	if (params->processFlags & kWPFScaled) {
		scale = params->scale;
	}
	int rotationAngle = 0;
	if (params->processFlags & kWPFRotate) {
		rotationAngle = params->angle;
	}
	int state = 0;
	if (params->processFlags & kWPFNewState) {
		state = params->img.state;
	}
	int flags = 0;
	if (params->processFlags & kWPFNewFlags) {
		flags = params->img.flags;
	}
	int po_x = 0;
	int po_y = 0;
	if (params->processFlags & kWPFSetPos) {
		po_x = params->img.x1;
		po_y = params->img.y1;
	}
	int shadow = 0;
	if (params->processFlags & kWPFShadow) {
		shadow = params->img.shadow;
	}
	int field_390 = 0;
	if (params->processFlags & 0x200000) {
		field_390 = params->img.field_390;
		debug(0, "displayWizComplexImage() unhandled flag 0x200000");
	}
	const Common::Rect *r = NULL;
	if (params->processFlags & kWPFClipBox) {
		r = &params->box;
	}
	int dstResNum = 0;
	if (params->processFlags & kWPFDstResNum) {
		dstResNum = params->dstResNum;
	}
	if (_vm->_game.heversion >= 99 && params->processFlags & kWPFRemapPalette) {
		remapWizImagePal(params);
		flags |= kWIFRemapPalette;
	}

	if (_vm->_fullRedraw && dstResNum == 0) {
		if (sourceImage != 0 || (params->processFlags & (kWPFScaled | kWPFRotate)))
			error("Can't do this command in the enter script.");

		assert(_imagesNum < ARRAYSIZE(_images));
		WizImage *pwi = &_images[_imagesNum];
		pwi->resNum = params->img.resNum;
		pwi->x1 = po_x;
		pwi->y1 = po_y;
		pwi->zorder = params->img.zorder;
		pwi->state = state;
		pwi->flags = flags;
		pwi->shadow = shadow;
		pwi->field_390 = field_390;
		pwi->palette = palette;
		++_imagesNum;
	} else {
		if (sourceImage != 0) {
			// TODO: Add support for kWPFMaskImg
			drawWizImage(params->sourceImage, state, po_x, po_y, params->img.zorder, shadow, field_390, r, flags, dstResNum, palette);
		} else if (params->processFlags & (kWPFScaled | kWPFRotate)) {
			drawWizComplexPolygon(params->img.resNum, state, po_x, po_y, shadow, rotationAngle, scale, r, flags, dstResNum, palette);
		} else {
			if (flags & kWIFIsPolygon) {
				drawWizPolygon(params->img.resNum, state, po_x, flags, shadow, dstResNum, palette);
			} else {
				drawWizImage(params->img.resNum, state, po_x, po_y, params->img.zorder, shadow, field_390, r, flags, dstResNum, palette);
			}
		}
	}
}

void Wiz::createWizEmptyImage(int resNum, int img_x, int img_y, int img_w, int img_h) {
	const uint16 flags = 0xB;
	int res_size = 0x1C;
	if (flags & 1) {
		res_size += 0x308;
	}
	if (flags & 2) {
		res_size += 0x10;
	}
	if (flags & 8) {
		res_size += 0x10C;
	}
	res_size += 8 + img_w * img_h;

	const uint8 *palPtr;
	if (_vm->_game.heversion >= 99) {
		palPtr = _vm->_hePalettes + 1024;
	} else {
		palPtr = _vm->_currentPalette;
	}
	uint8 *res_data = _vm->_res->createResource(rtImage, resNum, res_size);
	if (!res_data) {
		_vm->VAR(119) = -1;
	} else {
		_vm->VAR(119) = 0;
		WRITE_BE_UINT32(res_data, 'AWIZ'); res_data += 4;
		WRITE_BE_UINT32(res_data, res_size); res_data += 4;
		WRITE_BE_UINT32(res_data, 'WIZH'); res_data += 4;
		WRITE_BE_UINT32(res_data, 0x14); res_data += 4;
		WRITE_LE_UINT32(res_data, 0); res_data += 4;
		WRITE_LE_UINT32(res_data, img_w); res_data += 4;
		WRITE_LE_UINT32(res_data, img_h); res_data += 4;
		if (flags & 1) {
			WRITE_BE_UINT32(res_data, 'RGBS'); res_data += 4;
			WRITE_BE_UINT32(res_data, 0x308); res_data += 4;
			memcpy(res_data, palPtr, 0x300); res_data += 0x300;
		}
		if (flags & 2) {
			WRITE_BE_UINT32(res_data, 'SPOT'); res_data += 4;
			WRITE_BE_UINT32(res_data, 0x10); res_data += 4;
			WRITE_BE_UINT32(res_data, img_x); res_data += 4;
			WRITE_BE_UINT32(res_data, img_y); res_data += 4;
		}
		if (flags & 8) {
			WRITE_BE_UINT32(res_data, 'RMAP'); res_data += 4;
			WRITE_BE_UINT32(res_data, 0x10C); res_data += 4;
			WRITE_BE_UINT32(res_data, 0); res_data += 4;
			for (int i = 0; i < 256; ++i) {
				*res_data++ = i;
			}
		}
		WRITE_BE_UINT32(res_data, 'WIZD'); res_data += 4;
		WRITE_BE_UINT32(res_data, 8 + img_w * img_h); res_data += 4;
	}
	_vm->_res->setModified(rtImage, resNum);
}

void Wiz::fillWizRect(const WizParameters *params) {
	int state = 0;
	if (params->processFlags & kWPFNewState) {
		state = params->img.state;
	}
	uint8 *dataPtr = _vm->getResourceAddress(rtImage, params->img.resNum);
	if (dataPtr) {
		uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
		assert(wizh);
		int c = READ_LE_UINT32(wizh + 0x0);
		int w = READ_LE_UINT32(wizh + 0x4);
		int h = READ_LE_UINT32(wizh + 0x8);
		assert(c == 0);
		Common::Rect areaRect, imageRect(w, h);
		if (params->processFlags & kWPFClipBox) {
			if (!imageRect.intersects(params->box)) {
				return;
			}
			imageRect.clip(params->box);
		}
		if (params->processFlags & kWPFClipBox2) {
			areaRect = params->box2;
		} else {
			areaRect = imageRect;
		}
		uint16 color = _vm->VAR(93);
		if (params->processFlags & kWPFFillColor) {
			color = params->fillColor;
		}
		if (areaRect.intersects(imageRect)) {
			areaRect.clip(imageRect);
			uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
			assert(wizd);
			int dx = areaRect.width();
			int dy = areaRect.height();
			wizd += areaRect.top * w + areaRect.left;
			while (dy--) {
				memset(wizd, color, dx);
				wizd += w;
			}
		}
	}
	_vm->_res->setModified(rtImage, params->img.resNum);
}

struct drawProcP {
	Common::Rect *imageRect;
	uint8 *wizd;
	int width;
};

static void drawProc(int x, int y, int c, void *data) {
	drawProcP *param = (drawProcP *)data;

	if (param->imageRect->contains(x, y)) {
		*(param->wizd + y * param->width + x) = c;
	}
}

void Wiz::fillWizLine(const WizParameters *params) {
	if (params->processFlags & kWPFClipBox2) {
		int state = 0;
		if (params->processFlags & kWPFNewState) {
			state = params->img.state;
		}
		uint8 *dataPtr = _vm->getResourceAddress(rtImage, params->img.resNum);
		if (dataPtr) {
			uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
			assert(wizh);
			int c = READ_LE_UINT32(wizh + 0x0);
			int w = READ_LE_UINT32(wizh + 0x4);
			int h = READ_LE_UINT32(wizh + 0x8);
			assert(c == 0);
			Common::Rect imageRect(w, h);
			if (params->processFlags & kWPFClipBox) {
				if (!imageRect.intersects(params->box)) {
					return;
				}
				imageRect.clip(params->box);
			}
			uint16 color = _vm->VAR(93);
			if (params->processFlags & kWPFFillColor) {
				color = params->fillColor;
			}
			uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
			assert(wizd);
			int x1 = params->box2.left;
			int y1 = params->box2.top;
			int x2 = params->box2.right;
			int y2 = params->box2.bottom;

			drawProcP lineP;

			lineP.imageRect = &imageRect;
			lineP.wizd = wizd;
			lineP.width = w;

			if (params->processFlags & kWPFParams) {
				assert (params->params2 == 1); // Catch untested usage
				Graphics::drawThickLine(x1, y1, x2, y2, params->params1, color, drawProc, &lineP);
			} else {

				Graphics::drawLine(x1, y1, x2, y2, color, drawProc, &lineP);
			}
		}
	}
	_vm->_res->setModified(rtImage, params->img.resNum);
}

void Wiz::fillWizPixel(const WizParameters *params) {
	if (params->processFlags & kWPFClipBox2) {
		int px = params->box2.left;
		int py = params->box2.top;
		uint8 *dataPtr = _vm->getResourceAddress(rtImage, params->img.resNum);
		if (dataPtr) {
			int state = 0;
			if (params->processFlags & kWPFNewState) {
				state = params->img.state;
			}
			uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
			assert(wizh);
			int c = READ_LE_UINT32(wizh + 0x0);
			int w = READ_LE_UINT32(wizh + 0x4);
			int h = READ_LE_UINT32(wizh + 0x8);
			assert(c == 0);
			Common::Rect imageRect(w, h);
			if (params->processFlags & kWPFClipBox) {
				if (!imageRect.intersects(params->box)) {
					return;
				}
				imageRect.clip(params->box);
			}
			uint16 color = _vm->VAR(93);
			if (params->processFlags & kWPFFillColor) {
				color = params->fillColor;
			}
			if (imageRect.contains(px, py)) {
				uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), dataPtr, state, 0);
				assert(wizd);
				*(wizd + py * w + px) = color;
			}
		}
	}
	_vm->_res->setModified(rtImage, params->img.resNum);
}

void Wiz::remapWizImagePal(const WizParameters *params) {
	int st = (params->processFlags & kWPFNewState) ? params->img.state : 0;
	int num = params->remapNum;
	const uint8 *index = params->remapIndex;
	uint8 *iwiz = _vm->getResourceAddress(rtImage, params->img.resNum);
	assert(iwiz);
	uint8 *rmap = _vm->findWrappedBlock(MKID_BE('RMAP'), iwiz, st, 0) ;
	assert(rmap);
	WRITE_BE_UINT32(rmap, 0x01234567);
	while (num--) {
		uint8 idx = *index++;
		rmap[4 + idx] = params->remapColor[idx];
	}
	_vm->_res->setModified(rtImage, params->img.resNum);
}

void Wiz::processWizImage(const WizParameters *params) {
	byte filename[260];

	debug(3, "processWizImage: processMode %d", params->processMode);
	switch (params->processMode) {
	case 0:
		// Used in racedemo
		break;
	case 1:
		displayWizComplexImage(params);
		break;
	case 2:
		captureWizImage(params->img.resNum, params->box, (params->img.flags & kWIFBlitToFrontVideoBuffer) != 0, params->compType);
		break;
	case 3:
		if (params->processFlags & kWPFUseFile) {
			Common::File f;

			memcpy(filename, params->filename, 260);
			_vm->convertFilePath(filename, sizeof(filename));

			if (f.open((const char *)filename)) {
				uint32 id = f.readUint32BE();
				if (id == MKID_BE('AWIZ') || id == MKID_BE('MULT')) {
					uint32 size = f.readUint32BE();
					f.seek(0, SEEK_SET);
					byte *p = _vm->_res->createResource(rtImage, params->img.resNum, size);
					if (f.read(p, size) != size) {
						_vm->_res->nukeResource(rtImage, params->img.resNum);
						error("i/o error when reading '%s'", filename);
						_vm->VAR(_vm->VAR_GAME_LOADED) = -2;
						_vm->VAR(119) = -2;
					} else {
						_vm->_res->setModified(rtImage, params->img.resNum);
						_vm->VAR(_vm->VAR_GAME_LOADED) = 0;
						_vm->VAR(119) = 0;
					}
				} else {
					_vm->VAR(_vm->VAR_GAME_LOADED) = -1;
					_vm->VAR(119) = -1;
				}
				f.close();
			} else {
				_vm->VAR(_vm->VAR_GAME_LOADED) = -3;
				_vm->VAR(119) = -3;
				debug(0, "Unable to open for read '%s'", filename);
			}
		}
		break;
	case 4:
		if (params->processFlags & kWPFUseFile) {
			Common::DumpFile f;

			switch (params->fileWriteMode) {
			case 2:
				_vm->VAR(119) = -1;
				break;
			case 1:
				// TODO Write image to file
				break;
			case 0:
				memcpy(filename, params->filename, 260);
				_vm->convertFilePath(filename, sizeof(filename));

				if (!f.open((const char *)filename)) {
					debug(0, "Unable to open for write '%s'", filename);
					_vm->VAR(119) = -3;
				} else {
					byte *p = _vm->getResourceAddress(rtImage, params->img.resNum);
					uint32 size = READ_BE_UINT32(p + 4);
					if (f.write(p, size) != size) {
						error("i/o error when writing '%s'", params->filename);
						_vm->VAR(119) = -2;
					} else {
						_vm->VAR(119) = 0;
					}
					f.close();
				}
				break;
			default:
				error("processWizImage: processMode 4 unhandled fileWriteMode %d", params->fileWriteMode);
			}
		}
		break;
	case 6:
		remapWizImagePal(params);
		break;
	// HE 99+
	case 7:
		// Used in PuttsFunShop/SamsFunShop/soccer2004
		// TODO: Capture polygon
		_vm->_res->setModified(rtImage, params->img.resNum);
		break;
	case 8: {
			int img_w = 640;
			if (params->processFlags & kWPFUseDefImgWidth) {
				img_w = params->resDefImgW;
			}
			int img_h = 480;
			if (params->processFlags & kWPFUseDefImgHeight) {
				img_h = params->resDefImgH;
			}
			int img_x = 0;
			int img_y = 0;
			if (params->processFlags & 1) {
				img_x = params->img.x1;
				img_y = params->img.y1;
			}
			createWizEmptyImage(params->img.resNum, img_x, img_y, img_w, img_h);
		}
		break;
	case 9:
		fillWizRect(params);
		break;
	case 10:
		fillWizLine(params);
		break;
	case 11:
		fillWizPixel(params);
		break;
	case 12:
		fillWizFlood(params);
		break;
	case 13:
		// Used for text in FreddisFunShop/PuttsFunShop/SamsFunShop
		// TODO: Start Font
		break;
	case 14:
		// Used for text in FreddisFunShop/PuttsFunShop/SamsFunShop
		// TODO: End Font
		break;
	case 15:
		// Used for text in FreddisFunShop/PuttsFunShop/SamsFunShop
		// TODO: Create Font
		break;
	case 16:
		// TODO: Render Font String
		error("Render Font String");
		break;
	case 17:
		// Used in to draw circles in FreddisFunShop/PuttsFunShop/SamsFunShop
		// TODO: Ellipse
		_vm->_res->setModified(rtImage, params->img.resNum);
		break;
	default:
		error("Unhandled processWizImage mode %d", params->processMode);
	}
}

void Wiz::getWizImageDim(int resNum, int state, int32 &w, int32 &h) {
	uint8 *dataPtr = _vm->getResourceAddress(rtImage, resNum);
	assert(dataPtr);
	uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
	assert(wizh);
	w = READ_LE_UINT32(wizh + 0x4);
	h = READ_LE_UINT32(wizh + 0x8);
}

void Wiz::getWizImageSpot(int resId, int state, int32 &x, int32 &y) {
	uint8 *dataPtr = _vm->getResourceAddress(rtImage, resId);
	assert(dataPtr);
	uint8 *spotPtr = _vm->findWrappedBlock(MKID_BE('SPOT'), dataPtr, state, 0);
	if (spotPtr) {
		x = READ_LE_UINT32(spotPtr + 0);
		y = READ_LE_UINT32(spotPtr + 4);
	} else {
		x = 0;
		y = 0;
	}
}

int Wiz::getWizImageData(int resNum, int state, int type) {
	uint8 *dataPtr, *wizh;

	dataPtr = _vm->getResourceAddress(rtImage, resNum);
	assert(dataPtr);

	switch (type) {
	case 0:
		wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), dataPtr, state, 0);
		assert(wizh);
		return READ_LE_UINT32(wizh + 0x0);
	case 1:
		return (_vm->findWrappedBlock(MKID_BE('RGBS'), dataPtr, state, 0) != NULL) ? 1 : 0;
	case 2:
		return (_vm->findWrappedBlock(MKID_BE('RMAP'), dataPtr, state, 0) != NULL) ? 1 : 0;
	case 3:
		return (_vm->findWrappedBlock(MKID_BE('TRNS'), dataPtr, state, 0) != NULL) ? 1 : 0;
	case 4:
		return (_vm->findWrappedBlock(MKID_BE('XMAP'), dataPtr, state, 0) != NULL) ? 1 : 0;
	default:
		error("getWizImageData: Unknown type %d", type);
	}
}

int Wiz::getWizImageStates(int resNum) {
	const uint8 *dataPtr = _vm->getResourceAddress(rtImage, resNum);
	assert(dataPtr);
	if (READ_BE_UINT32(dataPtr) == MKID_BE('MULT')) {
		const byte *offs, *wrap;

		wrap = _vm->findResource(MKID_BE('WRAP'), dataPtr);
		if (wrap == NULL)
			return 1;

		offs = _vm->findResourceData(MKID_BE('OFFS'), wrap);
		if (offs == NULL)
			return 1;

		return _vm->getResourceDataSize(offs) / 4;
	} else {
		return 1;
	}
}

int Wiz::isWizPixelNonTransparent(int resNum, int state, int x, int y, int flags) {
	int ret = 0;
	uint8 *data = _vm->getResourceAddress(rtImage, resNum);
	assert(data);
	uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), data, state, 0);
	assert(wizh);
	int c = READ_LE_UINT32(wizh + 0x0);
	int w = READ_LE_UINT32(wizh + 0x4);
	int h = READ_LE_UINT32(wizh + 0x8);
	uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), data, state, 0);
	assert(wizd);
	if (x >= 0 && x < w && y >= 0 && y < h) {
		if (flags & kWIFFlipX) {
			x = w - x - 1;
		}
		if (flags & kWIFFlipY) {
			y = h - y - 1;
		}
		switch (c) {
		case 0:
			if (_vm->_game.heversion >= 99) {
				ret = getRawWizPixelColor(wizd, x, y, w, h, 1, _vm->VAR(_vm->VAR_WIZ_TCOLOR)) != _vm->VAR(_vm->VAR_WIZ_TCOLOR) ? 1 : 0;
			} else {
				ret = 0;
			}
			break;
		case 1:
			ret = isWizPixelNonTransparent(wizd, x, y, w, h, 1);
			break;
		case 2:
				ret = getRawWizPixelColor(wizd, x, y, w, h, 2, _vm->VAR(_vm->VAR_WIZ_TCOLOR)) != _vm->VAR(_vm->VAR_WIZ_TCOLOR) ? 1 : 0;
			break;
		case 4:
			// TODO: Unknown image type
			ret = 1;
			debug(0, "isWizPixelNonTransparent: Unhandled wiz compression type %d", c);
			break;
		case 5:
			ret = isWizPixelNonTransparent(wizd, x, y, w, h, 2);
			break;
		default:
			error("isWizPixelNonTransparent: Unhandled wiz compression type %d", c);
			break;
		}
	}
	return ret;
}

uint16 Wiz::getWizPixelColor(int resNum, int state, int x, int y) {
	uint16 color = 0;
	uint8 *data = _vm->getResourceAddress(rtImage, resNum);
	assert(data);
	uint8 *wizh = _vm->findWrappedBlock(MKID_BE('WIZH'), data, state, 0);
	assert(wizh);
	int c = READ_LE_UINT32(wizh + 0x0);
	int w = READ_LE_UINT32(wizh + 0x4);
	int h = READ_LE_UINT32(wizh + 0x8);
	uint8 *wizd = _vm->findWrappedBlock(MKID_BE('WIZD'), data, state, 0);
	assert(wizd);
	switch (c) {
	case 0:
		if (_vm->_game.heversion >= 99) {
			color = getRawWizPixelColor(wizd, x, y, w, h, 1, _vm->VAR(_vm->VAR_WIZ_TCOLOR));
		} else {
			color = _vm->VAR(_vm->VAR_WIZ_TCOLOR);
		}
		break;
	case 1:
		color = getWizPixelColor(wizd, x, y, w, h, 1, _vm->VAR(_vm->VAR_WIZ_TCOLOR));
		break;
	case 2:
		color = getRawWizPixelColor(wizd, x, y, w, h, 2, _vm->VAR(_vm->VAR_WIZ_TCOLOR));
		break;
	case 4:
		// TODO: Unknown image type
		debug(0, "getWizPixelColor: Unhandled wiz compression type %d", c);
		break;
	case 5:
		color = getWizPixelColor(wizd, x, y, w, h, 2, _vm->VAR(_vm->VAR_WIZ_TCOLOR));
		break;
	default:
		error("getWizPixelColor: Unhandled wiz compression type %d", c);
		break;
	}
	return color;
}

int ScummEngine_v90he::computeWizHistogram(int resNum, int state, int x, int y, int w, int h) {
	writeVar(0, 0);
	defineArray(0, kDwordArray, 0, 0, 0, 255);
	if (readVar(0) != 0) {
		Common::Rect rCapt(x, y, w + 1, h + 1);
		uint8 *data = getResourceAddress(rtImage, resNum);
		assert(data);
		uint8 *wizh = findWrappedBlock(MKID_BE('WIZH'), data, state, 0);
		assert(wizh);
		int c = READ_LE_UINT32(wizh + 0x0);
		w = READ_LE_UINT32(wizh + 0x4);
		h = READ_LE_UINT32(wizh + 0x8);
		Common::Rect rWiz(w, h);
		uint8 *wizd = findWrappedBlock(MKID_BE('WIZD'), data, state, 0);
		assert(wizd);
		if (rCapt.intersects(rWiz)) {
			rCapt.clip(rWiz);
			uint32 histogram[256];
			memset(histogram, 0, sizeof(histogram));
			switch (c) {
			case 0:
				_wiz->computeRawWizHistogram(histogram, wizd, w, rCapt);
				break;
			case 1:
				_wiz->computeWizHistogram(histogram, wizd, rCapt);
				break;
			default:
				error("computeWizHistogram: Unhandled wiz compression type %d", c);
				break;
			}
			for (int i = 0; i < 256; ++i) {
				writeArray(0, 0, i, histogram[i]);
			}
		}
	}
	return readVar(0);
}

} // End of namespace Scumm

#endif // ENABLE_HE
