#ifndef __SamGain_h__
#define __SamGain_h__

#include "SamAudioTool.h"
#include "CsFilter.h"
//#include "ARingBuffer.h"

#define SAM_GAIN_UUID "081AA124-0E4C-4fd7-A7BE-54AC502C336F"
#define SAM_GAIN_NAME "Gain"

class SamGain : public SamAudioTool
{
public:
	SamGain(SamToolMaster *toolMaster);

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamGain(toolMaster); }
	static AString uuid(void) { return SAM_GAIN_UUID; }
	static AString name(void) { return SAM_GAIN_NAME; }

	int setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig);
	void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);

	void process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples);
	void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	bool canDoSpeakerConfig(CsSpeakerConfig config);

	enum Parameters {
		pLevel = 0
	};

private:
	float m_level;
};

#endif // __SamGain_h__