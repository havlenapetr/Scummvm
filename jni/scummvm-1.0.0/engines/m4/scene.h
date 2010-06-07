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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/m4/scene.h $
 * $Id: scene.h 35648 2009-01-01 15:06:43Z sev $
 *
 */

#ifndef M4_SCENE_H
#define M4_SCENE_H

class View;

#include "m4/assets.h"
#include "m4/hotspot.h"
#include "m4/graphics.h"
#include "m4/viewmgr.h"

namespace M4 {

#define TITLE_SCENE_BURGER 951		// 951 = intro, 901 = demo menu, 971 = first scene
#define MAINMENU_SCENE_BURGER 903
#define FIRST_SCENE 101
#define MAX_CHK_FILENAME_SIZE 144

#define INTERFACE_HEIGHT 106
#define MADS_SURFACE_HEIGHT 156

enum MADSVerbs {
	kVerbLook	= 2,
	kVerbTake	= 3,
	kVerbPush	= 4,
	kVerbOpen	= 5,
	kVerbPut	= 6,
	kVerbTalkTo	= 7,
	kVerbGive	= 8,
	kVerbPull	= 9,
	kVerbClose	= 10,
	kVerbThrow	= 11,
	kVerbWalkTo	= 12
};

struct SceneResources {
	char artBase[MAX_CHK_FILENAME_SIZE];
	char pictureBase[MAX_CHK_FILENAME_SIZE];
	int32 hotspotCount;
	HotSpotList *hotspots;
	int32 parallaxCount;
	HotSpotList *parallax;
	int32 propsCount;
	HotSpotList *props;
	int32 frontY, backY;
	int32 frontScale, backScale;
	int16 depthTable[16];
	int32 railNodeCount;	// # of rails
};

class Scene: public View {
public:
	Scene(M4Engine *vm);
	~Scene();

	// TODO: perhaps move playIntro() someplace else?
	void playIntro();
	void loadScene(int sceneNumber);
	void loadSceneResources(int sceneNumber);
	void loadSceneHotSpotsMads(int sceneNumber);
	void loadSceneCodes(int sceneNumber, int index = 0);
	void loadSceneInverseColorTable(int sceneNumber);
	void loadSceneSprites(int sceneNumber);
	void loadSceneSpriteCodes(int sceneNumber);
	void showSprites();
	void checkHotspotAtMousePos(int x, int y);
	void checkHotspotAtMousePosMads(int x, int y);
	void showHotSpots();
	void showCodes();
	int getCurrentScene() { return _currentScene; }
	SceneResources getSceneResources() { return _sceneResources; }
	M4Surface *getBackgroundSurface() const { return _backgroundSurface; }
	byte *getInverseColorTable() const { return _inverseColorTable; }
	void update();
	void setMADSStatusText(const char *text) { strcpy(_statusText, text); }
	void showMADSV2TextBox(char *text, int x, int y, char *faceName);

	void onRefresh(RectList *rects, M4Surface *destSurface);
	bool onEvent(M4EventType eventType, int param1, int x, int y, bool &captureEvents);

private:
	int _currentScene;
	M4Surface *_backgroundSurface;
	M4Surface *_codeSurface;
	M4Surface *_madsInterfaceSurface;
	byte *_inverseColorTable;
	RGBList *_palData;
	RGBList *_interfacePal;
	SceneResources _sceneResources;
	HotSpotList _sceneHotspots;
	SpriteAsset *_sceneSprites;
	SpriteAsset *_walkerSprite;
	char _statusText[100];

	void nextCommonCursor();
};

} // End of namespace M4

#endif
