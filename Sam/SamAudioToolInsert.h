#ifndef __SamAudioToolInsert_h__
#define __SamAudioToolInsert_h__


#include "SamToolInsert.h"

#include "SamAudioTool.h"

#include "CsVstPlugin.h"

class SamAudioToolInsert : public SamToolInsert, public SamToolMaster
{
public:
	SamAudioToolInsert(AModel *parent, int index, CsSpeakerConfig speakerConfig);

	void parameterChanged(int id, float value, void *from);

	SamAudioTool *audioTool(int index = 0) { return m_audioTool[index]; }
	CsVstPlugin *vstPlugin(int index = 0) { return m_vstPlugin[index]; }

	void setAudioTool(AString toolUuid, int mode = 0);

	void setVstPlugin(TCHAR *filename, int mode = 0);

	void process(CsAudioBuffer *inputs, CsAudioBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples);
	void process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples);
	void processEvents(SamEvent *events, int numEvents);

	void resetTool(void);

	CsSpeakerConfig speakerConfig(void) { return m_speakerConfig; }
	int numInstances(void) { return m_numInstances; }

	void setActive(bool active, void* from = 0);

	bool isAssigned(void);

	bool isPerVoiceFx(void) { return (m_index < 0); }

	AString name(void);

	bool isLink(void) { return m_link; }
	void setLink(bool link);

	int currentSpeaker(void) { return m_currentSpeaker; }
	void setCurrentSpeaker(int index) { m_currentSpeaker = index; }

	int mode(void) { return m_mode; }

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamAudioToolItem {
		ItemActive = 0,
		ItemEditorSizeUpdate = 10
	};

	bool hasMidiInput(void) { return m_midiInput; }

protected:
private:
	void clear(void);

	CsSpeakerConfig m_speakerConfig;

	//bool m_perVoiceFx;
	SamAudioTool *m_audioTool[8];
	CsVstPlugin  *m_vstPlugin[8];
	int m_numInstances;
	
	CsSampleBuffer *m_inputBuffer;
	CsSampleBuffer *m_outputBuffer;
	
	bool m_link;
	int m_currentSpeaker;
	int m_mode;
	float *m_multiInput[2];
	float *m_multiOutput[2];

	bool m_midiInput;

	VstMidiEvent m_vstMidiEventBuffer[1024];
	VstEvents *m_vstEvents;

	
};

#endif //__SamAudioToolInsert_h__