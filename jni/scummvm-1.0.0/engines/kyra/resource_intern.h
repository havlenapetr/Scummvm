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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/kyra/resource_intern.h $
 * $Id: resource_intern.h 36021 2009-01-23 04:45:44Z fingolfin $
 *
 */

#ifndef KYRA_RESOURCE_INTERN_H
#define KYRA_RESOURCE_INTERN_H

#include "common/archive.h"
#include "common/hash-str.h"
#include "common/hashmap.h"
#include "common/str.h"
#include "common/list.h"

namespace Kyra {

class Resource;

class PlainArchive : public Common::Archive {
public:
	struct InputEntry {
		Common::String name;

		uint32 offset;
		uint32 size;
	};

	typedef Common::List<InputEntry> FileInputList;

	PlainArchive(Common::SharedPtr<Common::ArchiveMember> file, const FileInputList &files);

	bool hasFile(const Common::String &name);
	int listMembers(Common::ArchiveMemberList &list);
	Common::ArchiveMemberPtr getMember(const Common::String &name);
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const;
private:
	struct Entry {
		uint32 offset;
		uint32 size;
	};

	typedef Common::HashMap<Common::String, Entry, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> FileMap;

	Common::SharedPtr<Common::ArchiveMember> _file;
	FileMap _files;
};

class CachedArchive : public Common::Archive {
public:
	struct InputEntry {
		Common::String name;

		byte *data;
		uint32 size;
	};

	typedef Common::List<InputEntry> FileInputList;

	CachedArchive(const FileInputList &files);
	~CachedArchive();

	bool hasFile(const Common::String &name);
	int listMembers(Common::ArchiveMemberList &list);
	Common::ArchiveMemberPtr getMember(const Common::String &name);
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const;
private:
	struct Entry {
		byte *data;
		uint32 size;
	};

	typedef Common::HashMap<Common::String, Entry, Common::IgnoreCase_Hash, Common::IgnoreCase_EqualTo> FileMap;
	FileMap _files;
};


class ResArchiveLoader {
public:
	virtual ~ResArchiveLoader() {}
	virtual bool checkFilename(Common::String filename) const = 0;
	virtual bool isLoadable(const Common::String &filename, Common::SeekableReadStream &stream) const = 0;
	virtual Common::Archive *load(Common::SharedPtr<Common::ArchiveMember> file, Common::SeekableReadStream &stream) const = 0;
};

class ResLoaderPak : public ResArchiveLoader {
public:
	bool checkFilename(Common::String filename) const;
	bool isLoadable(const Common::String &filename, Common::SeekableReadStream &stream) const;
	Common::Archive *load(Common::SharedPtr<Common::ArchiveMember> file, Common::SeekableReadStream &stream) const;
};

class ResLoaderInsMalcolm : public ResArchiveLoader {
public:
	bool checkFilename(Common::String filename) const;
	bool isLoadable(const Common::String &filename, Common::SeekableReadStream &stream) const;
	Common::Archive *load(Common::SharedPtr<Common::ArchiveMember> file, Common::SeekableReadStream &stream) const;
};

class ResLoaderTlk : public ResArchiveLoader {
public:
	bool checkFilename(Common::String filename) const;
	bool isLoadable(const Common::String &filename, Common::SeekableReadStream &stream) const;
	Common::Archive *load(Common::SharedPtr<Common::ArchiveMember> file, Common::SeekableReadStream &stream) const;
};

class InstallerLoader {
public:
	static Common::Archive *load(Resource *owner, const Common::String &filename, const Common::String &extension, const uint8 offset);
};

} // end of namespace Kyra

#endif
