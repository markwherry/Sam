#include "SamCentipede.h"

#include "ALog.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamCentipede::SamCentipede(SamToolMaster *toolMaster)
: SamMusicTool(toolMaster, name(), uuid())
, m_rangeHigh(100)
, m_rangeLow(-100)
, m_velocity(0.0f)
, m_velocityInvert(0)
{
	addParameter(new SamToolParameter(pmrRangeHigh, "Range High", "cents", 0,  -100, 100));
	addParameter(new SamToolParameter(pmrRangeLow,  "Range Low",  "cents", 0, -100, 100));
	addParameter(new SamToolParameter(pmrVelocity,  "Vel. Influence", "%", 0, 0, 100));
	addParameter(new SamToolParameter(pmrVelocityInvert, "Vel. Invert", "", 0, 0, 1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamCentipede::~SamCentipede()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCentipede::processEvent(SamEvent *ev)
{
	//aLog("SamTranspose::processEvent()");

	int range = 0;

	switch(ev->type) {
		case SamEvent::NoteOn:
			range = m_rangeHigh-m_rangeLow;
			if(range != 0) {
				float tuning = m_rangeLow + (rand()%range);
				aLog("tuning = %f", tuning);
				if(m_velocity > 0) {
					float velocity = (float)ev->midiValue2/127.0f;
					if(m_velocityInvert) {
						velocity = 1.0 - velocity;
					}
					tuning *= (m_velocity * velocity);
					aLog("vel tuning = %f (%f)", tuning, m_velocity);
				}
				ev->tuning = (int)tuning;
			}
			break;
		case SamEvent::NoteOff:
			break;
		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamCentipede::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrRangeHigh: return parameterFromId(id)->normalisedFromValue(m_rangeHigh); break;
		case pmrRangeLow: return parameterFromId(id)->normalisedFromValue(m_rangeLow); break;
		case pmrVelocity: return parameterFromId(id)->normalisedFromValue(m_velocity) * 100; break;
		case pmrVelocityInvert: return parameterFromId(id)->normalisedFromValue(m_velocityInvert); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCentipede::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrRangeHigh:
			m_rangeHigh = parameterFromId(pmrRangeHigh)->valueFromNormalised(value);
			break;
		case pmrRangeLow:
			m_rangeLow = parameterFromId(pmrRangeLow)->valueFromNormalised(value);
			break;
		case pmrVelocity:
			m_velocity = (float)parameterFromId(pmrVelocity)->valueFromNormalised(value) / 100.0f;
			break;
		case pmrVelocityInvert:
			m_velocityInvert = (float)parameterFromId(pmrVelocityInvert)->valueFromNormalised(value);;
			break;
	}
}