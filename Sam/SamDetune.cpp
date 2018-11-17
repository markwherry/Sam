#include "SamDetune.h"

#include "ALog.h"

#include "SamGuiDetune.h"

#include <math.h>

SamDetune::SamDetune(SamToolMaster *toolMaster)
: SamMusicTool(toolMaster, "Detune", "2FED9152-E5EA-40bb-9E91-723BAD09F215")
{
	for(int i=0; i<128; i++) {
		addParameter(new SamToolParameter(pmrDetune+i, "Detune", "cents", 0, -100, +100));
		m_detune[i] = 0;
	}
}

SamDetune::~SamDetune()
{
}

QWidget* SamDetune::createEditor(void)
{
	SamGuiDetune *editor = new SamGuiDetune(0, (SamTool*)this);

	return (QWidget*)editor;
}

void SamDetune::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
			ev->tuning = rand() % 100;
		case SamEvent::NoteOff:
			//ev->midiValue1 += m_transpose;
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

float SamDetune::parameterValueNormalised(int id)
{
	switch(id) {
		//case pmrTranspose: return parameterFromId(id)->normalisedFromValue(m_transpose);
	}

	return 0.0f;
}

void SamDetune::setParameterValueNormalised(int id, float value)
{
	aLog("SamDetune::setParameterValueNormalised(%d, %f)", id, value);

	if(id >= pmrDetune && id < pmrDetune + 127) {
		int pitch = id - 1000;
		m_detune[pitch] = parameterFromId(pmrDetune)->valueFromNormalised(value);
		aLog("pitch = %d, detune = %d", pitch, m_detune[pitch]);
	}

	switch(id) {
		//case pmrTranspose:
		//	m_transpose = parameterFromId(pmrTranspose)->valueFromNormalised(value);
		//	break;
	}
}