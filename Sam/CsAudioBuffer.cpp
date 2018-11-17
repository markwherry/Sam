#include "CsAudioBuffer.h"

#include "ALog.h"
#include "AMemoryManager.h"
using namespace Atom;

#define USE_IPP_AB
#ifdef USE_IPP_AB
#include "ipps.h"
#endif

//#define USE_FW_AB

#ifdef USE_FW_AB
#include <fwSignal.h>
#endif

#include <math.h>

namespace Cs {

// --------------------------------------------------------------------------------------------------------------------------------------------
CsAudioBuffer::CsAudioBuffer(int numStreams, int numFrames)
: m_numStreams(numStreams)
, m_numFrames(numFrames)
, m_speakerConfig(CsSpeakerConfig(CsSpeakerConfig::Undefined))
{
	aLog("AudioBuffer::AudioBuffer(%d, %d)", numStreams, numFrames);
	//csAssert(m_numStreams < maxStreams, "m_numStreams is not < AUDIOBUFFER_MAXSTREAMS");

/*#ifdef USE_FW_AB
	m_data = fwsMalloc_32f(m_numStreams*m_numFrames);
#else*/
#ifdef USE_IPP_AB
	m_data = ippsMalloc_32f(m_numStreams*m_numFrames);
#else
	m_data = (float*)aMalloc(sizeof(float)*(m_numStreams*m_numFrames));
#endif
	//csAssert(m_data != 0, "m_data == 0");

	float *c = m_data;
	for (int i=0; i<m_numStreams; i++){
		m_streams[i] = c;
        c += m_numFrames;
	}
	m_streams[m_numStreams] = 0;

	zeroAll();
}

// --------------------------------------------------------------------------------------------------------------------------------------------
CsAudioBuffer::~CsAudioBuffer()
{
	//csDebug("AudioBuffer::~AudioBuffer()");
/*#ifdef USE_FW_AB
	fwsFree(m_data);
#else*/
#ifdef USE_IPP_AB
	ippsFree(m_data);
#else
	aFree(m_data, sizeof(float)*(m_numStreams*m_numFrames));
#endif

	//csDebug("EXIT: AudioBuffer::~AudioBuffer()");
}

// --------------------------------------------------------------------------------------------------------------------------------------------
float* CsAudioBuffer::stream(int stream)
{
	//csDebug("AudioBuffer::getStream()");
	//if(stream<0 || stream>=m_numStreams)
	//	return NULL;

	return m_streams[stream];
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::zeroAll(void)
{
	//csDebug("AudioBuffer::zeroAll()");
/*#ifdef USE_FW_AB
	fwsZero_32f(m_data, m_numStreams*m_numFrames);
#else*/
#ifdef USE_IPP_AB
	ippsZero_32f(m_data, m_numStreams*m_numFrames);
#else
	memset(m_data, 0, sizeof(float)*(m_numStreams*m_numFrames));
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::zeroStream(int stream)
{
	float *s = m_streams[stream];
	memset(s, 0, sizeof(float)*m_numFrames);
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::addAll(CsAudioBuffer *audioBuffer)
{
	//csDebug("AudioBuffer::addAll()");
/*#ifdef USE_FW_AB
	fwsAdd_32f_I(audioBuffer->m_data, m_data, m_numFrames * m_numStreams);
#else*/
#ifdef USE_IPP_AB
	ippsAdd_32f_I(audioBuffer->m_data, m_data, m_numFrames * m_numStreams);
#else
	for(int i=0; i<m_numStreams*m_numFrames; i++) {
		m_data[i]+=audioBuffer->m_data[i];
	}
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::addStream(int toStream, CsAudioBuffer *fromAudioBuffer, int fromStream)
{
	float *outputTo = m_streams[toStream];
	float *outputFrom = fromAudioBuffer->stream(fromStream);

/*#ifdef USE_FW_AB
	fwsAdd_32f_I(outputFrom, outputTo, m_numFrames);
#else*/
#ifdef USE_IPP_AB
	ippsAdd_32f_I(outputFrom, outputTo, m_numFrames);
#else
	for(int i=0; i<m_numFrames; i++){
		outputTo[i]+=outputFrom[i];
	}
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::addStreamWithGain(int toStream, CsAudioBuffer *fromAudioBuffer, int fromStream, float gain)
{
	float *outputTo = m_streams[toStream];
	float *outputFrom = fromAudioBuffer->stream(fromStream);
	
//#ifdef USE_IPP_AB
//	ippsAdd_32f_I(fromAudioBuffer->m_streams[fromStream], m_streams[toStream], m_numFrames);
//	ippsMulC_32f_I(gain, m_streams[toStream], m_numFrames);
//#else
	//for(int i=0; i<m_numFrames; i++){
		//outputTo[i] += (outputFrom[i] * gain);
	//}
//#endif

	int frames = m_numFrames;
	//float temp;
	while(--frames >= 0) {
		//aLog("frames= %d", frames);
		//temp = (*outputFrom++) * gain;
		//(*outputTo++) += temp;
		(*outputTo++) += (*outputFrom++) * gain;
	}

}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::mixStream(CsAudioBuffer *fromAudioBuffer, int fromStream, float panX, float panY)
{
	//if(m_numStreams==2) { // Mixing mono into stereo
		//csDebug("HERE!");
		addStream(0, fromAudioBuffer, fromStream);
		applyGainToStream(0, panX-1.0f);
		addStream(1, fromAudioBuffer, fromStream);
		applyGainToStream(1, panX);
	//} else csDebug("numStreams=%d", m_numStreams);
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::copyAll(CsAudioBuffer *audioBuffer)
{
#ifdef USE_IPP_AB
	ippsCopy_32f(audioBuffer->m_data, m_data, m_numStreams*m_numFrames);
#else
	memcpy(m_data, audioBuffer->m_data, sizeof(float)*(m_numStreams*m_numFrames));
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::applyGainToStream(int stream, float gain)
{
#ifdef USE_IPP_AB
	ippsMulC_32f_I(gain, m_streams[stream], m_numFrames);
#else
	float *buffer = m_streams[stream];
	for(int i=0; i<m_numFrames; i++) {
		buffer[i] *= gain;
	}
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::applyGain(float gain)
{
#ifdef USE_IPP_AB
	ippsMulC_32f_I(gain, m_data, m_numFrames*m_numStreams);
#else
	for(int i=0; i<m_numFrames*m_numStreams; i++) {
		m_data[i] *= gain;
	}
#endif
}

// --------------------------------------------------------------------------------------------------------------------------------------------
void CsAudioBuffer::applyGainRamp(float fromGain, float toGain)
{
	if(fromGain==toGain){
		//ippsMulC_32f_I(toGain, data, m_numFrames*m_numStreams);
		applyGain(fromGain);
	} else {
		float gainIncrement = (toGain-fromGain)/m_numFrames;

		for(int i=0; i<m_numStreams; i++){
			float *buffer = m_streams[i];
			float gain = fromGain;
			for(int j=0; j<m_numFrames; j++){
				buffer[j]=buffer[j]*gain;
				gain+=gainIncrement;
			}
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------
float CsAudioBuffer::rmsLevel(int stream)
{
	float *buffer = m_streams[stream];

	double sum = 0.0;

	for (int i=0; i<m_numFrames; ++i){
        const float sample = buffer[i];
        sum += sample * sample;
    }

    return (float) sqrt(sum / m_numFrames);
}

// --------------------------------------------------------------------------------------------------------------------------------------------
float CsAudioBuffer::peakLevel(int stream)
{
	float *buffer = m_streams[stream];

	float peak = 0.0;

	for(int i=0; i<m_numFrames; i++) {
		const float value = buffer[i];
		if(value>peak) {
			peak = buffer[i];
		}
	}

	return peak;
}

// --------------------------------------------------------------------------------------------------------------------------------------------
/*bool AudioBuffer::setSpeakerConfig(int speakerConfig)
{
	m_speakerConfig = speakerConfig;

	int numChannelsRequired[6] = { 0, 1, 2, 4, 5, 6 };

	if (m_numStreams<numChannelsRequired[m_speakerConfig]) {
		m_speakerConfig = 0;
		return false;
	}

	for(int i=0; i<m_numStreams; i++) {
		setStreamSpeaker(i, ASpeakerConfig::speakerWithinConfig(m_speakerConfig, i));
	}

	return true;
}*/

}