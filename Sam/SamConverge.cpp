#include "SamConverge.h"

#include "SamVoice.h"

#include "ALog.h"

SamConverge::SamConverge(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, "Converge", SAM_CONVERGE_UUID)
	, m_pitch(62)
	, m_midiController(8)
	, m_value(0.0f)
	, m_tracking(1)
	, m_pitchController(9)
{
	addParameter(new SamToolParameter(pmrPitch, "Pitch", "semitones", 0, 1, 127));
	addParameter(new SamToolParameter(pmrMidiController, "Midi Controller", "", 0, -1, 127));
	addParameter(new SamToolParameter(pmrTracking, "Tracking", "", 0, 0, 1));
	addParameter(new SamToolParameter(pmrPitchController, "Pitch Controller", "", 0, -1, 127));

	//for(int i=0; i<127; i++) {
	//	m_pitchOffsets[i] = 0;
	//}
}

SamConverge::~SamConverge()
{
}

void SamConverge::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
		case SamEvent::NoteOff:
			{
				float p = (m_pitch - ev->midiValue1) * 100;
				p *= m_value;
				ev->initialPitchMod = (int)p;
				//aLog("%d = (%d - %d) * 100", ev->tuning, m_pitch, ev->midiValue1);
				/*if(ev->type == SamEvent::NoteOn) {
					m_pitchOffsets[ev->midiValue1] = ev->tuning;
				} else {
					m_pitchOffsets[ev->midiValue1] = 0;
				}*/
			}
			break;
		/*case SamEvent::NoteOn:
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
			break;*/
		case SamEvent::Controller:
			{
				bool update = false;
				if(ev->midiValue1 == m_midiController) {
					m_value = 0.00787401575f * (float)ev->midiValue2; //(1.0f/127.0f)
					update = true;
				} else if(ev->midiValue1 == m_pitchController) {
					m_pitch = ev->midiValue2;
					update = true;
				}
				
				if(m_tracking && update) {
					SamEvent vev(SamEvent::VoicePitch);
					vev.timestamp = ev->timestamp;
					for(int i=0; i<toolMaster()->numVoices(); i++) {
						SamVoice *v = toolMaster()->voice(i);
						if(v->isPlaying()) {
							//aLog("voice(%d) is playing with id %d", i, v->id());
							vev.iValue = v->id();
							vev.fValue = (m_pitch - v->pitch()) * 100;
							vev.fValue *= m_value;
							toolMaster()->queueEvent(vev);
						}
					}
				}
				
			}
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

float SamConverge::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrPitch:           return parameterFromId(id)->normalisedFromValue(m_pitch); break;
		case pmrMidiController:  return parameterFromId(id)->normalisedFromValue(m_midiController); break;
		case pmrTracking:        return parameterFromId(id)->normalisedFromValue(m_tracking); break;
		case pmrPitchController: return parameterFromId(id)->normalisedFromValue(m_pitchController); break;
	}

	return 0.0f;
}

void SamConverge::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrPitch:
			m_pitch = parameterFromId(pmrPitch)->valueFromNormalised(value);
			break;
		case pmrMidiController:
			m_midiController = parameterFromId(pmrMidiController)->valueFromNormalised(value);
			break;
		case pmrTracking:
			m_tracking = parameterFromId(pmrTracking)->valueFromNormalised(value);
			break;
		case pmrPitchController:
			m_pitchController = parameterFromId(pmrPitchController)->valueFromNormalised(value);
			break;
	}
}