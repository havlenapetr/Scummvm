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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sci/sci.h $
 * $Id: sci.h 42398 2009-07-11 23:45:54Z thebluegr $
 *
 */

#ifndef SCI_H
#define SCI_H

#include "engines/engine.h"
#include "engines/advancedDetector.h"

#include "sci/resource.h"

namespace Sci {

class Console;
struct EngineState;
class Kernel;
class Vocabulary;

// our engine debug levels
enum kDebugLevels {
	kDebugLevelError      = 1 << 0,
	kDebugLevelNodes      = 1 << 1,
	kDebugLevelGraphics   = 1 << 2,
	kDebugLevelStrings    = 1 << 3,
	kDebugLevelMemory     = 1 << 4,
	kDebugLevelFuncCheck  = 1 << 5,
	kDebugLevelBresen     = 1 << 6,
	kDebugLevelSound      = 1 << 7,
	kDebugLevelGfxDriver  = 1 << 8,
	kDebugLevelBaseSetter = 1 << 9,
	kDebugLevelParser     = 1 << 10,
	kDebugLevelMenu       = 1 << 11,
	kDebugLevelSaid       = 1 << 12,
	kDebugLevelFile       = 1 << 13,
	kDebugLevelTime       = 1 << 14,
	kDebugLevelRoom       = 1 << 15,
	kDebugLevelAvoidPath  = 1 << 16,
	kDebugLevelDclInflate = 1 << 17,
	kDebugLevelVM         = 1 << 18,
	kDebugLevelScripts    = 1 << 19,
	kDebugLevelGC         = 1 << 20
};

struct SciGameDescription {
	ADGameDescription desc;
	uint32 flags;
	int res_version;
	int version;
};

enum SciGameVersions {
	SCI_VERSION_AUTODETECT = 0,
	SCI_VERSION_0 = 1,
	SCI_VERSION_01 = 2,
	SCI_VERSION_01_VGA_ODD = 3,
	SCI_VERSION_1 = 4,
	SCI_VERSION_1_1 = 5,
	SCI_VERSION_32 = 6
};

extern const char *versionNames[7];

enum SciGameFlags {
	// SCI0 flags

	/* Applies to all versions before 0.000.629
	 * Older SCI versions had simpler code for GetTime()
	 */
	GF_SCI0_OLDGETTIME		= (1 << 0)
};

class SciEngine : public Engine {
	friend class Console;
public:
	SciEngine(OSystem *syst, const SciGameDescription *desc);
	~SciEngine();

	// Engine APIs
	virtual Common::Error run();
	void pauseEngineIntern(bool pause);
	virtual GUI::Debugger *getDebugger();
	Console *getSciDebugger();

	const char* getGameID() const;
	int getResourceVersion() const;
	int getVersion() const;
	Common::Language getLanguage() const;
	Common::Platform getPlatform() const;
	uint32 getFlags() const;
	ResourceManager *getResMgr() const { return _resmgr; }
	Kernel *getKernel() const { return _kernel; }
	Vocabulary *getVocabulary() const { return _vocabulary; }

	Common::String getSavegameName(int nr) const;
	Common::String getSavegamePattern() const;

	/** Prepend 'TARGET-' to the given filename. */
	Common::String wrapFilename(const Common::String &name) const;

	/** Remove the 'TARGET-' prefix of the given filename, if present. */
	Common::String unwrapFilename(const Common::String &name) const;

private:
	const SciGameDescription *_gameDescription;
	ResourceManager *_resmgr;
	EngineState *_gamestate;
	Kernel *_kernel;
	Vocabulary *_vocabulary;
	Console *_console;
};

} // End of namespace Sci

#endif // SCI_H
