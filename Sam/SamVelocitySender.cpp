#include "SamVelocitySender.h"

//#include "SamVoice.h"
#include "SamParameter.h"

#include "AMath.h"
#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVelocitySender::SamVelocitySender(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, "Velocity Sender", SAM_VELOCITYSENDER_UUID)
	, m_send(1)
	, m_threshold(81)
	, m_scale(0)
	, m_invert(0)
	, m_min(0.0f)
	, m_max(1.0f)
{
	addParameter(new SamToolParameter(pmrSend, "Send", "", 0, 1, 8));
	addParameter(new SamToolParameter(pmrThreshold, "Threshold", "", 0, 1, 127));
	addParameter(new SamToolParameter(pmrScale, "Scale", "", 0, 0, 1));
	addParameter(new SamToolParameter(pmrInvert, "Invert", "", 0, 0, 1));
	addParameter(new SamToolParameter(pmrMin, "Min", "%", 0, 0, 100));
	addParameter(new SamToolParameter(pmrMax, "Max", "%", 0, 0, 100));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVelocitySender::~SamVelocitySender()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVelocitySender::processEvent(SamEvent *ev)
{
	aLog("SamVelocitySender::processEvent()");

	switch(ev->type) {
		case SamEvent::NoteOn: //aLog("SamEvent::NoteOn:");
			{
				//if(m_bypassed) return;

				SamEvent sendEv(SamEvent::Parameter);
				sendEv.subType = SamParameter::SendGain;
				sendEv.timestamp = ev->timestamp;
				sendEv.iValue = m_send - 1;

				float sendValue = m_min;

				if(ev->midiValue2 >= m_threshold) {
					if(m_scale) {
						float range = m_max - m_min;
						sendValue += AMath::pow((range/127.0f)*ev->midiValue2, 4);
						//sendValue += m_min;
					} else {
						sendValue = m_max;
					}
				}

				if(m_invert) {
					sendValue = 1.0f - sendValue;
				}
				
				sendEv.fValue = sendValue; //aLog("sendEv.fValue = %f", sendEv.fValue);
				toolMaster()->queueEvent(sendEv);

				
			}
			break;

		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamVelocitySender::parameterValueNormalised(int id)
{
	switch(id) {
		case pmrSend:      return parameterFromId(id)->normalisedFromValue(m_send); break;
		case pmrThreshold: return parameterFromId(id)->normalisedFromValue(m_threshold); break;
		case pmrScale:     return parameterFromId(id)->normalisedFromValue(m_scale); break;
		case pmrInvert:    return parameterFromId(id)->normalisedFromValue(m_invert); break;
		case pmrMin:       return parameterFromId(id)->normalisedFromValue(m_min) * 100.0f; break;
		case pmrMax:       return parameterFromId(id)->normalisedFromValue(m_max) * 100.0f; break;
	}

	return SamTool::parameterValueNormalised(id);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVelocitySender::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pmrSend:
			m_send = parameterFromId(pmrSend)->valueFromNormalised(value);
			break;
		case pmrThreshold:
			m_threshold = parameterFromId(pmrThreshold)->valueFromNormalised(value);
			break;
		case pmrScale:
			m_scale = parameterFromId(pmrScale)->valueFromNormalised(value);
			break;
		case pmrInvert:
			m_invert = parameterFromId(pmrInvert)->valueFromNormalised(value);
			break;
		case pmrMin:
			m_min = parameterFromId(pmrMin)->valueFromNormalised(value) / 100.0f;
			break;
		case pmrMax:
			m_max = parameterFromId(pmrMax)->valueFromNormalised(value) / 100.0f;
			break;
	}

	SamTool::setParameterValueNormalised(id, value);
}