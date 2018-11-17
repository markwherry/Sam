#ifndef __CsSynthComponent_h__
#define __CsSynthComponent_h__

#include "CsSampleBuffer.h"
#include "CsModulationSource.h"

#include "AList.h"
using namespace Atom;

namespace Cs {

class CsSynthComponent
{
public:
	CsSynthComponent(double sampleRate)
		: m_sampleRate(sampleRate)
		, m_release(false)
	{
	}

	virtual void renderBuffer(CsSampleBuffer *buffer, int frameOffset) = 0;
	virtual void reset(void) { }

	double sampleRate(void) { return m_sampleRate; }
	void setSampleRate(double sampleRate)
	{
		m_sampleRate = sampleRate;

		reset();
	}

	void addModulationSource(CsModulationSource *modSrc)
	{
		m_modulationSources.append(modSrc);
	}

	void setRelease(bool release) { m_release = release; }
	bool isRelease(void) { return m_release; }

protected:
	AList<CsModulationSource*> m_modulationSources;

private:
	double m_sampleRate;
	bool m_release;
};

}

#endif // __CsSynthComponent_h__