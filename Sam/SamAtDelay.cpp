#include "SamAtDelay.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAtDelay::SamAtDelay(SamToolMaster *toolMaster)
	: SamAudioTool(toolMaster, SAM_DELAY_NAME, SAM_DELAY_UUID)
	, m_wetLevel(0.5f)
	, m_dryLevel(1.0f)
	, m_feedback(0.3f)
	, m_groove(50)
	, m_quarterNotes(1.0f)
	, m_tempo(120.0f)
	, delayCounter(0)
{
	addParameter(new SamToolParameter(pFeedback, "Feedback", "%", 0, 0, 100));
	addParameter(new SamToolParameter(pLength, "Length", "", 0, 0, 3));
	addParameter(new SamToolParameter(pGroove, "Groove", "%", 0, 33, 75));
	addParameter(new SamToolParameter(pHiCut, "High Cut", "Hz", 0, 20, 20000));
	addParameter(new SamToolParameter(pDryOutput, "Dry Output", "%", 0, 0, 100));
	addParameter(new SamToolParameter(pWetOutput, "Wet Output", "%", 0, 0, 100));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAtDelay::setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig)
{
	if(inputConfig == CsSpeakerConfig(CsSpeakerConfig::Mono) && outputConfig == CsSpeakerConfig(CsSpeakerConfig::Mono)) {
		return SamAudioTool::setSpeakerConfig(inputConfig, outputConfig);	
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo)
{
	SamAudioTool::setAudioProcessInfo(audioProcessInfo); //csDebug("1");

	size_t fs = (size_t)audioProcessInfo.sampleRate;
	
	//csDebug("fs=%d", fs);

	size_t size = fs*sizeof(float)*10;
	delayBuffer = (float*)malloc(size); //csDebug("2");

	aLog("&&&&& delayBuffer=%d (size=%d), fs=%f", delayBuffer, size,audioProcessInfo.sampleRate); //csDebug("3");
	memset(delayBuffer, 0, size); aLog("4");

	filterLoPass = new CsFilter(CsFilter::LoPass, audioProcessInfo.sampleRate); //csDebug("5");
	filterLoPass->setCutoff(20000); //csDebug("6");
	//filterHiPass = new CsFilter(CsFilter::HiPass, audioProcessInfo.sampleRate);

	setDelayTime(500.0f); //csDebug("7");

	//csDebug("EXIT: D1::setAudioProcessInfo()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::process(CsSampleBuffer *inputs, CsSampleBuffer *outputs, int numInputs, int numOutputs, double timestamp, double samples)
{
	//aLog("SamGain::process() %d", inputs->numFrames);

	float *inputBuffer = inputs->buffer32[0];
	float *outputBuffer = outputs->buffer32[0];

	for(int i=0; i<inputs->numFrames; i++) {
		//outputBuffer[i] = inputBuffer[i]; // Testing only
		float x = inputBuffer[i];
		x = filterLoPass->getNextSample(x);
		//x = filterHiPass->getNextSample(x);

		float y = delayBuffer[delayCounter];
		delayBuffer[delayCounter] = x + (y*m_feedback);
		
		
		delayCounter++;
		if(delayCounter >= m_sampleDelayTime) {
			delayCounter = 0;
		}
		outputBuffer[i] = (inputBuffer[i]*m_dryLevel) + (y*m_wetLevel);//*/
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::start(void)
{
	size_t size = 44100*sizeof(float)*10;
	memset(delayBuffer, 0, size);
	filterLoPass->clearHistory();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::processEvent(SamEvent *ev)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamAtDelay::parameterValueNormalised(int id)
{
	switch(id) {
		case pFeedback:  return m_feedback; break;
		case pLength:
			if(m_quarterNotes==0.25f) return 0.0f;
			else if(m_quarterNotes==0.5f) return (1.0f/3.0f)*1.0f;
			else if(m_quarterNotes==1.0f) return (1.0f/3.0f)*2.0f;
			else if(m_quarterNotes==2.0f) return 1.0f;
			break;
		case pGroove:    return (1.0f/42.0f)*(m_groove-33); break;
		case pHiCut:     return (1.0f/20000.0f)*(filterLoPass->cutoff()-20.0f); break;
		case pDryOutput: return m_dryLevel; break;
		case pWetOutput: return m_wetLevel; break;
	}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::setParameterValueNormalised(int id, float value)
{
	switch(id) {
		case pFeedback:
			m_feedback = value;
			break;
		case pLength: {
			int mode = (int)3.0f*value;
			float qn = 1.0f;
			//csDebug("kParamLength %d", mode);
			if(mode==0) qn = 0.25f;
			else if(mode==1) qn = 0.5f;
			else if(mode==2) qn = 1.0f;
			else if(mode==3) qn = 2.0f;
			setDelayTimeQuarterNotes(qn);
							}
			break;
		case pGroove:
			m_groove = (int)((float)33.0f+(42.0f*value));
			setDelayTime(m_delayTime);
			break;
		case pHiCut:
			filterLoPass->setCutoff(20.0f+(19980.0f*value));
			break;
		case pDryOutput:
			m_dryLevel = value;
			break;
		case pWetOutput:
			m_wetLevel = value;
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAtDelay::setDelayTime(float delayTime)
{
	int oldSampleDelayTime = m_sampleDelayTime;
	m_delayTime = delayTime;
	delayTime = delayTime*(1+((m_groove-50)*0.02f));
	float sampleDelayTime = delayTime*(sampleRate()/1000.0f);
	if(sampleDelayTime<sampleRate()*10.0f) {
		m_sampleDelayTime = (int)sampleDelayTime;
	}
	
	if(delayCounter!=0) {
		if(m_sampleDelayTime>oldSampleDelayTime) {
			//csDebug("Adjusting buffer longer!");
			
			int adjustmentTime = m_sampleDelayTime - oldSampleDelayTime;

			float startValue = delayBuffer[oldSampleDelayTime-1];
			float endValue = delayBuffer[0];

			float diff = endValue - startValue;
			float diffStep = diff/adjustmentTime;

			for(int i = oldSampleDelayTime; i<m_sampleDelayTime; i++) {
				delayBuffer[i] = delayBuffer[i-1] + diffStep;  
			}
		
		} else if(m_sampleDelayTime<oldSampleDelayTime) {
			//csDebug("Buffer getting shorter.");
			
			
			
			if(delayCounter>=m_sampleDelayTime) {
				//csDebug("This shouldn't happen too often! :-)");
				delayCounter = 0;
				int adjustmentTime = 32;
				float startValue = delayBuffer[0];
				float endValue = delayBuffer[32];

				float diff = endValue - startValue;
				float diffStep = diff/adjustmentTime;

				for(int i = 0; i<32; i++) {
					delayBuffer[i] = delayBuffer[i-1] + diffStep;  
				}
			} else {

				int adjustmentTime = 32;

				float startValue = delayBuffer[m_sampleDelayTime-32];
				float endValue = delayBuffer[0];

				float diff = endValue - startValue;
				float diffStep = diff/adjustmentTime;

				for(int i = m_sampleDelayTime-32; i<m_sampleDelayTime; i++) {
					delayBuffer[i] = delayBuffer[i-1] + diffStep;  
				}

			}
		}
	}
}

void SamAtDelay::setDelayTimeTempo(float tempo)
{
	m_tempo = tempo;
	float ms = (60000.0f/m_tempo)*(m_quarterNotes/1.0f);
	setDelayTime(ms);
}

void SamAtDelay::setDelayTimeQuarterNotes(float quarterNotes)
{
	m_quarterNotes = quarterNotes;
	setDelayTimeTempo(m_tempo);
}
