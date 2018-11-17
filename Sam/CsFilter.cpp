#include "CsFilter.h"

#include <math.h>

#include <Alog.h>

namespace Cs {

CsFilter::CsFilter(int type, float sampleRate)
: m_type(type) 
, m_sampleRate(sampleRate)
, m_cutoff(sampleRate/2)
, m_resonance(1.0f)
{
	clearHistory();
	calculateCoefficients();
}

float CsFilter::getNextSample(float sample)
{
	float output = a1 * sample + a2 * in[0] + a3 * in[1] - b1*out[0] - b2*out[1];

	in[1] = in[0];
	in[0] = sample;

	out[1] = out[0];
	out[0] = output;

	return output;
}

void CsFilter::clearHistory(void)
{
	for(int i=0; i<2; i++) {
		in[i] = 0;
		out[i] = 0;
	}
}

void CsFilter::calculateCoefficients(void)
{
	float c = 1.0 / tan(3.141592653589793 * m_cutoff / m_sampleRate);

	a1 = 1.0 / ( 1.0 + m_resonance * c + c * c);
	a2 = 2* a1;
	a3 = a1;
	b1 = 2.0 * ( 1.0 - c*c) * a1;
	b2 = ( 1.0 - m_resonance * c + c * c) * a1;
}

// ---

CsSynthFilter::CsSynthFilter(double sampleRate, int numStreams)
: CsSynthComponent(sampleRate)
, m_numStreamsAllocated(numStreams)
{
	for(int i=0; i<m_numStreamsAllocated; i++) {
		filter[i] = new CsFilter(0, sampleRate);
	}
}

void CsSynthFilter::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
	for(int i=0; i<buffer->numStreams; i++) {
		float *stream = buffer->buffer32[i];
		for(int j=0; j<buffer->numFrames; j++) {
			stream[j] = filter[i]->getNextSample(stream[j]);
		}
	}
}

void CsSynthFilter::renderBufferWithModulation(CsSampleBuffer *buffer, int frameOffset, CsModulationSource *modulator, float min, float max)
{
	//modulator->debugModulationBuffer();

	float range = max-min;
	//aLog("range %f = max %f - min %f", range, max, min);

	int period = 8;

	for(int i=0; i<buffer->numStreams; i++) {
		int pos = 0;
		float *stream = buffer->buffer32[i];
		for(int j=0; j<buffer->numFrames/1; j += period) { //modulator->period()
			if(modulator->buffer()[j] >= 1.0f) {
				//aLog("%f", modulator->buffer()[j]);
				modulator->buffer()[j] = 1.0f;
			}
			setCutOff(min+(range*modulator->buffer()[j]));
			for(int k=0; k<period; k++) { //modulator->period()
				stream[pos] = filter[i]->getNextSample(stream[pos]);
				pos++;
			}
		}
	}
	//aLog("%f", filter[0]->cutoff());
}

void CsSynthFilter::renderBufferWithModulationOriginal(CsSampleBuffer *buffer, int frameOffset, CsModulationSource *modulator, float min, float max)
{
	//modulator->debugModulationBuffer();

	float range = max-min;
	//aLog("range %f = max %f - min %f", range, max, min);

	for(int i=0; i<buffer->numStreams; i++) {
		int pos = 0;
		float *stream = buffer->buffer32[i];
		for(int j=0; j<buffer->numFrames/1; j++) { //modulator->period()
			if(modulator->buffer()[j] >= 1.0f) {
				//aLog("%f", modulator->buffer()[j]);
				modulator->buffer()[j] = 1.0f;
			}
			setCutOff(min+(range*modulator->buffer()[j]));
			for(int k=0; k<1; k++) { //modulator->period()
				stream[pos] = filter[i]->getNextSample(stream[pos]);
				pos++;
			}
		}
	}
	//aLog("%f", filter[0]->cutoff());
}

void CsSynthFilter::setCutOff(float cutoff)
{
	//aLog("CsSynthFilter::setCutoff(%f)", cutoff);
	for(int i=0; i<m_numStreamsAllocated; i++) {
		filter[i]->setCutoff(cutoff);
	}
}

void CsSynthFilter::setResonance(float resonance)
{
	for(int i=0; i<m_numStreamsAllocated; i++) {
		filter[i]->setResonance(resonance);
	}
}

void CsSynthFilter::reset(void)
{
	for(int i=0; i<m_numStreamsAllocated; i++) {
		filter[i]->clearHistory();
	}
}

}