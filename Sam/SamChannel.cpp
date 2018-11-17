#include "SamChannel.h"

#include "SamEngine.h"
#include "SamProject.h"

#include "AMemoryManager.h"

#include "AMath.h"

#include <math.h>

//#define USE_FW_C

#ifdef USE_FW_C
#include <fwSignal.h>
#endif

#define USE_IPP_C
#ifdef USE_IPP_C
#include <ipps.h>
#endif

//#define LEGATO_DEBUG

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamChannel::SamChannel(AModel *parent)
	: AModel(parent)
	, m_numVoices(0)
	, m_updateNumVoices(0)
	, m_numVoicesPlaying(0)
	, m_midiInputChannel(1)
	, m_midiInputPort(0)
	, m_selected(false)
	, m_positionInList(0)
	//, m_muted(false)
	, m_mute(false)
	, m_solo(false)
	, m_preSoloMuteState(false)
	, m_midiMuted(false)
	, m_articulationLock(false)
	, m_articulationMode(false)
	, m_downbeatOffset(false)
	, m_currentArticulationIndex(0)
	, m_currentVersionIndex(0)
	, m_currentDefaultArticulationIndex(0)
	, m_defaultCfArticulationIndex(0)
	, m_defaultCfDefaultArticulationIndex(0)
	, m_lastArticulationMidiValue(0)
	//, m_currentLayerIndex(0)
	
	, m_instrument(0)
	, m_version(0)
	, m_streamMix(0)
	//, m_layer(0)
	, m_sustain(0)
//	, m_streamCoefficients(0)
//	, m_numAllocatedSets(0)
//	, m_numAllocatedStreams(0)
	, m_averageVoiceTime(0.0f)
	, m_voiceTimeTotal(0.0f)
	, m_voicesRendered(0)
	, m_nanosPerSample(1.0f)
	, m_currentZoneCrossfade(0)
	, m_lastNoteOnTimeStamp(0.0f)
	, m_lastNoteOffTimeStamp(0.0f)
	, m_currentPitchBend(0.0f)
	, m_releaseCounter(0)
	, m_lastPitch(0)
	, m_currentPitch(-1)
	, m_numPitchesDown(0)
	, m_crossfadeUpdated(false)
	, m_filterCutOffUpdated(false)
	, m_layerMutesEnabled(false)
	, m_nextVoiceId(0)
	, m_pitchBend(false)
	, m_pitchBendUp(200)
	, m_pitchBendDown(200)
	//, m_perVoiceVelocitySplit(81)
	//, m_currentPerVoiceGroup(0)
	, m_audioToolEventBufferCount(0)
{
	//m_label = "Hello!";
	for(int i=0; i<maxVoices+overVoices; i++) {
		m_voices[i] = 0;
	}

	for(int i=0; i<128; i++) {
		m_lastCellId[i] = -1;
		m_currentLayerIndex[i] = 0;
		m_layerSelectMode[i] = SamArticulation::SingleLayer;
	}

	m_events = new ARingBuffer<SamEvent>(1024);

	/*for(int i=0; i < maxOutputs; i++) {
		m_audioBuffer[i] = 0;
	}*/

	m_pitchBendModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 4, false, 256, 0.0f);
	//m_zoneCrossfadeModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 256, 0.0f);
	//m_volumeModulator = new CsModulator(samEngine->audioProcessInfo().bufferSize, 32, 256, 1.0f);
	m_expressionModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 256, 1.0f);
	m_articulationCrossfadeModulator = new CsModulator(44100.0, samEngine->audioProcessInfo().bufferSize, 32, false, 256, 1.0f);

	m_filterCurve = new CsCurve(128, 5, 0.022f, 1.0f);
	m_filterCurve->setDepth(0.35);
	m_curveArticulationCrossfade = new CsCurve();

	//m_smoothVolume = new CsSmoothValue(512);

	m_voiceBuffer = new CsSampleBuffer();
	m_voiceBuffer->buffer32 = 0;
	m_voiceBuffer->numFrames = 0;
	m_voiceBuffer->numStreams = 0;

	// Test Tone
	m_osc = new CsOscillator(44100.0);
	//m_oscBuffer = new CsSampleBuffer();

	for(int i=0; i<64; i++) {
		for(int j=0; j<64; j++) {
			for(int k=0; k<16; k++) {
				m_streamCoefficients[i][j][k] = 0.0f;
			}
		}
	}

	for(int i=0; i<128; i++) {
		m_previousLayerIndex[i] = 0;
		m_previousLayerSelectMode[i] = 0;
		m_cfCellCounter[i] = 0;
		m_performanceControls[i] = new SamPerformanceControls(this);
	}

	

	for(int i=0; i<maxMusicTools; i++) {
		m_musicToolInserts[i] = new SamMusicToolInsert(this, i);
		m_musicToolInternals[i] = new SamMusicToolInsert(this, i);
	}

	for(int i=0; i<128; i++) {
		m_pitchCounter[i] = 0;

		//m_pitchTimestamp[i] = -1.0f;
		m_notes[i].timestamp = -1.0f;
		m_notes[i].velocity = 0;
		m_notes[i].articulationIndex = 0;
		m_notes[i].aftertouch = 0;

		for(int j=0; j<128; j++) {
			m_layerMute[i][j] = false;
		}

		m_lastControllerValue[i] = -1;
	}

	samEngine->registerListener(this, SamEngine::ContextArticulation);


	// att mode -- to clean up
	m_attenuationStart = 21;
	m_attenuationEnd = 21+88;
	m_attenuationFadeIn = 0;
	m_attenuationFadeOut = 0;

	m_curveAttenuationFadeIn = new CsCurve(128, CsCurve::Linear, m_attenuationFadeIn);
	m_curveAttenuationFadeIn->setDepth(0.5);
	m_curveAttenuationFadeOut = new CsCurve(128, CsCurve::Bezier, m_attenuationFadeOut);
	m_curveAttenuationFadeOut->setStart(1.0f);
	m_curveAttenuationFadeOut->setEnd(0.0f);
	m_curveAttenuationFadeOut->setDepth(0.5);

	updateAttenuationsForMidiNotes();

	for(int i=0; i<128; i++) {
		
		m_midiStreamMixGain[i] = pow((32041.0f/AMath::pow((float)i, 2.0f)),-1.0f);
		m_midiSendGain[i] = pow((1.0f/127.0f)*i, 4.0f);
		

	}

	/*for(int i=0; i<128; i++) {
		for(int j=0; j<2; j++) {
			m_perVoiceGroup[i][j] = 0;
		}
	}*/
}

SamChannel::~SamChannel()
{
	aLog("+++++++++++++++++++++++++++++++++++++++++++++++");
	aLog("++++++++++ SamChannel::~SamChannel() ++++++++++");
	aLog("+++++++++++++++++++++++++++++++++++++++++++++++");

	samEngine->unregisterListener(this, SamEngine::ContextArticulation);

	if(refCount() > 1) {
		aLog("********** CHANNEL refCount() > 1 **********");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::main(void)
//task* SamChannel::execute(void)
{//return;
	//aLog("Channel::main()");

	if(!m_instrument) return;

	/*if(samEngine->isPlaying()) {
		for(int i=0; i<maxMusicTools; i++) {
			if(m_musicToolInserts[i]->isAssigned()) {
				m_musicToolInserts[i]->processRange(samEngine->currentFromTime(), samEngine->currentToTime());
			}
		}
	}*/

	// Process events
	// --------------
	int numEvents = m_events->size();
	
//	m_voiceTimer.start();

	if(numEvents > 0) {
		//aLog("SamChannel numEvents = %d", numEvents);
		for(int i=0; i<numEvents; i++) {
			SamEvent ev = m_events->get();

			if(ev.timestamp > samEngine->currentTimestamp()) {
				//csDebug("ev->timestamp > timestamp");
				m_events->add(ev);
			} else {
				//double nanosPerSample = 44100.0/1000000000.0;
				double delta = (ev.timestamp - samEngine->previousTimestamp())* m_nanosPerSample;
				//csDebug("delta=%f", delta);

				ev.delta = (int)delta;
				//cDebug("delta=%d", ev->delta);

				if(ev.delta<0){
					//csDebug("ev->delta<0");
					ev.delta = 0;
				}
				if(ev.delta>=m_audioProcessInfo.bufferSize){
					//csDebug("ev->delta>=%d", m_audioProcessInfo.bufferSize);
					ev.delta = m_audioProcessInfo.bufferSize-1;
				}

				processEvent(&ev);
				//delete ev; // Do we delete here, or do we put onto garbage list?
			}
		}
	}//*/

	/*m_voiceTimer.stop();

	m_voiceTimeTotal += m_voiceTimer.duration();
	m_voicesRendered++;
	m_averageVoiceTime = (m_voiceTimeTotal/m_voicesRendered)*1000000.0f;//*/

	// Update the modulation buffers
	// -----------------------------
	
	bool pitchUpdatedRequired = true;//m_pitchBendModulator->isUpdateRequired();
	if(pitchUpdatedRequired) {
		m_pitchBendModulator->update();
	}
	/*if(m_pitchBendModulator->targetValue() == 0.0) {
		m_pitchBendModulator->debug();
	}*/
	
	
	//m_zoneCrossfadeModulator->update();
	//m_volumeModulator->update();
	m_expressionModulator->update();
	if(m_articulationMode) {
		m_articulationCrossfadeModulator->update();
	}
	

	for(int i=0; i<m_audioOutputBuses.size(); i++) {
		m_audioOutputBuses[i]->audioBuffer()->zeroAll();
	}//*/

	m_numVoicesPlayingTemp = 0;

	for(int i=0; i<128; i++) {
		m_pitchCounter[i] = 0;
	}
	m_releaseCounter = 0;

	//aLog("Voices");

	for(int i=0; i<m_numVoices+overVoices; i++) { //aLog("voice %d", i);
		if(m_voices[i]->isPlaying()) {
			//aLog("We're going to render voice[%d]", i);
			if(pitchUpdatedRequired) { //aLog("pitch update required");
				m_voices[i]->setPitchMustUpdate();
			}
			
			if(m_crossfadeUpdated) { //aLog("cf updated");
				
				if(!m_voices[i]->isReleasing()) {
					int cf = m_currentZoneCrossfade + m_notes[m_voices[i]->pitch()].aftertouch;
					if(m_voices[i]->isZoneCrossfade() && !m_voices[i]->layer()->isShortNoteMode()) {
						m_voices[i]->setZoneCrossfade(cf);
					}
					
				}
			}

	//		if(m_voices[i]->isFilterEnabled()) { // && m_filterCutOffUpdated) {// && m_crossfadeUpdated) {
	//			m_voices[i]->setFilterCutOff(m_performanceControls->filterCutOff());
	//		}

			if(m_voices[i]->isFilterEnabled() && m_filterCutOffUpdated && m_voices[i]->zone()->isFilterParameter()) {
				 m_voices[i]->setFilterCutOff(m_performanceControls[m_voices[i]->articulationIndex()]->filterCutOff());
			}

			//if(m_voices[i]->isZoneCrossfade() && !m_voices[i]->isReleasing() && m_currentZoneCrossfade != m_voices[i]->currentZoneCrossfadeMidiValue()) {
			//	m_voices[i]->setZoneCrossfade(m_currentZoneCrossfade);
			//}
			//if(m_voices[i]->isFilterEnabled()) {
			//	m_voices[i]->setFilterCutOff(m_currentZoneCrossfade);
			//}
#ifdef USE_VOICE_TIMER
			m_voiceTimer.start();
#endif
			
			if(m_voices[i]->render(m_voiceBuffer)) { //aLog("render trye");
				for(int j=0; j<m_voiceBuffer->numStreams; j++) { //aLog("stream %d", j);
					float *stream = m_audioOutputBuses[m_voices[i]->outputIndex()]->audioBuffer()->stream(j);
/*#ifdef USE_FW_C
					fwsAdd_32f_I(m_voiceBuffer->buffer32[j], stream, m_audioProcessInfo.bufferSize);
#else*/
#ifdef USE_IPP_C
					ippsAdd_32f_I(m_voiceBuffer->buffer32[j], stream, m_audioProcessInfo.bufferSize);
#else
					for (int k=0; k<m_audioProcessInfo.bufferSize; k++){
						//outputs[0]->buffer32[i][j] += m_voiceBuffer->buffer32[i][j];
						stream[k] += m_voiceBuffer->buffer32[j][k];
						//aLog("stream[%d] = %f", k, stream[k]);;
					}
#endif
				}
			}

#ifdef USE_VOICE_TIMER
			m_voiceTimer.stop();

			m_voiceTimeTotal += m_voiceTimer.duration();
			m_voicesRendered++;
			m_averageVoiceTime = (m_voiceTimeTotal/m_voicesRendered)*1000000.0f;
#endif
			
			m_numVoicesPlayingTemp++;
			if(!m_voices[i]->isRelease()) {
				//if(!m_voices[i]->isZoneCrossfade()) {
					m_pitchCounter[m_voices[i]->pitch()]++;
				//}
			} else {
				m_releaseCounter++;
			}
		}
	}//*/

	m_numVoicesPlaying = m_numVoicesPlayingTemp;

	m_crossfadeUpdated = false;
	m_filterCutOffUpdated = false;

	// Moved to AudioBus
	/*int sampleIndex = 0;
	for(int i=0; i<m_audioOutputBuses[0]->audioBuffer()->numFrames()/m_volumeModulator->period(); i++) {
		for(int j=0; j<m_audioOutputBuses[0]->audioBuffer()->numStreams(); j++) {
			float *stream = m_audioOutputBuses[0]->audioBuffer()->stream(j);
			for(int k=0; k<m_volumeModulator->period(); k++) {
				stream[sampleIndex+k] *= m_volumeModulator->buffer()[i];//m_modVolume->modulationBuffer()[i];
				//aLog("sample[%d][%d] * %f", j, sampleIndex+k, m_modVolume->modulationBuffer()[i]);
			}
		}
		sampleIndex += m_volumeModulator->period();
	}*/

	//aLog("avt = %f", m_averageVoiceTime);
	/*m_osc->renderBuffer(m_voiceBuffer);
	for(int i=0; i<m_voiceBuffer->numStreams; i++) {
				float *stream = m_audioOutputBuses[0]->audioBuffer()->stream(i);
				for (int j=0; j<m_audioProcessInfo.bufferSize; j++){
					//outputs[0]->buffer32[i][j] += m_voiceBuffer->buffer32[i][j];
					stream[j] += m_voiceBuffer->buffer32[i][j];
				}
			}//*/

	//aLog("About to process audio buses...");

	// Process Audio Buses
	// -------------------
	for(int i=0; i<m_audioOutputBuses.size(); i++) {
		//aLog("Processing audioOutputBus %d", i);

		for(int j=0; j<SamAudioBus::maxInserts; j++) {
			if(m_audioOutputBuses[i]->audioInsert(j)->hasMidiInput()  ||
			   m_audioOutputBuses[i]->audioInsert(j)->isActiveLearn() ||
			   m_audioOutputBuses[i]->audioInsert(j)->activeController() > -1) {

				m_audioOutputBuses[i]->audioInsert(j)->processEvents(m_audioToolEventBuffer, m_audioToolEventBufferCount);
			}
		}

		m_audioToolEventBufferCount = 0;
		
		m_audioOutputBuses[i]->main();
		//if(m_numVoicesPlaying > 0) aLog("[%d] rms = %f", i, m_audioOutputBuses[i]->audioBuffer()->rmsLevel(0));
		//aLog("Finished processing audioOutputBus %d", i);
	}

	if(m_numVoices != m_updateNumVoices) {
		aLog("Need to update number of voices...");
		allocateVoices(m_updateNumVoices);
	}

	//aLog("----------");

	//return NULL;
}

void SamChannel::onState(AStateEvent *ev)
{
	if(!m_instrument) return;

	switch(ev->item()) {
		case SamArticulation::ItemZoneCrossfadeResponseTime:
			//aLog("SamArticulation::ItemZoneCrossfadeResponseTime: %d", (44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneCrossfadeResponseTime());
			//m_zoneCrossfadeModulator->setRate((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneCrossfadeResponseTime());
			for(int i=0; i<m_numVoices+overVoices; i++) {
				//m_voices[i]->setZoneCrossfadeResponseTime(m_zoneCrossfadeModulator->rate());
				m_voices[i]->setZoneCrossfadeResponseTime((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneCrossfadeResponseTime());
			}
			break;
		case SamArticulation::ItemZoneFilterResponseTime:
			//aLog("SamArticulation::ItemZoneFilterResponseTime: %d", (44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneFilterResponseTime());
			for(int i=0; i<m_numVoices+overVoices; i++) {
				m_voices[i]->setZoneFilterResponseTime((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneFilterResponseTime());
			}
			break;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo)
{
	aLog("***** SamChannel::setAudioProcessInfo() %d", audioProcessInfo.bufferSize);

	m_audioProcessInfo = audioProcessInfo; 

	m_nanosPerSample = m_audioProcessInfo.sampleRate/1000000000.0;

	// Modulators
	m_pitchBendModulator->setBufferSize(m_audioProcessInfo.bufferSize);
	//m_zoneCrossfadeModulator->setBufferSize(m_audioProcessInfo.bufferSize);
	//m_volumeModulator->setSize(m_audioProcessInfo.bufferSize);
	m_expressionModulator->setBufferSize(m_audioProcessInfo.bufferSize);
	m_articulationCrossfadeModulator->setBufferSize(m_audioProcessInfo.bufferSize);

	if(!m_instrument) {
		return;
	}

	//if(m_voiceBuffer) {
	
		if(m_voiceBuffer->buffer32) {
			for(int i=0; i<m_voiceBuffer->numStreams; i++) {
				if(m_voiceBuffer->buffer32[i]) {
					free(m_voiceBuffer->buffer32[i]);
					//fwsFree(m_voiceBuffer->buffer32[i]);
				}
			}
			free(m_voiceBuffer->buffer32);
		}
	//}
	m_voiceBuffer->numStreams = m_streamMix->speakerConfig().numSpeakers();
	m_voiceBuffer->numFrames = m_audioProcessInfo.bufferSize;
	m_voiceBuffer->buffer32 = (float**)malloc(sizeof(float*)*m_voiceBuffer->numStreams);
	
	for(int i=0; i<m_voiceBuffer->numStreams; i++) {
		m_voiceBuffer->buffer32[i] = (float*)malloc(sizeof(float)*m_voiceBuffer->numFrames);
		//m_voiceBuffer->buffer32[i] = fwsMalloc_32f(m_voiceBuffer->numFrames);
	}

	for(int i=0; i<numAudioOutputs(); i++) {
		m_audioOutputBuses[i]->setAudioProcessInfo(audioProcessInfo);
		/*if(m_audioBuffer[i]) {
			delete m_audioBuffer[i];
			m_audioBuffer[i] = 0;
		}
		if(m_streamMix) {
			m_audioBuffer[i] = new Core::AudioBuffer(m_streamMix->speakerConfig().numSpeakers(), m_audioProcessInfo.bufferSize);
		}*/
	}

	

	// Test Tone
	/*if(m_streamMix) {
		m_osc->setSampleRate(audioProcessInfo.sampleRate);
		if(m_oscBuffer->buffer32) {
			coreFree(m_oscBuffer->buffer32, sizeof(float*)*m_streamMix->speakerConfig().numSpeakers());
		}
		m_oscBuffer->buffer32 = (float**)coreMalloc(sizeof(float*)*m_streamMix->speakerConfig().numSpeakers());
		m_oscBuffer->numStreams = m_streamMix->speakerConfig().numSpeakers();
		m_oscBuffer->numFrames = audioProcessInfo.bufferSize;
	}*/

	// Allocate Voices
	allocateVoices(m_numVoices);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::processMidiMessage(CsMidiMessage midiMessage)
{
	//aLog("SamChannel::processMidiMessage(%02x)", midiMessage.type);

	if(!m_instrument || m_midiMuted) return;

	SamEvent ev(SamEvent::Unassigned);
	ev.timestamp = midiMessage.timestamp;
	ev.articulationIndex = m_currentArticulationIndex; // These were originally in the notes section only
	ev.layerIndex = 0;                                 // but it made sense to put them here.

	switch(midiMessage.type) {
		case CsMidiMessage::NoteOff:
		case CsMidiMessage::NoteOn:
			ev.type = SamEvent::NoteOff;
			if(midiMessage.type==SamEvent::NoteOn && midiMessage.data[2] > 0) {
				ev.type = SamEvent::NoteOn;
			}
			
			ev.midiValue1 = midiMessage.data[1]; //aLog("ev.midiValue1 = %d", ev.midiValue1);
			ev.midiValue2 = midiMessage.data[2];

			
			
			//stateManager()->updateState(Engine::ContextKeyboard,ev->value1,this,(float)ev->value2);
			break;

		case CsMidiMessage::Aftertouch:
			{
				//aLog("CsMidiMessage::Aftertouch: %d %d", midiMessage.data[1], midiMessage.data[2]);
				SamArticulation *articulation = m_version->articulation(m_currentArticulationIndex);
				if(articulation->polyphonicModulation()) {
					ev.type = SamEvent::Aftertouch;
					ev.midiValue1 = midiMessage.data[1];
					ev.midiValue2 = midiMessage.data[2];
				}
			}
			break;

		case CsMidiMessage::Controller:
		//	if(!samEngine->isMidiControllerFloodgate() || (m_lastControllerValue[midiMessage.data[1]] != midiMessage.data[2])) {
				ev.type = SamEvent::Controller;
				
				ev.midiValue1 = midiMessage.data[1];
				ev.midiValue2 = midiMessage.data[2];

		//		m_lastControllerValue[midiMessage.data[1]] = midiMessage.data[2];
		//	} /*else {
		//		aLog("Blocking Controller %d with value %d", midiMessage.data[1], midiMessage.data[2]);
		//	}*/
			break;

		case CsMidiMessage::ProgramChange:
			ev.type    = SamEvent::Parameter;
			if( midiMessage.data[1] == 127) {
				ev.subType = SamParameter::DefaultCfArticulation;
			} else {
				ev.subType = SamParameter::Articulation;
				ev.iValue  = midiMessage.data[1];
			}
			break;

		case CsMidiMessage::PitchBend:
			ev.type = SamEvent::PitchBend;
			
			ev.midiValue1 = midiMessage.data[1];
			ev.midiValue2 = midiMessage.data[2];
			
			unsigned short value;
			value = (unsigned short)midiMessage.data[2];
			value <<= 7;
			value |= (unsigned short)midiMessage.data[1];

			ev.fValue = value * (2.0/16383.0) - 1.0f;
			if(value == 8192) ev.fValue = 0.0f;
			//aLog("CsMidiMessage::PitchBend %d -> %f", value, ev.fValue);
		
	}
	
	if(ev.type != SamEvent::Unassigned) {
		//m_events->add(ev);
		addEvent(ev);
	} /*else {
		aLog("Unassigned");
	}*/

	updateState(SamEngine::ContextChannel, ItemMidiInput, this, 0, (float)(midiMessage.data[2]*65536)+(midiMessage.data[1]*256)+(midiMessage.data[0]));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::processMidiEvent(CsMidiMessage midiMessage)
{
	SamEvent *ev = 0;

	if(midiMessage.type == CsMidiMessage::NoteOff || midiMessage.type == CsMidiMessage::NoteOn) {
		ev = new SamEvent(SamEvent::NoteOff);
		if(midiMessage.type==SamEvent::NoteOn && midiMessage.data[2] > 0) {
			ev->type = SamEvent::NoteOn;
		}
		ev->timestamp = midiMessage.timestamp;
		ev->midiValue1 = midiMessage.data[1];
		ev->midiValue2 = midiMessage.data[2];
		
		//stateManager()->updateState(Engine::ContextKeyboard,ev->value1,this,(float)ev->value2);
	} else if(midiMessage.type == CsMidiMessage::Controller) {
		ev = new SamEvent(SamEvent::Controller);
		ev->timestamp = midiMessage.timestamp;	
		ev->midiValue1 = midiMessage.data[1];
		ev->midiValue2 = midiMessage.data[2];
	} else if(midiMessage.type == CsMidiMessage::PitchBend) {
		ev = new SamEvent(SamEvent::PitchBend);
		ev->timestamp = midiMessage.timestamp;
		ev->midiValue1 = midiMessage.data[1];
		ev->midiValue2 = midiMessage.data[2];
		
		unsigned short value;
		value = (unsigned short)midiMessage.data[2];
		value <<= 7;
		value |= (unsigned short)midiMessage.data[1];

		ev->fValue = value * (2.0/16384.0) - 1.0f;
	}
	
	if(ev) {
		m_events->add(ev);
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::processEvent(SamEvent *ev)
{
	if(!m_instrument) return;

	//aLog("SamChannel::processEvent()");

	if(ev->type == SamEvent::NoteOn     ||
	   ev->type == SamEvent::NoteOff    ||
	   ev->type == SamEvent::Aftertouch ||
	   ev->type == SamEvent::Controller ||
	   ev->type == SamEvent::PitchBend) {
		m_audioToolEventBuffer[m_audioToolEventBufferCount++] = *ev;
	}

	switch(ev->type) {
		case SamEvent::Parameter:
			//cDebug("SamEvent::Parameter:");
			doParameter(ev);
			break;
		case SamEvent::NoteOn:
			//aLog("SamEvent::NoteOn:");
			m_notes[ev->midiValue1].aftertouch = 0;
			//if(!m_midiMuted) {
				doNoteOn(ev);
			//}
			break;
		case SamEvent::NoteOff:
			//cDebug("SamEvent::NoteOff:");
			doNoteOff(ev);
			break;
		case SamEvent::Aftertouch:
			m_notes[ev->midiValue1].aftertouch = ev->midiValue2;
			m_crossfadeUpdated = true;
			break;
		case SamEvent::Controller:
			//cDebug("SamEvent::Controller");
			doController(ev);
			break;
		case SamEvent::Program:
			doProgram(ev);
			break;
		case SamEvent::PitchBend:
			//aLog("SamEvent::PitchBend %f ",ev->fValue);
			//m_pitchBend = ev->value;
			m_pitchBendModulator->setTargetValue(ev->fValue);
			m_currentPitchBend = ev->fValue;
			break;
		case SamEvent::Tempo:
			/*CsAudioToolEvent atEvent;
			atEvent.type = CsAudioToolEvent::kTempo;
			atEvent.value = samEvent->floatValue;
			for(int i=0; i<m_outputs; i++) {
				m_audioToolInsert[i]->processEvent(samEvent->pitch, &atEvent);
			}//*/
			break;
		case SamEvent::VoicePitch:
			//aLog("SamEvent::VoicePitch %d %f", ev->iValue, ev->fValue);
			for(int i=0; i<m_numVoices; i++) {
				if(m_voices[i]->id() == ev->iValue) {
					m_voices[i]->setPitchModulation(ev->fValue);
				}
			}
			break;
		
	}

	// Check if the Event is mapped to a Parameter
	// -------------------------------------------

	bool parameterHandled = false;
	int oldType = 0;

	SamArticulation *articulation = m_version->articulation(m_currentArticulationIndex);

	//aLog("numParameters = %d", articulation->numParameters());
	for(int i=0; i<articulation->numParameters(); i++) {
		//aLog("parameter %d", i);
		int midiType = articulation->parameter(i)->midiType();
		switch(midiType) {
			case SamParameter::spmtNotePitch:
				//qDebug("SamParameter::spmtNotePitch");
				if(ev->type == SamEvent::NoteOn) {
					if(ev->midiValue1 == articulation->parameter(i)->midiValue()) {
						ev->subType = articulation->parameter(i)->source();
						if(articulation->parameter(i)->type() == SamParameter::sptTrigger) {
							ev->iValue = articulation->parameter(i)->triggerValue();
							parameterHandled = true;
						}
					}
				}
				break;
			case SamParameter::spmtNoteVelocity:
				/*if(midiEvent.type == CsMidiEvent::meNoteOn) {
				}*/
				break;
			case SamParameter::spmtController:
				//aLog("SamParameter::spmtController:");
				//qDebug("SamParameter::spmtController: [1]%d [2]%d", midiEvent.data[1], midiEvent.data[2]);
				//m_version->articulation(m_currentArticulation)->parameter(i)->debug();
				if(ev->type == SamEvent::Controller) {
					if(ev->midiValue1 == articulation->parameter(i)->midiController()) {// && midiEvent.data[2] == m_version->articulation(m_currentArticulation)->parameter(i)->midiValue()) {
						
						switch(articulation->parameter(i)->type()) {
							case SamParameter::sptTrigger:
								if(ev->midiValue2 == articulation->parameter(i)->midiValue()) {
									ev->subType = articulation->parameter(i)->source();
									ev->iValue = articulation->parameter(i)->triggerValue();
									parameterHandled = true;
								}
								break;
							case SamParameter::sptRange:
								ev->subType = articulation->parameter(i)->source();
								ev->iValue = articulation->parameter(i)->sourceControl();
								int v = ev->midiValue2;
								if(articulation->parameter(i)->isInverted()) {
									v = 127-v;
								}
								ev->fValue = articulation->parameter(i)->curve()->point(v);
								parameterHandled = true;
								break;
						}
					}
				}
				break;
		}

		if(parameterHandled) {
			oldType = ev->type;
			ev->type = SamEvent::Parameter;
			doParameter(ev);
			parameterHandled = false;
			ev->type = oldType;
		}
	}

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::addEvent(SamEvent ev)
{
	if(!m_instrument) return;

	//aLog("Add event");
	if(ev.type != SamEvent::Parameter) { //aLog("1");

		// Process the internal Music Tools ...
		/*for(int i=0; i<maxMusicTools; i++) { //aLog("2");
			if(m_musicToolInternals[i]->isAssigned()) {
				m_musicToolInternals[i]->processEvent(&ev);
			}
		}*/

		// Then process the Channel Music Tools
		for(int i=0; i<maxMusicTools; i++) { //aLog("2");
			if(m_musicToolInserts[i]->isAssigned()) {
				m_musicToolInserts[i]->processEvent(&ev);
			}
		}
	}

	m_events->add(ev);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::queueEvent(SamEvent ev)
{
	if(!m_instrument) return;

	m_events->add(ev);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::doNoteOn(SamEvent *ev)
{
	//aLog("SamChannel::doNoteOn(pitch = %d)", ev->midiValue1);
	//int pitch = ev->midiValue1;
	//int velocity = ev->midiValue2;

	/*if(m_performanceControls->minimumVelocity() != 0) {
		int range = 127 - m_performanceControls->minimumVelocity();
		float c = (float)ev->midiValue2 * 0.00787402;
		c *= (float)range;
		ev->midiValue2 = m_performanceControls->minimumVelocity()+(int)c;
	}*/

	m_numPitchesDown++;

#ifdef LEGATO_DEBUG
	if(m_notes[ev->midiValue1].velocity > 0) {
		aLog("***** Note %d already triggered *****", ev->midiValue1);
	}
#endif

	m_notes[ev->midiValue1].timestamp = ev->timestamp;
	m_notes[ev->midiValue1].velocity  = ev->midiValue2;
	

	ev->triggeredPitch = ev->midiValue1;

	if(m_articulationMode) {
		ev->articulationIndex = m_currentDefaultArticulationIndex; //m_version->defaultArticulation();
		playNote(ev);
		if(m_currentArticulationIndex > -1 && m_currentArticulationIndex != m_currentDefaultArticulationIndex) { // m_version->defaultArticulation()
			SamEvent ev_duplicate(*ev);
			ev_duplicate.articulationIndex = m_currentArticulationIndex;
			playNote(&ev_duplicate);
		}
	} else {
		if(ev->articulationIndex > -1) {
			playNote(ev);
		}
	}

	m_notes[ev->midiValue1].articulationIndex = ev->articulationIndex;

	//m_lastPitch = ev->midiValue1;

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::playNote(SamEvent *ev)
{
	//aLog("SamChannel::playNote()");

	int artIndex = ev->articulationIndex;
	SamArticulation *articulation = m_version->articulation(artIndex);
	

	// Calculate the Layer to play
	// ---------------------------
	//aLog("articulation->switchLayerTime() = %d", articulation->switchLayerTime());
	if(m_layerSelectMode[artIndex] == SamArticulation::CycleLayers && ev->timestamp >= m_lastNoteOnTimeStamp+(double)((double)1000000.0*(double)articulation->switchLayerTime())) { //qDebug("1");
		m_currentLayerIndex[artIndex]++;
	}
	if(m_currentLayerIndex[artIndex] >= articulation->numLayers()) { //qDebug("4");
		m_currentLayerIndex[artIndex] = 0;
	}
	//aLog("m_currentLayerIndex = %d", m_currentLayerIndex);
	//SamLayer *layer = articulation->layer(m_currentLayerIndex);

	switch(m_layerSelectMode[artIndex]) {
		case SamArticulation::SingleLayer:
		case SamArticulation::CycleLayers:
			playNoteOnLayer(ev, m_currentLayerIndex[artIndex]);
			break;
		case SamArticulation::AllLayers:
			for(int i=0; i<articulation->numLayers(); i++) {
				playNoteOnLayer(ev, i);
			}
			break;
		case SamArticulation::Legato:
			
			int interval = ev->midiValue1-m_lastPitch;
#ifdef LEGATO_DEBUG
			aLog("m_numPitchesDown = %d (m_lastPitch = %d, interval = %d)", m_numPitchesDown, m_lastPitch, ev->midiValue1-m_lastPitch);
#endif
			
			int originalPitch = ev->midiValue1;
			int originalVelocity = ev->midiValue2;
			for(int i=0; i<m_numVoices+overVoices; i++) {
				if(m_voices[i]->isPlaying() && !m_voices[i]->isReleasing()) { //tp was pitch
					ev->midiValue1 = m_voices[i]->pitch();
					//aLog("Before: ev->midiValue2 = %d", ev->midiValue2);
					voiceOff(i, ev);
					//aLog("After: ev->midiValue2 = %d", ev->midiValue2);
				}
			}
			ev->midiValue1 = originalPitch;
			ev->midiValue2 = originalVelocity;
			//aLog("After-After: ev->midiValue2 = %d", ev->midiValue2);

			if(m_numPitchesDown > 1) {
#ifdef LEGATO_DEBUG
				aLog("m_numPitchesDown > 1");
#endif
				for(int i=0; i<articulation->numLayers(); i++) {
#ifdef LEGATO_DEBUG
					aLog("Layer %d/%d il=%d, ih=%d", i+1, articulation->numLayers(), articulation->layer(i)->intervalLow(), articulation->layer(i)->intervalHigh());
#endif
					if(articulation->layer(i)->intervalRecorded() != 0) {
						if(articulation->layer(i)->intervalHigh() > articulation->layer(i)->intervalLow()) {
							if(interval >= articulation->layer(i)->intervalLow() && interval <= articulation->layer(i)->intervalHigh()) {
#ifdef LEGATO_DEBUG
								aLog("Play");
#endif
								playNoteOnLayer(ev, i);
							}
						} else {
							if(interval >= articulation->layer(i)->intervalHigh() && interval <= articulation->layer(i)->intervalLow()) { 
#ifdef LEGATO_DEBUG
								aLog("Play (Reversed)");
#endif
								playNoteOnLayer(ev, i);
							}
						}
					}
				}
			} else {
#ifdef LEGATO_DEBUG
				aLog("First pitch");
#endif
				for(int i=0; i<articulation->numLayers(); i++) {
					if(articulation->layer(i)->intervalRecorded() == 0) { 
						playNoteOnLayer(ev, i);
					}
				}
			}
			break;
	}

	/*if(m_layerSelectMode < SamArticulation::AllLayers) {
		playNoteOnLayer(ev, articulation->layer(m_currentLayerIndex));
	} else {
		for(int i=0; i<articulation->numLayers(); i++) {
			playNoteOnLayer(ev, articulation->layer(i));
		}
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::playNoteOnLayer(SamEvent *ev, int layerIndex)
{
	//aLog("SamChannel::playNoteOnLayer()");

	SamArticulation *articulation = m_version->articulation(ev->articulationIndex);
	SamLayer *layer = articulation->layer(layerIndex);

#ifdef LEGATO_DEBUG
	aLog("SamChannel::playNoteOnLayer(%s) pitch = %d", layer->name().toChar(), ev->midiValue1);
#endif

	if(m_layerMutesEnabled && m_layerMute[ev->articulationIndex][layerIndex]) {
		return;
	}

	if(ev->midiValue1 < layer->midiPitchLow() || ev->midiValue1 > layer->midiPitchHigh()) {
		return;
	}

	if(ev->midiValue2 < layer->midiVelocityLow() || ev->midiValue2 > layer->midiVelocityHigh()) {
		return;
	}

	if(layer->pedalMode() > 0) {
		if(m_sustain && layer->pedalMode() != 2) {
			return;
		} else if(!m_sustain && layer->pedalMode() !=1) {
			return;
		}
	}

	double timeFromLastOff = (ev->timestamp - m_lastNoteOffTimeStamp)/1000000.0f;
	//aLog("timeFromLastOff = %f", timeFromLastOff);

	if(layer->timeFromLastNoteMin() != layer->timeFromLastNoteMax()) {// && m_lastNoteOffTimeStamp > 0.0f) {
		if(layer->isTimeFromLastNoteMaxInf()) {
			if(ev->timestamp >= m_lastNoteOffTimeStamp+((double)layer->timeFromLastNoteMin()*1000000.0f)) {
				// In range, so we need to play
			} else {
				// Out of range, so we return
				//aLog("Infinity: out of range");
				return;
			}
		} else {
			if(ev->timestamp >= m_lastNoteOffTimeStamp+((double)layer->timeFromLastNoteMin()*1000000.0f) && ev->timestamp <= m_lastNoteOffTimeStamp+((double)layer->timeFromLastNoteMax()*1000000.0f)) {
				// In range, so we need to play
			} else {
				// Out of range, so we return
				//aLog("Range: out of range [%f] (%f, %f)", ev->timestamp, m_lastNoteOffTimeStamp+((double)layer->timeFromLastNoteMin()*1000000.0f), m_lastNoteOffTimeStamp+((double)layer->timeFromLastNoteMax()*1000000.0f));
				return;
			}
		}
	}

	//aLog("Playing layer: %s", layer->name().toChar());

	m_currentPitch = ev->midiValue1;

	int aliasNote = layer->aliasNote(ev->midiValue1);
	//aLog("aliasNote = %d, ev->midiValue1 = %d
	if(aliasNote>-1) {
		ev->midiValue1 = aliasNote;
	}

	if(layer->zoneSelectMode() == 0 || layer->zoneSelectMode() == 2) { // Select one zone via velocity
		//aLog("zoneSelectMode() == 0");
		
		if(layer->zoneSelectMode() == 2) {
			ev->midiValue2 = m_currentZoneCrossfade;
			if(ev->midiValue2 < 1) {
				ev->midiValue2 = 1;
			}
			//aLog("ev->midiValue2 = %d", ev->midiValue2);
		}

		//for(int i=0; i<layer->numZones(); i++) {

		//APrecisionTimer lookupTime;
		//lookupTime.start();
		SamZone *zone = layer->getZone(ev->midiValue1, ev->midiValue2);
		//lookupTime.stop();
		//aLog("getZone() time = %f", lookupTime.duration());
		if(zone) {
		//if(layer->zone(i)->isMidiNoteInRange(ev->midiValue1) && layer->zone(i)->isMidiVelocityInRange(ev->midiValue2)) {
		//	SamZone *zone = layer->zone(i);
			SamVoice *voice = nextVoice(layer, ev->midiValue1);
			if(voice) {
				if(zone->numCells() > 0) { //aLog("Assign");
					voice->assign(++m_nextVoiceId, ev, zone->cell(nextCellId(zone, ev->midiValue2)), false);
				}
			} else {
				//aLog("Voice could not be allocated.");
			}
		} else {
			//aLog("Zone could not be found.");
		}

		//}
	} else if (layer->zoneSelectMode() == 1) { // Select all zones for a single pitch and crossfade
		//aLog("zoneSelectMode() == 1");

		int layerIndex = ((SamArticulation*)layer->parent())->layerIndex(layer);
		
		//int cellId = nextCellId(layer->zone(0), ev->midiValue2); //aLog("cellId = %d, numCells = %d", cellId, layer->zone(0)->numCells());
		//aLog("m_cfCellCounter = %d, max = %d", m_cfCellCounter[layerIndex], layer->maximumCells());
		
		SamZone *zone = 0;
		for(AListIterator<SamZone*>item = layer->zones()->begin(); item != layer->zones()->end(); item++) {
			zone = *item;
			if(ev->midiValue1 >= zone->midiNoteLow() && ev->midiValue1 <= zone->midiNoteHigh()) {
				//aLog("   %d (%d,%d)", ev->midiValue1, layer->zone(i)->midiNoteLow(), layer->zone(i)->midiNoteHigh());
				SamVoice *voice = nextVoice(layer, ev->midiValue1);
				if(voice) {
					if(zone->numCells() > 0) {
						//voice->assign(ev, layer->zone(i)->cell(nextCellId(layer->zone(i), ev->midiValue2)), false);
						if(m_cfCellCounter[layerIndex] < zone->numCells()) {
							voice->assign(++m_nextVoiceId, ev, zone->cell(m_cfCellCounter[layerIndex]), false);
							//aLog("triggered");
						} else {
							voice->assign(++m_nextVoiceId, ev, zone->cell(0), false);
							//aLog("triggered 0");
						}
					}
				} else {
					//aLog("Voice could not be allocated for crossfade.");
				}
			}
		}

		/*for(int i=0; i<layer->numZones(); i++) {
			//aLog("[%d/%d] %d (%d,%d)", i+1, layer->numZones(), layer->zone(i)->midiNoteLow(), layer->zone(i)->midiNoteHigh());
			if(ev->midiValue1 >= layer->zone(i)->midiNoteLow() && ev->midiValue1 <= layer->zone(i)->midiNoteHigh()) {
				//aLog("   %d (%d,%d)", ev->midiValue1, layer->zone(i)->midiNoteLow(), layer->zone(i)->midiNoteHigh());
				SamVoice *voice = nextVoice(layer, ev->midiValue1);
				if(voice) {
					if(layer->zone(i)->numCells() > 0) {
						//voice->assign(ev, layer->zone(i)->cell(nextCellId(layer->zone(i), ev->midiValue2)), false);
						if(m_cfCellCounter[layerIndex] < layer->zone(i)->numCells()) {
							voice->assign(ev, layer->zone(i)->cell(m_cfCellCounter[layerIndex]), false);
							//aLog("triggered");
						} else {
							voice->assign(ev, layer->zone(i)->cell(0), false);
							//aLog("triggered 0");
						}
					}
				} else {
					//aLog("Voice could not be allocated for crossfade.");
				}
			}
		}*/

		m_cfCellCounter[layerIndex]++;
		if(m_cfCellCounter[layerIndex] >= layer->maximumCells()) {
			m_cfCellCounter[layerIndex] = 0;
		}
	}

	m_lastNoteOnTimeStamp = ev->timestamp;

	if(m_nextVoiceId > 32768) {
		m_nextVoiceId = 0;
	}

#ifdef LEGATO_DEBUG
	aLog("ev->midiValue1 = %d, ev->triggeredPitch = %d", ev->midiValue1, ev->triggeredPitch);
#endif
	ev->midiValue1 = ev->triggeredPitch; // Return this to the original pitch, because otherwise there will be problems when triggering multiple Layers/Articulations
	m_lastPitch = ev->midiValue1;
#ifdef LEGATO_DEBUG
	aLog("***** m_lastPitch = %d *****", m_lastPitch);
#endif
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::doNoteOff(SamEvent *ev)
{
	m_lastNoteOffTimeStamp = ev->timestamp;

	m_numPitchesDown--;
	if(m_numPitchesDown < 0) {
#ifdef LEGATO_DEBUG
		aLog("***** m_numPitchesDown < 0 (%d) *****", m_numPitchesDown);
#endif
		m_numPitchesDown = 0;
	}
	m_notes[ev->midiValue1].timestamp = -1.0f;

	ev->triggeredPitch = ev->midiValue1;

#ifdef LEGATO_DEBUG
	aLog("SamChannel::doNoteOff(%d)", ev->midiValue1);
#endif
	

	if(m_layerSelectMode[ev->articulationIndex] == SamArticulation::Legato) {
		
		//SamArticulation *articulation = m_version->articulation(ev->articulationIndex);
		

		//int originalPitch = ev->midiValue1;
			for(int i=0; i<m_numVoices+overVoices; i++) {
				if(m_voices[i]->isPlaying() && !m_voices[i]->isReleasing() && m_voices[i]->pitch() == ev->midiValue1) { //tp was pitch
					//ev->midiValue1 = m_voices[i]->pitch();
					voiceOff(i, ev); // Claudius problem: ev->m2 (velocity) is being changed here.
				}
			}
			//ev->midiValue1 = originalPitch;

		if(m_numPitchesDown > 0) {
#ifdef LEGATO_DEBUG
			aLog("Need to retrigger a note.");
#endif
			int nextPitch = -1;
			for(int i=0; i<128; i++) {
				if(m_notes[i].timestamp > 0.0f) {
					if(nextPitch < 0) {
						nextPitch = i;
					} else {
						if(m_notes[i].timestamp > m_notes[nextPitch].timestamp) {
							nextPitch = i;
						}
					}
				}
			}
			int interval = nextPitch-m_currentPitch;
			if(nextPitch > -1 && interval != 0) {
				SamArticulation *articulation = m_version->articulation(m_notes[nextPitch].articulationIndex);
				
				int originalPitch = ev->midiValue1;
				int originalVelocity = ev->midiValue2;
#ifdef LEGATO_DEBUG
				aLog("nextPitch = %d (m_currentPitch = %d, interval = %d)", nextPitch, m_currentPitch, interval);
#endif
				for(int i=0; i<articulation->numLayers(); i++) {
#ifdef LEGATO_DEBUG
					aLog("Layer %d/%d il=%d, ih=%d", i+1, articulation->numLayers(), articulation->layer(i)->intervalLow(), articulation->layer(i)->intervalHigh());
#endif
					if(articulation->layer(i)->intervalRecorded() != 0) {
						ev->midiValue1 = nextPitch;
						ev->triggeredPitch = nextPitch;
						ev->midiValue2 = m_notes[nextPitch].velocity;
						if(articulation->layer(i)->intervalHigh() > articulation->layer(i)->intervalLow()) {
							if(interval >= articulation->layer(i)->intervalLow() && interval <= articulation->layer(i)->intervalHigh()) { 
#ifdef LEGATO_DEBUG
								aLog("Play");
#endif
								playNoteOnLayer(ev, i);
							}
						} else {
							if(interval >= articulation->layer(i)->intervalHigh() && interval <= articulation->layer(i)->intervalLow()) { 
#ifdef LEGATO_DEBUG
								aLog("Play (Reversed)");
#endif
								playNoteOnLayer(ev, i);
							}
						}
					}
				}
				ev->midiValue1 = originalPitch;
				ev->midiValue2 = originalVelocity;
				ev->triggeredPitch = originalPitch;

			}
		} else {
			/*int originalPitch = ev->midiValue1;
			for(int i=0; i<m_numVoices+overVoices; i++) {
				ev->midiValue1 = m_voices[i]->pitch();
				voiceOff(i, ev);
			}
			ev->midiValue1 = originalPitch;*/
		}
	} else {

		// Not Legato Mode (Original Behaviour)
		// ------------------------------------
		for (int i=0; i<m_numVoices+overVoices; i++) {
//#ifdef LEGATO_DEBUG
			//aLog("i=%d/%d playing=%d (triggeredPitch = %d[%d])", i, m_numVoices-1, m_voices[i]->isPlaying(), m_voices[i]->triggeredPitch(), ev->midiValue1);
//#endif
			if(m_voices[i]->isPlaying() && m_voices[i]->triggeredPitch() == ev->midiValue1 && !m_voices[i]->isReleasing()) { //tp was pitch
				//aLog("Here");
				if(m_sustain){
					//aLog("Sustain");
					m_voices[i]->setSustained();
				} else {
					voiceOff(i, ev);
				}
			}
		}
	}

	m_notes[ev->triggeredPitch].velocity = 0;

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::voiceOff(int voiceIndex, SamEvent *ev) 
{
//#ifdef LEGATO_DEBUG
	//aLog("SamChannel::voiceOff(%d)", voiceIndex);
//#endif

	int aliasNote =  m_version->articulation(m_voices[voiceIndex]->articulationIndex())->layer(m_voices[voiceIndex]->layerIndex())->aliasNote(ev->midiValue1);
	if(aliasNote>-1) {
		ev->midiValue1 = aliasNote;
	}

	bool shouldTrigger = true;
	if(m_voices[voiceIndex]->isZoneCrossfade() && m_voices[voiceIndex]->isZoneCrossfadeSilent()) {
		//aLog("  > No trigger %d && %d", (int)m_voices[voiceIndex]->isZoneCrossfade(), (int)m_voices[voiceIndex]->isZoneCrossfadeSilent());
		shouldTrigger = false;
	}

	if(m_voices[voiceIndex]->cell()->hasRelease() && shouldTrigger) {

		if(m_articulationMode) {
			ev->articulationIndex = m_voices[voiceIndex]->articulationIndex();
		}

		float releaseGain = m_voices[voiceIndex]->zone()->releaseGainLinear();
		//aLog("releaseGain = %f", releaseGain);
		if(m_voices[voiceIndex]->framePosition() < m_voices[voiceIndex]->cell()->loopStart()) {
			releaseGain *= m_voices[voiceIndex]->zone()->preLoopReleaseGainLinear();
			//aLog("     preLoop > releaseGain = %f", releaseGain);
		}

		if(m_voices[voiceIndex]->cell()->isSourceRelease()) {
			//aLog("Source is release.");
			//aLog("%d + %d < %d", m_voices[voiceIndex]->framePosition(), ev->delta, m_voices[voiceIndex]->cell()->releaseStart());
			if(m_voices[voiceIndex]->framePosition()+ev->delta < m_voices[voiceIndex]->cell()->releaseStart()) {
				SamVoice *voice = nextReleaseVoice(m_voices[voiceIndex]->layer(), m_voices[voiceIndex]->pitch());
				if(voice) {
					//aLog("Playing release...");
					ev->midiValue2 = m_voices[voiceIndex]->velocity();
					voice->assign(++m_nextVoiceId, ev, m_voices[voiceIndex]->cell(), true, releaseGain, m_voices[voiceIndex]->lastCutOff());
				} else {
					//aLog("Release voice could not be allocated.");
				}	
			} else {
				// Set release-in-place
				m_voices[voiceIndex]->setReleaseInPlace();
			}
		} else {
			SamVoice *voice = nextReleaseVoice(m_voices[voiceIndex]->layer(), m_voices[voiceIndex]->pitch());
			if(voice) {
				//aLog("Playing release...");
				ev->midiValue2 = m_voices[voiceIndex]->velocity();
				voice->assign(++m_nextVoiceId, ev, m_voices[voiceIndex]->cell(), true, releaseGain, m_voices[voiceIndex]->lastCutOff());
			} else {
				//aLog("Release voice could not be allocated.");
			}
		}
	}/* else {
		aLog("No release.");
	}*/

	if(!m_voices[voiceIndex]->isReleasing()) {
		//aLog("Ending...");
		m_voices[voiceIndex]->end(ev->delta);
	}

	if(m_nextVoiceId > 32768) {
		m_nextVoiceId = 0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::doNoteOff(SamEvent *ev)
{
	//aLog("SamChannel::doNoteOff(%d)", ev->midiValue1);
	for (int i=0; i<m_numVoices; i++) {
		//aLog("i=%d/%d", i, m_numVoices);
		if(m_voices[i]->isPlaying() && m_voices[i]->pitch() == ev->midiValue1 && !m_voices[i]->isReleasing()) {
			if(m_sustain){
				m_voices[i]->setSustained();
			} else {
				if(m_voices[i]->cell()->isSourceRelease()) {
					//aLog("Source is release.");
					//aLog("%d + %d < %d", m_voices[i]->framePosition(), ev->delta, m_voices[i]->cell()->releaseStart());
					if(m_voices[i]->framePosition()+ev->delta < m_voices[i]->cell()->releaseStart()) {
						SamVoice *voice = nextVoice(m_voices[i]->pitch());
						if(voice) {
							aLog("Playing release...");
							ev->midiValue2 = m_voices[i]->velocity();
							voice->assign(ev, m_voices[i]->cell(), true);
						} else {
							aLog("Release voice could not be allocated.");
						}	
					} else {
						// Set release-in-place
						m_voices[i]->setReleaseInPlace();
					}
				} 
				if(!m_voices[i]->isReleasing()) {
					m_voices[i]->end(ev->delta);
				}
			}
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::doController(SamEvent *ev)
{
	// Core controllers are hard-wired, but it would be nice to make them assignable at some point...

	if(samEngine->isChannelPitchBendRemoteActive() && ev->midiValue1 == samEngine->channelPitchBendRemoteController()) {
		if(!m_pitchBend && samEngine->isChannelPitchBendRemoteActiveOnMidi()) {
			setPitchBend(true);
		}
		if(samEngine->isChannelPitchBendRemoteActiveOnMidi() && ev->midiValue2 == 127) {
			setPitchBend(false);
		} else {
			setPitchBendUp(samEngine->channelPitchBendRemoteUp(ev->midiValue2));
			setPitchBendDown(samEngine->channelPitchBendRemoteDown(ev->midiValue2));
		}
	}
	
	switch(ev->midiValue1) {
		case 1: // Modulation
			//m_zoneCrossfadeModulator->setTargetValue((1.0f/127.0f)*ev->midiValue2);
			m_currentZoneCrossfade = ev->midiValue2;
			m_crossfadeUpdated = true;
			return;
			break;
		case 7: // Volume
			//m_modVolume->addValue(255, samEngine->midiVolumeToGain(ev->midiValue2));
			//m_smoothVolume->updateValue(samEngine->midiVolumeToGain(ev->midiValue2), ev->delta);
			//m_volumeModulator->setTargetValue(samEngine->midiVolumeToGain(ev->midiValue2));
			for(int i=0; i<m_audioOutputBuses.size(); i++) { 
				m_audioOutputBuses[i]->setTargetVolume(samEngine->midiVolumeToGain(ev->midiValue2));
			}
			//handled = true;
			return;
			break;
		case 11: // Expression
			m_expressionModulator->setTargetValue(samEngine->expressionCurve()->point(ev->midiValue2));
			return;
			break;
		case 64: // Sustain
			if(m_layerSelectMode[m_currentArticulationIndex] != SamArticulation::Legato) {
				if(ev->midiValue2 == 0) {
					
					int m1 = ev->midiValue1;
					int m2 = ev->midiValue2;

					m_sustain = false;
					
						for (int i=0; i<m_numVoices+overVoices; i++) {
							if(m_voices[i]->isSustained()) {
								//doNoteOff(i, samEvent->delta, voice[i]->pitch(), voice[i]->triggeredPitch(), 0);
								ev->midiValue1 = m_voices[i]->pitch();
								//doNoteOff(ev);
								//m_voices[i]->end(ev->delta);
								voiceOff(i, ev);
							}
						}

					ev->midiValue1 = m1;
					ev->midiValue2 = m2;
					
				} else {
					m_sustain = true;
				}
			}
			return;
			break;
		case 70: // Varitaion -- let's do articulation crossfading with this.
			//m_articulationCrossfadeModulator->setTargetValue((1.0f/127.0f)*ev->midiValue2);
			m_lastArticulationMidiValue = ev->midiValue2;
			m_articulationCrossfadeModulator->setTargetValue(m_curveArticulationCrossfade->point(ev->midiValue2));
			return;
			break;
		case 74: // Brightness (Filter cut-off)
			//aLog("74: %d %f", ev->midiValue2, ev->fValue);
			ev->fValue = m_filterCurve->point(ev->midiValue2);
			//aLog("74: %d %f", ev->midiValue2, ev->fValue);
			if(ev->midiValue2 < 127) {
				for(int i=0; i<m_audioOutputBuses.size(); i++) { 
					m_audioOutputBuses[i]->setFilterActive(true);
					m_audioOutputBuses[i]->setFilterCutOff(ev->fValue * (44100.0/2.0f));
				}
			} else {
				for(int i=0; i<m_audioOutputBuses.size(); i++) { 
					m_audioOutputBuses[i]->setFilterActive(false);
				}
			}
			return;
			break;
		case 91:
			{
				//int numBuses = m_audioOutputBuses.size();
				for(int i=0; i<m_audioOutputBuses.size(); i++) {
					//m_audioOutputBuses[i]->audioSend(0)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
					m_audioOutputBuses[i]->audioSend(0)->setGain(m_midiSendGain[ev->midiValue2]);
				}
				return;
			}
			break;
		case 92:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(1)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(1)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 93:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(2)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(2)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 94:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(3)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(3)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 102:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(4)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(4)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 103:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(5)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(5)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 104:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(6)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(6)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 105:
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				//m_audioOutputBuses[i]->audioSend(7)->setGain(pow((1.0f/127.0f)*ev->midiValue2, 4.0f));
				m_audioOutputBuses[i]->audioSend(7)->setGain(m_midiSendGain[ev->midiValue2]);
			}
			return;
			break;
		case 123: // All Notes Off
			allNotesOff();
			return;
			break;
	}

	int numStreams = streamMix()->numStreams();

	for(int i=0; i<numStreams; i++) {
	//while(i-->-1) {
		//aLog("Stream %d/%d (%d/%d)", i+1,streamMix()->numStreams(),streamMix()->stream(i)->midiGainController(), ev->midiValue2);
		if(streamMix()->stream(i)->midiGainController() == ev->midiValue1) {
			//qDebug("--> MIDI Controller Stream <--");
			ev->type = SamEvent::Parameter;
			ev->subType = SamParameter::StreamGain;
			ev->iValue = i;
			//ev->fValue = pow((32041.0f/AMath::pow((float)ev->midiValue2, 2.0f)),-1.0f);
			//ev->fValue = samEngine->midiStreamMixGain(ev->midiValue2);
			ev->fValue = m_midiStreamMixGain[ev->midiValue2];
			//if(samPreferences->isConstrainMidiMixerVolume()) {
				//ev->fValue = pow((pow(179.0f,2.0f)/pow((float)ev->midiValue2, 2.0f)),-1.0f);
			//} else {
			//	samEvent->floatValue = pow((pow(90.0f,2.0f)/pow((float)midiEvent.data[2], 2.0f)),-1.0f);
			//}
			doParameter(ev);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::doParameter(SamEvent *ev)
{
	//aLog("SamChannel::doParameter() art=%d - %d, %f", ev->articulationIndex, ev->subType, ev->fValue);

	switch(ev->subType) {
		case SamParameter::LayerSelectMode:
			//aLog("SamParameter::LayerSelectMode:");
			if(m_layerSelectMode[m_currentArticulationIndex] != SamArticulation::Legato) {
				setLayerSelectMode(ev->iValue);
			}
			break;
		case SamParameter::LayerSelect:
			//aLog("SamParameter::LayerSelect:");
			setLayerSelectMode(0);
			setCurrentLayerIndex(ev->iValue);
			break;
		case SamParameter::StreamGain:
			//cDebug("SamParameter::StreamMixGain:");
			{
				int group = m_streamMix->stream(ev->iValue)->group();
				if(group < 0) {
					m_streamMix->stream(ev->iValue)->setGain(ev->fValue, ev->from);
				} else {
					for(int i=0; i<m_streamMix->numStreams(); i++) {
						if(m_streamMix->stream(i)->group() == group) {
							m_streamMix->stream(i)->setGain(ev->fValue, ev->from);
						}
					}
				}
			}
			//updateStreamMixer();
			updateStreamCoefficients();
			break;
		case SamParameter::StreamPanX:
			m_streamMix->stream(ev->iValue)->setPanX(ev->fValue);
			updateStreamCoefficients();
			break;
		case SamParameter::StreamPanY:
			m_streamMix->stream(ev->iValue)->setPanY(ev->fValue);
			updateStreamCoefficients();
			break;
		case SamParameter::StreamMute:
			{
				int group = m_streamMix->stream(ev->iValue)->group();
				if(group < 0) {
					m_streamMix->stream(ev->iValue)->setMute((bool)ev->fValue);
				} else {
					for(int i=0; i<m_streamMix->numStreams(); i++) {
						if(m_streamMix->stream(i)->group() == group) {
							m_streamMix->stream(i)->setMute((bool)ev->fValue);
						}
					}
				}
			}
			updateStreamCoefficients();
			break;
		case SamParameter::StreamSolo:
			{
				int group = m_streamMix->stream(ev->iValue)->group();
				if(group < 0) {
					m_streamMix->stream(ev->iValue)->setSolo((bool)ev->fValue);
				} else {
					for(int i=0; i<m_streamMix->numStreams(); i++) {
						if(m_streamMix->stream(i)->group() == group) {
							m_streamMix->stream(i)->setSolo((bool)ev->fValue);
						}
					}
				}
			}
			updateStreamCoefficients();
			break;
		case SamParameter::Articulation:
			if(ev->articulationIndex > -1) {
					//int articulationIndex = -1;

				for(int i=0; i<m_version->numArticulations(); i++) {
					if(m_version->articulation(i)->id() == ev->iValue) {
						ev->iValue = i;
						break;
					}
				}
			}

			

			if(m_articulationMode && m_numVoicesPlaying > 0 &&  m_lastArticulationMidiValue == 127) {
				setCurrentDefaultArticulationIndex(ev->iValue, ev->from);
			} else {
				setCurrentArticulationIndex(ev->iValue, ev->from);
			}
			
			break;
		case SamParameter::ArticulationDefault:
			{

				if(ev->articulationIndex > -1) {
					//int articulationIndex = -1;

					for(int i=0; i<m_version->numArticulations(); i++) {
						if(m_version->articulation(i)->id() == ev->iValue) {
							ev->iValue = i;
							break;
						}
					}
				}

				//m_currentDefaultArticulationIndex = ev->iValue;
				setCurrentDefaultArticulationIndex(ev->iValue, ev->from);
			}
			break;
		case SamParameter::DefaultCfArticulation:
			setCurrentArticulationIndex(m_defaultCfArticulationIndex);
			setCurrentDefaultArticulationIndex(m_defaultCfDefaultArticulationIndex);
			break;
		case SamParameter::ArticulationLock:
			setArticulationLock((bool)ev->iValue, this);
			break;
		case SamParameter::ArticulationMode:
			setArticulationMode((bool)ev->iValue, this);
			break;
		case SamParameter::EnvelopeAttack:
			//aLog("SamParameter::EnvelopeAttack %f", ev->fValue);
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeAttack(ev->fValue);
			} else aLog("SamParameter::EnvelopeAttack: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::EnvelopeAttackOffset:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
			//qDebug("SamParameter::spsSamplerEnvelopeAttack %f", samEvent->floatValue);
				m_performanceControls[ev->articulationIndex]->setEnvelopeAttackOffset(ev->fValue);
			} else aLog("SamParameter::EnvelopeAttackOffset: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::EnvelopeHold:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeHold(ev->fValue);
			} else aLog("SamParameter::EnvelopeHold: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeSustain:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeSustain(ev->fValue);
			} else aLog("SamParameter::EnvelopeSustain: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeDecay:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeDecay(ev->fValue);
			} else aLog("SamParameter::EnvelopeDecay: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeRelease:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeRelease(ev->fValue);
			} else aLog("SamParameter::EnvelopeRelease: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::EnvelopeReleaseAttack:
			//aLog("SamParameter::EnvelopeAttack %f", ev->fValue);
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeReleaseAttack(ev->fValue);
			} else aLog("SamParameter::EnvelopeReleaseAttack: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::EnvelopeReleaseHold:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeReleaseHold(ev->fValue);
			} else aLog("SamParameter::EnvelopeReleaseHold: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeReleaseSustain:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeReleaseSustain(ev->fValue);
			} else aLog("SamParameter::EnvelopeReleaseSustain: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeReleaseDecay:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeReleaseDecay(ev->fValue);
			} else aLog("SamParameter::EnvelopeReleaseDecay: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::EnvelopeReleaseRelease:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setEnvelopeReleaseRelease(ev->fValue);
			} else aLog("SamParameter::EnvelopeReleaseRelease: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::SampleStart:
			//qDebug("SamParameter::spsSamplerSampleStart %f", samEvent->floatValue);
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setSampleStart(ev->fValue);
			} else aLog("SamParameter::SampleStart: INVALID Articulation Index = %d", ev->articulationIndex);
			break;
		case SamParameter::CellSampleStart:
			//qDebug("SamParameter::spsSamplerCellSampleStartAdjust: %f", samEvent->floatValue);
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setCellSampleStart(ev->fValue);
			} else aLog("SamParameter::CellSampleStart: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::MinimumVelocity:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setMinimumVelocity(ev->fValue);
			} else aLog("SamParameter::MinimumVelocity: INVALID Articulation Index = %d", ev->articulationIndex);
			break;

		case SamParameter::FilterCutOff:
			if(ev->articulationIndex > -1 && ev->articulationIndex < 128) {
				m_performanceControls[ev->articulationIndex]->setFilterCutOff(ev->fValue);
			} else aLog("SamParameter::FilterCutOff: INVALID Articulation Index = %d", ev->articulationIndex);
			m_filterCutOffUpdated = true;
			break;

		case SamParameter::MusicTool:
		case SamParameter::MusicTool+1:
		case SamParameter::MusicTool+2:
		case SamParameter::MusicTool+3:
		case SamParameter::MusicTool+4:
		case SamParameter::MusicTool+5:
		case SamParameter::MusicTool+6:
		case SamParameter::MusicTool+7:
			m_musicToolInserts[ev->subType - SamParameter::MusicTool]->processEvent(ev);
			break;
		
			//m_musicToolInserts[1]->musicTool()->processEvent(ev);
			//break;

		case SamParameter::SendGain:
			//aLog("SamParameter::SendGain:");
			for(int i=0; i<m_audioOutputBuses.size(); i++) {
				m_audioOutputBuses[i]->audioSend(ev->iValue)->setGain(ev->fValue);
			}
			break;

		case SamParameter::ChannelMute:
			setMute((bool)ev->iValue);
			break;
		case SamParameter::ChannelSolo:
			setSolo((bool)ev->iValue);
			break;
		
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::doProgram(SamEvent *ev)
{
	int articulationIndex = -1;

	for(int i=0; i<m_version->numArticulations(); i++) {
		if(m_version->articulation(i)->id() == ev->midiValue1) {
			articulationIndex = i;
			break;
		}
	}

	setCurrentArticulationIndex(articulationIndex);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::allNotesOff(void)
{
	for(int i=0; i<m_numVoices+overVoices; i++) {
		if(m_voices[i]->isPlaying()) { 
			m_voices[i]->end(0);
		}
	}
}

/*void SamChannel::updateStreamMixer(void)
{
	// This is such a hack, there has to be better (more efficient) way of doing this, rather than updating all voices every time a change is made.
	// On the other hand, this approach will make interpolation possible...
	for(int i=0; i<m_numVoices; i++) {
		m_voices[i]->updateStreamMixer();
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::assignInstrument(SamInstrument *instrument, int versionIndex, int voices)
{
	aLog("SamChannel::assignInstrument()");

	m_instrument = instrument;

	if(!m_instrument) {
		return;
	}

	//m_instrument = instrument;
	m_currentVersionIndex = versionIndex;
	m_version = m_instrument->version(m_currentVersionIndex);
	//m_layer = m_version->articulation(0)->layer(0);
	//m_preferredVoices = m_version->numVoices();

	m_currentDefaultArticulationIndex = m_version->defaultArticulation();
	setCurrentArticulationIndex(m_currentDefaultArticulationIndex); // m_version->defaultArticulation()
	

	// Configure audio buffers and outputs
	// -----------------------------------
	if(m_streamMix) {
		delete m_streamMix;
	}
	m_streamMix = new SamStreamMix(this, *instrument->streamMix(0)); // The instrument must contain at least one Stream Mix

	AList<SamAudioBus*> oldAudioBuses;
	if(m_audioOutputBuses.size() > 0) {
		for(int i=0; i<m_audioOutputBuses.size(); i++) {
			oldAudioBuses.append(m_audioOutputBuses[i]);
		}
		m_audioOutputBuses.clear();
	}

	aLog("Creating %d outputs...", m_version->numOutputs());
	for(int i=0; i<m_version->numOutputs(); i++) {
		SamAudioBus *audioBus = new SamAudioBus(this, i, "", m_streamMix->speakerConfig().numSpeakers(), m_streamMix->speakerConfig());
		audioBus->setName(m_version->output(i)->name());
		//audioBus->setAudioOutputBusId(100, 0);
		//audioBus->setAudioProcessInfo(m_audioProcessInfo); // This is superfluous (probably) because it will be called when the Channel's setAPI function is called after this function returns
		
		
		if(oldAudioBuses.size() > i) {
			audioBus->setAudioOutputBusId(oldAudioBuses[i]->audioOutputBusId());
		}
		
		
		m_audioOutputBuses.append(audioBus);

		/*if(m_audioBuffer[i]) {
			delete m_audioBuffer[i];
			m_audioBuffer[i] = 0;
		}
		m_audioBuffer[i] = new Core::AudioBuffer(m_streamMix->speakerConfig().numSpeakers(), m_audioProcessInfo.bufferSize); */
	}

	// Allocate Voices
	// ---------------
	if(voices < 0) {
		voices = m_version->numVoices();
	}
	allocateVoices(voices);

	updateStreamCoefficients();

	setAudioProcessInfo(m_audioProcessInfo);

	// Set-up default parameters
	// -------------------------
	/*for(int i=0; i<m_version->articulation(m_currentArticulationIndex)->numParameters(); i++) {
		SamEvent *evParam = new SamEvent();
		evParam->timestamp = 0;
		evParam->type = SamEvent::Parameter;
		evParam->subType = m_version->articulation(m_currentArticulationIndex)->parameter(i)->source();
		switch(m_version->articulation(m_currentArticulationIndex)->parameter(i)->type()) {
			//case SamParameter::sptTrigger:
			//	if(m_version->articulation(m_currentArticulation)->parameter(i)->defaultValue()>0.0f) {
			//		samParameterEvent.integerValue = m_version->articulation(m_currentArticulation)->parameter(i)->triggerValue();
			//	}
			//	break;
			case SamParameter::sptRange:
				//samParameterEvent.integerValue = m_version->articulation(m_currentArticulation)->parameter(i)->sourceControl();
				evParam->fValue = m_version->articulation(m_currentArticulationIndex)->parameter(i)->defaultValue();
				break;
		}

		aLog("===DEFAULT PARAMETERS===");
		doParameter(evParam);
		aLog("END:===DEFAULT PARAMETERS===");
		delete evParam;
	}*/

	for(int i=0; i<m_version->numArticulations(); i++) {
		for(int j=0; j < m_version->articulation(i)->numParameters(); j++) {
			SamParameter *param = m_version->articulation(i)->parameter(j);
			SamEvent *evParam = new SamEvent(SamEvent::Parameter);
			evParam->subType = param->source();
			evParam->articulationIndex = i;
			switch(param->type()) {
				case SamParameter::sptTrigger:
					// TODO
					break;

				case SamParameter::sptRange:
					evParam->fValue = param->defaultValue();
					aLog(">===Default Parameter %d/%d for Articulation %d ===", j+1, m_version->articulation(i)->numParameters(), i);
					doParameter(evParam);
					aLog(">==================================================");
					break;
			}
			
			delete evParam;
		}
	}


	//m_performanceControls[m_currentArticulationIndex]->debug();

	// This needs to come after the parameters have been set-up. Otherwise, if there's a Parameter to control Layer selection
	// the default Layer settings might be ignored.
	m_layerSelectMode[m_currentArticulationIndex] = m_version->articulation(m_currentArticulationIndex)->layerSelectMode();
	for(int i=0; i<m_version->numArticulations(); i++) {
		m_previousLayerIndex[i] = m_version->articulation(i)->defaultLayer();
		m_previousLayerSelectMode[i] = m_version->articulation(i)->layerSelectMode();
	}
	

	//m_zoneCrossfadeModulator->setRate((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneCrossfadeResponseTime());

	
	//aLog("m_layerSelectMode = %d", m_layerSelectMode);
	aLog("EXIT: SamChannel::assignInstrument()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamMusicToolInsert* SamChannel::musicToolInsert(int index, bool instrument)
{
	SamMusicToolInsert *insert = 0;

	if(instrument) {
		insert = m_musicToolInternals[index];
	} else {
		insert = m_musicToolInserts[index];
	}

	return insert;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setMidiInputPort(int port, void *from, bool shouldUpdate)
{
	m_midiInputPort = port;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemMidiInputPort, this, from);
	}

	//((SamProject*)parent())->updateChannelMidiAssignments();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setMidiInputChannel(int channel, void *from, bool shouldUpdate)
{
	m_midiInputChannel = channel;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemMidiInputChannel, this, from);
	}

	//((SamProject*)parent())->updateChannelMidiAssignments();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setSelected(bool selected, void *from, bool shouldUpdate)
{
	m_selected = selected;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemSelected, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::setMuted(bool muted, void *from, bool shouldUpdate)
{
	m_muted = muted;

	if(shouldUpdate) {
		//samEngine->stateManager()->updateState(SamEngine::ContextChannel, ItemSelected, this, from);
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setMute(bool mute, void *from, bool shouldUpdate)
{
	m_mute = mute;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemMute, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setSolo(bool solo, void *from, bool shouldUpdate)
{
	SamProject *project = (SamProject*)parent(); //m_preSoloMuteState

	int ncs = project->numChannelsSolo();
	bool firstSolo = false;
	if(ncs == 0) {
		firstSolo = true;
	}

	m_solo = solo;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemSolo, this, from);
	}

	

	

	if(m_solo) {
		for(int i=0; i<project->numChannels(); i++) {
			SamChannel *ch = project->channel(i);
			if(firstSolo) {
				ch->setPreSoloMuteState(ch->isMute());
			}
			
			if(ch != this) {
				if(!ch->isSolo()) {
					ch->setMute(true);
				}
			} else {
				ch->setMute(false);
			}
		}
	} else {
		if(ncs-1 < 1) {
		for(int i=0; i<project->numChannels(); i++) {
			SamChannel *ch = project->channel(i);
		//	if(!ch->isSolo()) {
				ch->setMute(ch->preSoloMuteState());
			//}
		}
		} else {
			setMute(true);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setMidiMuted(bool muted, void *from, bool shouldUpdate)
{
	m_midiMuted = muted;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemMidiMuted, this, from);
	}

	if(m_midiMuted) {
		SamEvent ev(SamEvent::Controller);
		ev.midiValue1 = 123;
		ev.subType = SamParameter::StreamGain;
		ev.timestamp = 0;
		addEvent(ev);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setArticulationLock(bool lock, void *from, bool shouldUpdate)
{
	m_articulationLock = lock;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemArticulationLock, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setArticulationMode(bool mode, void *from, bool shouldUpdate)
{
	m_articulationMode = mode;

	//static bool firstTime = false;
	//if(m_articulationMode && firstTime) {
		setCurrentArticulationIndex(m_defaultCfArticulationIndex);
		setCurrentDefaultArticulationIndex(m_defaultCfDefaultArticulationIndex);
	//	firstTime = true;
	//}

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemArticulationMode, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setDownbeatOffset(bool downbeatOffset, void *from, bool shouldUpdate)
{
	aLog("setDownbeatOffset()");
	
	m_downbeatOffset = downbeatOffset;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemDownbeatOffset, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setLayerMutesEnabled(bool enabled, void *from, bool shouldUpdate)
{
	aLog("setLayerMutesEnabled()");
	
	m_layerMutesEnabled = enabled;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemLayerMutesEnabled, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setCurrentArticulationIndex(int articulationIndex, void *from, bool shouldUpdate)
{
	aLog("SamChannel::setCurrentArticulationIndex(%d)", articulationIndex);

	if(!m_articulationLock && (articulationIndex >= 0 && articulationIndex < m_version->numArticulations())) {
	
		if(m_articulationMode && !m_instrument->version(0)->articulation(articulationIndex)->containsLoops()) {
			return;
		}

		// Store Layer settings for the current Articulation
		m_previousLayerIndex[m_currentArticulationIndex] = m_currentLayerIndex[articulationIndex];
		m_previousLayerSelectMode[m_currentArticulationIndex] = m_layerSelectMode[m_currentArticulationIndex];

		// Set the new Articulation as the current Articulation
		int oldArticulationIndex = m_currentArticulationIndex;
		m_currentArticulationIndex = articulationIndex;

		//if(m_currentArticulationIndex >= m_version->numArticulations()) {
		//	m_currentArticulationIndex = -1;
		//}

		// Restore the Layer settings for the new Articulation
		setCurrentLayerIndex(m_previousLayerIndex[m_currentArticulationIndex]);
		m_layerSelectMode[m_currentArticulationIndex] = m_previousLayerSelectMode[m_currentArticulationIndex];


		

		// Set the new Articulation as the current Articulation
		
		static int notes[128];

		if(m_articulationMode) {
			if(m_lastArticulationMidiValue == 0 && m_numVoicesPlaying > 0) {
				
				if(m_numVoicesPlaying > 0) {
					for(int i=0; i<128; i++) {
						notes[i] = false;
					}

					for(int i=0; i<m_numVoices; i++) {
						if(m_voices[i]->isPlaying() && !m_voices[i]->isReleasing() && m_voices[i]->articulationIndex() == oldArticulationIndex) {
							//aLog("Releasing voice");
							notes[m_voices[i]->pitch()] = m_voices[i]->velocity();
							m_voices[i]->release();
						}
					}

					for(int i=0; i<128; i++) {
						if(notes[i] > 0) {
							SamEvent ev(SamEvent::NoteOn);
							ev.midiValue1 = i;
							ev.midiValue2 = notes[i];
							ev.triggeredPitch = ev.midiValue1;
							ev.articulationIndex = m_currentArticulationIndex;
							ev.timestamp = samEngine->currentTimestamp();
							//aLog("Playing note %d", i);
							playNote(&ev);
						}
					}
				}
			}
		}
			


		if(shouldUpdate) {
			updateState(SamEngine::ContextChannel, ItemArticulation, this, from);
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setCurrentDefaultArticulationIndex(int articulationIndex, void *from, bool shouldUpdate)
{
	//aLog("&#& SamChannel::setCurrentDefaultArticulationIndex(%d)", articulationIndex);

	static int notes[128];

	

	if(!m_articulationLock && (articulationIndex >= 0 && articulationIndex < m_version->numArticulations())) {

		// Set the new Articulation as the current Articulation
		
		if(m_articulationMode) {

			if(!m_instrument->version(0)->articulation(articulationIndex)->containsLoops()) {
				return;
			}

			if(m_lastArticulationMidiValue == 127 && m_numVoicesPlaying > 0) {
				
				if(m_numVoicesPlaying > 0) {
					for(int i=0; i<128; i++) {
						notes[i] = false;
					}

					for(int i=0; i<m_numVoices; i++) {
						if(m_voices[i]->isPlaying() && !m_voices[i]->isReleasing() && m_voices[i]->articulationIndex() == m_currentDefaultArticulationIndex) {
							//aLog("Releasing voice");
							notes[m_voices[i]->pitch()] = m_voices[i]->velocity();
							m_voices[i]->release();
						}
					}

					m_currentDefaultArticulationIndex = articulationIndex;

					for(int i=0; i<128; i++) {
						if(notes[i] > 0) {
							SamEvent ev(SamEvent::NoteOn);
							ev.midiValue1 = i;
							ev.midiValue2 = notes[i];
							ev.triggeredPitch = ev.midiValue1;
							ev.articulationIndex = articulationIndex;
							ev.timestamp = samEngine->currentTimestamp();
							playNote(&ev);
						}
					}
				}
			}
			
		}
			
		m_currentDefaultArticulationIndex = articulationIndex;


		if(shouldUpdate) {
			updateState(SamEngine::ContextChannel, ItemArticulation, this, from);
		}

	} 
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setDefaultCfArticulations(int articulationIndex, int defaultArticulationIndex)
{
	if(m_articulationMode) {
		m_defaultCfArticulationIndex = articulationIndex;
		m_defaultCfDefaultArticulationIndex = defaultArticulationIndex;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setCurrentLayerIndex(int layerIndex, void *from, bool shouldUpdate)
{
	if(m_currentArticulationIndex > -1) {
		for(int i=0; i<128; i++) {
			m_lastCellId[i] = -1;
		}
		if(layerIndex >= 0 && layerIndex < m_version->articulation(m_currentArticulationIndex)->numLayers()) {
			m_currentLayerIndex[m_currentArticulationIndex] = layerIndex;
			
			if(shouldUpdate) {
				updateState(SamEngine::ContextChannel, ItemLayer, this, from);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setLayerSelectMode(int mode, void *from, bool shouldUpdate)
{
	m_layerSelectMode[m_currentArticulationIndex] = mode;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemLayer, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamLayer* SamChannel::layer(void)
{
	if(m_currentArticulationIndex > -1) {
		if(m_currentLayerIndex[m_currentArticulationIndex] > -1) {
			return m_version->articulation(m_currentArticulationIndex)->layer(m_currentLayerIndex[m_currentArticulationIndex]);
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamChannel::totalSetGain(int setIndex)
{
	float gain = 0.0f;
	for(int i=0; i<m_streamMix->numStreams(); i++) {
		if(m_streamMix->stream(i)->setId() == setIndex) {
			if(!m_streamMix->stream(i)->mute() && (!m_streamMix->isSolo() || (m_streamMix->isSolo() && m_streamMix->stream(i)->solo()))) {
				//aLog("stream(%d) gain=%f", i, m_streamMix->stream(i)->gain());
				gain += m_streamMix->stream(i)->gain();
			}
		}
	}

	return gain;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamChannel::isSetMuted(int setIndex)
{
	int numStreamsInSet = 0;
	int numStreamsMuted = 0;
	for(int i=0; i<m_streamMix->numStreams(); i++) {
		if(m_streamMix->stream(i)->setId() == setIndex) {
			numStreamsInSet++;
			if(m_streamMix->stream(i)->mute()) {
				numStreamsMuted++;
			}
		}
	}

	if(numStreamsInSet == numStreamsMuted) {
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setStreamMix(SamStreamMix *streamMix)
{
	aLog("========== SamChannel::setStreamMix() current = %s, new = %s ==========", m_streamMix->speakerConfig().name().toChar(), streamMix->speakerConfig().name().toChar());
	m_streamMix->setStreamMix(streamMix);

	setSpeakerConfig(m_streamMix->speakerConfig());

	//updateStreamCoefficients();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setSpeakerConfig(CsSpeakerConfig config, void *from, bool shouldUpdate)
{
	aLog("========== SamChannel::setSpeakerConfig(%s) ==========", config.name().toChar());

	/*if(m_streamMix->speakerConfig() == config) {
		aLog("New config is the same as the current config.");
		return;
	}*/

	m_streamMix->setSpeakerConfig(config);

	for(int i=0; i<m_audioOutputBuses.size(); i++) {
		m_audioOutputBuses[i]->setSpeakerConfig(config);
	}

	allocateVoices(m_numVoices);

	updateStreamCoefficients();

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemConfig, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::setPerVoiceFxTool(AString toolUuid)
{
	aLog("SamChannel::setPerVoiceFxTool(%s)", toolUuid.toChar());
	
	m_perVoiceFxToolUuid = toolUuid;

	aLog("m_perVoiceFxToolUuid = %s", m_perVoiceFxToolUuid.toChar());

	allocateVoices(m_numVoices);

	// Initialise preset table
	// -----------------------

	//AList<SamToolParameter*> m_parameterList[8];

	
	for(int i=0; i<8; i++) {
		if(m_parameterList[i].size() > 0) {
			for(int j=0; j<m_parameterList[i].size(); j++) {
				delete m_parameterList[i][j];
			}
			m_parameterList[i].clear();
		}
		if(m_voices[0]->audioToolInsert()) {
			for(int j=0; j<m_voices[0]->audioToolInsert()->audioTool()->numParameters(); j++) {
				SamToolParameter *pr = new SamToolParameter(*m_voices[0]->audioToolInsert()->audioTool()->parameter(j));
				pr->setValue(m_voices[0]->audioToolInsert()->audioTool()->parameterValueNormalised(pr->id()));
				m_parameterList[i].append(pr);
				//aLog("[%d] %s", j, pr->name().toChar());
			}
		} else aLog("NO AUDIO TOOL INSERT!");
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::setPerVoiceGroup(int pitch, int velocityZone, char group)
{
	m_perVoiceGroup[pitch][velocityZone] = group;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamChannel::setPerVoiceParameter(char group, int id, float value)
{
	aLog("setPerVoiceParameter(%d, %d, %f) %d", group, id, value, m_parameterList[group].size());
	//if(id < 0) {
	//	m_parameterList[group][-id]->setValue(value);
	//} else {
		for(int i=0; i<m_parameterList[group].size(); i++) {
			if(m_parameterList[group][i]->id() == id) {
				m_parameterList[group][i]->setValue(value); aLog("OK");
			}
		}
	//}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setAttenuationForMidiNotes(int attenuationStart, int attenuationEnd, int attenuationFadeIn, int attenuationFadeOut)
{
	//qDebug("SamChannel::setAttenuationForMidiNotes(inQ=%f, outQ=%f)", attenuationFadeInQ, attenuationFadeOutQ);

	m_attenuationStart = attenuationStart;
	m_attenuationEnd = attenuationEnd;
	m_attenuationFadeIn = attenuationFadeIn;
	m_attenuationFadeOut = attenuationFadeOut;
	//curveAttenuationFadeIn->setQ(attenuationFadeInQ);
	//curveAttenuationFadeOut->setQ(attenuationFadeOutQ);

	updateAttenuationsForMidiNotes();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::updateAttenuationsForMidiNotes(void)
{
	aLog("SamChannel::updateAttenuationsForMidiNotes()");
	aLog("m_attenuationStart = %d, m_attenuationEnd = %d", m_attenuationStart, m_attenuationEnd);

	m_curveAttenuationFadeIn->setLength(m_attenuationFadeIn);
	m_curveAttenuationFadeOut->setLength(m_attenuationFadeOut);

	for(int i=0; i<128; i++) {
		m_midiNoteGain[i] = 0.0f;
	}

	for(int i=m_attenuationStart; i<m_attenuationEnd; i++) {
		m_midiNoteGain[i] = 1.0f;
	}

	for(int i=0; i<m_curveAttenuationFadeIn->length(); i++) {
		m_midiNoteGain[m_attenuationStart+i] = m_curveAttenuationFadeIn->point(i);
	}

	for(int i=0; i<m_curveAttenuationFadeOut->length(); i++) {
		m_midiNoteGain[(m_attenuationEnd-m_attenuationFadeOut)+i] = m_curveAttenuationFadeOut->point(i);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setPitchBend(bool pitchBend, void *from, bool shouldUpdate)
{
	m_pitchBend = pitchBend;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemPitchBend, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setPitchBendUp(int pitchBendUp, void *from, bool shouldUpdate)
{
	m_pitchBendUp = pitchBendUp;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemPitchBend, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::setPitchBendDown(int pitchBendDown, void *from, bool shouldUpdate)
{
	m_pitchBendDown = pitchBendDown;

	if(shouldUpdate) {
		updateState(SamEngine::ContextChannel, ItemPitchBend, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamChannel::readElement(AXmlDocument *document, AXmlElement *element)
{
	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamChannel::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("instrumentUuid", m_instrument->uuid());
	element->setAttribute("version", m_currentVersionIndex);
	element->setAttribute("midiInputPort", m_midiInputPort);
	element->setAttribute("midiInputChannel", m_midiInputChannel);
	element->setAttribute("midiMuted", (int)m_midiMuted);
	element->setAttribute("position", m_positionInList);
	element->setAttribute("articulationLock", (int)m_articulationLock);
	element->setAttribute("articulationMode", (int)m_articulationMode);
	element->setAttribute("currentArticulation", m_currentArticulationIndex);
	element->setAttribute("currentDefaultArticulation", m_currentDefaultArticulationIndex);
	element->setAttribute("defaultCfArticulationIndex", m_defaultCfArticulationIndex);
	element->setAttribute("defaultCfDefaultArticulationIndex", m_defaultCfDefaultArticulationIndex);
	element->setAttribute("voices", m_numVoices);
	element->setAttribute("downbeatOffset", (int)m_downbeatOffset);
	if(m_label.length() > 0) element->setAttribute("label", m_label);

	AXmlElement *elPb = document->createElement("PitchBend");
	elPb->setAttribute("enabled", (int)m_pitchBend);
	elPb->setAttribute("up", m_pitchBendUp);
	elPb->setAttribute("down", m_pitchBendDown);
	element->addChildElement(elPb);

	AXmlElement *elAtt = document->createElement("MidiNoteAttenuation");
	elAtt->setAttribute("attenuationStart", m_attenuationStart);
	elAtt->setAttribute("attenuationEnd", m_attenuationEnd);
	elAtt->setAttribute("attenuationFadeIn", m_attenuationFadeIn);
	elAtt->setAttribute("attenuationFadeOut", m_attenuationFadeOut);
	elAtt->setAttribute("attenuationFadeInQ", m_curveAttenuationFadeIn->depth());
	elAtt->setAttribute("attenuationFadeOutQ", m_curveAttenuationFadeOut->depth());
	element->addChildElement(elAtt);

	AXmlElement *elArtCf = document->createElement("ArticulationCrossfadeCurve");
	elArtCf->setAttribute("start", m_curveArticulationCrossfade->start());
	elArtCf->setAttribute("end", m_curveArticulationCrossfade->end());
	elArtCf->setAttribute("q", m_curveArticulationCrossfade->depth());
	elArtCf->setAttribute("shape", m_curveArticulationCrossfade->shape());
	element->addChildElement(elArtCf);

	AXmlElement *elStreamMix = document->createElement("StreamMix");
	element->addChildElement(elStreamMix);
	m_streamMix->writeElement(document, elStreamMix);

	AXmlElement *elOutputBuses = document->createElement("OutputBuses");
	element->addChildElement(elOutputBuses);
	for(int i=0; i<m_audioOutputBuses.size(); i++) {
		AXmlElement *elOutputBus = document->createElement("OutputBus");
		m_audioOutputBuses[i]->writeElement(document, elOutputBus);
		elOutputBuses->addChildElement(elOutputBus);
	}

	bool musicToolAssigned = false;
	for(int i=0; i<maxMusicTools; i++) {
		if(m_musicToolInserts[i]->isAssigned()) {
			musicToolAssigned = true;
			break;
		}
	}

	if(musicToolAssigned) {
		AXmlElement *elMusicTools = document->createElement("MusicToolInserts");
		element->addChildElement(elMusicTools);
		for(int i=0; i<maxMusicTools; i++) {
			if(m_musicToolInserts[i]->isAssigned()) {
				AXmlElement *elMusicTool = document->createElement("MusicToolInsert");
				m_musicToolInserts[i]->writeElement(document, elMusicTool);
				elMusicTools->addChildElement(elMusicTool);
			}
		}
	}

	/*if(m_perVoiceFxToolUuid.length() > 0) {
		AXmlElement *elPerVoiceFx = document->createElement("PerVoiceFx");
		elPerVoiceFx->setAttribute("audioToolUuid", m_perVoiceFxToolUuid);
		elPerVoiceFx->setAttribute("velocitySplit", m_perVoiceVelocitySplit);
		element->addChildElement(elPerVoiceFx);

		for(int i=0; i<8; i++) {
			AXmlElement *elPr = document->createElement(AString("PresetGroup")+AString::number(i));
			for(int j=0; j<m_parameterList[i].size(); j++) {
				AXmlElement *elementParameter = document->createElement("Parameter");
				elementParameter->setAttribute("id", m_parameterList[i][j]->id());
				elementParameter->setAttribute("name", m_parameterList[i][j]->name());
				elementParameter->setAttribute("value", m_parameterList[i][j]->value());
				elementParameter->setAttribute("midiController", m_parameterList[i][j]->midiController());
				elementParameter->setAttribute("midiMin", m_parameterList[i][j]->midiMin());
				elementParameter->setAttribute("midiMax", m_parameterList[i][j]->midiMax());
				elPr->addChildElement(elementParameter);
			}
			elPerVoiceFx->addChildElement(elPr);
		}

		for(int i=0; i<2; i++) {
			AXmlElement *elV = document->createElement(AString("VelocityPresetMap")+AString::number(i));
			for(int j=0; j<128; j++) {
				elV->setAttribute(AString("p")+AString::number(j), AString::number(m_perVoiceGroup[j][i])); 
			}
			elPerVoiceFx->addChildElement(elV);
		}

	}*/

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::updateStreamCoefficients(void)
{
	//aLog("SamChannel::updateStreamCoefficients()");

	/*if(!m_streamCoefficients) {
		for(int j=0; j<m_numAllocatedSets; j++) {
			for(int i=0; i<m_numAllocatedStreams[j]; i++) {
				free(m_streamCoefficients[j][i]);
			}
			free(m_streamCoefficients[j]);
			m_numAllocatedStreams[j] = 0;
		}
		free(m_streamCoefficients);
		m_numAllocatedSets = 0;
	}*/

	float left;
	float center;
	float right;

	//cDebug("numSets = %d", instrument()->elementPool()->numSets());
	//m_streamCoefficients = (float***)malloc(sizeof(float*)*instrument()->elementPool()->numSets());
	//m_numAllocatedSets = instrument()->elementPool()->numSets();
	//m_numAllocatedStreams = (int*)malloc(sizeof(int*)*m_numAllocatedSets);
	for(int j=0; j<instrument()->elementPool()->numSets(); j++) {
		//aLog("set %d/%d", j+1, instrument()->elementPool()->numSets());
		//m_streamCoefficients[j] = (float**)malloc(sizeof(float*)*instrument()->elementPool()->set(j)->streams());
		//cDebug("A");
		
		//m_numAllocatedStreams[j] = instrument()->elementPool()->set(j)->streams();
		for(int i=0; i<instrument()->elementPool()->set(j)->streams(); i++) {
			//aLog("stream %d/%d", i+1, instrument()->elementPool()->set(j)->streams());
			//m_streamCoefficients[j][i] = (float*)malloc(sizeof(float)*m_streamMix->speakerConfig().numSpeakers());

			SamStream *s = m_streamMix->getStream(j, i);
			float mute = 0;
			if(!s->mute() && (!m_streamMix->isSolo() || (m_streamMix->isSolo() && s->solo())))mute = 1;
			//aLog("mute[%d] = %f", i, mute);
			if(s) {
				switch(m_streamMix->speakerConfig().config()) {
					case CsSpeakerConfig::Mono:
						//m_streamCoefficients[j][i][0] = 1.0f;
						break;
					case CsSpeakerConfig::Stereo:
						m_streamCoefficients[j][i][0] = (((1.0f-s->panX())*(1.0f-s->panY())) * s->gain())*mute; //
						m_streamCoefficients[j][i][1] = (((s->panX())*(1.0f-s->panY())) * s->gain())*mute;
						break;
					case CsSpeakerConfig::Surround40:
						m_streamCoefficients[j][i][0] = (((1.0f-s->panX())*(1.0f-s->panY())) * s->gain())*mute; //
						m_streamCoefficients[j][i][1] = (((s->panX())*(1.0f-s->panY())) * s->gain())*mute;
						m_streamCoefficients[j][i][2] = (((1.0f-s->panX())*(s->panY())) * s->gain())*mute;
						m_streamCoefficients[j][i][3] = (((s->panX())*(s->panY())) * s->gain())*mute;
						break;
					case CsSpeakerConfig::Surround50:
						center = 1.0f;
						if(s->panX() < 0.5) {
							left = 1.0f-(s->panX()*2.0f);
							center = 1.0f - left;
						} else {
							left = 0.0f;
						}
						if(s->panX() > 0.5) {
							right = (s->panX()-0.5)*2.0f;
							center = 1.0f - right;
						} else {
							right = 0.0f;
						}
						//aLog("s->panX() = %f, l = %f, c = %f, r = %f", s->panX(), left, center, right);
						m_streamCoefficients[j][i][0] = ((left*(1.0f-s->panY())) * s->gain())*mute;
						m_streamCoefficients[j][i][1] = center * (1.0-s->panY()) * s->gain()*mute;
						m_streamCoefficients[j][i][2] = ((right*(1.0f-s->panY())) * s->gain())*mute;
						m_streamCoefficients[j][i][3] = (((1.0f-s->panX())*(s->panY())) * s->gain())*mute;
						m_streamCoefficients[j][i][4] = (((s->panX())*(s->panY())) * s->gain())*mute;
						break;
				}
			}
		}
	}

	//aLog("EXIT: SamChannel::updateStreamCoefficients()");
}

/*void SamChannel::updateStreamCoefficients(void)
{
	//cDebug("SamChannel::updateStreamCoefficients()");

	if(!m_streamCoefficients) {
		for(int j=0; j<m_numAllocatedSets; j++) {
			for(int i=0; i<m_numAllocatedStreams[j]; i++) {
				free(m_streamCoefficients[j][i]);
			}
			free(m_streamCoefficients[j]);
			m_numAllocatedStreams[j] = 0;
		}
		free(m_streamCoefficients);
		m_numAllocatedSets = 0;
	}

	//cDebug("numSets = %d", instrument()->elementPool()->numSets());
	m_streamCoefficients = (float***)malloc(sizeof(float*)*instrument()->elementPool()->numSets());
	m_numAllocatedSets = instrument()->elementPool()->numSets();
	m_numAllocatedStreams = (int*)malloc(sizeof(int*)*m_numAllocatedSets);
	for(int j=0; j<instrument()->elementPool()->numSets(); j++) {
		//cDebug("set %d/%d", j+1, instrument()->elementPool()->numSets());
		m_streamCoefficients[j] = (float**)malloc(sizeof(float*)*instrument()->elementPool()->set(j)->streams());
		//cDebug("A");
		
		m_numAllocatedStreams[j] = instrument()->elementPool()->set(j)->streams();
		for(int i=0; i<instrument()->elementPool()->set(j)->streams(); i++) {
			 
			m_streamCoefficients[j][i] = (float*)malloc(sizeof(float)*m_streamMix->speakerConfig().numSpeakers());

			SamStream *s = m_streamMix->getStream(j, i);

			switch(m_streamMix->speakerConfig().config()) {
				case Core::SpeakerConfig::Stereo:
					m_streamCoefficients[j][i][0] = (((1.0f-s->panX())*(1.0f-s->panY())) * s->gain()); //
					m_streamCoefficients[j][i][1] = (((s->panX())*(1.0f-s->panY())) * s->gain());
					break;
				case Core::SpeakerConfig::Surround40:
					m_streamCoefficients[j][i][0] = (((1.0f-s->panX())*(1.0f-s->panY())) * s->gain()); //
					m_streamCoefficients[j][i][1] = (((s->panX())*(1.0f-s->panY())) * s->gain());
					m_streamCoefficients[j][i][2] = (((1.0f-s->panX())*(s->panY())) * s->gain());
					m_streamCoefficients[j][i][3] = (((s->panX())*(s->panY())) * s->gain());
					break;
			}
		}
	}

	//cDebug("EXIT: SamChannel::updateStreamCoefficients()");
}*/

/*void SamChannel::updateStreamCoefficients(void)
{
	if(!m_streamCoefficients) {
		free(m_streamCoefficients);
	}

	m_streamCoefficients = (float**)malloc(sizeof(float)*m_numStreams);
	for(int i=0; i<m_numStreams; i++) {
		m_streamCoefficients[i] = (float*)malloc(sizeof(float)*m_streamMix->speakerConfig().numSpeakers());

		SamStream *s = m_streamMix->getStream(m_setId, i);

		switch(m_streamMix->speakerConfig().config()) {
			case Core::SpeakerConfig::Surround40:
				m_streamCoefficients[i][0] = (((1.0f-s->panX())*(1.0f-s->panY())) * s->gain()); //
				m_streamCoefficients[i][1] = (((s->panX())*(1.0f-s->panY())) * s->gain());
				m_streamCoefficients[i][2] = (((1.0f-s->panX())*(s->panY())) * s->gain());
				m_streamCoefficients[i][3] = (((s->panX())*(s->panY())) * s->gain());
				break;
		}
	}
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::allocateVoices(int voices)
{
	aLog("SamChannel::allocateVoices(%d)", voices);
	
	freeVoices();

	m_numVoices = voices;
	m_updateNumVoices = voices;

	if (m_numVoices>maxVoices)
		m_numVoices = maxVoices;
	if (m_numVoices<1)
		m_numVoices = 1;

	for (int i=0; i<m_numVoices+overVoices; i++) {
		aLog("Allocating voice %d/%d",i+1,m_numVoices);
		m_voices[i] = new SamVoice(this);
		m_voices[i]->setPitchBendModulator(m_pitchBendModulator);
		m_voices[i]->setAudioProcessInfo(m_audioProcessInfo);
		m_voices[i]->setZoneCrossfadeResponseTime((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneCrossfadeResponseTime());
		m_voices[i]->setZoneFilterResponseTime((44100/1000) * m_version->articulation(m_currentArticulationIndex)->zoneFilterResponseTime());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamChannel::freeVoices(void)
{
	for (int i=0; i<maxVoices+overVoices; i++) {
		if(m_voices[i]!=0) {
			delete m_voices[i];
			m_voices[i] = 0;
		}
	}
	m_numVoices = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVoice* SamChannel::nextVoice(SamLayer *layer, int pitch)
{
	//aLog("SamChannel::nextVoice(%d) = m_pitchCounter = %d", pitch, m_pitchCounter[pitch]);

	bool killVoice = false;

	int maxVoicesPerPitch = 7;
	if(layer->zoneSelectMode() == 1) {
		if(layer->isShortNoteMode()) {
			maxVoicesPerPitch *= 2;//layer->numZonesByPitch(pitch); 
		} else {
			maxVoicesPerPitch *= layer->numZonesByPitch(pitch); 
		}
	}

	if(m_pitchCounter[pitch] > maxVoicesPerPitch) {
		//aLog("KILL: m_pitchCounter[%d] = %d", pitch, m_pitchCounter[pitch]);
		killVoice = true;
	} else {
		if(m_numVoicesPlaying > m_numVoices-1) {
			//aLog("KILL: m_numVoicesPlaying > m_numVoices-1 (%d > %d)", m_numVoicesPlaying, m_numVoices-1);
			killVoice = true;
		} else {
			if(samEngine->audioManager()->lastBufferUsage() > 0.8f) {
				//aLog("KILL: buffer");
				killVoice = true;
			}
		}
	}

	if(killVoice) {
		int oldestVoice = -1;
		//int oldestVoiceOfCurrentPitch = -1;

		//aLog("We need to kill something... (%d > %d)", m_numVoicesPlaying, m_numVoices-3);
		
		for (int i=0; i<m_numVoices+overVoices; i++) {
			//aLog("voice[%d]: p=%d, s=%d", i, m_voices[i]->isPlaying(), m_voices[i]->stolen());
			if(m_voices[i]->isPlaying() && !m_voices[i]->stolen()) {
				//aLog("Here!");
				if(oldestVoice < 0) {
					oldestVoice = i;
				} else if(m_voices[i]->timestamp() < m_voices[oldestVoice]->timestamp()) {
					oldestVoice = i;
				}

				/*if(m_voices[i]->pitch() == pitch) {
					if(oldestVoiceOfCurrentPitch < 0) {
						oldestVoiceOfCurrentPitch = i;
					} else if(m_voices[i]->timestamp() < m_voices[oldestVoiceOfCurrentPitch]->timestamp()) {
						oldestVoiceOfCurrentPitch = i;
					} 
				}*/
			}
		}
		//cDebug("About to kill voice %d", oldestVoice);

		/*if(oldestVoiceOfCurrentPitch > -1) {
			//cDebug("Kill oldestVoiceOfCurrentPitch %d", oldestVoiceOfCurrentPitch);
			m_voices[oldestVoiceOfCurrentPitch]->setStolen(441);
		} else */if(oldestVoice > -1) {
			//aLog("Kill oldestVoice %d", oldestVoice);
			m_voices[oldestVoice]->setStolen(255);
			//m_voices[oldestVoice]->release();
		} //else aLog("NoKill!");
	}
	
	for (int i=0; i<m_numVoices+overVoices; i++) {
		if (!m_voices[i]->isPlaying()) {
			m_numVoicesPlaying++;
			//aLog("Returning voice %d", i);
			return m_voices[i];
		}
	}

	aLog("***** Should not get here *****");

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVoice* SamChannel::nextReleaseVoice(SamLayer *layer, int pitch)
{
	//aLog("m_releaseCounter = %d", m_releaseCounter);

	bool killVoice = false;

	if(m_releaseCounter > layer->releaseVoices()) {
		killVoice = true;
	} else {
		if(m_numVoicesPlaying > m_numVoices-1) {
			killVoice = true;
		} else {
			if(samEngine->audioManager()->lastBufferUsage() > 0.8f) {
				killVoice = true;
			}
		}
	}

	if(killVoice) {
		int oldestVoice = -1;
		//int oldestVoiceOfCurrentPitch = -1;

		//aLog("[Release] We need to kill something... (%d > %d)", m_numVoicesPlaying, m_numVoices-3);
		
		for (int i=0; i<m_numVoices+overVoices; i++) {
			if(m_voices[i]->isPlaying() && !m_voices[i]->stolen() && m_voices[i]->isRelease()) {
				if(oldestVoice < 0) {
					oldestVoice = i;
				} else if(m_voices[i]->timestamp() < m_voices[oldestVoice]->timestamp()) {
					oldestVoice = i;
				}

				/*if(m_voices[i]->pitch() == pitch) {
					if(oldestVoiceOfCurrentPitch < 0) {
						oldestVoiceOfCurrentPitch = i;
					} else if(m_voices[i]->timestamp() < m_voices[oldestVoiceOfCurrentPitch]->timestamp()) {
						oldestVoiceOfCurrentPitch = i;
					} 
				}*/
			}
		}
		//cDebug("About to kill voice %d", oldestVoice);

		/*if(oldestVoiceOfCurrentPitch > -1) {
			//cDebug("Kill oldestVoiceOfCurrentPitch %d", oldestVoiceOfCurrentPitch);
			m_voices[oldestVoiceOfCurrentPitch]->setStolen(441);
		} else */if(oldestVoice > -1) {
			//cDebug("Kill oldestVoice %d", oldestVoice);
			m_voices[oldestVoice]->setStolen(441);
		}
	}
	
	for (int i=0; i<m_numVoices+overVoices; i++) {
		if (!m_voices[i]->isPlaying()) {
			m_numVoicesPlaying++;
			//aLog("Returning voice %d", i);
			return m_voices[i];
		}
	}

	aLog("***** Release: Should not get here *****");

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamChannel::nextCellId(SamZone *zone, int velocity)
{
	//aLog("SamChannel::nextCellId()");

	int cellToPlay = 0;
	int layerIndex = 0;
	SamArticulation *articulation = (SamArticulation*)zone->parent()->parent();
	layerIndex = articulation->layerIndex((SamLayer*)zone->parent());
	if(layerIndex < 0 || layerIndex > 127) {
		layerIndex = 0;
	}


	if (zone->isPredictableVelocity()) {
		if (zone->midiVelocityHigh()-zone->midiVelocityLow() > 0) {
			if(velocity == zone->midiVelocityHigh()) {
				m_lastCellId[layerIndex] = zone->predictableVelocityCellIndex();
				return m_lastCellId[layerIndex];
			}
		}
	}

	switch (zone->triggerMode()) {

	case SamZone::FirstCellOnly:// aLog("SamZone::FirstCellOnly");
			cellToPlay = zone->singleCellId();
			break;

		case SamZone::RoundRobin: //aLog("SamZone::RoundRobin");
			cellToPlay = m_lastCellId[layerIndex];
			cellToPlay++;
			break;

		case SamZone::Random: //aLog("SamZone::Random");
			if (zone->numCells()==0) {
				cellToPlay = 0;
			} else {
				cellToPlay = rand()%zone->numCells();
				//aLog("cellToPlay = %d, m_lastCellId[layerIndex] = %d", cellToPlay, m_lastCellId[layerIndex]);
				if(cellToPlay == m_lastCellId[layerIndex]) {
					cellToPlay++;
				}
			}
			break;
	};

	if (cellToPlay >= zone->numCells()) {
		cellToPlay = 0;
	}

	m_lastCellId[layerIndex] = cellToPlay;

	//aLog("m_lastCellId[%d] = %d (triggerMode = %d)", layerIndex, m_lastCellId[layerIndex], zone->triggerMode());

	return m_lastCellId[layerIndex];
}