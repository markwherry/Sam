#include "CsLfo.h"

namespace Cs {

CsLfo::CsLfo(double sampleRate, int bufferSize, int samplePeriod)
	: CsModulationSource(sampleRate, bufferSize, samplePeriod, false)
{
	m_oscillator = new CsOscillator(sampleRate);
	m_oscillator->setFrequency(1);
	m_oscillator->setShape(CsOscillator::Square);

	m_sampleBuffer.buffer32    = (float**)malloc(sizeof(float*) * 1);
	m_sampleBuffer.buffer32[0] = m_buffer;
	m_sampleBuffer.numFrames   = bufferSize;
	m_sampleBuffer.numStreams  = 1;
}

CsLfo::~CsLfo()
{
}

/*void CsLfo::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
}*/

void CsLfo::update(int frameOffset)
{
	m_oscillator->renderBuffer(&m_sampleBuffer, frameOffset);
}

void CsLfo::reset(void)
{
	m_oscillator->reset();
}

void CsLfo::setBufferSize(int bufferSize)
{
	CsModulationSource::setBufferSize(bufferSize);

	m_sampleBuffer.numFrames = bufferSize;
}

void CsLfo::setFrequency(float frequency)
{
	m_oscillator->setFrequency(frequency);
}

}