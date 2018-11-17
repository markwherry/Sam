#include "CsModulator.h"

#include "AMemoryManager.h"
#include "ALog.h"

namespace Cs {

CsModulator::CsModulator(double sampleRate, int bufferSize, int period, bool modulateInRelease, int rate, float initialValue)
: CsModulationSource(sampleRate, bufferSize, period, modulateInRelease)
, m_rate(rate)
, m_initialValue(initialValue)
, m_value(initialValue)
, m_count(0)
, m_step(0)
, m_updateRequired(false)
, m_envValue(1.0f)
{
	//setSize(size);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::setBufferSize(int bufferSize)
{
	CsModulationSource::setBufferSize(bufferSize);
	/*if(m_buffer) {
		aFree(m_buffer, sizeof(float)*(m_size/m_period));
	}

	m_size = size;

	m_buffer = (float*)aMalloc(sizeof(float)*(m_size/m_period));*/

	for(int i=0; i<bufferSize; i++) {
		m_buffer[i] = m_initialValue;
	}
}

void CsModulator::setTargetValue(float value)
{
	m_targetValue = value;

	float range = m_buffer[bufferSize()-1] - m_targetValue;

	m_step = range/(float)m_rate;

	m_count = m_rate;

	m_updateRequired = true;
}

void CsModulator::resetValue(float value)
{
	m_value = value;
	for(int i=0; i<bufferSize(); i++) {
		m_buffer[i] = value;
	}
	m_count = 0;
	m_updateRequired = false;
}

void CsModulator::update(int frameOffset)
{
	//int counter = 0;
	

	for(int i=0; i<bufferSize(); i++) {
		//aLog("m_count = %d, m_value = %f", m_count, m_value);
		if(m_count > 0) {
			m_buffer[i] = m_value -= m_step;
			m_count--;
		} else {
			m_buffer[i] = m_value;
		}
		/*if(m_envelope) {
			m_buffer[i] *= m_envelope->gain();
			for(int j=0; j<m_period; j++) {
				m_envelope->incrementSample();
			}
		}*/
	}
	

	m_updateRequired = false;
}

void CsModulator::debug(void)
{
	float sum = 0.0f;
	//aLog("CsModulator::debug()");
	for(int i=0; i<bufferSize(); i++) {
		//aLog("[%d] %f\n", i, m_buffer[i]);
		sum += m_buffer[i];
	}

	aLog("CsModulator::debug() sum = %f (m_value = %f)", sum, m_value);
}

}