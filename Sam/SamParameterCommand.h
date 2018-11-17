#ifndef __SamParameterCommand_h__
#define __SamParameterCommand_h__

#include "ACommand.h"
#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "SamParameter.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class SamParameterCommand : public ACommand
{
public:
	SamParameterCommand(SamParameter *parameter, int propertyId, float value);
	SamParameterCommand(SamParameter *parameter, int propertyId, AString text);
	SamParameterCommand(SamParameter *parameter, int propertyId, bool state);

	void execute(ACommandManager *commandManager = 0, int id = 0);

	enum SamParameterPropertyId {

		Name             =  0,
		Source           =  1,
		Type             =  2,

		MidiType         = 10,
		MidiController   = 11,

		TriggerValue     = 20,
		TriggerMidiValue = 21,

		RangeCurveStart  = 30,
		RangeCurveEnd    = 31,
		RangeCurveShape  = 32,
		RangeCurveDepth  = 33,
		RangeInverted    = 34,
		DefaultValue     = 35
	};

protected:
private:
	SamParameter *m_parameter;
	int m_propertyId;
	float m_value;
	AString m_text;
	bool m_state;
};

#endif