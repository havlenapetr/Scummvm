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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/agi/preagi_common.h $
 * $Id: preagi_common.h 30405 2008-01-11 10:59:43Z thebluegr $
 *
 */



#ifndef AGI_PREAGI_COMMON_H
#define AGI_PREAGI_COMMON_H

namespace Agi {

// default attributes
#define IDA_DEFAULT		0x0F
#define IDA_DEFAULT_REV	0xF0

#define IDI_SND_OSCILLATOR_FREQUENCY	1193180
#define IDI_SND_TIMER_RESOLUTION		0.0182

#define kColorDefault 0x1337

#define IDI_MAX_ROW_PIC	20

enum SelectionTypes {
	kSelYesNo,
	kSelNumber,
	kSelSpace,
	kSelAnyKey,
	kSelBackspace
};

} // End of namespace Agi

#endif
