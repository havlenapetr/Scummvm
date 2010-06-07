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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/backends/fs/ps2/ps2-fs-factory.cpp $
 * $Id: ps2-fs-factory.cpp 39102 2009-03-04 03:55:00Z sunmax $
 */

#if defined(__PLAYSTATION2__)
#include "backends/fs/ps2/ps2-fs-factory.h"
#include "backends/fs/ps2/ps2-fs.cpp"

DECLARE_SINGLETON(Ps2FilesystemFactory);

AbstractFSNode *Ps2FilesystemFactory::makeRootFileNode() const {
	return new Ps2FilesystemNode();
}

AbstractFSNode *Ps2FilesystemFactory::makeCurrentDirectoryFileNode() const {
	return new Ps2FilesystemNode();
}

AbstractFSNode *Ps2FilesystemFactory::makeFileNodePath(const Common::String &path) const {
	return new Ps2FilesystemNode(path, true);
}
#endif
