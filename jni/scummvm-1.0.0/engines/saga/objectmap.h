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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/objectmap.h $
 * $Id: objectmap.h 35262 2008-12-06 14:38:04Z thebluegr $
 *
 */

// Object map / Object click-area module header file

#ifndef SAGA_OBJECTMAP_H
#define SAGA_OBJECTMAP_H

namespace Saga {


class HitZone {
private:
	struct ClickArea {
		int pointsCount;
		Point *points;
	};

public:
	HitZone(MemoryReadStreamEndian *readStream, int index, int sceneNumber);
	~HitZone();

	int getNameIndex() const {
		return _nameIndex;
	}
	int getSceneNumber() const {
		return _nameIndex;
	}
	int getActorsEntrance() const {
		return _scriptNumber;
	}
	int getScriptNumber() const {
		return _scriptNumber;
	}
	int getRightButtonVerb() const {
		return _rightButtonVerb;
	}
	int getFlags() const {
		return _flags;
	}
	void setFlag(HitZoneFlags flag) {
		_flags |= flag;
	}
	void clearFlag(HitZoneFlags flag) {
		_flags &= ~flag;
	}
	int getDirection() const {
		return ((_flags >> 4) & 0xF);
	}
	uint16 getHitZoneId() const {
		return objectIndexToId(kGameObjectHitZone, _index);
	}
	uint16 getStepZoneId() const {
		return objectIndexToId(kGameObjectStepZone, _index);
	}
	bool getSpecialPoint(Point &specialPoint) const;
	void draw(SagaEngine *vm, int color);	// for debugging
	bool hitTest(const Point &testPoint);

private:
	int _flags;				// Saga::HitZoneFlags
	int _clickAreasCount;
	int _rightButtonVerb;
	int _nameIndex;
	int _scriptNumber;
	int _index;

	ClickArea *_clickAreas;
};


class ObjectMap {
public:
	ObjectMap(SagaEngine *vm) : _vm(vm) {
		_hitZoneList = NULL;
		_hitZoneListCount = 0;

	}
	~ObjectMap(void) {
		freeMem();
	}
	void load(const byte *resourcePointer, size_t resourceLength);
	void freeMem(void);
	void draw(const Point& testPoint, int color, int color2);	// for debugging
	int hitTest(const Point& testPoint);
	HitZone *getHitZone(int16 index) {
		if ((index < 0) || (index >= _hitZoneListCount)) {
			return NULL;
		}
		return _hitZoneList[index];
	}

	void cmdInfo(void);

private:
	SagaEngine *_vm;

	int _hitZoneListCount;
	HitZone **_hitZoneList;
};

} // End of namespace Saga

#endif
