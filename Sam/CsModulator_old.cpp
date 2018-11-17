#include "CsModulator.h"

#include "AMemoryManager.h"
#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsModulator::CsModulator(int bufferSize, int period, float initialValue)
: m_period(period)
, m_points(0)
, m_modulationBuffer(0)
, m_updateRequired(true)
, m_value(initialValue)
{
	setBufferSize(bufferSize);
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsModulator::~CsModulator()
{
	if(m_points) {
		int numPoints = m_points->size();
		for(int i=0; i<numPoints; i++) {
			CsModulationPoint point = m_points->get();
			//delete point;
		}
		delete m_points;
		aFree(m_modulationBuffer, sizeof(sizeof(float)*(m_bufferSize/m_period)));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::setBufferSize(int bufferSize)
{
	if(m_points) {
		int numPoints = m_points->size();
		for(int i=0; i<numPoints; i++) {
			CsModulationPoint point = m_points->get();
			//delete point;
		}
		delete m_points;
		aFree(m_modulationBuffer, sizeof(sizeof(float)*(m_bufferSize/m_period)));
	}

	m_bufferSize = bufferSize;

	m_points = new ARingBuffer<CsModulationPoint>(m_bufferSize);
	m_modulationBuffer = (float*)aMalloc(sizeof(float)*(m_bufferSize/m_period));

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::setValue(float value)
{
	m_value = value;

	int numPoints = m_points->size();
	for(int i=0; i<numPoints; i++) {
		CsModulationPoint point = m_points->get();
		//delete point;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::addValue(int delta, float value)
{
	aLog("CsModulator::addValue(%d, %f)", delta, value);

	if(delta>=m_bufferSize) {
		delta = m_bufferSize-1;
	}
	CsModulationPoint point(delta/m_period, value);

	m_points->add(point);

	m_updateRequired = true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::update(void)
{
	int numPoints = m_points->size();
	if(numPoints<1) {
		for(int i=0; i<m_bufferSize/m_period; i++) {
			m_modulationBuffer[i] = m_value;
		}
	} else {
		int pos = 0;
		for(int i=0; i<numPoints; i++) {
			CsModulationPoint point = m_points->get();
			for(int j=pos; j<point.m_delta; j++) {
				float t = (float)(j-pos)/((point.m_delta-pos));
				m_modulationBuffer[j] = (1-t)*m_value+(t*point.m_value);
			}
			m_value = point.m_value;
			pos = point.m_delta;
		}
		if(pos < m_bufferSize/m_period) {
			for(int i=pos; i<m_bufferSize/m_period; i++) {
				m_modulationBuffer[i] = m_value;
			}
		}
	}

	m_updateRequired = false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsModulator::debugModulationBuffer(void)
{
	for(int i=0; i<m_bufferSize/m_period; i++) {
		aLog("[%d] %f", i, m_modulationBuffer[i]);
	}
}

}