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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/sound/musicplugin.cpp $
 * $Id: musicplugin.cpp 32695 2008-06-13 16:04:43Z jvprat $
 *
 */

#include "sound/musicplugin.h"

MusicDevice::MusicDevice(MusicPluginObject const *musicPlugin, Common::String name, MusicType mt) :
	_musicDriverName(musicPlugin->getName()), _musicDriverId(musicPlugin->getId()),
	_name(name), _type(mt) {
}

Common::String MusicDevice::getCompleteName() {
	Common::String name;

	if (_name.empty()) {
		// Default device, just show the driver name
		name = _musicDriverName;
	} else {
		// Show both device and driver names
		name = _name;
		name += " [";
		name += _musicDriverName;
		name += "]";
	}

	return name;
}
