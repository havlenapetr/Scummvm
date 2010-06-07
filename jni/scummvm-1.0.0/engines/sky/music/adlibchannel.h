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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/music/adlibchannel.h $
 * $Id: adlibchannel.h 30664 2008-01-27 19:47:41Z jvprat $
 *
 */

#ifndef SKY_MUSIC_ADLIBCHANNEL_H
#define SKY_MUSIC_ADLIBCHANNEL_H

#include "sky/music/musicbase.h"
#include "sound/fmopl.h"

namespace Sky {

typedef struct {
	uint8 ad_Op1, ad_Op2;
	uint8 sr_Op1, sr_Op2;
	uint8 ampMod_Op1, ampMod_Op2;
	uint8 waveSelect_Op1, waveSelect_Op2;
	uint8 bindedEffect;
	uint8 feedBack;
	uint8 totOutLev_Op1, totOutLev_Op2;
	uint8 scalingLevel;
	uint8 pad1, pad2, pad3;
} InstrumentStruct;

typedef struct {
	uint16 eventDataPtr;
	int32 nextEventTime;
	uint16 loopPoint;
	uint8 adlibChannelNumber;
	uint8 lastCommand;
	bool channelActive;
	uint8 note;
	uint8 adlibReg1, adlibReg2;
	InstrumentStruct *instrumentData;
	uint8 assignedInstrument;
	uint8 channelVolume;
	uint8 padding; // field_12 / not used by original driver
	uint8 tremoVibro;
	uint8 freqOffset;
	uint16 frequency;
} AdlibChannelType;

class AdlibChannel : public ChannelBase {
public:
	AdlibChannel (FM_OPL *opl, uint8 *pMusicData, uint16 startOfData);
	virtual ~AdlibChannel(void);
	virtual uint8 process(uint16 aktTime);
	virtual void updateVolume(uint16 pVolume);
	virtual bool isActive(void);
private:
	FM_OPL *_opl;
	uint8 *_musicData;
	AdlibChannelType _channelData;

	InstrumentStruct *_instruments;
	uint16 *_frequenceTable;
	uint8 *_instrumentMap;
	uint8 *_registerTable, *_opOutputTable;
	uint8 *_adlibRegMirror;

	// normal subs
	void setRegister(uint8 regNum, uint8 value);
	int32 getNextEventTime(void);
	uint16 getNextNote(uint8 param);
	void adlibSetupInstrument(void);
	void setupInstrument(uint8 opcode);
	void setupChannelVolume(uint8 volume);
	void stopNote(void);

	// Streamfunctions from Command90hTable
	void com90_caseNoteOff(void);       // 0
	void com90_stopChannel(void);       // 1
	void com90_setupInstrument(void);   // 2
	uint8 com90_updateTempo(void);      // 3
	//void com90_dummy(void);           // 4
	void com90_getFreqOffset(void);     // 5
	void com90_getChannelVolume(void);  // 6
	void com90_getTremoVibro(void);     // 7
	void com90_loopMusic(void);         // 8
	void com90_keyOff(void);            // 9
	//void com90_error(void);           // 10
	//void com90_doLodsb(void);         // 11
	void com90_setLoopPoint(void);      // 12
	//void com90_do_two_Lodsb(void);    // 13
};

} // End of namespace Sky

#endif //ADLIBCHANNEL_H
