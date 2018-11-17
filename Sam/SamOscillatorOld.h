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
	void renderBufferOld(CsSampleBuffer *buffer, int frameOffset);
	
	void assign(SamCell *cell, SamElement *element, int offset, bool release);

	void setPitchModulationBuffer(float *pitchModulationBuffer) { m_pitchModulationBuffer = pitchModulationBuffer; }

	bool isSamplePointerOutOfRange(void);

	inline double samplePointer(void) const { return m_samplePointer; }

protected:
private:
	int fillBuffer(int fromFrame, int numFrames, float **buffer, int outputOffset = 0);
	int fillBufferOld(int fromFrame, int numFrames, float **buffer);
	void pan(float sample, int stream, CsSampleBuffer *buffer, int pos);

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

	float *m_pitchModulationBuffer;

	bool m_release;

	
};

/*
#include <AObject.h>
#include <AOscillator.h>
using namespace Atom;

#include "SamElement.h"

class SamOscillator : public AObject
{
public:
	SamOscillator(AObject *parent, int numStreams);
	~SamOscillator();

	//void assignCell(SamCell *cell, bool release, SamStreamMix *streamMix, int offset);
	void assignElement(SamElement *element);
	int renderBuffer(int fromFrame, int numFrames, float **buffer);
	//bool getNextFrame(float* output, double pitchIncrement, float gain);
	//bool getNextFrames(float output[][8], int numFrames, double pitchIncrement, float gain);
	//bool advanceNextFrame(double pitchIncrement);
	//bool advanceNextFrames(int numFrames, double pitchIncrement);
	//SamCell* cell(void) { return m_cell; }

	//inline double samplePosition(void) const { return m_samplePosition; }

	//bool cellHasRelease(void) { return m_cell->hasRelease(); }
	//bool cellIsSourceRelease(void) { return m_cell->isSourceRelease(); }

	//bool isReleaseMarkerPast(void);

private:
	//void incrementSamplePosition(double pitchIncrement);
	//void pan(float *output, float inputSample, SamStream *stream, int directId);
	//float intToFloatScaler;
	//int m_numFrames;
	//SamCell *m_cell;
	//bool m_release;
	//SamStreamMix *m_streamMix;
	//double m_samplePosition;
	float m_gain;

	SamElement *m_element;
	float **decompressionBuffer;
	int decompressionBufferBlockIndex;
	int m_numStreamsAllocated;

	AOscillator *osc;
};*/

#endif // __SamOscillator_h__