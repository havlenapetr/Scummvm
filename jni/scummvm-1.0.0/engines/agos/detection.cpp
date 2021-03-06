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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/agos/detection.cpp $
 * $Id: detection.cpp 44287 2009-09-24 03:53:37Z Kirben $
 *
 */

#include "base/plugins.h"

#include "engines/advancedDetector.h"
#include "common/config-manager.h"
#include "common/savefile.h"
#include "common/system.h"

#include "agos/intern.h"
#include "agos/agos.h"

namespace AGOS {

struct AGOSGameDescription {
	ADGameDescription desc;

	int gameType;
	int gameId;
	uint32 features;
};

}

/**
 * Conversion table mapping old obsolete target names to the
 * corresponding new target and platform combination.
 *
 */
static const ADObsoleteGameID obsoleteGameIDsTable[] = {
	{"simon1acorn", "simon1", Common::kPlatformAcorn},
	{"simon1amiga", "simon1", Common::kPlatformAmiga},
	{"simon1cd32", "simon1", Common::kPlatformAmiga},
	{"simon1demo", "simon1", Common::kPlatformPC},
	{"simon1dos", "simon1", Common::kPlatformPC},
	{"simon1talkie", "simon1", Common::kPlatformPC},
	{"simon1win", "simon1", Common::kPlatformWindows},
	{"simon2dos", "simon2", Common::kPlatformPC},
	{"simon2talkie", "simon2", Common::kPlatformPC},
	{"simon2mac", "simon2", Common::kPlatformMacintosh},
	{"simon2win", "simon2", Common::kPlatformWindows},
	{0, 0, Common::kPlatformUnknown}
};

static const PlainGameDescriptor simonGames[] = {
#ifdef ENABLE_PN
	{"pn", "Personal Nightmare"},
#endif
	{"elvira1", "Elvira - Mistress of the Dark"},
	{"elvira2", "Elvira II - The Jaws of Cerberus"},
	{"waxworks", "Waxworks"},
	{"simon1", "Simon the Sorcerer 1"},
	{"simon2", "Simon the Sorcerer 2"},
#ifdef ENABLE_AGOS2
	{"feeble", "The Feeble Files"},
	{"dimp", "Demon in my Pocket"},
	{"jumble", "Jumble"},
	{"puzzle", "NoPatience"},
	{"swampy", "Swampy Adventures"},
#endif
	{0, 0}
};

#include "agos/detection_tables.h"

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)AGOS::gameDescriptions,
	// Size of that superset structure
	sizeof(AGOS::AGOSGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	simonGames,
	// Structure for autoupgrading obsolete targets
	obsoleteGameIDsTable,
	// Name of single gameid (optional)
	0,
	// List of files for file-based fallback detection (optional)
	0,
	// Flags
	0,
	// Additional GUI options (for every game}
	Common::GUIO_NOLAUNCHLOAD
};

using namespace AGOS;

class AgosMetaEngine : public AdvancedMetaEngine {
public:
	AgosMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	virtual const char *getName() const {
		return "AGOS";
	}

	virtual const char *getOriginalCopyright() const {
		return "AGOS (C) Adventure Soft";
	}

	virtual bool hasFeature(MetaEngineFeature f) const;
	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const;
	virtual SaveStateList listSaves(const char *target) const;
	virtual int getMaximumSaveSlot() const;
};

bool AgosMetaEngine::hasFeature(MetaEngineFeature f) const {
	return
		(f == kSupportsListSaves);
}

bool AGOS::AGOSEngine::hasFeature(EngineFeature f) const {
	return
		(f == kSupportsRTL);
}

bool AgosMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const AGOS::AGOSGameDescription *gd = (const AGOS::AGOSGameDescription *)desc;
	bool res = true;

	switch (gd->gameType) {
#ifdef ENABLE_PN
	case AGOS::GType_PN:
		*engine = new AGOS::AGOSEngine_PN(syst);
		break;
#endif
	case AGOS::GType_ELVIRA1:
		*engine = new AGOS::AGOSEngine_Elvira1(syst);
		break;
	case AGOS::GType_ELVIRA2:
		*engine = new AGOS::AGOSEngine_Elvira2(syst);
		break;
	case AGOS::GType_WW:
		*engine = new AGOS::AGOSEngine_Waxworks(syst);
		break;
	case AGOS::GType_SIMON1:
		*engine = new AGOS::AGOSEngine_Simon1(syst);
		break;
	case AGOS::GType_SIMON2:
		*engine = new AGOS::AGOSEngine_Simon2(syst);
		break;
#ifdef ENABLE_AGOS2
	case AGOS::GType_FF:
		if (gd->features & GF_DEMO)
			*engine = new AGOS::AGOSEngine_FeebleDemo(syst);
		else
			*engine = new AGOS::AGOSEngine_Feeble(syst);
		break;
	case AGOS::GType_PP:
		if (gd->gameId == GID_DIMP)
			*engine = new AGOS::AGOSEngine_DIMP(syst);
		else
			*engine = new AGOS::AGOSEngine_PuzzlePack(syst);
		break;
#endif
	default:
		res = false;
		error("AGOS engine: unknown gameType");
	}
	if (res) {
		((AGOS::AGOSEngine *)*engine)->_gameDescription = gd;
	}

	return res;
}

SaveStateList AgosMetaEngine::listSaves(const char *target) const {
	Common::SaveFileManager *saveFileMan = g_system->getSavefileManager();
	Common::StringList filenames;
	Common::String saveDesc;
	Common::String pattern = target;
	pattern += ".???";

	filenames = saveFileMan->listSavefiles(pattern);
	sort(filenames.begin(), filenames.end());	// Sort (hopefully ensuring we are sorted numerically..)

	SaveStateList saveList;
	for (Common::StringList::const_iterator file = filenames.begin(); file != filenames.end(); file++) {
		// Obtain the last 3 digits of the filename, since they correspond to the save slot
		int slotNum = atoi(file->c_str() + file->size() - 3);

		if (slotNum >= 0 && slotNum <= 999) {
			Common::InSaveFile *in = saveFileMan->openForLoading(*file);
			if (in) {
				saveDesc = file->c_str();
				saveList.push_back(SaveStateDescriptor(slotNum, saveDesc));
				delete in;
			}
		}
	}

	return saveList;
}

int AgosMetaEngine::getMaximumSaveSlot() const { return 999; }

#if PLUGIN_ENABLED_DYNAMIC(AGOS)
	REGISTER_PLUGIN_DYNAMIC(AGOS, PLUGIN_TYPE_ENGINE, AgosMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(AGOS, PLUGIN_TYPE_ENGINE, AgosMetaEngine);
#endif

namespace AGOS {

int AGOSEngine::getGameId() const {
	return _gameDescription->gameId;
}

int AGOSEngine::getGameType() const {
	return _gameDescription->gameType;
}

uint32 AGOSEngine::getFeatures() const {
	return _gameDescription->features;
}

const char *AGOSEngine::getExtra() const {
	return _gameDescription->desc.extra;
}

Common::Language AGOSEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

Common::Platform AGOSEngine::getPlatform() const {
	return _gameDescription->desc.platform;
}

const char *AGOSEngine::getFileName(int type) const {
	for (int i = 0; _gameDescription->desc.filesDescriptions[i].fileType; i++) {
		if (_gameDescription->desc.filesDescriptions[i].fileType == type)
			return _gameDescription->desc.filesDescriptions[i].fileName;
	}
	return NULL;
}

} // End of namespace AGOS
