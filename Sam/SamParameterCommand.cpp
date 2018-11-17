#include "SamParameterCommand.h"

#include "ALog.h"

SamParameterCommand::SamParameterCommand(SamParameter *parameter, int propertyId, float value)
: m_parameter(parameter)
, m_propertyId(propertyId)
, m_value(value)
, m_state(false)
{
	aLog("SamParameterCommand::SamParameterCommand(value)");
}

SamParameterCommand::SamParameterCommand(SamParameter *parameter, int propertyId, AString text)
: m_parameter(parameter)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
, m_state(false)
{
	aLog("SamParameterCommand::SamParameterCommand(text)");
}

SamParameterCommand::SamParameterCommand(SamParameter *parameter, int propertyId, bool state)
: m_parameter(parameter)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_state(state)
{
	aLog("SamParameterCommand::SamParameterCommand(state)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamParameterCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamParameterCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_parameter) {
		aLog("!m_parameter");
		return;
	}

	switch(m_propertyId) {
		case Name:
			m_parameter->setName(m_text);
			break;
		case Source:
			m_parameter->setSource((int)m_value);
			break;
		case Type:
			m_parameter->setType((int)m_value);
			break;

		case MidiType:
			m_parameter->setMidiType((int)m_value);
			break;
		case MidiController:
			m_parameter->setMidiController((int)m_value);
			break;

		case TriggerValue:
			m_parameter->setTriggerValue((int)m_value);
			break;
		case TriggerMidiValue:
			m_parameter->setMidiValue((int)m_value);
			break;

		case RangeCurveStart:
			m_parameter->curve()->setStart(m_value);
			break;
		case RangeCurveEnd:
			m_parameter->curve()->setEnd(m_value);
			break;
		case RangeCurveShape:
			m_parameter->curve()->setShape(m_value);
			break;
		case RangeCurveDepth:
			m_parameter->curve()->setDepth(m_value);
			break;
		case RangeInverted:
			m_parameter->setInverted(m_state);
			break;
		case DefaultValue:
			m_parameter->setDefaultValue(m_value/100.0);
			break;
	}
}