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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/exereader.h $
 * $Id: exereader.h 38842 2009-02-24 15:33:40Z sev $
 *
 */

#ifndef SCI_EXEREADER_H
#define SCI_EXEREADER_H

#include "common/stream.h"
#include "common/util.h"

namespace Sci {

Common::Platform getGameExePlatform(Common::SeekableReadStream *exeStream);
Common::String readSciVersionFromExe(Common::SeekableReadStream *exeStream, Common::Platform platform);
bool getSciVersionFromString(Common::String versionString, int *version, Common::Platform platform);

} // End of namespace Sci

#endif // SCI_EXEREADER_H
