#include "SamRunulator.h"

#include "ALog.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamRunulator::SamRunulator(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, name(), uuid())
	, m_sampleStart(4000)
	, m_attackOffset(40)
{
	addParameter(new SamToolParameter(pmrSampleStart, "Sample Start", "samples", 4000,  0, 44100));
	addParameter(new SamToolParameter(pmrAttackOffset,  "Attack Offset",  "ms", 40, -500, 500));

	memset(m_noteDown, 0, 128);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamRunulator::~SamRunulator()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamRunulator::processEvent(SamEvent *ev)
{
	//aLog("SamRunulator::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
			//aLog("SamEvent::NoteOn %d += %d", ev->startOffset, m_sampleStart);
			if(isNoteDown()) {
				//aLog("Note Down");
				ev->startOffset += m_sampleStart;
				ev->attackOffset += m_attackOffset;
			} //else aLog("First!");
			m_noteDown[ev->midiValue1] = 1;
			break;
		case SamEvent::NoteOff:
			m_noteDown[ev->midiValue1] = 0;
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamRunulator::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrSampleStart: return parameterFromId(id)->normalisedFromValue(m_sampleStart); break;
		case pmrAttackOffset: return parameterFromId(id)->normalisedFromValue(m_attackOffset); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamRunulator::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrSampleStart:
			m_sampleStart = parameterFromId(pmrSampleStart)->valueFromNormalised(value);
			break;
		case pmrAttackOffset:
			m_attackOffset = parameterFromId(pmrAttackOffset)->valueFromNormalised(value);
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamRunulator::isNoteDown(void)
{
	for(int i=0; i<128; i++) {
		if(m_noteDown[i]) return true;
	}

	return false;
}