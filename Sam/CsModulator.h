#ifndef __CsModulator_h__
#define __CsModulator_h__

#include "CsModulationSource.h"

namespace Cs {

class CsModulator : public CsModulationSource
{
public:
	CsModulator(double sampleRate, int bufferSize, int period, bool modulateInRelease, int rate, float initialValue);

	void setBufferSize(int size);
	void setTargetValue(float value);
	void resetValue(float value);
	void update(int frameOffset = 0);

	//float* buffer(void) { return m_buffer; }
	//int period(void) { return m_period; }
	float value(void) { return m_value; }

	float firstValue(void) { return m_buffer[0]; }
	float lastValue(void) { return m_buffer[bufferSize()-1]; }

	void debug(void);

	bool isUpdateRequired(void) { return m_updateRequired; }
	float targetValue(void) { return m_targetValue; }

	void setRate(int rate) { m_rate = rate; }
	int rate(void) { return m_rate; }

protected:
private:
	float m_initialValue;
	float m_targetValue;
	int m_count;
	float m_value;
	float m_step;
	int m_rate;
	bool m_updateRequired;
	float m_envValue;
};

}

#endif