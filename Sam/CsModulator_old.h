#ifndef __CsModulator_h__
#define __CsModulator_h__

#include "ARingBuffer.h"
using namespace Atom;

namespace Cs {

class CsModulator;

class CsModulationPoint
{
	friend CsModulator;

public:
	CsModulationPoint(int delta = 0, float value = 1.0f)
		: m_delta(delta)
		, m_value(value)
	{
	}

private:
	int m_delta;
	float m_value;
};

class CsModulator
{
public:
	CsModulator(int bufferSize, int period, float initialValue = 1.0f);
	~CsModulator();

	void setBufferSize(int bufferSize);
	void setValue(float value);
	void addValue(int delta, float value);
	bool isUpdateRequired(void) { return m_updateRequired; }
	void update(void);
	float* modulationBuffer(void) { return m_modulationBuffer; }

	//void clear(void);

	void debugModulationBuffer(void);
	int period(void) { return m_period; }

protected:
private:
	int m_bufferSize;
	int m_period;
	ARingBuffer<CsModulationPoint> *m_points;
	float *m_modulationBuffer;
	bool m_updateRequired;
	float m_value;
};

}

#endif