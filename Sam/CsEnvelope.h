#ifndef __CsEnvelope_h__
#define __CsEnvelope_h__

#include "CsModulationSource.h"
#include "CsSampleBuffer.h"
//#include "Curve.h"
#include "AList.h"
using namespace Atom;

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------
class CsEnvelopeStage
{
public:
	enum CsEnvelopeStageType {
		Unassigned = 0,
		Ramp = 1,
		Sustain = 2,
		Hold = 3
	};
	CsEnvelopeStage(CsEnvelopeStageType stageType = Unassigned, int time = 0, float gain = 0.0f, float step = 0.0f, int shape = 0)
		: m_stageType(stageType)
		, m_time(time)
		, m_gain(gain)
		, m_step(step)
		, m_initialGain(0.0)
	{
	}
	//~EnvelopeStage();

	CsEnvelopeStageType stageType(void) { return m_stageType; }
	
	inline float time(void) { return m_time; }
	void setTime(float time) { m_time = time; }

	inline float initialGain(void) { return m_initialGain; }
	void setInitialGain(float gain) { m_initialGain = gain; }

	inline float gain(void) { return m_gain; }
	void setGain(float gain) { m_gain = gain; }

	inline float step(void) { return m_step; }
	void setStep(float step) { m_step = step; }

	inline int shape(void) { return m_shape; }
	void setShape(int shape) { m_shape = shape; }

protected:
private:
	CsEnvelopeStageType m_stageType;
	float m_time;
	float m_gain;
	float m_initialGain;
	float m_step;
	int m_shape;
};

// ------------------------------------------------------------------------------------------------------------------------
class CsEnvelope : public CsModulationSource
{
public:
	CsEnvelope(double sampleRate, int bufferSize, int samplePeriod = 1);
	~CsEnvelope();

	void renderBuffer(CsSampleBuffer *buffer, int frameOffset);
	//void advance(CsSampleBuffer *buffer, int frameOffset);
	void update(int frameOffset = 0);
	void advance(int frames);
	void reset(void);

	void createStage(CsEnvelopeStage::CsEnvelopeStageType stageType);
	void setStage(int stageIndex, float timeMs, float gain, int curveShape = 0, float curveDepth = 0.5);

	void setCurrentStage(int stage);

	void setInitialGain(float initialGain) { m_initialGain = initialGain; }

	inline void incrementSample(void);
	//void incrementSamplePeriod(void);

	void setReleased(void);
	void setReleaseCounter(int releaseCounter) { m_releaseCounter = releaseCounter; }
	void setAutoRelease(bool autoRelease = true) { m_autoRelease = autoRelease; }

	inline float gain(void) { return m_gain; }
	inline bool released(void) const { return m_released; }
	inline bool finished(void) const { return m_finished; }

	void setOffset(int offset) { m_offset = offset; }

	void debug(void);

protected:
	void calculateGain(void);
private:
	void setNextStage(void);
	double samplesPerMs;
	AList<CsEnvelopeStage*> envelopeStageList;
	int m_sustainStageIndex;
	float m_initialGain;
	int m_releaseCounter;
	bool m_autoRelease;
	bool m_released;
	bool m_finished;
	float m_gain;
	float m_linearGain;
	int m_sampleCounter;
	int m_time;
	int m_stage;
	int m_offset;

};

// ------------------------------------------------------------------------------------------------------------------------
class CsEnvelopeAsr : public CsEnvelope
{
public:
	CsEnvelopeAsr(double sampleRate, int bufferSize, int samplePeriod = 1);

	void setEnvelope(int attackTime, float sustainGain, double releaseTime);

protected:
private:
};

// ------------------------------------------------------------------------------------------------------------------------
class CsEnvelopeAhdsr : public CsEnvelope
{
public:
	CsEnvelopeAhdsr(double sampleRate, int bufferSize, int samplePeriod = 1);

	void setEnvelope(float attackTime, double holdTime, double decayTime, float sustainGain, double releaseTime, int attackShape = 1, int decayShape = 2, int releaseShape = 2, float attackDepth = 0.5, float decayDepth = 0.5, float releaseDepth = 0.5, float attackGain = 1.0f);

protected:
private:
};

// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

}

#endif