#ifndef __CsFilter_h__
#define __CsFilter_h__

#include "CsSynthComponent.h"
#include "CsModulator.h"

namespace Cs
{

class CsFilter
{
public:
	CsFilter(int type, float sampleRate);

	inline float getNextSample(float sample);

	void clearHistory(void);

	inline float cutoff(void) { return m_cutoff; }
	void setCutoff(float cutoff) { m_cutoff = cutoff; calculateCoefficients(); }

	void setResonance(float resonance) { m_resonance = resonance; calculateCoefficients(); }

	enum CsFilterTypes {
		LoPass = 0,
		HiPass = 1
	};

protected:
private:
	void calculateCoefficients(void);
	
	float m_sampleRate;
	int m_type;
	float m_cutoff;
	float m_resonance;
	float a1, a2, a3;
	float b1, b2;
	float in[2];
	float out[2];
};

class CsSynthFilter : public CsSynthComponent
{
public:
	CsSynthFilter(double sampleRate, int numStreams);

	inline void renderBuffer(CsSampleBuffer *buffer, int frameOffset = 0);
	void renderBufferWithModulation(CsSampleBuffer *buffer, int frameOffset, CsModulationSource *modulator, float min, float max);
	void renderBufferWithModulationOriginal(CsSampleBuffer *buffer, int frameOffset, CsModulationSource *modulator, float min, float max);
	void reset(void);

	void setCutOff(float cutoff);
	float cutOff(int stream = 0) { return filter[stream]->cutoff(); }

	void setResonance(float resonance);

protected:
private:
	CsFilter *filter[16];
	int m_numStreamsAllocated;
};

}

#endif