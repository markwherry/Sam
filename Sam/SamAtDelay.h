#ifndef __SamAtDelay_h__
#define __SamAtDelay_h__

#include "SamAudioTool.h"
#include "CsFilter.h"
//#include "ARingBuffer.h"

#define SAM_DELAY_UUID "56CE39C0-648C-43E3-96F0-E1ACECCBBADB"
#define SAM_DELAY_NAME "Simple Delay"

class SamAtDelay : public SamAudioTool
{
public:
	SamAtDelay(SamToolMaster *toolMaster);

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamAtDelay(toolMaster); }
	static AString uuid(void) { return SAM_DELAY_UUID; }
	static AString name(void) { return SAM_DELAY_NAME; }

	int setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig);
	void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);

	void process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples);
	void processEvent(SamEvent *ev);

	void start(void);

	float parameterValueNormalised(int id);
	void setParameterValueNormalised(int id, float value);

	bool canDoSpeakerConfig(CsSpeakerConfig config);

	enum Parameters {
		pFeedback = 0,
		pLength = 1,
		pGroove = 2,
		pHiCut = 3,
		pDryOutput = 4,
		pWetOutput = 5
	};

private:
	void setDelayTime(float delayTime);
	void setDelayTimeTempo(float tempo);
	void setDelayTimeQuarterNotes(float quarterNotes);

	float *delayBuffer;
	float m_delayTime;
	int m_sampleDelayTime;
	int delayCounter;
	float m_wetLevel;
	float m_dryLevel;
	float m_feedback;
	CsFilter *filterLoPass;
	//CsFilter *filterHiPass;
	int m_groove;
	float m_quarterNotes;
	float m_tempo;
};

#endif // __SamAtDelay_h__