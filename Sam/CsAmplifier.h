#ifndef __CsAmplifier_h__
#define __CsAmplifier_h__

#include "CsSynthComponent.h"

#include "Alog.h"

//#define USE_FW_AMP

#ifdef USE_FW_AMP
#include <fwSignal.h>
#endif

#define USE_IPP_AMP

#ifdef USE_IPP_AMP
#include "ipps.h"
#endif

namespace Cs {

class CsAmplifier : public CsSynthComponent
{
public:
	CsAmplifier(double sampleRate);
	~CsAmplifier();

	inline void renderBuffer(CsSampleBuffer *buffer, int frameOffset = 0)
	{
/*#ifdef USE_FW_AMP
		for(int i=0; i<buffer->numStreams; i++) {
			fwsMulC_32f_I(m_gain, buffer->buffer32[i]+frameOffset, buffer->numFrames-frameOffset);
			for(int j=0; j<m_modulationSources.size(); j++) {
				if(!(isRelease() && !m_modulationSources[j]->isModulateInRelease())) {
					fwsMul_32f_I(m_modulationSources[j]->buffer(), buffer->buffer32[i], buffer->numFrames);
				}
			}
		}*/

#ifdef USE_IPP_AMP
		for(int i=0; i<buffer->numStreams; i++) {
			ippsMulC_32f_I(m_gain, buffer->buffer32[i]+frameOffset, buffer->numFrames-frameOffset);
			for(int j=0; j<m_modulationSources.size(); j++) {
				//aLog("ms[%d] isRelease=%d isModInRel=%d", j, isRelease(), m_modulationSources[j]->isModulateInRelease());
				if(!(isRelease() && !m_modulationSources[j]->isModulateInRelease())) { //aLog("HERE");
					ippsMul_32f_I(m_modulationSources[j]->buffer(), buffer->buffer32[i], buffer->numFrames);
				}
			}
		}
#else
		// Gain
		for(int i=0; i<buffer->numStreams; i++) {
			for(int j=frameOffset; j<buffer->numFrames; j++) {
				buffer->buffer32[i][j] *= m_gain;
			}
		}

		// Modulation Sources
		for(int k=0; k<m_modulationSources.size(); k++) {
			if(!(isRelease() && !m_modulationSources[k]->isModulateInRelease())) {
				for(int i=0; i<buffer->numStreams; i++) {
					for(int j=frameOffset; j<buffer->numFrames; j++) {
						buffer->buffer32[i][j] *= m_modulationSources[k]->buffer()[j];
					}
				}
			}
		}
#endif
	}

	inline void renderBufferWithoutConstantGain(CsSampleBuffer *buffer, int frameOffset = 0)
	{
/*#ifdef USE_FW_AMP
		for(int i=0; i<m_modulationSources.size(); i++) {
			if(!(isRelease() && !m_modulationSources[i]->isModulateInRelease())) {
				for(int j=0; j<buffer->numStreams; j++) {
					fwsMul_32f_I(m_modulationSources[i]->buffer(), buffer->buffer32[j], buffer->numFrames);
				}
			}
		}
#elif*/
#ifdef USE_IPP_AMP
		for(int i=0; i<m_modulationSources.size(); i++) {
			if(!(isRelease() && !m_modulationSources[i]->isModulateInRelease())) {
				for(int j=0; j<buffer->numStreams; j++) {
					ippsMul_32f_I(m_modulationSources[i]->buffer(), buffer->buffer32[j], buffer->numFrames);
				}
			}
		}
#else
		for(int k=0; k<m_modulationSources.size(); k++) {
			if(!(isRelease() && !m_modulationSources[k]->isModulateInRelease())) {
				for(int i=0; i<buffer->numStreams; i++) {
					for(int j=frameOffset; j<buffer->numFrames; j++) {
						buffer->buffer32[i][j] *= m_modulationSources[k]->buffer()[j];
					}
				}
			}
		}
#endif
	}

	void setGain(float gain) { m_gain = gain; }
	inline float gain(void) { return m_gain; }

protected:
private:
	int m_maxBufferSize;
	//float *m_buffer;
	float m_gain;
};

}

#endif // __CsAmplifier_h__