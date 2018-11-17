#ifndef __CsLfo_h__
#define __CsLfo_h__

#include "CsModulationSource.h"
#include "CsOscillator.h"

namespace Cs {

class CsLfo : public CsModulationSource
{
public:
	CsLfo(double sampleRate, int bufferSize, int samplePeriod = 1);
	~CsLfo();

	//void renderBuffer(CsSampleBuffer *buffer, int frameOffset);

	void update(int frameOffset = 0);
	void reset(void);
	void setBufferSize(int bufferSize);

	void setFrequency(float frequency);
	float frequency(void) { return m_oscillator->frequency(); }

protected:
private:
	CsOscillator *m_oscillator;
	CsSampleBuffer m_sampleBuffer;
};

}

#endif