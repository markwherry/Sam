#include "CsEnvelope.h"

#include "ALog.h"
#include "AMath.h"

//#include <math.h>

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------
CsEnvelope::CsEnvelope(double sampleRate, int bufferSize, int samplePeriod)
	: CsSynthComponent(sampleRate)
	, m_samplePeriod(samplePeriod)
	, m_sustainStageIndex(-1)
	, m_initialGain(0.0)
	, m_releaseCounter(-1)
	, m_autoRelease(false)
	, m_released(false)
	, m_finished(false)
	, m_gain(0.0)
	, m_linearGain(0.0)
	, m_sampleCounter(0)
	, m_time(0)
	, m_stage(0)
{
	samplesPerMs = sampleRate/1000.0;
}

// ------------------------------------------------------------------------------------------------------------------------
CsEnvelope::~CsEnvelope()
{
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::renderBuffer(CsSampleBuffer *buffer, int frameOffset)
{
	//csDebug("CsEnvelope::renderBuffer() gain = %f", gain());
	for(int i=frameOffset; i<buffer->numFrames; i++) {
		for(int j=0; j<buffer->numStreams; j++) {
			buffer->buffer32[j][i] *= gain();
		}
		incrementSample();
	}
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::advance(CsSampleBuffer *buffer, int frameOffset)
{
	//csDebug("CsEnvelope::advnace() gain = %f", gain());
	
	for(int i=frameOffset; i<buffer->numFrames; i++) {
		incrementSample();
	}
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::reset(void)
{
	samplesPerMs = sampleRate()/1000.0;

	m_gain = 0.0f;
	m_linearGain = 0.0f;
	m_time = 0;
	m_stage = 0;
	m_releaseCounter = -1;
	m_released = false;
	m_finished = false;
	m_sampleCounter = 0;
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::createStage(CsEnvelopeStage::CsEnvelopeStageType stageType)
{
	CsEnvelopeStage *envelopeStage = new CsEnvelopeStage(stageType);
	envelopeStageList.append(envelopeStage);

	if(stageType==CsEnvelopeStage::Sustain) {
		m_sustainStageIndex = envelopeStageList.size()-1;
	}
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::setStage(int stageIndex, float timeMs, float gain, int curveShape, float curveDepth)
{
	//aLog("CsEnvelope::setStage(%d, %f, %f, %d, %f)", stageIndex, timeMs, gain, curveShape, curveDepth);

	float time = timeMs * samplesPerMs;

	envelopeStageList[stageIndex]->setTime(time);
	envelopeStageList[stageIndex]->setGain(gain);
	

	// Calculate gain steps for ramps (attack and decay stages)
	// The release gain step is calculated in setReleased because you never know what m_gain will be when the envelope is released
	// ie. the envelope could be released in the attack or decay stage, not necessarily the release stage
	if(envelopeStageList[stageIndex]->stageType() == CsEnvelopeStage::Ramp) {

		envelopeStageList[stageIndex]->setShape(curveShape);
		
		if(time==0.0) {
			//CSDEBUG("TIME IS 0 (%d, %f, %f)",stageId, timeMs, gain);
			//envelopeStageList[stageIndex]->setStep(0.0);
			
		} else {
			float previousGain = m_initialGain;
			if(stageIndex>0) {
				previousGain = envelopeStageList[stageIndex-1]->gain();
			}
			envelopeStageList[stageIndex]->setStep((gain-previousGain)/(time/m_samplePeriod));
			//CSDEBUG("gain = %f, previousGain=%f", gain, previousGain);

			//if(envelopeStageList[stageIndex].curve) {
			//	delete envelopeStageList[stageId].curve;
			//}
			//envelopeStageList[stageIndex]->curve = new CsCurve(((int)time/m_samplePeriod)+1, curveShape, previousGain, gain);
			//envelopeStageList[stageIndex]->curve()->setStart(previousGain);
			//envelopeStageList[stageIndex]->curve()->setDepth(curveDepth);
			//envelopeStageList[stageIndex]->curve()->debug();
			
		}
	}

	//aLog("EXIT: AEnvelope::setStage(%d)", stageIndex);
}

// ------------------------------------------------------------------------------------------------------------------------
void CsEnvelope::incrementSample(void)
{
	m_sampleCounter++;
	if(m_sampleCounter >= m_samplePeriod) {
		m_sampleCounter = 0;
		incrementSamplePeriod();
	}
}

void CsEnvelope::incrementSamplePeriod(void)
{
	//CSDEBUG("CsEnvelope::incrementSamplePeriod() gain=%f, time=%f", m_gain, m_time);
	m_time += m_samplePeriod;
	if(m_releaseCounter>0){
		m_releaseCounter -= m_samplePeriod;
		if(m_releaseCounter<1) {
			setReleased();
		}
	}
	calculateGain();
}

void CsEnvelope::setReleased(void)
{
	m_released = true;

	//if (released) {	
		if(m_sustainStageIndex>0) {
			m_stage = m_sustainStageIndex+1;
		} else {
			m_stage = envelopeStageList.size()-1;
		}
		
		//CSDEBUG("CsEnvelope::setReleased() release time = %f",envelopeStageList[m_stage].time);
		
		envelopeStageList[m_stage]->setStep( (envelopeStageList[m_stage]->gain()-m_gain)/(envelopeStageList[m_stage]->time()/m_samplePeriod) );
		//envelopeStageList[m_stage].curve->setStart(m_gain);
		//envelopeStageList[m_stage].curve->setEnd(envelopeStageList[m_stage].gain);

		//envelopeStageList[m_stage]->curve()->setStart(m_gain);


		//OLD//m_linearGain = m_gain;
		
		m_time = 0;
	//}
}

void CsEnvelope::calculateGain(void)
{
	if(m_finished) return;

	switch(envelopeStageList[m_stage]->stageType()) {
		
		case CsEnvelopeStage::Ramp:
			if (envelopeStageList[m_stage]->time()==0) {
				//CSDEBUG("HERE!!!");
				m_gain = 1.0f;
				m_linearGain = 1.0f;
				//m_gain = envelopeStageList[m_stage].curve->point(envelopeStageList[m_stage].curve->length()-1);
			} else if (m_time < envelopeStageList[m_stage]->time()) {

				int t = (int)(m_time/(float)m_samplePeriod);
				//CSDEBUG("m_time = %f", m_time);
				//CSDEBUG("(%d/%d) m_time=%f m_time/m_samplePeriod=%f (%d)", t, envelopeStageList[m_stage].curve->length(), m_time, m_time/(float)m_samplePeriod, m_samplePeriod);

				
				
				//if(t>=envelopeStageList[m_stage]->curve()->length()) {
					//CSDEBUG("OUT! (%d/%d) m_time=%f m_time/m_samplePeriod=%f (%d)", t, envelopeStageList[m_stage].curve->length(), m_time, m_time/(float)m_samplePeriod, m_samplePeriod);
				//}
				
				m_linearGain += envelopeStageList[m_stage]->step();
				//m_gain = m_linearGain;
				//m_gain = pow(m_linearGain, 2.0f);
				m_gain = AMath::pow(m_linearGain, 2);
				
			}
			if (m_time>=envelopeStageList[m_stage]->time()) {
				//CSDEBUG("Setting next stage");
				setNextStage();
			}
			break;
		
		case CsEnvelopeStage::Sustain:
			m_gain = envelopeStageList[m_stage]->gain();
			m_linearGain = m_gain;
			if (m_autoRelease) {
				setReleased ();
			}
			break;

		case CsEnvelopeStage::Hold:
			//CSDEBUG("CsEnvelopeStage::esHold:");
			m_gain = envelopeStageList[m_stage]->gain();
			m_linearGain = m_gain;
			if (m_time>=envelopeStageList[m_stage]->time()) {
				//CSDEBUG("Hold expiring");// m_time = %f (%f)", m_time
				setNextStage();
			}
			break;//*/

	}
}

void CsEnvelope::setNextStage(void)
{
	//CSDEBUG("CsEnvelope::setNextStage(%d) %f", m_stage+1, m_gain);
	m_time = 0;			
	m_stage++;

	if(m_stage >= envelopeStageList.size()) {
		m_finished = true;
		m_gain = envelopeStageList[envelopeStageList.size()-1]->gain();
	} else {

	//CSDEBUG("now on stage=%d/%d", m_stage+1, envelopeStageList.size());
	//CSDEBUG("gain %f", envelopeStageList[m_stage].gain);
	}

	m_linearGain = m_gain;
}

void CsEnvelope::setCurrentStage(int stage)
{
	if(stage<envelopeStageList.size()) {
		m_time = 0;			
		m_stage = stage;
		calculateGain();
	}
}

void CsEnvelope::debug(void)
{
	aLog("CsEnvelope::debug() %d stages", envelopeStageList.size());

	for(int i=0; i<envelopeStageList.size(); i++) {
		aLog("[%d] %d - %f, %f", i, envelopeStageList[i]->stageType(), envelopeStageList[i]->time(), envelopeStageList[i]->gain());//, envelopeStageList[i]->step());
	}
}
// ------------------------------------------------------------------------------------------------------------------------
CsEnvelopeAsr::CsEnvelopeAsr(double sampleRate, int bufferSize, int samplePeriod)
	: CsEnvelope(sampleRate, bufferSize, samplePeriod)
{
	createStage(CsEnvelopeStage::Ramp);
	createStage(CsEnvelopeStage::Sustain);
	createStage(CsEnvelopeStage::Ramp);
}

void CsEnvelopeAsr::setEnvelope(int attackTime, float sustainGain, double releaseTime)
{
	//aLog("setEnvelope(%f, %f, %f)", attackTime, sustainGain, releaseTime);
	reset();
	setStage(0, attackTime, sustainGain);
	setStage(1, 0.0, sustainGain);
	setStage(2, releaseTime, 0.0);
	calculateGain();
}

// ------------------------------------------------------------------------------------------------------------------------
CsEnvelopeAhdsr::CsEnvelopeAhdsr(double sampleRate, int bufferSize, int samplePeriod)
	: CsEnvelope(sampleRate, bufferSize, samplePeriod)
{
	createStage(CsEnvelopeStage::Ramp);
	createStage(CsEnvelopeStage::Hold);
	createStage(CsEnvelopeStage::Ramp);
	createStage(CsEnvelopeStage::Sustain);
	createStage(CsEnvelopeStage::Ramp);
}

void CsEnvelopeAhdsr::setEnvelope(float attackTime, double holdTime, double decayTime, float sustainGain, double releaseTime, int attackShape, int decayShape, int releaseShape, float attackDepth, float decayDepth, float releaseDepth)
{
	//csDebug("aT=%f, hT=%f, dT=%f, sG=%f, rT=%f, aS=%d, dS=%d, rS=%d, aD=%f, dD=%f, rD=%f", attackTime, holdTime, decayTime, sustainGain, releaseTime, attackShape, decayShape, releaseShape, attackDepth, decayDepth, releaseDepth);
	reset();
	setStage(0, attackTime, 1.0, attackShape, attackDepth);
	setStage(1, holdTime, 1.0);
	setStage(2, decayTime, sustainGain, decayShape, decayDepth);
	setStage(3, 0, sustainGain);
	setStage(4, releaseTime, 0.0, releaseShape, releaseDepth);
	calculateGain();
}

// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

}