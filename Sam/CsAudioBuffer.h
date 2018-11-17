#ifndef __CsAudioBuffer_h__
#define __CsAudioBuffer_h__

#include "CsSpeakerConfig.h"

//#define AUDIOBUFFER_MAXSTREAMS 256

namespace Cs {

class CsAudioBuffer
{
public:
	CsAudioBuffer(int numStreams, int numFrames);
	~CsAudioBuffer();

	float*  stream(int stream);
	float** streams(void) { return m_streams; }

	void zeroAll(void);
	void zeroStream(int stream);
	void addAll(CsAudioBuffer *audioBuffer);

	void addStream(int toStream, CsAudioBuffer *fromAudioBuffer, int fromStream);
	void addStreamWithGain(int toStream, CsAudioBuffer *fromAudioBuffer, int fromStream, float gain);
	void mixStream(CsAudioBuffer *fromAudioBuffer, int fromStream, float panX, float panY);

	void applyGainToStream(int stream, float gain);
	void applyGain(float gain);
	void applyGainRamp(float fromGain, float toGain);

	void copyAll(CsAudioBuffer *audioBuffer);

	float rmsLevel(int stream);
	float peakLevel(int stream);

	inline int numStreams(void) const { return m_numStreams; }
	inline int numFrames(void) const { return m_numFrames; }
	
	//inline int speakerConfig(void) const { return m_speakerConfig; }
	//bool setSpeakerConfig(int speakerConfig);

	//inline int streamSpeaker(int stream) { return m_streamSpeaker[stream]; }
	//void setStreamSpeaker(int stream, int speaker) { m_streamSpeaker[stream] = speaker; }

	float *data(void) { return m_data; }

	static const int maxStreams = 256;

protected:
private:
	int m_numStreams;
	int m_numFrames;
	CsSpeakerConfig m_speakerConfig;
	int m_streamSpeaker[maxStreams];
	float *m_data;
	float *m_streams[maxStreams];
};

}

#endif // __AAudioBuffer_h__