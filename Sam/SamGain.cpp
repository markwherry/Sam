#include "SamGain.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGain::SamGain(SamToolMaster *toolMaster)
	: SamAudioTool(toolMaster, SAM_GAIN_NAME, SAM_GAIN_UUID)
	, m_level(1.0f)
{
	addParameter(new SamToolParameter(pLevel, "Level", "%", 0, 0, 100));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGain::setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig)
{
	if(inputConfig == CsSpeakerConfig(CsSpeakerConfig::Mono) && outputConfig == CsSpeakerConfig(CsSpeakerConfig::Mono)) {
		return SamAudioTool::setSpeakerConfig(inputConfig, outputConfig);	
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGain::setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGain::process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples)
{
	//aLog("SamGain::process() %d", inputs->numFrames);

	int frames = inputs->numFrames;

	while(frames--) {
		*outputs->buffer32[0]++ = *inputs->buffer32[0]++ * m_level;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGain::processEvent(SamEvent *ev)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamGain::parameterValueNormalised(int id)
{
	switch(id) {
		case pLevel: return parameterFromId(id)->normalisedFromValue(m_level*100.0f); break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGain::setParameterValueNormalised(int id, float value)
{
	//aLog("SamGain::setParameterValueNormalised(%d, %f)", id, value);

	switch(id) {
		case pLevel:
			m_level = parameterFromId(pLevel)->valueFromNormalised(value);
			m_level = m_level / 100.0f;
			//aLog("m_level = %f", m_level);
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//bool SamGain::canDoSpeakerConfig(CsSpeakerConfig config)
//{
//	return config.config() == CsSpeakerConfig::Mono;
//}
