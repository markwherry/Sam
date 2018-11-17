#ifndef __SamOscillator_h__
#define __SamOscillator_h__

#include "CsSampleBuffer.h"

#include "SamCell.h"
#include "SamElement.h"
#include "SamStreamMix.h"

class SamVoice;
class SamChannel;

class SamOscillator
{
public:
	SamOscillator(SamVoice *voice, int numStreams, SamStreamMix *streamMix, int setId);
	~SamOscillator();

	void advanceSamplePointer(CsSampleBuffer *buffer, int frameOffset);

	void renderBuffer(CsSampleBuffer *buffer, int frameOffset);
	
	void assign(SamCell *cell, SamElement *element, int offset, bool release);

	void setPitchModulationBuffer(float *pitchModulationBuffer) { m_pitchModulationBuffer = pitchModulationBuffer; }

	bool isSamplePointerOutOfRange(void);

	inline double samplePointer(void) const { return m_samplePointer; }

	SamElement* element(void) const { return m_element; }

protected:
private:
	int fillBuffer(int fromFrame, int numFrames, float **buffer, int outputOffset = 0);

	SamVoice *m_voice;
	SamChannel *m_channel;
	SamStreamMix *m_streamMix;
	SamCell *m_cell;
	SamElement *m_element;

	int m_numStreams;
	int m_setId;

	

	float **decompressionBuffer;
	int decompressionBufferBlockIndex;
	
	double m_samplePointer;
	float m_inputSamplePointer;
	float m_gain;

	float **frame;

	float **inputBuffer;
	int m_maxInputBufferSize;
	int m_maxInputBufferSizeFloat;

	float *m_pitchModulationBuffer;

	bool m_release;

	
};

#endif // __SamOscillator_h__