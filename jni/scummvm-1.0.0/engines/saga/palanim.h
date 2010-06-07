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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/palanim.h $
 * $Id: palanim.h 30238 2008-01-05 12:45:14Z dreammaster $
 *
 */

// Palette animation module header file

#ifndef SAGA_PALANIM_H
#define SAGA_PALANIM_H

namespace Saga {

#define PALANIM_CYCLETIME 100

struct PalanimEntry {
	uint16 pal_count;
	uint16 color_count;
	uint16 cycle;
	byte *pal_index;
	Color *colors;
};

class PalAnim {
 public:
	PalAnim(SagaEngine *vm);
	~PalAnim(void);

	int loadPalAnim(const byte *, size_t);
	int cycleStart();
	int cycleStep(int vectortime);
	int freePalAnim();

 private:
	SagaEngine *_vm;

	bool _loaded;
	uint16 _entryCount;
	PalanimEntry *_entries;
};

} // End of namespace Saga

#endif

