#ifndef __SamAudioTool_h__
#define __SamAudioTool_h__

#include "SamTool.h"
#include "SamEvent.h"

#include "CsAudioBuffer.h"
#include "CsSampleBuffer.h"
using namespace Cs;

#include "CsSpeakerConfig.h"
#include "CsAudioProcessInfo.h"

#include "AList.h"

class SamAudioTool : public SamTool
{
public:
	SamAudioTool(SamToolMaster *toolMaster, AString name, AString uuid);
	~SamAudioTool();

	virtual void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo) { m_audioProcessInfo = audioProcessInfo; }
	virtual void process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples) = 0;
	virtual void processEvent(SamEvent *ev) = 0;

	//virtual bool canDoSpeakerConfig(CsSpeakerConfig config) { return false; }
	virtual int setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig);
	virtual CsSpeakerConfig speakerConfig(void) { return m_speakerConfig; }

	int numAudioOutputs(void) { return outputSetupList.size(); }
	CsSpeakerConfig outputSetup(int outputId);

protected:
	void addAudioOutput(CsSpeakerConfig speakerConfig);
	int bufferSize(void) { return m_audioProcessInfo.bufferSize; }
	double sampleRate(void) { return m_audioProcessInfo.sampleRate; }

private:
	AList<CsSpeakerConfig> outputSetupList;
	CsAudioProcessInfo m_audioProcessInfo;
	CsSpeakerConfig m_speakerConfig;
};

#endif