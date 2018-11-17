#include "SamTranspose.h"

#include "ALog.h"

SamTranspose::SamTranspose(SamToolMaster *toolMaster)
: SamMusicTool(toolMaster, "Transpose", "132FB96F-726F-4a5b-B7ED-A404059B8C13")
, m_transpose(12)
{
	addParameter(new SamToolParameter(pmrTranspose, "Transpose", "semitones", 0, -24, +24));
}

SamTranspose::~SamTranspose()
{
}

void SamTranspose::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
		case SamEvent::NoteOff:
			ev->midiValue1 += m_transpose;
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

float SamTranspose::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrTranspose: return parameterFromId(id)->normalisedFromValue(m_transpose);
	}

	return 0.0f;
}

void SamTranspose::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrTranspose:
			m_transpose = parameterFromId(pmrTranspose)->valueFromNormalised(value);
			break;
	}
}