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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/graphics/iff.cpp $
 * $Id: iff.cpp 41465 2009-06-12 08:51:05Z peres001 $
 */

#include "graphics/iff.h"
#include "graphics/surface.h"

#include "common/util.h"


namespace Common {

// this really belongs to iff_container.cpp, but we don't want
// to put only this in a source file
char *ID2string(Common::IFF_ID id) {
	static char str[] = "abcd";

	str[0] = (char)(id >> 24 & 0xff);
	str[1] = (char)(id >> 16 & 0xff);
	str[2] = (char)(id >>  8 & 0xff);
	str[3] = (char)(id >>  0 & 0xff);

	return str;
}

}


namespace Graphics {

void BMHD::load(Common::ReadStream *stream) {
	assert(stream);
	stream->read(this, sizeof(BMHD));
	width = FROM_BE_16(width);
	height = FROM_BE_16(height);
	x = FROM_BE_16(x);
	y = FROM_BE_16(y);
	transparentColor = FROM_BE_16(transparentColor);
	pageWidth = FROM_BE_16(pageWidth);
	pageHeight = FROM_BE_16(pageHeight);
}


void ILBMDecoder::loadHeader(Common::ReadStream *stream) {
	_header.load(stream);
}

void ILBMDecoder::loadBitmap(uint32 mode, byte *buffer, Common::ReadStream *stream) {
	assert(stream);
	uint32 numPlanes = MIN(mode & ILBM_UNPACK_PLANES, (uint32)_header.depth);
	assert(numPlanes == 1 || numPlanes == 2 || numPlanes == 3 || numPlanes == 4 || numPlanes == 5 || numPlanes == 8);

	bool packPixels = (mode & ILBM_PACK_PLANES) != 0;
	if (numPlanes != 1 && numPlanes != 2 && numPlanes != 4) {
		packPixels = false;
	}

	uint32 outPitch = _header.width;
	if (packPixels) {
		outPitch /= (8 / numPlanes);
	}
	byte *out = buffer;

	switch (_header.pack) {
	case 1: {	// PackBits compressed bitmap
		Graphics::PackBitsReadStream packStream(*stream);

		// setup a buffer to hold enough data to build a line in the output
		uint32 scanlineWidth = ((_header.width + 15)/16) << 1;
		byte *scanline = new byte[scanlineWidth * _header.depth];

		for (uint i = 0; i < _header.height; ++i) {
			byte *s = scanline;
			for (uint32 j = 0; j < _header.depth; ++j) {
				packStream.read(s, scanlineWidth);
				s += scanlineWidth;
			}

			planarToChunky(out, outPitch, scanline, scanlineWidth, numPlanes, packPixels);
			out += outPitch;
		}

		delete []scanline;
		break;
	}

	default:
		// implement other compression types here!
		error("only RLE compressed ILBM files are supported");
		break;
	}
}

void ILBMDecoder::planarToChunky(byte *out, uint32 outPitch, byte *in, uint32 inWidth, uint32 nPlanes, bool packPlanes) {
	byte pix, ofs, bit;
	byte *s;

	uint32 pixels = outPitch;
	if (packPlanes) {
		pixels *= (8 / nPlanes);
	}

	for (uint32 x = 0; x < pixels; ++x) {

		pix = 0;
		ofs = x >> 3;
		bit = 0x80 >> (x & 7);

		// first build a pixel by scanning all the usable planes in the input
		s = in;
		for (uint32 plane = 0; plane < nPlanes; ++plane) {
			if (s[ofs] & bit) {
				pix |= (1 << plane);
			}
			s += inWidth;
		}


		// then output the pixel according to the requested packing
		if (!packPlanes) {
			out[x] = pix;
		} else
		if (nPlanes == 1) {
			out[x/8] |= (pix << (x & 7));
		} else
		if (nPlanes == 2) {
			out[x/4] |= (pix << ((x & 3) << 1));
		} else
		if (nPlanes == 4) {
			out[x/2] |= (pix << ((x & 1) << 2));
		}
	}

}


void PBMDecoder::loadHeader(Common::ReadStream *stream) {
	_header.load(stream);
}


void PBMDecoder::loadBitmap(byte *buffer, Common::ReadStream *stream) {
	uint32 outSize = _header.width * _header.height;

	switch (_header.pack) {
	case 0:
		stream->read(buffer, outSize);
		break;

	case 1: {
		PackBitsReadStream packStream(*stream);
		packStream.read(buffer, outSize);
		break;
		}
	}
}


struct PBMLoader {
	PBMDecoder _decoder;
	Surface *_surface;
	byte *_colors;

	void load(Common::ReadStream &input, Surface &surface, byte *&colors) {
		_surface = &surface;
		_colors = colors;
		Common::IFFParser parser(&input);
		Common::Functor1Mem< Common::IFFChunk&, bool, PBMLoader > c(this, &PBMLoader::callback);
		parser.parse(c);
	}

	bool callback(Common::IFFChunk &chunk) {
		switch (chunk._type) {
		case ID_BMHD:
			_decoder.loadHeader(chunk._stream);
			break;

		case ID_CMAP:
			if (_colors) {
				chunk._stream->read(_colors, chunk._size);
			}
			break;

		case ID_BODY:
			if (_surface) {
				_surface->create(_decoder._header.width, _decoder._header.height, 1);
				_decoder.loadBitmap((byte*)_surface->pixels, chunk._stream);
			}
			return true;	// stop the parser
		}

		return false;
	}
};

void decodePBM(Common::ReadStream &input, Surface &surface, byte *colors) {
	PBMLoader loader;
	loader.load(input, surface, colors);
}




PackBitsReadStream::PackBitsReadStream(Common::ReadStream &input) : _input(&input) {
}

PackBitsReadStream::~PackBitsReadStream() {
}

bool PackBitsReadStream::eos() const {
	return _input->eos();
}

uint32 PackBitsReadStream::read(void *dataPtr, uint32 dataSize) {
	byte *out = (byte*)dataPtr;
	uint32 left = dataSize;

	uint32 lenR = 0, lenW = 0;
	while (left > 0 && !_input->eos()) {
		lenR = _input->readByte();

		if (lenR == 128) {
			// no-op
			lenW = 0;
		} else if (lenR <= 127) {
			// literal run
			lenR++;
			lenW = MIN(lenR, left);
			for (uint32 j = 0; j < lenW; j++) {
				*out++ = _input->readByte();
			}
			for ( ; lenR > lenW; lenR--) {
				_input->readByte();
			}
		} else {  // len > 128
			// expand run
			lenW = MIN((256 - lenR) + 1, left);
			byte val = _input->readByte();
			memset(out, val, lenW);
			out += lenW;
		}

		left -= lenW;
	}

	return dataSize - left;
}


}
