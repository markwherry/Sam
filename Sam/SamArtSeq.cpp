#include "SamArtSeq.h"

#include "ALog.h"

#include "SamArtSeqGui.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArtSeq::SamArtSeq(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, "Articulation Sequencer", "8FC771A8-A190-403D-A506-3072BE56CBED")
	, m_numSteps(1)
	, m_currentStep(0)
{
	for(int i=0; i<maxSteps; i++) {
		m_steps[i] = 0;
		addParameter(new SamToolParameter(pmrStep1+i, AString("Step ")+AString::number(i+1), "", 0, 0, 127));
	}

	addParameter(new SamToolParameter(pmrNumSteps, "Num. of Steps", "", 1, 1, maxSteps));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArtSeq::~SamArtSeq()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeq::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
			if(ev->midiValue1 == 21) {
				//aLog("Reset");
				m_currentStep = 0;
			} else {
				//aLog("[%d] %d", m_currentStep, m_steps[m_currentStep]);
				ev->articulationIndex = m_steps[m_currentStep];
				m_currentStep++;
				if(m_currentStep >= m_numSteps) {
					m_currentStep = 0;
				}
			}
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamArtSeq::parameterValueNormalised(int id)
{
	if(id >= 1000 && id < 1000+maxSteps) {
		return parameterFromId(id)->normalisedFromValue(m_steps[id-1000]);
	}

	switch(id) {
		case pmrNumSteps: return parameterFromId(id)->normalisedFromValue(m_numSteps);
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeq::setParameterValueNormalised(int id, float value)
{
	if(id >= 1000 && id < 1000+maxSteps) {
		m_steps[id-1000] = parameterFromId(pmrStep1+(id-1000))->valueFromNormalised(value);
		return;
	}

	switch(id) {
		case pmrNumSteps:
			m_currentStep = 0;
			m_numSteps = parameterFromId(pmrNumSteps)->valueFromNormalised(value);
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
QWidget* SamArtSeq::createEditor(void)
{
	//return 0;

	return new SamArtSeqGui(this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArtSeq::setNumSteps(int steps)
{
	m_currentStep = 0;
	m_numSteps = steps;
	if(m_numSteps < 1) m_numSteps = 1;
}