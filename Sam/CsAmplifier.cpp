#include "CsAmplifier.h"

#include "AMemoryManager.h"



namespace Cs {

CsAmplifier::CsAmplifier(double sampleRate)
: CsSynthComponent(sampleRate)
//, m_maxBufferSize(maxBufferSize)
, m_gain(1.0f)
{
	/*m_buffer = (float*)aMalloc(sizeof(float)*m_maxBufferSize);
#ifdef USE_FW
	fwsSet_32f(1.0f, m_buffer, m_maxBufferSize);
#else
	for(int i=0; i<m_maxBufferSize; i++) {
		m_buffer[i] = 1.0f;
	}
#endif*/
}

CsAmplifier::~CsAmplifier()
{
	//aFree(m_buffer, sizeof(float)*m_maxBufferSize);
}

/*void CsAmplifier::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
#ifdef USE_FW
	for(int i=0; i<buffer->numStreams; i++) {
		fwsMulC_32f_I(m_gain, buffer->buffer32[i]+frameOffset, buffer->numFrames-frameOffset);
		for(int j=0; j<m_modulationSources.size(); j++) {
			if(!(isRelease() && !m_modulationSources[j]->isModulateInRelease())) {
				fwsMul_32f_I(m_modulationSources[j]->buffer(), buffer->buffer32[i], buffer->numFrames);
			}
		}
	}
#else
	//for(int i=frameOffset; i<buffer->numFrames; i++) {
	//	for(int j=0; j<buffer->numStreams; j++) {
	//		buffer->buffer32[j][i] *= (m_gain * m_buffer[i]);
	//	}
	//}
#endif
}*/

/*void CsAmplifier::renderBufferWithoutConstantGain(CsSampleBuffer *buffer, int frameOffset)
{
#ifdef USE_FW
	for(int i=0; i<m_modulationSources.size(); i++) {
		if(!(isRelease() && !m_modulationSources[i]->isModulateInRelease())) {
			for(int j=0; j<buffer->numStreams; j++) {
				fwsMul_32f_I(m_modulationSources[i]->buffer(), buffer->buffer32[j], buffer->numFrames);
			}
		}
	}
#else
	//for(int i=frameOffset; i<buffer->numFrames; i++) {
	//	for(int j=0; j<buffer->numStreams; j++) {
	//		buffer->buffer32[j][i] *= (m_gain * m_buffer[i]);
	//	}
	//}
#endif
}*/

}