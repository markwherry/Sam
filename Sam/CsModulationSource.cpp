#include "CsModulationSource.h"

CsModulationSource::CsModulationSource(double sampleRate, int bufferSize, int period, bool modulateInRelease)
	: m_sampleRate(sampleRate)
	, m_bufferSize(bufferSize)
	, m_period(period)
	, m_modulateInRelease(modulateInRelease)
	, m_buffer(0)
{
	setBufferSize(m_bufferSize);
}

CsModulationSource::~CsModulationSource()
{
	setBufferSize(0);
}

void CsModulationSource::setBufferSize(int bufferSize)
{	
	if(m_buffer) {
		aFree(m_buffer, m_bufferSize);
		m_buffer = 0;
	}
	m_bufferSize = bufferSize;
	if(m_bufferSize > 0) {
		m_buffer = (float*)aMalloc(sizeof(float)*m_bufferSize);

/*#ifdef USE_FW_MOD
		fwsSet_32f(1.0f, m_buffer, m_bufferSize);
#else*/
#ifdef USE_IPP_MOD
		ippsSet_32f(1.0f, m_buffer, m_bufferSize);
#else
		for(int i=0; i<m_bufferSize; i++) {
			m_buffer[i] = 1.0f;
		}
#endif
	}
}