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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/graphics/thumbnail.h $
 * $Id: thumbnail.h 34053 2008-08-20 14:03:34Z lordhoto $
 */

#ifndef GRAPHICS_THUMBNAIL_H
#define GRAPHICS_THUMBNAIL_H

#include "common/stream.h"
#include "graphics/surface.h"

namespace Graphics {

/**
 * Checks for presence of the thumbnail save header.
 * Seeks automatically back to start position after check.
 *
 * @param in	stream to check for header
 */
bool checkThumbnailHeader(Common::SeekableReadStream &in);

/**
 * Skips a thumbnail header, if present.
 *
 * @param in	stream to process
 */
bool skipThumbnailHeader(Common::SeekableReadStream &in);

/**
 * Lodas a thumbnail from the given input stream.
 * The loaded thumbnail will be automatically converted to the
 * current overlay pixelformat.
 */
bool loadThumbnail(Common::SeekableReadStream &in, Graphics::Surface &to);

/**
 * Saves a thumbnail to the given write stream.
 * Automatically creates a thumbnail from screen contents.
 */
bool saveThumbnail(Common::WriteStream &out);

/**
 * Saves a (given) thumbnail to the given write stream.
 */
bool saveThumbnail(Common::WriteStream &out, const Graphics::Surface &thumb);

} // end of namespace Graphics

#endif

