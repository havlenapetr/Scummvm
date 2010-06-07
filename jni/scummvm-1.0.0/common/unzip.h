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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/common/unzip.h $
 * $Id: unzip.h 36021 2009-01-23 04:45:44Z fingolfin $
 */

#ifndef COMMON_UNZIP_H
#define COMMON_UNZIP_H

#ifdef USE_ZLIB

#include "common/scummsys.h"
#include "common/archive.h"

typedef void *unzFile;

namespace Common {

class ZipArchive : public Archive {
	void *_zipFile;

public:
	/**
	 * Open the .zip archive with the given file name.
	 */
	ZipArchive(const String &name);

	/**
	 * Open the .zip archive to which the given FSNode refers to.
	 */
	ZipArchive(const FSNode &node);

	/**
	 * Open a .zip file from a stream. This takes ownership of the stream,
	 * in particular, it is closed when the ZipArchive is deleted.
	 */
	ZipArchive(SeekableReadStream *stream);


	~ZipArchive();

	bool isOpen() const;

	virtual bool hasFile(const String &name);
	virtual int listMembers(ArchiveMemberList &list);
	virtual ArchiveMemberPtr getMember(const String &name);
	virtual SeekableReadStream *createReadStreamForMember(const String &name) const;
};

}	// End of namespace Common

#endif // USE_ZLIB

#endif /* _unz_H */
