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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/totfile.cpp $
 * $Id: totfile.cpp 41838 2009-06-24 21:48:27Z drmccoy $
 *
 */

#include "common/str.h"
#include "common/stream.h"

#include "gob/gob.h"
#include "gob/totfile.h"
#include "gob/dataio.h"
#include "gob/videoplayer.h"

namespace Gob {

TOTFile::TOTFile(GobEngine *vm) : _vm(vm) {
	_stream = 0;

	memset(_header, 0, 128);
}

TOTFile::~TOTFile() {
	unload();
}

bool TOTFile::load(const Common::String &fileName) {
	// Trying to open normally
	_stream = _vm->_dataIO->getDataStream(fileName.c_str());

	if (!_stream)
		// Trying to open from video
		_stream = _vm->_vidPlayer->getExtraData(fileName.c_str());

	if (!_stream)
		return false;

	if (_stream->read(_header, 128) != 128)
		return false;

	_stream->seek(0);

	return true;
}

void TOTFile::unload() {
	delete _stream;

	_stream = 0;
}

Common::SeekableReadStream *TOTFile::getStream() const {
	return _stream;
}

bool TOTFile::getProperties(Properties &props) const {
	if (!_stream)
		return false;

	// Offset 39-41: Version in "Major.Minor" string form
	if (_header[40] != '.')
		return false;

	props.versionMajor = _header[39] - '0';
	props.versionMinor = _header[41] - '0';

	props.variablesCount = READ_LE_UINT32(_header + 44);

	props.textsOffset     = READ_LE_UINT32(_header + 48);
	props.resourcesOffset = READ_LE_UINT32(_header + 52);

	props.animDataSize = READ_LE_UINT16(_header + 56);

	props.imFileNumber   = _header[59];
	props.exFileNumber   = _header[60];
	props.communHandling = _header[61];

	for (int i = 0; i < 14; i++)
		props.functions[i] = READ_LE_UINT16(_header + 100 + i * 2);

	props.scriptEnd = _stream->size();
	if (props.textsOffset > 0)
		props.scriptEnd = MIN(props.scriptEnd, props.textsOffset);
	if (props.resourcesOffset > 0)
		props.scriptEnd = MIN(props.scriptEnd, props.resourcesOffset);

	return true;
}

Common::String TOTFile::createFileName(const Common::String &base, bool &isLOM) {
	isLOM = false;

	const char *dot;
	if ((dot = strrchr(base.c_str(), '.'))) {
		// fileName includes an extension

		if (!scumm_stricmp(dot + 1, "LOM"))
			isLOM = true;

		return base;
	}

	return base + ".tot";
}

Common::String TOTFile::getFileBase(const Common::String &fileName) {
	const char *dot;
	if ((dot = strrchr(fileName.c_str(), '.'))) {
		// fileName includes an extension
		return Common::String(fileName.c_str(), dot);
	}

	return fileName;
}

} // End of namespace Gob
