#ifndef __SamAudioBus_h__
#define __SamAudioBus_h__

#include "SamBus.h"

#include "AModel.h"
#include "ATaskOld.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "CsSpeakerConfig.h"
#include "CsAudioProcessInfo.h"
#include "CsAudioBuffer.h"
#include "CsOscillator.h"
#include "CsFilter.h"
#include "CsModulator.h"
using namespace Cs;

#include "SamAudioToolInsert.h"
#include "SamAudioSend.h"

class SamProject;

class SamAudioBus : public SamBus, public ATask
{
public:
	SamAudioBus(AModel *parent, int id = -1, AString name = "", int streams = 0, CsSpeakerConfig config = CsSpeakerConfig());
	~SamAudioBus();

	static const int maxSends      = 8;
	static const int maxInserts    = 4;

	virtual void main(void);

	CsAudioProcessInfo audioProcessInfo(void) { return m_audioProcessInfo; }
	virtual void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);

	int numStreams(void) { return m_streams; }
	CsSpeakerConfig speakerConfig(void) { return m_speakerConfig; }
	void setSpeakerConfig(CsSpeakerConfig config);
	bool isHardware(void) { return m_hardware; }

	CsAudioBuffer* audioBuffer(void) { return m_audioBuffer; }
	void setAudioBuffer(CsAudioBuffer *audioBuffer);

	CsAudioBuffer* audioBufferPreFader(void) { return m_audioBufferPreFader; }

	int audioOutputBusId(void) { return m_audioOutputBusId; }
	void setAudioOutputBusId(int id, void *from = 0);

	SamProject* project(void) { return m_project; }

	void setTestTone(bool testTone, void* from = 0);
	bool isTestTone(void) { return m_testTone; }

	SamAudioSend* audioSend(int id) { return m_audioSends[id]; }
	int numAudioSends(void) { return maxSends; }

	SamAudioToolInsert* audioInsert(int index) { return m_audioToolInserts[index]; }
	int numAudioInserts(void) { return maxInserts; }

	bool isFilterActive(void) { return m_filterActive; }
	void setFilterActive(bool active = true) { m_filterActive = active; }

	void setFilterCutOff(float cutOff) { m_filter->setCutOff(cutOff); }

	void setTargetVolume(float gain) { m_volumeModulator->setTargetValue(gain); }

	bool doesBusSendToAudioBusId(int audioBusId);

	int numPreFadeSends(void);

	// Items
	enum AudioBusItems {
		ItemSelected         =  0,
		ItemOutputBusId      = 10
	};

	

	virtual int readElement(AXmlDocument *document, AXmlElement *element);
	virtual int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
	void allocateBuffer(void);
	void freeBuffer(void);

	void setHardware(bool hardware) { m_hardware = hardware; }
	CsAudioBuffer *m_audioBuffer;
	CsAudioBuffer *m_audioBufferPreFader;
	CsOscillator *m_osc;
	CsSampleBuffer *m_oscBuffer;

private:
	SamProject *m_project;
	int m_streams;
	CsSpeakerConfig m_speakerConfig;
	bool m_hardware;
	CsAudioProcessInfo m_audioProcessInfo;
	int m_audioOutputBusId;
	bool m_testTone;
	SamAudioToolInsert *m_audioToolInserts[maxInserts];
	SamAudioSend *m_audioSends[maxSends];
	CsSynthFilter* m_filter;
	bool m_filterActive;
	CsSampleBuffer* m_processBuffer;
	CsModulator *m_volumeModulator;

};

#endif