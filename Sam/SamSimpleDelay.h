#ifndef __SamSimpleDelay_h__
#define __SamSimpleDelay_h__

#include "SamAudioTool.h"
#include "CsFilter.h"
//#include "ARingBuffer.h"

class SamSimpleDelay : public SamAudioTool
{
public:
	SamSimpleDelay(SamToolMaster *toolMaster);

	static SamTool* createInstance(SamToolMaster *toolMaster) { return (SamTool*)new SamSimpleDelay(toolMaster); }
	static AString uuid(void) { return "4B36BF4E-474C-48bc-9DD6-114924CD4CF7"; }
	static AString name(void) { return "Simple Delay"; }

	void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);
	void process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples);
	void processEvent(SamEvent *ev);

	float parameterValueNormalised(int id);

	enum Parameters {
		kParamFeedback = 0,
		kParamLength = 1,
		kParamGroove = 2,
		kParamHiCut = 3,
		kParamDryOutput = 4,
		kParamWetOutput = 5
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

#endif // __SamSimpleDelay_h__