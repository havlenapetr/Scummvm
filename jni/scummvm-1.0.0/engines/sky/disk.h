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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/disk.h $
 * $Id: disk.h 30238 2008-01-05 12:45:14Z dreammaster $
 *
 */

#ifndef SKY_DISK_H
#define SKY_DISK_H


#include "common/scummsys.h"
#include "common/str.h"
#include "sky/rnc_deco.h"

#define MAX_FILES_IN_LIST 60

namespace Common {
	class File;
}

namespace Sky {

class Disk {
public:
	Disk();
	~Disk(void);

	uint8 *loadFile(uint16 fileNr);
	uint16 *loadScriptFile(uint16 fileNr);
	bool fileExists(uint16 fileNr);

	uint32 determineGameVersion();

	uint32 _lastLoadedFileSize;

	void fnMiniLoad(uint16 fileNum);
	void fnCacheFast(uint16 *fList);
	void fnCacheChip(uint16 *fList);
	void fnCacheFiles(void);
	void fnFlushBuffers(void);
	uint32 *giveLoadedFilesList(void) { return _loadedFilesList; }
	void refreshFilesList(uint32 *list);

protected:
	uint8 *getFileInfo(uint16 fileNr);
	void dumpFile(uint16 fileNr);

	uint32 _dinnerTableEntries;
	uint8 *_dinnerTableArea;
	Common::File *_dataDiskHandle;
	RncDecoder _rncDecoder;

	uint16 _buildList[MAX_FILES_IN_LIST];
	uint32 _loadedFilesList[MAX_FILES_IN_LIST];
};

} // End of namespace Sky

#endif
