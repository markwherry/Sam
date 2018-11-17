#ifndef __SamEvent_h__
#define __SamEvent_h__

class SamEvent
{
public:
	enum SamEventTypes {
		Unassigned = 0,
		NoteOff    = 0x80,
		NoteOn     = 0x90,
		Aftertouch = 0xA0,
		Controller = 0xB0,
		Program    = 0xC0,
		Pressure   = 0xD0,
		PitchBend  = 0xE0,

		Parameter  = 0xA00,
		Tempo      = 0xB00,

		VoiceVolume = 0xC00,
		VoicePitch  = 0xC01
	};

	SamEvent(int t = Unassigned)
		: type(t)
		, subType(0)
		, channel(0)
		, timestamp(0.0f)
		, delta(0)
		, iValue(0)
		, fValue(0.0f)
		, midiValue1(0)
		, midiValue2(0)
		, articulationIndex(-1)
		, layerIndex(0)
		, from(0)
		//, grouped(0)
		//, start(0)
		, startOffset(0)
		, attackOffset(0)
		, tuning(0)
		, initialPitchMod(0)
		, triggeredPitch(0)
	{
	}

	SamEvent(const SamEvent& ev)
		: type(ev.type)
		, subType(ev.subType)
		, channel(ev.channel)
		, timestamp(ev.timestamp)
		, delta(ev.delta)
		, iValue(ev.iValue)
		, fValue(ev.fValue)
		, midiValue1(ev.midiValue1)
		, midiValue2(ev.midiValue2)
		, articulationIndex(ev.articulationIndex)
		, layerIndex(ev.layerIndex)
		, from(ev.from)
		//, grouped(ev.grouped)
		//, start(ev.start)
		, startOffset(ev.startOffset)
		, attackOffset(ev.attackOffset)
		, tuning(ev.tuning)
		, initialPitchMod(ev.initialPitchMod)
		, triggeredPitch(ev.triggeredPitch)
	{
	}

	// Variables declared in decreasing size order for 64-bit alignment optimisation, per PVS
	double timestamp;
	void *from;
	float fValue;
	int type;
	int subType;
	int channel;
	int delta;
	int iValue;
	
	int articulationIndex;
	int layerIndex;
	
	int triggeredPitch;
	//int grouped;
	int startOffset;
	int attackOffset;
	int tuning;
	int initialPitchMod;

	char midiValue1;
	char midiValue2;
	
protected:
private:
};

#endif