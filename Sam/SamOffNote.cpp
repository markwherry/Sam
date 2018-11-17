#include "SamOffNote.h"

#include "ALog.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOffNote::SamOffNote(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, name(), uuid())
	, m_repeats(0)
	, m_velocity(0)
	, m_fade(0)
{
	addParameter(new SamToolParameter(pmrRepeats, "Repeat Notes", "", 0,  0, 8));
	addParameter(new SamToolParameter(pmrFade, "Velocity Mode", "", 0, 0, 1));
	addParameter(new SamToolParameter(pmrVelocity, "Adjust Velocity", "", 0, -64, +64));
	
	for(int i=0; i<128; i++) {
		m_previousVelocities[i] = 0;
		m_previousTimestamps[i] = 0.0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOffNote::~SamOffNote()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamOffNote::processEvent(SamEvent *ev)
{
	//aLog("SamOffNote::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
			m_previousVelocities[ev->midiValue1] = ev->midiValue2;
			m_previousTimestamps[ev->midiValue1] = ev->timestamp;
			break;

		case SamEvent::NoteOff:
			{
				SamEvent nev = *ev;
				
				//nev.timestamp += (m_delay * 1000000);
				//nev.midiValue1 += m_pitch;
				//nev.initialPitchMod = (ev->midiValue1 - nev.midiValue1) * 100;
				//nev.midiValue2 = ev->midiValue2;
				nev.type = SamEvent::NoteOn;
				nev.midiValue2 = m_previousVelocities[nev.midiValue1] + m_velocity; 

				if(nev.midiValue2 > 127) nev.midiValue2 = 127;
				if(nev.midiValue2 < 0)   nev.midiValue2 = 0;
				//aLog("nev.midiValue2 = %d (m_velocity = %d)", nev.midiValue2, m_velocity);

				toolMaster()->queueEvent(nev);

				double length = nev.timestamp - m_previousTimestamps[ev->midiValue1];

				nev.timestamp += length;
				nev.type = SamEvent::NoteOff;
				toolMaster()->queueEvent(nev);

				


				for(int i=0; i<m_repeats; i++) {
					// Return the event to the pre-note
					nev.type = SamEvent::NoteOn;
					

					
					if(m_fade) {
						nev.midiValue2 += m_velocity;
						aLog("midiValue2 = %d", nev.midiValue2);
						if(nev.midiValue2 > 127) nev.midiValue2 = 127;
						if(nev.midiValue2 < 0)   nev.midiValue2 = 0;
					}
					toolMaster()->queueEvent(nev);
					nev.timestamp += length;
					nev.type = SamEvent::NoteOff;
					toolMaster()->queueEvent(nev);
					
				}
			}
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamOffNote::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrRepeats:  return parameterFromId(id)->normalisedFromValue(m_repeats); break;
		case pmrVelocity: return parameterFromId(id)->normalisedFromValue(m_velocity); break;
		case pmrFade    : return parameterFromId(id)->normalisedFromValue(m_fade); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamOffNote::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrRepeats:
			m_repeats = parameterFromId(pmrRepeats)->valueFromNormalised(value);
			break;
		case pmrVelocity:
			m_velocity = parameterFromId(pmrVelocity)->valueFromNormalised(value);
			break;
		case pmrFade:
			m_fade = parameterFromId(pmrFade)->valueFromNormalised(value);
			break;
	}
}