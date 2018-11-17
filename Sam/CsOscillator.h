#ifndef __CsOscillator_h__
#define __CsOscillator_h__

#include "CsSynthComponent.h"

namespace Cs {

class CsOscillator : public CsSynthComponent
{
public:
	CsOscillator(double sampleRate);

	void renderBuffer(CsSampleBuffer *buffer, int frameOffset = 0);
	void reset(void);
	
	void setAmplitude(double a) { m_amplitude = a; }
	void setFrequency(int f);
	double frequency(void) { return m_frequency; }
	void setShape(int shape);

	enum Shape {
		Sine     = 0,
		Square   = 1,
		Triangle = 2
	};

private:
	double m_amplitude;
	double m_frequency;
	double m_phase;
	int m_shape;
	double m_count;
	double m_step;
};

}

#endif // __Oscillator_h__