#ifndef __CsMidiEvent_h__
#define __CsMidiEvent_h__

#include "APlatform.h"

namespace Cs {

class CsMidiMessage
{
public:
	enum CsMidiEventType {
		Unidentified        = 0x00,
		NoteOff             = 0x80,
		NoteOn              = 0x90,
		Aftertouch          = 0xA0,
		Controller          = 0xB0,
		ProgramChange       = 0xC0,
		ChannelPressure     = 0xD0,
		PitchBend           = 0xE0,
		SystemExclusive     = 0xF0,
		MtcQuarterFrame     = 0xF1,
		SongPositionPointer = 0xF2,
		SongSelect          = 0xF3,
		TuneRequest         = 0xF6,
		MidiClock           = 0xF8,
		MidiTick            = 0xF9,
		MidiStart           = 0xFA,
		MidiContinue        = 0xFB,
		MidiStop            = 0xFC,
		ActiveSense         = 0xFE
	};

	CsMidiMessage(int t = Unidentified);

	void identify(void);
	int defineLength(void);
	unsigned short twoByteValue(void);
	void buildFirstByte(void);
	void buildFromWinMme(DWORD dwParam);

	unsigned char type;
	int length;
	unsigned char data[256];
	double timestamp;
	void *handle;
	//int user;
	unsigned char device;
	unsigned char port;
	unsigned char channel;
	
};

}

#endif