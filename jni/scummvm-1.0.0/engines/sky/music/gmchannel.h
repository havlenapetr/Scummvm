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
 * $URL: https://scummvm.svn.sourceforge.net/svnroot/scummvm/scummvm/tags/release-1-0-0/engines/sky/music/gmchannel.h $
 * $Id: gmchannel.h 27024 2007-05-30 21:56:52Z fingolfin $
 *
 */

#ifndef SKY_MUSIC_GMCHANNEL_H
#define SKY_MUSIC_GMCHANNEL_H

#include "sky/music/musicbase.h"

class MidiDriver;

namespace Sky {

typedef struct {
	uint16 eventDataPtr;
	int32 nextEventTime;
	uint16 loopPoint;
	uint8 midiChannelNumber;
	uint8 note;
	bool channelActive;
} MidiChannelType;

class GmChannel : public ChannelBase {
public:
	GmChannel(uint8 *pMusicData, uint16 startOfData, MidiDriver *pMidiDrv, const byte *pInstMap, const byte *veloTab);
	~GmChannel(void);
	virtual void stopNote(void);
	virtual uint8 process(uint16 aktTime);
	virtual void updateVolume(uint16 pVolume);
	virtual bool isActive(void);
private:
	const byte *_instMap;
	const byte *_veloTab;
	MidiDriver *_midiDrv;
	uint8 *_musicData;
	uint16 _musicVolume;
	MidiChannelType _channelData;
	uint8 _currentChannelVolume;
	//-                          normal subs
	void setRegister(uint8 regNum, uint8 value);
	int32 getNextEventTime(void);
	uint16 getNextNote(uint8 param);
	void adlibSetupInstrument(void);
	void setupInstrument(uint8 opcode);
	void setupChannelVolume(uint8 volume);
	//-                          Streamfunctions from Command90hTable
	void com90_caseNoteOff(void);        // 0
	void com90_stopChannel(void);        // 1
	void com90_setupInstrument(void);    // 2
	uint8 com90_updateTempo(void);       // 3
	//void com90_dummy(void);            // 4
	void com90_getPitch(void);           // 5
	void com90_getChannelVolume(void);   // 6
	//void com90_skipTremoVibro(void);   // 7
	void com90_loopMusic(void);        // 8
	void com90_keyOff(void);             // 9
	//void com90_error(void);            // 10
	void com90_getChannelPanValue(void); // 11
	void com90_setLoopPoint(void);     // 12
	void com90_getChannelControl(void);     // 13
};

} // End of namespace Sky

#endif //SKYGMCHANNEL_H
