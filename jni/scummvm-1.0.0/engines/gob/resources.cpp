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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/gob/resources.cpp $
 * $Id: resources.cpp 42552 2009-07-17 00:55:56Z drmccoy $
 *
 */

#include "common/util.h"
#include "common/endian.h"
#include "common/stream.h"

#include "gob/gob.h"
#include "gob/resources.h"
#include "gob/totfile.h"
#include "gob/dataio.h"
#include "gob/game.h"
#include "gob/global.h"

namespace Gob {

Resource::Resource(byte *data, int32 size, bool needFree,
		int16 width, int16 height) {

	_data     = data;
	_size     = size;
	_width    = width;
	_height   = height;
	_needFree = needFree;

	_stream = new Common::MemoryReadStream(data, size);
}

Resource::~Resource() {
	delete _stream;

	if (_needFree)
		delete[] _data;
}

byte *Resource::getData() const {
	return _data;
}

int32 Resource::getSize() const {
	return _size;
}

int16 Resource::getWidth() const {
	return _width;
}

int16 Resource::getHeight() const {
	return _height;
}

Common::MemoryReadStream *Resource::stream() const {
	return _stream;
}


TextItem::TextItem(byte *data, int32 size) {
	_data = data;
	_size = size;

	_stream = new Common::MemoryReadStream(data, size);
}

TextItem::~TextItem() {
	delete _stream;
}

byte *TextItem::getData() const {
	return _data;
}

int32 TextItem::getSize() const {
	return _size;
}

Common::MemoryReadStream *TextItem::stream() const {
	return _stream;
}


Resources::TOTResourceTable::TOTResourceTable() {
	items = 0;
}

Resources::TOTResourceTable::~TOTResourceTable() {
	delete[] items;
}


Resources::EXTResourceTable::EXTResourceTable() {
	itemsCount = 0;
	unknown    = 0;
	items      = 0;
}

Resources::EXTResourceTable::~EXTResourceTable() {
	delete[] items;
}


Resources::TOTTextTable::TOTTextTable() {
	items    = 0;
	data     = 0;
	needFree = false;
}

Resources::TOTTextTable::~TOTTextTable() {
	delete[] items;
	if (needFree)
		delete[] data;
}


Resources::Resources(GobEngine *vm) : _vm(vm) {
	unload(false);
}

Resources::~Resources() {
	unload();
}

bool Resources::load(const Common::String &fileName) {
	unload();

	Common::String fileBase;

	_totFile = TOTFile::createFileName(fileName, _hasLOM);

	if (_hasLOM) {
		warning("Stub: Resources::load(%s)", fileName.c_str());
		unload();
		return false;
	}

	fileBase = TOTFile::getFileBase(fileName);

	_extFile = fileBase + ".ext";

	bool hasTOTRes = loadTOTResourceTable();
	bool hasEXTRes = loadEXTResourceTable();

	if (!hasTOTRes && !hasEXTRes)
		return false;

	if (hasTOTRes) {
		if (!loadTOTTextTable(fileBase)) {
			unload();
			return false;
		}

		if (!loadIMFile()) {
			unload();
			return false;
		}
	}

	if (hasEXTRes) {
		if (!loadEXFile()) {
			unload();
			return false;
		}
	}

	return true;
}

void Resources::unload(bool del) {
	if (del) {
		delete _totResourceTable;
		delete _extResourceTable;
		delete _totTextTable;

		delete[] _totData;
		delete[] _imData;

		_totFile.clear();
		_extFile.clear();
		_exFile.clear();
	}

	_totResourceTable = 0;
	_extResourceTable = 0;
	_totTextTable = 0;
	_totData = 0;
	_totSize = 0;
	_imData = 0;
	_imSize = 0;
}

bool Resources::isLoaded() const {
	return (_totResourceTable != 0);
}

bool Resources::loadTOTResourceTable() {
	TOTFile totFile(_vm);

	if (!totFile.load(_totFile))
		return false;

	TOTFile::Properties totProps;
	if (!totFile.getProperties(totProps))
		return false;

	Common::SeekableReadStream *stream = totFile.getStream();
	if (!stream)
		return false;

	if ((totProps.resourcesOffset == 0xFFFFFFFF) ||
	    (totProps.resourcesOffset == 0))
		// No resources here
		return false;

	_totResourceTable = new TOTResourceTable;

	stream->seek(totProps.resourcesOffset);
	_totResourceTable->itemsCount = stream->readSint16LE();

	_totResourceTable->dataOffset = totProps.resourcesOffset + kTOTResTableSize +
	                                _totResourceTable->itemsCount * kTOTResItemSize;


	uint32 resSize = _totResourceTable->itemsCount * kTOTResItemSize +
	                 kTOTResTableSize;

	// Would the table actually fit into the TOT?
	if ((totProps.resourcesOffset + resSize) > ((uint32) stream->size()))
		return false;

	_totResourceTable->unknown = stream->readByte();
	_totResourceTable->items = new TOTResourceItem[_totResourceTable->itemsCount];

	for (int i = 0; i < _totResourceTable->itemsCount; ++i) {
		TOTResourceItem &item = _totResourceTable->items[i];

		item.offset = stream->readSint32LE();
		item.size   = stream->readUint16LE();
		item.width  = stream->readSint16LE();
		item.height = stream->readSint16LE();

		if (item.offset < 0) {
			item.type = kResourceIM;
			item.index = -item.offset - 1;
		} else
			item.type = kResourceTOT;
	}

	_totResStart = totProps.scriptEnd;
	_totSize = stream->size() - _totResStart;
	if (_totSize <= 0)
		return false;

	if (!stream->seek(totProps.scriptEnd))
		return false;

	_totData = new byte[_totSize];
	if (stream->read(_totData, _totSize) != _totSize)
		return false;

	return !stream->err();
}

bool Resources::loadEXTResourceTable() {
	_extResourceTable = new EXTResourceTable;

	DataStream *stream = _vm->_dataIO->getDataStream(_extFile.c_str());
	if (!stream)
		return false;

	_extResourceTable->itemsCount = stream->readSint16LE();
	_extResourceTable->unknown    = stream->readByte();

	if (_extResourceTable->itemsCount > 0)
		_extResourceTable->items = new EXTResourceItem[_extResourceTable->itemsCount];

	for (int i = 0; i < _extResourceTable->itemsCount; i++) {
		EXTResourceItem &item = _extResourceTable->items[i];

		item.offset = stream->readUint32LE();
		item.size   = stream->readUint16LE();
		item.width  = stream->readUint16LE();
		item.height = stream->readUint16LE();

		if (item.offset < 0) {
			item.type = kResourceEX;
			item.offset = -item.offset - 1;
		} else {
			item.type = kResourceEXT;
			item.offset += kEXTResTableSize +
			               kEXTResItemSize * _extResourceTable->itemsCount;
		}

		item.packed = (item.width & 0x8000) != 0;

		item.width &= 0x7FFF;
	}

	delete stream;
	return true;
}

bool Resources::loadTOTTextTable(const Common::String &fileBase) {
	TOTFile totFile(_vm);

	if (!totFile.load(_totFile))
		return false;

	TOTFile::Properties totProps;
	if (!totFile.getProperties(totProps))
		return false;

	Common::SeekableReadStream *stream = totFile.getStream();
	if (!stream)
		return false;

	if (totProps.textsOffset == ((uint32) -1))
		// No texts
		return true;

	_totTextTable = new TOTTextTable;

	bool fromTOT;
	if (totProps.textsOffset == 0) {
		fromTOT                 = false;
		_totTextTable->data     = loadTOTLocTexts(fileBase, _totTextTable->size);
		_totTextTable->needFree = true;
	} else {
		fromTOT                 = true;
		_totTextTable->data     = _totData + totProps.textsOffset - _totResStart;
		_totTextTable->needFree = false;
		_totTextTable->size     = _totSize - totProps.textsOffset;
	}

	if (_totTextTable->data) {
		Common::MemoryReadStream totTextTable(_totTextTable->data, _totTextTable->size);
		_totTextTable->itemsCount = totTextTable.readSint16LE() & 0x3FFF;

		_totTextTable->items = new TOTTextItem[_totTextTable->itemsCount];
		for (int i = 0; i < _totTextTable->itemsCount; ++i) {
			TOTTextItem &item = _totTextTable->items[i];

			item.offset = totTextTable.readSint16LE();
			item.size   = totTextTable.readSint16LE();

			if (fromTOT)
				item.offset -= (totProps.textsOffset - _totResStart);
		}
	}

	return true;
}

bool Resources::loadIMFile() {
	TOTFile totFile(_vm);

	if (!totFile.load(_totFile))
		return false;

	TOTFile::Properties totProps;
	if (!totFile.getProperties(totProps))
		return false;

	if ((totProps.communHandling != 0) && (totProps.imFileNumber == 0))
		// No IM file
		return true;

	Common::String imFile = "commun.im";

	char num = totProps.imFileNumber + '0';
	if (num == '0')
		num = '1';

	imFile += num;

	DataStream *stream = _vm->_dataIO->getDataStream(imFile.c_str());
	if (!stream)
		return true;

	_imSize = stream->size();
	if (_imSize <= 0) {
		_imSize = 0;
		delete stream;
		return true;
	}

	_imData = new byte[_imSize];
	if (stream->read(_imData, _imSize) != _imSize) {
		delete[] _imData;
		_imData = 0;
		_imSize = 0;
	}

	delete stream;
	return true;
}

bool Resources::loadEXFile() {
	TOTFile totFile(_vm);

	if (!totFile.load(_totFile))
		return false;

	TOTFile::Properties totProps;
	if (!totFile.getProperties(totProps))
		return false;

	_exFile = "commun.ex";
	_exFile += totProps.exFileNumber + '0';

	if (!_vm->_dataIO->existData(_exFile.c_str())) {
		_exFile.clear();
		return true;
	}

	return true;
}

Common::String Resources::getLocTextFile(const Common::String &fileBase,
		int language) {

	Common::String locTextFile = fileBase + ".";
	switch (language) {
	case kLanguageFrench:
		locTextFile += "dat";
		break;
	case kLanguageGerman:
		locTextFile += "all";
		break;
	case kLanguageSpanish:
		locTextFile += "esp";
		break;
	case kLanguageItalian:
		locTextFile += "ita";
		break;
	case kLanguageAmerican:
		locTextFile += "usa";
		break;
	case kLanguageDutch:
		locTextFile += "ndl";
		break;
	case kLanguageKorean:
		locTextFile += "kor";
		break;
	case kLanguageHebrew:
		locTextFile += "isr";
		break;
	default:
		locTextFile += "ang";
		break;
	}

	if (!_vm->_dataIO->existData(locTextFile.c_str()))
		locTextFile.clear();

	return locTextFile;
}

byte *Resources::loadTOTLocTexts(const Common::String &fileBase, int32 &size) {
	Common::String locTextFile;

	locTextFile = getLocTextFile(fileBase, _vm->_global->_languageWanted);

	if (!locTextFile.empty()) {

		_vm->_global->_foundLanguage = true;
		_vm->_global->_language = _vm->_global->_languageWanted;

	} else if (!_vm->_global->_foundLanguage) {

		// Trying US for GB and vice versa
		if (_vm->_global->_languageWanted == kLanguageBritish) {

			locTextFile = getLocTextFile(fileBase, kLanguageAmerican);
			if (!locTextFile.empty())
				_vm->_global->_language = kLanguageAmerican;

		} else if (_vm->_global->_languageWanted == kLanguageAmerican) {

			locTextFile = getLocTextFile(fileBase, kLanguageBritish);
			if (!locTextFile.empty())
				_vm->_global->_language = kLanguageBritish;

		}

		if (locTextFile.empty()) {
			// Looking for the first existing language
			for (int i = 0; i < 10; i++) {
				locTextFile = getLocTextFile(fileBase, i);
				if (!locTextFile.empty()) {
					_vm->_global->_language = i;
					break;
				}
			}
		}

	}

	debugC(1, kDebugFileIO, "Using language %d for %s",
			_vm->_global->_language, _totFile.c_str());

	if (locTextFile.empty())
		return 0;

	size = _vm->_dataIO->getDataSize(locTextFile.c_str());
	return _vm->_dataIO->getData(locTextFile.c_str());
}

Resource *Resources::getResource(uint16 id, int16 *width, int16 *height) const {
	if (_hasLOM) {
		warning("Stub: Resources::getResource(): Has LOM");
		return 0;
	}

	Resource *resource = 0;
	if (id >= 30000)
		resource = getEXTResource(id - 30000);
	else
		resource = getTOTResource(id);

	if (!resource)
		return 0;

	if (width)
		*width  = resource->getWidth();
	if (height)
		*height = resource->getHeight();

	return resource;
}

TextItem *Resources::getTextItem(uint16 id) const {
	if (!_totTextTable || !_totTextTable->data)
		return 0;

	if (id >= _totTextTable->itemsCount)
		return 0;

	TOTTextItem &totItem = _totTextTable->items[id];

	if ((totItem.offset == 0xFFFF) || (totItem.size == 0))
		return 0;
	if ((totItem.offset + totItem.size) > (_totTextTable->size))
		return 0;

	return new TextItem(_totTextTable->data + totItem.offset, totItem.size);
}

byte *Resources::getTexts() const {
	if (!_totTextTable)
		return 0;

	return _totTextTable->data;
}

Resource *Resources::getTOTResource(uint16 id) const {
	if (id >= _totResourceTable->itemsCount) {
		warning("Trying to load non-existent TOT resource (%s, %d/%d)",
				_totFile.c_str(), id, _totResourceTable->itemsCount - 1);
		return 0;
	}

	TOTResourceItem &totItem = _totResourceTable->items[id];

	byte *data = 0;
	if (totItem.type == kResourceIM)
		data = getIMData(totItem);
	if (totItem.type == kResourceTOT)
		data = getTOTData(totItem);

	if (!data) {
		warning("Failed to load TOT resource (%s, %d/%d, %d)",
				_totFile.c_str(), id, _totResourceTable->itemsCount - 1, totItem.type);
		return 0;
	}

	return new Resource(data, totItem.size, false, totItem.width, totItem.height);
}

Resource *Resources::getEXTResource(uint16 id) const {
	if (!_extResourceTable || (id > _extResourceTable->itemsCount)) {
		warning("Trying to load non-existent EXT resource (%s, %d/%d)",
				_totFile.c_str(), id,
				_extResourceTable ? (_extResourceTable->itemsCount - 1) : -1);
		return 0;
	}

	EXTResourceItem &extItem = _extResourceTable->items[id];

	uint32 size = extItem.size;

	if (extItem.width & 0x4000)
		size += 1 << 16;
	if (extItem.width & 0x2000)
		size += 2 << 16;
	if (extItem.width & 0x1000)
		size += 4 << 16;
	if (extItem.height == 0)
		size += extItem.width << 16;

	byte *data = 0;
	if (extItem.type == kResourceEXT)
		data = getEXTData(extItem, size);
	if (extItem.type == kResourceEX)
		data = getEXData(extItem, size);

	if (!data) {
		warning("Failed to load EXT resource (%s, %d/%d, %d)",
				_totFile.c_str(), id, _extResourceTable->itemsCount - 1, extItem.type);
		return 0;
	}

	if (extItem.packed) {
		byte *packedData = data;

		size = READ_LE_UINT32(packedData);
		data = new byte[size];

		_vm->_dataIO->unpackData(packedData, data);

		delete[] packedData;
	}

	return new Resource(data, size, true, extItem.width & 0xFFF, extItem.height);
}

byte *Resources::getTOTData(TOTResourceItem &totItem) const {
	if (totItem.size == 0)
		return 0;

	int32 offset = _totResourceTable->dataOffset + totItem.offset - _totResStart;

	if ((offset < 0) || (((uint32) (offset + totItem.size)) > _totSize))
		return 0;

	return _totData + offset;
}

byte *Resources::getIMData(TOTResourceItem &totItem) const {
	if (totItem.size == 0)
		return 0;

	int32 indexOffset = totItem.index * 4;
	if ((indexOffset < 0) || (((uint32) indexOffset) >= _imSize))
		return 0;

	uint32 offset = READ_LE_UINT32(_imData + indexOffset);
	if ((offset + totItem.size) > _imSize)
		return 0;

	return _imData + offset;
}

byte *Resources::getEXTData(EXTResourceItem &extItem, uint32 size) const {
	DataStream *stream = _vm->_dataIO->getDataStream(_extFile.c_str());
	if (!stream)
		return 0;

	if (!stream->seek(extItem.offset)) {
		delete stream;
		return 0;
	}

	byte *data = new byte[extItem.packed ? (size + 2) : size];
	if (stream->read(data, size) != size) {
		delete[] data;
		delete stream;
		return 0;
	}

	delete stream;
	return data;
}

byte *Resources::getEXData(EXTResourceItem &extItem, uint32 size) const {
	DataStream *stream = _vm->_dataIO->getDataStream(_exFile.c_str());
	if (!stream)
		return 0;

	if (!stream->seek(extItem.offset)) {
		delete stream;
		return 0;
	}

	byte *data = new byte[extItem.packed ? (size + 2) : size];
	if (stream->read(data, size) != size) {
		delete[] data;
		delete stream;
		return 0;
	}

	delete stream;
	return data;
}

} // End of namespace Gob
