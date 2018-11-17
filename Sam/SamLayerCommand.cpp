#include "SamLayerCommand.h"

#include "ALog.h"

#include "SamInstrument.h"

SamLayerCommand::SamLayerCommand(SamLayer *layer, int propertyId, float value)
: m_layer(layer)
, m_propertyId(propertyId)
, m_value(value)
, m_state(false)
{
	aLog("SamLayerCommand::SamLayerCommand(value)");
}

SamLayerCommand::SamLayerCommand(SamLayer *layer, int propertyId, AString text)
: m_layer(layer)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_text(text)
, m_state(false)
{
	aLog("SamLayerCommand::SamLayerCommand(text)");
}

SamLayerCommand::SamLayerCommand(SamLayer *layer, int propertyId, bool state)
: m_layer(layer)
, m_propertyId(propertyId)
, m_value(0.0f)
, m_state(state)
{
	aLog("SamLayerCommand::SamLayerCommand(state)");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayerCommand::execute(ACommandManager *commandManager, int id)
{
	aLog("SamLayerCommand::execute()");

	ACommand::execute(commandManager, id);

	if(!m_layer) {
		aLog("!m_layer");
		return;
	}

	switch(m_propertyId) {
		case Name:
			m_layer->setName(m_text);
			break;
		case ZoneSelectMode:
			m_layer->setZoneSelectMode((int)m_value);
			break;
		case ShortNoteMode:
			m_layer->setShortNoteMode(m_state);
			break;
		case FineTune:
			m_layer->setFineTune((int)m_value);
			break;
		case PitchBendUp:
			m_layer->setPitchBendUp((int)m_value);
			break;
		case PitchBendDown:
			m_layer->setPitchBendDown((int)m_value);
			break;
		case ReleaseVoices:
			m_layer->setReleaseVoices((int)m_value);
			break;
		case DownbeatSampleOffset:
			m_layer->setDownbeatSampleOffset((int)m_value);
			break;
		case DownbeatEnvelopeOffset:
			m_layer->setDownbeatEnvelopeOffset((int)m_value);
			break;
		case MidiVelocityLow:
			m_layer->setMidiVelocityLow((int)m_value);
			break;
		case MidiVelocityHigh:
			m_layer->setMidiVelocityHigh((int)m_value);
			break;
		case MidiPitchLow:
			m_layer->setMidiPitchLow((int)m_value);
			break;
		case MidiPitchHigh:
			m_layer->setMidiPitchHigh((int)m_value);
			break;
		case PedalMode:
			aLog("PedalMode: %d", (int)m_value);
			m_layer->setPedalMode((int)m_value);
			break;
		case IntervalLow:
			m_layer->setIntervalLow((int)m_value);
			break;
		case IntervalHigh:
			m_layer->setIntervalHigh((int)m_value);
			break;
		case IntervalRecorded:
			m_layer->setIntervalRecorded((int)m_value);
			break;
		case TimeMin:
			m_layer->setTimeFromLastNoteMin((int)m_value);
			break;
		case TimeMax:
			m_layer->setTimeFromLastNoteMax((int)m_value);
			break;
		case TimeMaxInf:
			m_layer->setTimeFromLastNoteMaxInf(m_state);
			break;
		case Gain:
			m_layer->setGain(m_value);
			break;
	}
}