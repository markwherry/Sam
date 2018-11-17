#ifndef __CsModulationSource_h__
#define __CsModulationSource_h__

#include "AMemoryManager.h"
#include "ALog.h"

//#define USE_FW_MOD

#ifdef USE_FW_MOD
#include <fwSignal.h>
#endif

#define USE_IPP_MOD

#ifdef USE_IPP_MOD
#include "ipps.h"
#endif

class CsModulationSource
{
public:
	CsModulationSource(double sampleRate, int bufferSize, int period, bool modulateInRelease);
	~CsModulationSource();

	virtual void update(int frameOffset = 0) { }
	virtual void reset(void) { }

	inline int bufferSize(void) { return m_bufferSize; }
	virtual void setBufferSize(int bufferSize);

	inline double sampleRate(void) { return m_sampleRate; }
	void setSampleRate(double sampleRate)
	{
		m_sampleRate = sampleRate;

		reset();
	}

	inline int period(void) { return m_period; }

	inline float *buffer(void) { return m_buffer; }

	bool isModulateInRelease(void) { return m_modulateInRelease; }

	inline void add(CsModulationSource *src)
	{
		//aLog("add");
		if(m_bufferSize == src->bufferSize()) {
			//aLog("Equal");
			//for(int i=0; i<m_bufferSize; i++) {
			//	m_buffer[i] *= src->buffer()[i];
			//}
			ippsMul_32f_I(src->buffer(), m_buffer, m_bufferSize);
		}
	}

	inline void copy(CsModulationSource *src)
	{
		if(m_bufferSize == src->bufferSize()) {
			//for(int i=0; i<m_bufferSize; i++) {
			//	m_buffer[i] = src->buffer()[i];
			//}
			ippsCopy_32f(src->buffer(), m_buffer, m_bufferSize);
		}
	}

protected:
	float *m_buffer;

private:
	int m_bufferSize;
	float m_sampleRate;
	int m_period;
	bool m_modulateInRelease;
};

#endif