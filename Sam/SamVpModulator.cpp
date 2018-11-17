#include "SamVpModulator.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVpModulator::SamVpModulator(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, SAM_VP_MODULATOR_NAME, SAM_VP_MODULATOR_UUID)
	, m_pc(51)
	, m_threshold(0)
	, m_scale(0)
{
	//addParameter(new SamToolParameter(pmrPc, "Performance Control", "semitones", 0, -12, 12));
	addParameter(new SamToolParameter(pmrThreshold, "Threshold", "vel", 0, 0, 127));
	addParameter(new SamToolParameter(pmrScale, "Delay", "%", 0, 0, 100));

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVpModulator::~SamVpModulator()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVpModulator::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn:
			{
				SamEvent sendEv(SamEvent::Parameter);
				sendEv.subType = m_pc;
				sendEv.timestamp = ev->timestamp;
				sendEv.articulationIndex = ev->articulationIndex;
				sendEv.fValue = 0;

				//aLog("Articulation Index = %d", ev->articulationIndex);

				//toolMaster()->queueEvent(sendEv);
			}
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamVpModulator::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrPc:        return parameterFromId(id)->normalisedFromValue(m_pc); break;
		case pmrThreshold: return parameterFromId(id)->normalisedFromValue(m_threshold); break;
		case pmrScale:     return parameterFromId(id)->normalisedFromValue(m_scale); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVpModulator::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrPc:
			m_pc = parameterFromId(pmrPc)->valueFromNormalised(value);
			break;
		case pmrThreshold:
			m_threshold = parameterFromId(pmrThreshold)->valueFromNormalised(value);
			break;
		case pmrScale:
			m_scale = parameterFromId(pmrScale)->valueFromNormalised(value);
			break;
	}
}