#include "SamDoubler.h"

#include "SamVoice.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamDoubler::SamDoubler(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, "Doubler", SAM_DOUBLER_UUID)
	, m_pitch(2)
	, m_velocity(0)
	, m_delay(0)
{
	addParameter(new SamToolParameter(pmrPitch, "Doubled Pitch", "semitones", 0, -12, 12));
	addParameter(new SamToolParameter(pmrVelocity, "Velocity Adjust", "", 0, -64, 64));
	addParameter(new SamToolParameter(pmrDelay, "Delay", "ms", 0, 0, 250));

	parameter(2)->setMidiMax(0.5);

	//for(int i=0; i<127; i++) {
	//	m_pitchOffsets[i] = 0;
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamDoubler::~SamDoubler()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamDoubler::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
		case SamEvent::NoteOff:
			{

				//float p = (m_pitch - ev->midiValue1) * 100;
				//p *= m_value;
				//ev->initialPitchMod = (int)p;
				//aLog("%d = (%d - %d) * 100", ev->tuning, m_pitch, ev->midiValue1);

				SamEvent nev = *ev;
				
				nev.timestamp += (m_delay * 1000000);
				nev.midiValue1 += m_pitch;
				nev.initialPitchMod = (ev->midiValue1 - nev.midiValue1) * 100;
				//nev.midiValue2 = ev->midiValue2;
				if(nev.type == SamEvent::NoteOn) {
					//nev.timestamp += (m_delay * 1000000);
					nev.midiValue2 += m_velocity;
				}
				if(nev.midiValue2 > 127) nev.midiValue2 = 127;
				if(nev.midiValue2 < 0)   nev.midiValue2 = 0;
				//aLog("nev.midiValue2 = %d (m_velocity = %d)", nev.midiValue2, m_velocity);

				toolMaster()->queueEvent(nev);
			}
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamDoubler::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrPitch:    return parameterFromId(id)->normalisedFromValue(m_pitch); break;
		case pmrVelocity: return parameterFromId(id)->normalisedFromValue(m_velocity); break;
		case pmrDelay:    return parameterFromId(id)->normalisedFromValue(m_delay); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamDoubler::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrPitch:
			m_pitch = parameterFromId(pmrPitch)->valueFromNormalised(value);
			break;
		case pmrVelocity:
			m_velocity = parameterFromId(pmrVelocity)->valueFromNormalised(value);
			break;
		case pmrDelay:
			m_delay = parameterFromId(pmrDelay)->valueFromNormalised(value);
			break;
	}
}