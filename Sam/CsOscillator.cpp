#include "CsOscillator.h"

#include <math.h>

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsOscillator::CsOscillator(double sampleRate)
	: CsSynthComponent(sampleRate)
	, m_phase(0)
	, m_amplitude(1.0)
	, m_frequency(440.0)
	, m_shape(Sine)
	, m_count(0.0)
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsOscillator::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
	switch(m_shape) {
		case Sine:
			{
				double freq = m_frequency * 2.0 * 3.14159265359 / sampleRate();
				for(int i=frameOffset; i<buffer->numFrames; i++) {
					float sample = sin(m_phase);
					m_phase += freq;
					for(int j=0; j<buffer->numStreams; j++) {
						buffer->buffer32[j][i] = sample * m_amplitude;
					}
				}
			}
			break;
		case Square:
			{
				float sample;
				for(int i=frameOffset; i<buffer->numFrames; i++) {
					if(m_count<((sampleRate()/m_frequency)/2.0)) {
						sample = 1.0f;
					} else {
						sample = -1.0f;
					}
					m_count += 1.0f;
					if(m_count>=(sampleRate()/m_frequency)) {
						m_count = 0.0f;
					}
					for(int j=0; j<buffer->numStreams; j++) {
						buffer->buffer32[j][i] = sample * m_amplitude;
					}
				}
			}
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsOscillator::reset(void)
{
	m_phase = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsOscillator::setFrequency(int f)
{
	m_frequency = f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsOscillator::setShape(int shape)
{
	m_shape = shape;

	if(m_shape == Triangle) {
		m_step = m_frequency / 4;
	}
}

}