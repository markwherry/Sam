#include "SamStepRepeater.h"

#include "ALog.h"

#include "SamGuiStepRepeater.h"

SamStepRepeater::SamStepRepeater(SamToolMaster *toolMaster)
: SamMusicTool(toolMaster, "Step Repeater", "AD540906-F4AC-45ae-B183-7A76D65D2132")
, m_numSteps(16)
{
	addParameter(new SamToolParameter(pmrNumSteps, "Steps", "", 0, 1, maxSteps));
	for(int i=0; i<maxSteps; i++) {
		addParameter(new SamToolParameter(pmrStep+i, AString("Step ")+AString::number(i+1), "", 0, 0, 127));
		addParameter(new SamToolParameter(pmrMute+i, AString("Mute ")+AString::number(i+1), "", 0, 0, 1));
		addParameter(new SamToolParameter(pmrAccent+i, AString("Accent ")+AString::number(i+1), "", 0, 0, 1));
	}

	m_lastTo = 0;
	m_numNotes = 0;
	m_startTime = 0;
	for(int i=0; i<128; i++) {
		m_pitch[i] = false;
	}
	m_stepLength = 960/4;
	m_active = false;
	m_position = 0;
	m_stepCounter = 0;
	m_numSteps = 16;
	for(int i=0; i<maxSteps; i++) {
		m_steps[i].velocity = 64;
		m_steps[i].mute = false;
		m_steps[i].accent = false;
		m_steps[i].divisor = 1;
		m_steps[i].gate = 1.0f;
	}
	m_steps[0].velocity = 100;
	m_steps[4].velocity = 100;
	m_steps[8].velocity = 100;
	m_steps[12].velocity = 100;

	m_velocityModulator = 1.0f;

	m_humanise = 24;
}

SamStepRepeater::~SamStepRepeater()
{
}

QWidget* SamStepRepeater::createEditor(void)
{
	SamGuiStepRepeater *editor = new SamGuiStepRepeater(0, (SamTool*)this);

	return (QWidget*)editor;
}

void SamStepRepeater::processEvent(SamEvent *ev)
{
	//aLog("SamStepRepeater::processEvent()");

	//if(ev->type == SamEvent::Parameter) {
	//	setParameterValueNormalised(ev->iValue, ev->fValue);
	//	return;
	//}

	if(toolMaster()->isPlaying() && (ev->type == SamEvent::NoteOn || ev->type == SamEvent::NoteOff)) {

		if(ev->type == SamEvent::NoteOn) {
			if(ev->midiValue1 >= 0 && ev->midiValue1 <= 127) {
				//aLog("midiValue1 = %d", ev->midiValue1);
				if(m_numNotes == 0) {
					m_startTime = ev->start;
					m_position = 0;
					m_stepCounter = 0;
					m_active = true;
				}
				m_numNotes++;
				m_pitch[ev->midiValue1] = true;
			}
		} else {
			if(ev->midiValue1 >= 0 && ev->midiValue1 <= 127) {
				m_numNotes--;
				m_pitch[ev->midiValue1] = false;
				if(m_numNotes < 1) {
					m_active = false;
				}
			}
		}

		ev->type = SamEvent::Unassigned;
	} /*else if(samEngine->isPlaying() && ev->type == SamEvent::seCrossfade) {
		
		m_velocityModulator = (1.0f/127.0f) * (float)ev->integerValue;
		//qDebug("m_velocityModulator = %f (%d)", m_velocityModulator, ev.integerValue);
		ev->type = SamEvent::Unidentified;

	}*/
}

void SamStepRepeater::processRange(CsPpqnTime from, CsPpqnTime to)
{
	//aLog("SamStepSequencer::processRange(%d, %d)", from, to);

	if(m_lastTo != from) {
		aLog("*** BIG FUCKING PROBLEM ***");
	}


	

	if(!m_active && m_startTime >= from && m_startTime < to && m_numNotes > 0) {
		m_active = true;
		from = m_startTime;
	}

	if(m_active) {
		//aLog("m_active");
		for(int i=from; i<to; i++) {
			if(m_position % m_stepLength == 0) {
				if(!m_steps[m_stepCounter].mute) {
					int start = i;
					//aLog("start = %d", start);
					for(int j=0; j<128; j++) {
						//if(j == 60) aLog("60 = %d", (int)m_pitch[j]);
						if(m_pitch[j]) {
							float velocity = (float)m_steps[m_stepCounter].velocity;
							if(!m_steps[m_stepCounter].accent) {
								velocity *= m_velocityModulator;
							}
							//aLog("velocity = %f", velocity);
							if(m_numNotes > 1 && m_humanise > 0) {
								start = (i-(m_humanise/2)) + (rand()%m_humanise);
								//qDebug("-start = %d", start);
							}
							playNote(j, (int)velocity, start, m_stepLength-1);
						}
					}
				}
				m_stepCounter++;
			}
			m_position++;
			if(m_position >= (m_numSteps * m_stepLength)) {
				m_position = 0;
				m_stepCounter = 0;
			}
		}
	}
	


	/*for(CsInt64 i = from; i < to; i++) {
		bool played = false;
		if(i%(samEngine->ppqnResolution()*4)==0){
			playNote(72, 100, i, 960);

			played = true;
		}
		if(i%(samEngine->ppqnResolution())==0 && !played){
			playNote(72, 64, i, 960);
		}
	}//*/

	m_lastTo = to;
}

float SamStepRepeater::parameterValueNormalised(int id)
{
	if(id >= pmrStep && id < pmrStep+maxSteps) {
		return parameterFromId(id)->normalisedFromValue(m_steps[id-pmrStep].velocity);
	} else if(id >= pmrMute && id < pmrMute+maxSteps) {
		return (float)((int)m_steps[id-pmrMute].mute);
	} else if(id >= pmrMute && id < pmrAccent+maxSteps) {
		return (float)((int)m_steps[id-pmrAccent].accent);
	} else {
		switch(id) {
			case pmrNumSteps: return parameterFromId(id)->normalisedFromValue(m_numSteps);
		}
	}
	

	return 0.0f;
}

void SamStepRepeater::setParameterValueNormalised(int id, float value)
{
	//aLog("SamStepRepeater::setParameterValueNormalised(%d, %f)", id, value);

	if(id >= pmrStep && id < pmrStep+maxSteps) { 
		m_steps[id-pmrStep].velocity = (int)(127.0 * value);
	} else if(id >= pmrMute && id < pmrMute+maxSteps) { 
		m_steps[id-pmrMute].mute = (bool)((int)value);
		//aLog("%d", (int)m_steps[id-pmrStep].mute);
	} else if(id >= pmrAccent && id < pmrAccent+maxSteps) { 
		m_steps[id-pmrAccent].accent = (bool)((int)value);
	} else {
		switch(id) {
			case pmrNumSteps:
				m_numSteps = parameterFromId(pmrNumSteps)->valueFromNormalised(value);
				break;
		}
	}
}

void SamStepRepeater::playNote(int pitch, int velocity, int start, int length)
{
	//aLog("SamStepRepeater::playNote(%d, %d, %d, %d)", pitch, velocity, start, length);
	//qDebug("velocity = %d", velocity);
	SamEvent ev(SamEvent::NoteOn);
	ev.start = start;
	ev.delta = 0;
	ev.midiValue1 = pitch;
	ev.midiValue2 = velocity;
	toolMaster()->queueEvent(ev);


	ev.type = SamEvent::NoteOff;
	ev.start += (length-1);
	toolMaster()->queueEvent(ev);
}