#ifndef __SamAudioHardwareBus_h__
#define __SamAudioHardwareBus_h__

#include "SamAudioBus.h"

class SamAudioHardwareBus : public SamAudioBus
{
public:
	SamAudioHardwareBus(AModel *parent, int id = -1, AString name = "", int streams = 0, CsSpeakerConfig config = CsSpeakerConfig());
	~SamAudioHardwareBus();

	//void setAudioProcessInfo(Core::AudioProcessInfo audioProcessInfo);

	void main(void);

	int streamHardwareChannel(int stream) { return m_streamHardwareChannel[stream]; }
	void setStreamHardwareChannel(int stream, int channel) { m_streamHardwareChannel[stream] = channel; }

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
private:
	int m_streamHardwareChannel[16];
	
};

#endif // __SamAudioHardwareBus_h__