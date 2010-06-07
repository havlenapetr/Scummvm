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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/saga/animation.h $
 * $Id: animation.h 35467 2008-12-21 15:59:05Z thebluegr $
 *
 */

// Background animation management module private header

#ifndef SAGA_ANIMATION_H
#define SAGA_ANIMATION_H

namespace Saga {

#define MAX_ANIMATIONS 10
#define DEFAULT_FRAME_TIME 140

#define SAGA_FRAME_START 0xF
#define SAGA_FRAME_END 0x3F
#define SAGA_FRAME_NOOP 0x1F
#define SAGA_FRAME_REPOSITION 0x30
#define SAGA_FRAME_ROW_END 0x2F
#define SAGA_FRAME_LONG_COMPRESSED_RUN	0x20
#define SAGA_FRAME_LONG_UNCOMPRESSED_RUN	0x10
#define SAGA_FRAME_COMPRESSED_RUN 0x80
#define SAGA_FRAME_UNCOMPRESSED_RUN 0x40
#define SAGA_FRAME_EMPTY_RUN 0xC0

enum AnimationState {
	ANIM_PLAYING = 0x01,
	ANIM_PAUSE = 0x02,
	ANIM_STOPPING = 0x03
};

enum AnimationFlags {
	ANIM_FLAG_NONE = 0x00,
	ANIM_FLAG_ENDSCENE = 0x01	// When animation ends, dispatch scene end event
};

// Cutaway info array member. Cutaways are basically animations with a really
// bad attitude.
struct Cutaway {
	uint16 backgroundResourceId;
	uint16 animResourceId;
	int16 cycles;
	int16 frameRate;
};

// Animation info array member
struct AnimationData {
	byte *resourceData;
	size_t resourceLength;

	uint16 magic;

	uint16 screenWidth;
	uint16 screenHeight;

	byte unknown06;
	byte unknown07;

	int16 maxFrame;
	int16 loopFrame;

	int16 start;

	int16 currentFrame;
	size_t *frameOffsets;

	uint16 completed;
	uint16 cycles;

	int frameTime;

	AnimationState state;
	int16 linkId;
	uint16 flags;

	AnimationData(const byte *animResourceData, size_t animResourceLength) {
		memset(this, 0, sizeof(*this));
		resourceLength = animResourceLength;
		resourceData = (byte*)malloc(animResourceLength);
		memcpy(resourceData, animResourceData, animResourceLength);
	}
	~AnimationData() {
		free(frameOffsets);
		free(resourceData);
	}
};

class Anim {
public:
	Anim(SagaEngine *vm);
	~Anim(void);

	void loadCutawayList(const byte *resourcePointer, size_t resourceLength);
	void freeCutawayList(void);
	int playCutaway(int cut, bool fade);
	void endCutaway(void);
	void returnFromCutaway(void);
	void clearCutaway(void);
	void showCutawayBg(int bg);

	void startVideo(int vid, bool fade);
	void endVideo(void);
	void returnFromVideo(void);

	void load(uint16 animId, const byte *animResourceData, size_t animResourceLength);
	void freeId(uint16 animId);
	void play(uint16 animId, int vectorTime, bool playing = true);
	void link(int16 animId1, int16 animId2);
	void setFlag(uint16 animId, uint16 flag);
	void clearFlag(uint16 animId, uint16 flag);
	void setFrameTime(uint16 animId, int time);
	void reset(void);
	void animInfo(void);
	void cutawayInfo(void);
	void setCycles(uint16 animId, int cycles);
	void stop(uint16 animId);
	void finish(uint16 animId);
	void resume(uint16 animId, int cycles);
	void resumeAll();
	int16 getCurrentFrame(uint16 animId);
	int getFrameTime(uint16 animId);
	int getCycles(uint16 animId);
	bool isPlaying(uint16 animId);

	bool hasAnimation(uint16 animId) {
		if (animId >= MAX_ANIMATIONS) {
			if (animId < MAX_ANIMATIONS + ARRAYSIZE(_cutawayAnimations))
				return (_cutawayAnimations[animId - MAX_ANIMATIONS] != NULL);
			return false;
		}
		return (_animations[animId] != NULL);
	}

	bool hasCutaway(void) { return _cutawayActive; }
	void setCutAwayMode(int mode) { _cutAwayMode = mode; }
	int cutawayListLength() { return _cutawayListLength; }
	int cutawayBgResourceID(int cutaway) { return _cutawayList[cutaway].backgroundResourceId; }
	int cutawayAnimResourceID(int cutaway) { return _cutawayList[cutaway].animResourceId; }

private:
	void decodeFrame(AnimationData *anim, size_t frameOffset, byte *buf, size_t bufLength);
	int fillFrameOffsets(AnimationData *anim, bool reallyFill = true);

	void validateAnimationId(uint16 animId) {
		if (animId >= MAX_ANIMATIONS) {
			// Cutaway
			if (animId >= MAX_ANIMATIONS + ARRAYSIZE(_cutawayAnimations))
				error("validateAnimationId: animId out of range");
			if (_cutawayAnimations[animId - MAX_ANIMATIONS] == NULL) {
				error("validateAnimationId: animId=%i unassigned", animId);
			}
		} else {
			// Animation
			if (_animations[animId] == NULL) {
				error("validateAnimationId: animId=%i unassigned.", animId);
			}
		}
	}

	bool isLongData() const {
		if ((_vm->getGameId() == GID_ITE) && (_vm->getPlatform() != Common::kPlatformMacintosh)) {
			return false;
		}
		return true;
	}

	AnimationData* getAnimation(uint16 animId) {
		validateAnimationId(animId);
		if (animId >= MAX_ANIMATIONS)
			return _cutawayAnimations[animId - MAX_ANIMATIONS];
		return _animations[animId];
	}

	uint16 getAnimationCount() const {
		uint16 i = 0;
		for (; i < MAX_ANIMATIONS; i++) {
			if (_animations[i] == NULL) {
				break;
			}
		}
		return i;
	}

	SagaEngine *_vm;
	AnimationData *_animations[MAX_ANIMATIONS];
	AnimationData *_cutawayAnimations[2];
	Cutaway *_cutawayList;
	PalEntry saved_pal[PAL_ENTRIES];
	int _cutawayListLength;
	bool _cutawayActive;
	int _cutAwayMode;
	bool _cutAwayFade;
};

} // End of namespace Saga

#endif				// ANIMATION_H_
