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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/kyra/items_lok.cpp $
 * $Id: items_lok.cpp 40715 2009-05-19 01:13:29Z lordhoto $
 *
 */

#include "kyra/kyra_lok.h"
#include "kyra/seqplayer.h"
#include "kyra/screen.h"
#include "kyra/resource.h"
#include "kyra/sound.h"
#include "kyra/sprites.h"
#include "kyra/wsamovie.h"
#include "kyra/animator_lok.h"
#include "kyra/text.h"

#include "common/system.h"
#include "common/savefile.h"

namespace Kyra {

int KyraEngine_LoK::findDuplicateItemShape(int shape) {
	static const uint8 dupTable[] = {
		0x48, 0x46, 0x49, 0x47, 0x4a, 0x46, 0x4b, 0x47,
		0x4c, 0x46, 0x4d, 0x47, 0x5b, 0x5a, 0x5c, 0x5a,
		0x5d, 0x5a, 0x5e, 0x5a, 0xFF, 0xFF
	};

	int i = 0;

	while (dupTable[i] != 0xFF) {
		if (dupTable[i] == shape)
			return dupTable[i+1];
		i += 2;
	}
	return -1;
}

void KyraEngine_LoK::addToNoDropRects(int x, int y, int w, int h) {
	for (int rect = 0; rect < ARRAYSIZE(_noDropRects); ++rect) {
		if (_noDropRects[rect].top == -1) {
			_noDropRects[rect].left = x;
			_noDropRects[rect].top = y;
			_noDropRects[rect].right = x + w;
			_noDropRects[rect].bottom = y + h;
			break;
		}
	}
}

void KyraEngine_LoK::clearNoDropRects() {
	memset(_noDropRects, -1, sizeof(_noDropRects));
}

byte KyraEngine_LoK::findFreeItemInScene(int scene) {
	assert(scene < _roomTableSize);
	Room *room = &_roomTable[scene];
	for (int i = 0; i < 12; ++i) {
		if (room->itemsTable[i] == 0xFF)
			return i;
	}
	return 0xFF;
}

byte KyraEngine_LoK::findItemAtPos(int x, int y) {
	assert(_currentCharacter->sceneId < _roomTableSize);
	const uint8 *itemsTable = _roomTable[_currentCharacter->sceneId].itemsTable;
	const uint16 *xposOffset = _roomTable[_currentCharacter->sceneId].itemsXPos;
	const uint8 *yposOffset = _roomTable[_currentCharacter->sceneId].itemsYPos;

	int highestYPos = -1;
	byte returnValue = 0xFF;

	for (int i = 0; i < 12; ++i) {
		if (*itemsTable != 0xFF) {
			int xpos = *xposOffset - 11;
			int xpos2 = *xposOffset + 10;
			if (x > xpos && x < xpos2) {
				assert(*itemsTable < ARRAYSIZE(_itemTable));
				int itemHeight = _itemTable[*itemsTable].height;
				int ypos = *yposOffset + 3;
				int ypos2 = ypos - itemHeight - 3;

				if (y > ypos2 && ypos > y) {
					if (highestYPos <= ypos) {
						returnValue = i;
						highestYPos = ypos;
					}
				}
			}
		}
		++xposOffset;
		++yposOffset;
		++itemsTable;
	}

	return returnValue;
}

void KyraEngine_LoK::placeItemInGenericMapScene(int item, int index) {
	static const uint16 itemMapSceneMinTable[] = {
		0x0000, 0x0011, 0x006D, 0x0025, 0x00C7, 0x0000
	};
	static const uint16 itemMapSceneMaxTable[] = {
		0x0010, 0x0024, 0x00C6, 0x006C, 0x00F5, 0x0000
	};

	int minValue = itemMapSceneMinTable[index];
	int maxValue = itemMapSceneMaxTable[index];

	while (true) {
		int room = _rnd.getRandomNumberRng(minValue, maxValue);
		assert(room < _roomTableSize);
		int nameIndex = _roomTable[room].nameIndex;
		bool placeItem = false;

		switch (nameIndex) {
		case 0:  case 1:   case 2:   case 3:
		case 4:  case 5:   case 6:   case 11:
		case 12: case 16:  case 17:  case 20:
		case 22: case 23:  case 25:  case 26:
		case 27: case 31:  case 33:  case 34:
		case 36: case 37:  case 58:  case 59:
		case 60: case 61:  case 83:  case 84:
		case 85: case 104: case 105: case 106:
			placeItem = true;
			break;

		case 51:
			if (room != 46)
				placeItem = true;
			break;

		default:
			break;
		}

		if (placeItem) {
			Room *roomPtr = &_roomTable[room];
			if (roomPtr->northExit == 0xFFFF && roomPtr->eastExit == 0xFFFF && roomPtr->southExit == 0xFFFF && roomPtr->westExit == 0xFFFF)
				placeItem = false;
			else if (_currentCharacter->sceneId == room)
				placeItem = false;
		}

		if (placeItem) {
			if (!processItemDrop(room, item, -1, -1, 2, 0))
				continue;
			break;
		}
	}
}

void KyraEngine_LoK::setHandItem(uint16 item) {
	_screen->hideMouse();
	setMouseItem(item);
	_itemInHand = item;
	_screen->showMouse();
}

void KyraEngine_LoK::removeHandItem() {
	_screen->hideMouse();
	_screen->setMouseCursor(1, 1, _shapes[0]);
	_itemInHand = -1;
	_screen->showMouse();
}

void KyraEngine_LoK::setMouseItem(uint16 item) {
	if (item == 0xFFFF)
		_screen->setMouseCursor(1, 1, _shapes[6]);
	else
		_screen->setMouseCursor(8, 15, _shapes[216+item]);
}

void KyraEngine_LoK::wipeDownMouseItem(int xpos, int ypos) {
	if (_itemInHand == -1)
		return;
	xpos -= 8;
	ypos -= 15;
	_screen->hideMouse();
	backUpItemRect1(xpos, ypos);
	int y = ypos;
	int height = 16;

	while (height >= 0) {
		restoreItemRect1(xpos, ypos);
		_screen->setNewShapeHeight(_shapes[216+_itemInHand], height);
		uint32 nextTime = _system->getMillis() + 1 * _tickLength;
		_screen->drawShape(0, _shapes[216+_itemInHand], xpos, y, 0, 0);
		_screen->updateScreen();
		y += 2;
		height -= 2;
		delayUntil(nextTime);
	}
	restoreItemRect1(xpos, ypos);
	_screen->resetShapeHeight(_shapes[216+_itemInHand]);
	removeHandItem();
	_screen->showMouse();
}

void KyraEngine_LoK::setupSceneItems() {
	uint16 sceneId = _currentCharacter->sceneId;
	assert(sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[sceneId];
	for (int i = 0; i < 12; ++i) {
		uint8 item = currentRoom->itemsTable[i];
		if (item == 0xFF || !currentRoom->needInit[i])
			continue;

		int xpos = 0;
		int ypos = 0;

		if (currentRoom->itemsXPos[i] == 0xFFFF) {
			xpos = currentRoom->itemsXPos[i] = _rnd.getRandomNumberRng(24, 296);
			ypos = currentRoom->itemsYPos[i] = _rnd.getRandomNumberRng(_northExitHeight & 0xFF, 130);
		} else {
			xpos = currentRoom->itemsXPos[i];
			ypos = currentRoom->itemsYPos[i];
		}

		_lastProcessedItem = i;

		int stop = 0;
		while (!stop) {
			stop = processItemDrop(sceneId, item, xpos, ypos, 3, 0);
			if (!stop) {
				xpos = currentRoom->itemsXPos[i] = _rnd.getRandomNumberRng(24, 296);
				ypos = currentRoom->itemsYPos[i] = _rnd.getRandomNumberRng(_northExitHeight & 0xFF, 130);
				if (countItemsInScene(sceneId) >= 12)
					break;
			} else {
				currentRoom->needInit[i] = 0;
			}
		}
	}
}

int KyraEngine_LoK::countItemsInScene(uint16 sceneId) {
	assert(sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[sceneId];

	int items = 0;

	for (int i = 0; i < 12; ++i) {
		if (currentRoom->itemsTable[i] != 0xFF)
			++items;
	}

	return items;
}

int KyraEngine_LoK::processItemDrop(uint16 sceneId, uint8 item, int x, int y, int unk1, int unk2) {
	int freeItem = -1;
	uint8 itemIndex = findItemAtPos(x, y);
	if (unk1)
		itemIndex = 0xFF;

	if (itemIndex != 0xFF) {
		exchangeItemWithMouseItem(sceneId, itemIndex);
		return 0;
	}

	assert(sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[sceneId];

	if (unk1 != 3) {
		for (int i = 0; i < 12; ++i) {
			if (currentRoom->itemsTable[i] == 0xFF) {
				freeItem = i;
				break;
			}
		}
	} else {
		freeItem = _lastProcessedItem;
	}

	if (freeItem == -1)
		return 0;

	if (sceneId != _currentCharacter->sceneId) {
		addItemToRoom(sceneId, item, freeItem, x, y);
		return 1;
	}

	int itemHeight = _itemTable[item].height;
	_lastProcessedItemHeight = itemHeight;

	if (x == -1 && x == -1) {
		x = _rnd.getRandomNumberRng(16, 304);
		y = _rnd.getRandomNumberRng(_northExitHeight & 0xFF, 135);
	}

	int xpos = x;
	int ypos = y;
	int destY = -1;
	int destX = -1;
	int running = 1;

	while (running) {
		if ((_northExitHeight & 0xFF) <= ypos) {
			bool running2 = true;

			if (_screen->getDrawLayer(xpos, ypos) > 1) {
				if (((_northExitHeight >> 8) & 0xFF) != ypos)
					running2 = false;
			}

			if (_screen->getDrawLayer2(xpos, ypos, itemHeight) > 1) {
				if (((_northExitHeight >> 8) & 0xFF) != ypos)
					running2 = false;
			}

			if (!isDropable(xpos, ypos)) {
				if (((_northExitHeight >> 8) & 0xFF) != ypos)
					running2 = false;
			}

			int xpos2 = xpos;
			int xpos3 = xpos;

			while (running2) {
				if (isDropable(xpos2, ypos)) {
					if (_screen->getDrawLayer2(xpos2, ypos, itemHeight) < 7) {
						if (findItemAtPos(xpos2, ypos) == 0xFF) {
							destX = xpos2;
							destY = ypos;
							running = 0;
							running2 = false;
						}
					}
				}

				if (isDropable(xpos3, ypos)) {
					if (_screen->getDrawLayer2(xpos3, ypos, itemHeight) < 7) {
						if (findItemAtPos(xpos3, ypos) == 0xFF) {
							destX = xpos3;
							destY = ypos;
							running = 0;
							running2 = false;
						}
					}
				}

				if (!running2)
					continue;

				xpos2 -= 2;
				if (xpos2 < 16)
					xpos2 = 16;

				xpos3 += 2;
				if (xpos3 > 304)
					xpos3 = 304;

				if (xpos2 > 16)
					continue;
				if (xpos3 < 304)
					continue;
				running2 = false;
			}
		}

		if (((_northExitHeight >> 8) & 0xFF) == ypos) {
			running = 0;
			destY -= _rnd.getRandomNumberRng(0, 3);

			if ((_northExitHeight & 0xFF) < destY)
				continue;

			destY = (_northExitHeight & 0xFF) + 1;
			continue;
		}
		ypos += 2;
		if (((_northExitHeight >> 8) & 0xFF) >= ypos)
			continue;
		ypos = (_northExitHeight >> 8) & 0xFF;
	}

	if (destX == -1 || destY == -1)
		return 0;

	if (unk1 == 3) {
		currentRoom->itemsXPos[freeItem] = destX;
		currentRoom->itemsYPos[freeItem] = destY;
		return 1;
	}

	if (unk1 == 2)
		itemSpecialFX(x, y, item);

	if (unk1 == 0)
		removeHandItem();

	itemDropDown(x, y, destX, destY, freeItem, item);

	if (unk1 == 0 && unk2 != 0) {
		assert(_itemList && _droppedList);
		updateSentenceCommand(_itemList[item], _droppedList[0], 179);
	}

	return 1;
}

void KyraEngine_LoK::exchangeItemWithMouseItem(uint16 sceneId, int itemIndex) {
	_screen->hideMouse();
	_animator->animRemoveGameItem(itemIndex);
	assert(sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[sceneId];

	int item = currentRoom->itemsTable[itemIndex];
	currentRoom->itemsTable[itemIndex] = _itemInHand;
	_itemInHand = item;
	_animator->animAddGameItem(itemIndex, sceneId);
	snd_playSoundEffect(53);

	setMouseItem(_itemInHand);
	assert(_itemList && _takenList);
	updateSentenceCommand(_itemList[_itemInHand], _takenList[1], 179);
	_screen->showMouse();
	clickEventHandler2();
}

void KyraEngine_LoK::addItemToRoom(uint16 sceneId, uint8 item, int itemIndex, int x, int y) {
	assert(sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[sceneId];
	currentRoom->itemsTable[itemIndex] = item;
	currentRoom->itemsXPos[itemIndex] = x;
	currentRoom->itemsYPos[itemIndex] = y;
	currentRoom->needInit[itemIndex] = 1;
}

int KyraEngine_LoK::checkNoDropRects(int x, int y) {
	if (_lastProcessedItemHeight < 1 || _lastProcessedItemHeight > 16)
		_lastProcessedItemHeight = 16;
	if (_noDropRects[0].left == -1)
		return 0;

	for (int i = 0; i < ARRAYSIZE(_noDropRects); ++i) {
		if (_noDropRects[i].left == -1)
			break;

		int xpos = _noDropRects[i].left;
		int ypos = _noDropRects[i].top;
		int xpos2 = _noDropRects[i].right;
		int ypos2 = _noDropRects[i].bottom;

		if (xpos > x + 16)
			continue;
		if (xpos2 <= x)
			continue;
		if (y < ypos)
			continue;
		if (ypos2 <= y - _lastProcessedItemHeight)
			continue;
		return 1;
	}

	return 0;
}

int KyraEngine_LoK::isDropable(int x, int y) {
	x -= 8;
	y -= 1;

	if (checkNoDropRects(x, y))
		return 0;

	for (int xpos = x; xpos < x + 16; ++xpos) {
		if (_screen->getShapeFlag1(xpos, y) == 0)
			return 0;
	}
	return 1;
}

void KyraEngine_LoK::itemDropDown(int x, int y, int destX, int destY, byte freeItem, int item) {
	assert(_currentCharacter->sceneId < _roomTableSize);
	Room *currentRoom = &_roomTable[_currentCharacter->sceneId];
	if (x == destX && y == destY) {
		currentRoom->itemsXPos[freeItem] = destX;
		currentRoom->itemsYPos[freeItem] = destY;
		currentRoom->itemsTable[freeItem] = item;
		snd_playSoundEffect(0x32);
		_animator->animAddGameItem(freeItem, _currentCharacter->sceneId);
		return;
	}
	_screen->hideMouse();
	if (y <= destY) {
		int tempY = y;
		int addY = 2;
		int drawX = x - 8;
		int drawY = 0;

		backUpItemRect0(drawX, y - 16);

		while (tempY < destY) {
			restoreItemRect0(drawX, tempY - 16);
			tempY += addY;
			if (tempY > destY)
				tempY = destY;
			++addY;
			drawY = tempY - 16;
			backUpItemRect0(drawX, drawY);
			uint32 nextTime = _system->getMillis() + 1 * _tickLength;
			_screen->drawShape(0, _shapes[216+item], drawX, drawY, 0, 0);
			_screen->updateScreen();
			delayUntil(nextTime);
		}

		bool skip = false;
		if (x == destX) {
			if (destY - y <= 16)
				skip = true;
		}

		if (!skip) {
			snd_playSoundEffect(0x47);
			if (addY < 6)
				addY = 6;

			int xDiff = (destX - x) << 4;
			xDiff /= addY;
			int startAddY = addY;
			addY >>= 1;
			if (destY - y <= 8)
				addY >>= 1;
			addY = -addY;
			int unkX = x << 4;
			while (--startAddY) {
				drawX = (unkX >> 4) - 8;
				drawY = tempY - 16;
				restoreItemRect0(drawX, drawY);
				tempY += addY;
				unkX += xDiff;
				if (tempY > destY)
					tempY = destY;
				++addY;
				drawX = (unkX >> 4) - 8;
				drawY = tempY - 16;
				backUpItemRect0(drawX, drawY);
				uint32 nextTime = _system->getMillis() + 1 * _tickLength;
				_screen->drawShape(0, _shapes[216+item], drawX, drawY, 0, 0);
				_screen->updateScreen();
				delayUntil(nextTime);
			}
			restoreItemRect0(drawX, drawY);
		} else {
			restoreItemRect0(drawX, tempY - 16);
		}
	}
	currentRoom->itemsXPos[freeItem] = destX;
	currentRoom->itemsYPos[freeItem] = destY;
	currentRoom->itemsTable[freeItem] = item;
	snd_playSoundEffect(0x32);
	_animator->animAddGameItem(freeItem, _currentCharacter->sceneId);
	_screen->showMouse();
}

void KyraEngine_LoK::dropItem(int unk1, int item, int x, int y, int unk2) {
	if (processItemDrop(_currentCharacter->sceneId, item, x, y, unk1, unk2))
		return;
	snd_playSoundEffect(54);
	assert(_noDropList);
	if (12 == countItemsInScene(_currentCharacter->sceneId))
		drawSentenceCommand(_noDropList[0], 6);
	else
		drawSentenceCommand(_noDropList[1], 6);
}

void KyraEngine_LoK::itemSpecialFX(int x, int y, int item) {
	if (item == 41)
		itemSpecialFX1(x, y, item);
	else
		itemSpecialFX2(x, y, item);
}

void KyraEngine_LoK::itemSpecialFX1(int x, int y, int item) {
	uint8 *shape = _shapes[216+item];
	x -= 8;
	int startY = y;
	y -= 15;
	_screen->hideMouse();
	backUpItemRect0(x, y);
	for (int i = 1; i <= 16; ++i) {
		_screen->setNewShapeHeight(shape, i);
		--startY;
		restoreItemRect0(x, y);
		uint32 nextTime = _system->getMillis() + 1 * _tickLength;
		_screen->drawShape(0, shape, x, startY, 0, 0);
		_screen->updateScreen();
		delayUntil(nextTime);
	}
	restoreItemRect0(x, y);
	_screen->showMouse();
}

void KyraEngine_LoK::itemSpecialFX2(int x, int y, int item) {
	x -= 8;
	y -= 15;
	int yAdd = (int8)(((16 - _itemTable[item].height) >> 1) & 0xFF);
	backUpItemRect0(x, y);
	if (item >= 80 && item <= 89)
		snd_playSoundEffect(55);

	for (int i = 201; i <= 205; ++i) {
		restoreItemRect0(x, y);
		uint32 nextTime = _system->getMillis() + 3 * _tickLength;
		_screen->drawShape(0, _shapes[i], x, y + yAdd, 0, 0);
		_screen->updateScreen();
		delayUntil(nextTime);
	}

	for (int i = 204; i >= 201; --i) {
		restoreItemRect0(x, y);
		uint32 nextTime = _system->getMillis() + 3 * _tickLength;
		_screen->drawShape(0, _shapes[216+item], x, y, 0, 0);
		_screen->drawShape(0, _shapes[i], x, y + yAdd, 0, 0);
		_screen->updateScreen();
		delayUntil(nextTime);
	}
	restoreItemRect0(x, y);
}

void KyraEngine_LoK::magicOutMouseItem(int animIndex, int itemPos) {
	int videoPageBackUp = _screen->_curPage;
	_screen->_curPage = 0;
	int x = 0, y = 0;
	if (itemPos == -1) {
		Common::Point mouse = getMousePos();
		x = mouse.x - 12;
		y = mouse.y - 18;
	} else {
		x = _itemPosX[itemPos] - 4;
		y = _itemPosY[itemPos] - 3;
	}

	if (_itemInHand == -1 && itemPos == -1)
		return;

	int tableIndex = 0, loopStart = 0, maxLoops = 0;
	if (animIndex == 0) {
		tableIndex = _rnd.getRandomNumberRng(0, 5);
		loopStart = 35;
		maxLoops = 9;
	} else if (animIndex == 1) {
		tableIndex = _rnd.getRandomNumberRng(0, 11);
		loopStart = 115;
		maxLoops = 8;
	} else if (animIndex == 2) {
		tableIndex = 0;
		loopStart = 124;
		maxLoops = 4;
	} else {
		tableIndex = -1;
	}

	if (animIndex == 2)
		snd_playSoundEffect(0x5E);
	else
		snd_playSoundEffect(0x37);
	_screen->hideMouse();
	backUpItemRect1(x, y);

	for (int shape = _magicMouseItemStartFrame[animIndex]; shape <= _magicMouseItemEndFrame[animIndex]; ++shape) {
		restoreItemRect1(x, y);
		uint32 nextTime = _system->getMillis() + 4 * _tickLength;
		_screen->drawShape(0, _shapes[216+_itemInHand], x + 4, y + 3, 0, 0);
		if (tableIndex == -1)
			_screen->drawShape(0, _shapes[shape], x, y, 0, 0);
		else
			specialMouseItemFX(shape, x, y, animIndex, tableIndex, loopStart, maxLoops);
		_screen->updateScreen();
		delayUntil(nextTime);
	}

	if (itemPos != -1) {
		restoreItemRect1(x, y);
		_screen->fillRect(_itemPosX[itemPos], _itemPosY[itemPos], _itemPosX[itemPos] + 15, _itemPosY[itemPos] + 15, 12, 0);
		backUpItemRect1(x, y);
	}

	for (int shape = _magicMouseItemStartFrame2[animIndex]; shape <= _magicMouseItemEndFrame2[animIndex]; ++shape) {
		restoreItemRect1(x, y);
		uint32 nextTime = _system->getMillis() + 4 * _tickLength;
		_screen->drawShape(0, _shapes[216+_itemInHand], x + 4, y + 3, 0, 0);
		if (tableIndex == -1)
			_screen->drawShape(0, _shapes[shape], x, y, 0, 0);
		else
			specialMouseItemFX(shape, x, y, animIndex, tableIndex, loopStart, maxLoops);
		_screen->updateScreen();
		delayUntil(nextTime);
	}
	restoreItemRect1(x, y);
	if (itemPos == -1) {
		_screen->setMouseCursor(1, 1, _shapes[0]);
		_itemInHand = -1;
	} else {
		_characterList[0].inventoryItems[itemPos] = 0xFF;
		_screen->hideMouse();
		_screen->fillRect(_itemPosX[itemPos], _itemPosY[itemPos], _itemPosX[itemPos] + 15, _itemPosY[itemPos] + 15, 12, 0);
		_screen->showMouse();
	}
	_screen->showMouse();
	_screen->_curPage = videoPageBackUp;
}

void KyraEngine_LoK::magicInMouseItem(int animIndex, int item, int itemPos) {
	int videoPageBackUp = _screen->_curPage;
	_screen->_curPage = 0;
	int x = 0, y = 0;
	if (itemPos == -1) {
		Common::Point mouse = getMousePos();
		x = mouse.x - 12;
		y = mouse.y - 18;
	} else {
		x = _itemPosX[itemPos] - 4;
		y = _itemPosX[itemPos] - 3;
	}
	if (item < 0)
		return;

	int tableIndex = -1, loopStart = 0, maxLoops = 0;
	if (animIndex == 0) {
		tableIndex = _rnd.getRandomNumberRng(0, 5);
		loopStart = 35;
		maxLoops = 9;
	} else if (animIndex == 1) {
		tableIndex = _rnd.getRandomNumberRng(0, 11);
		loopStart = 115;
		maxLoops = 8;
	} else if (animIndex == 2) {
		tableIndex = 0;
		loopStart = 124;
		maxLoops = 4;
	}

	_screen->hideMouse();
	backUpItemRect1(x, y);
	if (animIndex == 2)
		snd_playSoundEffect(0x5E);
	else
		snd_playSoundEffect(0x37);

	for (int shape = _magicMouseItemStartFrame[animIndex]; shape <= _magicMouseItemEndFrame[animIndex]; ++shape) {
		restoreItemRect1(x, y);
		uint32 nextTime = _system->getMillis() + 4 * _tickLength;
		if (tableIndex == -1)
			_screen->drawShape(0, _shapes[shape], x, y, 0, 0);
		else
			specialMouseItemFX(shape, x, y, animIndex, tableIndex, loopStart, maxLoops);
		_screen->updateScreen();
		delayUntil(nextTime);
	}

	for (int shape = _magicMouseItemStartFrame2[animIndex]; shape <= _magicMouseItemEndFrame2[animIndex]; ++shape) {
		restoreItemRect1(x, y);
		uint32 nextTime = _system->getMillis() + 4 * _tickLength;
		if (tableIndex == -1)
			_screen->drawShape(0, _shapes[shape], x, y, 0, 0);
		else
			specialMouseItemFX(shape, x, y, animIndex, tableIndex, loopStart, maxLoops);
		_screen->updateScreen();
		delayUntil(nextTime);
	}
	restoreItemRect1(x, y);
	if (itemPos == -1) {
		_screen->setMouseCursor(8, 15, _shapes[216+item]);
		_itemInHand = item;
	} else {
		_characterList[0].inventoryItems[itemPos] = item;
		_screen->hideMouse();
		_screen->drawShape(0, _shapes[216+item], _itemPosX[itemPos], _itemPosY[itemPos], 0, 0);
		_screen->showMouse();
	}
	_screen->showMouse();
	_screen->_curPage = videoPageBackUp;
}

void KyraEngine_LoK::specialMouseItemFX(int shape, int x, int y, int animIndex, int tableIndex, int loopStart, int maxLoops) {
	static const uint8 table1[] = {
		0x23, 0x45, 0x55, 0x72, 0x84, 0xCF, 0x00, 0x00
	};
	static const uint8 table2[] = {
		0x73, 0xB5, 0x80, 0x21, 0x13, 0x39, 0x45, 0x55, 0x62, 0xB4, 0xCF, 0xD8
	};
	static const uint8 table3[] = {
		0x7C, 0xD0, 0x74, 0x84, 0x87, 0x00, 0x00, 0x00
	};
	int tableValue = 0;
	if (animIndex == 0)
		tableValue = table1[tableIndex];
	else if (animIndex == 1)
		tableValue = table2[tableIndex];
	else if (animIndex == 2)
		tableValue = table3[tableIndex];
	else
		return;
	processSpecialMouseItemFX(shape, x, y, tableValue, loopStart, maxLoops);
}

void KyraEngine_LoK::processSpecialMouseItemFX(int shape, int x, int y, int tableValue, int loopStart, int maxLoops) {
	uint8 shapeColorTable[16];
	uint8 *shapePtr = _shapes[shape] + 10;
	if (_flags.useAltShapeHeader)
		shapePtr += 2;

	for (int i = 0; i < 16; ++i)
		shapeColorTable[i] = shapePtr[i];

	for (int i = loopStart; i < loopStart + maxLoops; ++i) {
		for (int i2 = 0; i2 < 16; ++i2) {
			if (shapePtr[i2] == i)
				shapeColorTable[i2] = (i + tableValue) - loopStart;
		}
	}
	_screen->drawShape(0, _shapes[shape], x, y, 0, 0x8000, shapeColorTable);
}

void KyraEngine_LoK::updatePlayerItemsForScene() {
	if (_itemInHand >= 29 && _itemInHand < 33) {
		++_itemInHand;
		if (_itemInHand > 33)
			_itemInHand = 33;
		_screen->hideMouse();
		_screen->setMouseCursor(8, 15, _shapes[216+_itemInHand]);
		_screen->showMouse();
	}

	bool redraw = false;
	for (int i = 0; i < 10; ++i) {
		uint8 item = _currentCharacter->inventoryItems[i];
		if (item >= 29 && item < 33) {
			++item;
			if (item > 33)
				item = 33;
			_currentCharacter->inventoryItems[i] = item;
			redraw = true;
		}
	}

	if (redraw) {
		_screen->hideMouse();
		redrawInventory(0);
		_screen->showMouse();
	}

	if (_itemInHand == 33)
		magicOutMouseItem(2, -1);

	_screen->hideMouse();
	for (int i = 0; i < 10; ++i) {
		uint8 item = _currentCharacter->inventoryItems[i];
		if (item == 33)
			magicOutMouseItem(2, i);
	}
	_screen->showMouse();
}

void KyraEngine_LoK::redrawInventory(int page) {
	int videoPageBackUp = _screen->_curPage;
	_screen->_curPage = page;
	_screen->hideMouse();
	for (int i = 0; i < 10; ++i) {
		_screen->fillRect(_itemPosX[i], _itemPosY[i], _itemPosX[i] + 15, _itemPosY[i] + 15, 12, page);
		if (_currentCharacter->inventoryItems[i] != 0xFF) {
			uint8 item = _currentCharacter->inventoryItems[i];
			_screen->drawShape(page, _shapes[216+item], _itemPosX[i], _itemPosY[i], 0, 0);
		}
	}
	_screen->showMouse();
	_screen->_curPage = videoPageBackUp;
	_screen->updateScreen();
}

void KyraEngine_LoK::backUpItemRect0(int xpos, int ypos) {
	_screen->rectClip(xpos, ypos, 3<<3, 24);
	_screen->copyRegionToBuffer(_screen->_curPage, xpos, ypos, 3<<3, 24, _itemBkgBackUp[0]);
}

void KyraEngine_LoK::restoreItemRect0(int xpos, int ypos) {
	_screen->rectClip(xpos, ypos, 3<<3, 24);
	_screen->copyBlockToPage(_screen->_curPage, xpos, ypos, 3<<3, 24, _itemBkgBackUp[0]);
}

void KyraEngine_LoK::backUpItemRect1(int xpos, int ypos) {
	_screen->rectClip(xpos, ypos, 4<<3, 32);
	_screen->copyRegionToBuffer(_screen->_curPage, xpos, ypos, 4<<3, 32, _itemBkgBackUp[1]);
}

void KyraEngine_LoK::restoreItemRect1(int xpos, int ypos) {
	_screen->rectClip(xpos, ypos, 4<<3, 32);
	_screen->copyBlockToPage(_screen->_curPage, xpos, ypos, 4<<3, 32, _itemBkgBackUp[1]);
}

} // end of namespace Kyra

