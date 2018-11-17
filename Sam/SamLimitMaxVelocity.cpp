#include "SamLimitMaxVelocity.h"

#include "ALog.h"

SamLimitMaxVelocity::SamLimitMaxVelocity(SamToolMaster *toolMaster)
: SamMusicTool(toolMaster, "Limit Max Velocity", "689114F1-82D0-404e-B444-391BEBC0FAD9")
, m_maxVelocity(127)
, m_midiController(-1)
{
	addParameter(new SamToolParameter(pmrMaxVelocity, "Max Velocity", "semitones", 0, 1, 127));
	addParameter(new SamToolParameter(pmrMidiController, "Midi Controller", "", 0, -1, 127));
}

SamLimitMaxVelocity::~SamLimitMaxVelocity()
{
}

void SamLimitMaxVelocity::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
		//case SamEvent::NoteOff:
			{
				if(m_maxVelocity == 1) {
					ev->midiValue2 = 1;
				} else {
					float depth = (1.0/127.0) * (float)ev->midiValue2;
					float vel = (float)m_maxVelocity * depth;
					ev->midiValue2 = (int)vel;
					//aLog("ev->midiValue2 = %d (m_maxVelocity = %d)", ev->midiValue2, m_maxVelocity);
				}
			}
			break;
		case SamEvent::Controller:
			if(m_midiController > -1) {
				if(ev->midiValue1 == m_midiController) {
					m_maxVelocity = ev->midiValue2;
					if(m_maxVelocity < 1) {
						m_maxVelocity = 1;
					}
				}
			}
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

float SamLimitMaxVelocity::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrMaxVelocity: return parameterFromId(id)->normalisedFromValue(m_maxVelocity); break;
		case pmrMidiController: return parameterFromId(id)->normalisedFromValue(m_midiController); break;
	}

	return 0.0f;
}

void SamLimitMaxVelocity::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrMaxVelocity:
			m_maxVelocity = parameterFromId(pmrMaxVelocity)->valueFromNormalised(value);
			break;
		case pmrMidiController:
			m_midiController = parameterFromId(pmrMidiController)->valueFromNormalised(value);
			break;
	}
}