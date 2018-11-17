#ifndef __SamLayerCommand_h__
#define __SamLayerCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamLayer.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamLayerCommand : public ACommand
{
public:
	SamLayerCommand(SamLayer *layer, int propertyId, float value);
	SamLayerCommand(SamLayer *layer, int propertyId, AString text);
	SamLayerCommand(SamLayer *layer, int propertyId, bool state);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamLayerPropertyId {
		Name                   =  0,
		ZoneSelectMode         =  1,
		FineTune               =  2,
		DownbeatSampleOffset   =  3,
		DownbeatEnvelopeOffset =  4,
		MidiVelocityLow        =  5,
		MidiVelocityHigh       =  6,
		MidiPitchLow           =  7,
		MidiPitchHigh          =  8,
		IntervalLow            =  9,
		IntervalHigh           = 10,
		IntervalRecorded       = 11,
		TimeMin                = 12,
		TimeMax                = 13,
		TimeMaxInf             = 14,
		ShortNoteMode          = 15,
		ReleaseVoices          = 16,
		Gain                   = 17,
		PitchBendUp            = 18,
		PitchBendDown          = 19,
		PedalMode              = 20
	};

protected:
private:
	SamLayer *m_layer;
	int m_propertyId;
	float m_value;
	AString m_text;
	bool m_state;
};

#endif