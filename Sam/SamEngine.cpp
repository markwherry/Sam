#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

#include "ALog.h"
#include "AMemoryManager.h"
#include "APrecisionTimer.h"

#include "AMath.h"

// Music Tools
#include "SamStepRepeater.h"
#include "SamTranspose.h"
#include "SamLimitMaxVelocity.h"
#include "SamDetune.h"
#include "SamCentipede.h"
#include "SamConverge.h"
#include "SamCrossfadeStringsSetup.h"
#include "SamVelocitySender.h"
#include "SamDoubler.h"
#include "SamRunulator.h"
#include "SamOffNote.h"
#include "SamVpModulator.h"
#include "SamArtSeq.h"

// Audio Tools
#include "SamGain.h"
#include "SamAtDelay.h"

SamEngine* SamEngine::m_instance = 0;

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEngine::SamEngine(AModel *parent)
: AModel(parent)
, m_testTone(false)
, m_multiCore(true)
, m_totalVoicesPlaying(0)
, m_totalVoicesPeak(0)
, m_midiControllerFloodgate(false)
, m_loadSampleDataWithInstrument(true)
, m_channelPitchBendRemoteActive(false)
, m_channelPitchBendRemoteActiveOnMidi(false)
, m_channelPitchBendRemoteController(5)
{
	m_instance = this;

	m_audioManager = new CsAudioManager(this);

	m_midiManager = new CsMidiManager(this);

	m_realTimeCommandQueue =  new ARingBuffer<CsRealTimeCommand*>(1024);

	m_taskQueue = new ATaskQueue();
	//tsi = new task_scheduler_init(task_scheduler_init::automatic);
	//aLog("task_scheduler_init::default_num_threads() = %d", task_scheduler_init::default_num_threads());
	//m_taskList = new task_list();

	m_osc = new CsOscillator(44100.0);
	m_osc->setAmplitude(0.0625f); // -24dB
	m_oscBuffer = new CsSampleBuffer();
	m_oscBuffer->buffer32 = 0;

	m_project = new SamProject(this);

	// Build MIDI Frequency/Decibel/Gain Tables
	for(int i=0; i<128; i++) {
		m_midiFrequencyTable[i] = 8.1758 * pow(2,((double)i/12));
		float db = -(20*log10((pow(90.0f,2.0f)/pow(i,2.0f))));
		m_midiVolumeGain[i] = pow(10, (db/20.0f));
		//aLog("m_midiVolumeGain[%d] = %f", i, m_midiVolumeGain[i]);
		//m_rootPitchFrequencyTable[i] = 8.1758 * pow(2.0f,((float)(i*100.0f)/1200.0f));


		//m_midiStreamMixGain[i] = pow((32041.0f/AMath::pow((float)i, 2.0f)),-1.0f);
		//m_midiSendGain[i] = pow((1.0f/127.0f)*i, 4.0f);
		

	}

	// Build MIDI/Cent Frequency Table
	for(int i=0; i < 128 * 100; i++) {
		m_centFreqTable[i] = 8.1758f * pow(2.0f,(float)i/1200);
		//printf("%d = %f\n", i, centFreqTable[i]);
	}

	m_expressionCurve = new CsCurve(128, CsCurve::LinearSquared);
	//m_expressionCurve->print();

	m_musicToolManager = new SamToolManager();
	m_musicToolManager->registerTool(SamArtSeq::createInstance, SamArtSeq::uuid(), SamArtSeq::name());
	m_musicToolManager->registerTool(SamCentipede::createInstance, SamCentipede::uuid(), SamCentipede::name());
	m_musicToolManager->registerTool(SamConverge::createInstance, SamConverge::uuid(), SamConverge::name());
	m_musicToolManager->registerTool(SamCrossfadeStringsSetup::createInstance, SamCrossfadeStringsSetup::uuid(), SamCrossfadeStringsSetup::name());
	//m_musicToolManager->registerTool(SamDetune::createInstance, SamDetune::uuid(), SamDetune::name());
	m_musicToolManager->registerTool(SamDoubler::createInstance, SamDoubler::uuid(), SamDoubler::name());
	m_musicToolManager->registerTool(SamLimitMaxVelocity::createInstance, SamLimitMaxVelocity::uuid(), SamLimitMaxVelocity::name());
	//m_musicToolManager->registerTool(SamStepRepeater::createInstance, SamStepRepeater::uuid(), SamStepRepeater::name());
	m_musicToolManager->registerTool(SamOffNote::createInstance, SamOffNote::uuid(), SamOffNote::name());
	m_musicToolManager->registerTool(SamRunulator::createInstance, SamRunulator::uuid(), SamRunulator::name());
	m_musicToolManager->registerTool(SamTranspose::createInstance, SamTranspose::uuid(), SamTranspose::name());
	m_musicToolManager->registerTool(SamVelocitySender::createInstance, SamVelocitySender::uuid(), SamVelocitySender::name());
	//m_musicToolManager->registerTool(SamVpModulator::createInstance, SamVpModulator::uuid(), SamVpModulator::name());
	
	
	m_audioToolManager = new SamToolManager();
	//m_audioToolManager->registerTool(SamGain::createInstance, SamGain::uuid(), SamGain::name());
	//m_audioToolManager->registerTool(SamAtDelay::createInstance, SamAtDelay::uuid(), SamAtDelay::name());

	m_vstPluginManager = new CsVstPluginManager();

	m_database = new SamDatabase();

	m_aboutToStartPlaying = false;
	m_playing = false;
	m_playStartPosition = 0.0f;
	m_playStartTimestamp = 0.0f;
	m_playStartSample = 0.0f;

	for(int i=0; i<128; i++) {
		for(int j=0; j<16; j++) {
			for(int k=0; k<128; k++) {
				m_lastControllerValue[i][j][k] = -1;
			}
		}

		if(i<12) {
			m_channelPitchBendRemoteUp[i] = (i+1) * 100;
			m_channelPitchBendRemoteDown[i] = (i+1) * 100;
		} else {
			m_channelPitchBendRemoteUp[i] = 200;
			m_channelPitchBendRemoteDown[i] = 200;
		}

	}




	m_timerItems   = 0;
	m_timerTotal   = 0.0f;;
	m_timerAverage = 0.0f;


	m_midiMessages = new ARingBuffer<CsMidiMessage>(4096);
	m_waitForMidiMessage = new AWaitEvent();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEngine::~SamEngine()
{
	aLog("Deleting m_musicToolManager...");
	delete m_musicToolManager;
	aLog("Deleting m_project...");
	delete m_project;
	aLog("Deleting m_oscBuffer...");
	delete m_oscBuffer;
	aLog("Deleting m_osc...");
	delete m_osc;
	aLog("Deleting m_taskQueue...");
	delete m_taskQueue;
	aLog("Deleting m_midiManager...");
	delete m_midiManager;
	aLog("Deleting m_audioManager...");
	delete m_audioManager;
	aLog("EXIT: SamEngine::~SamEngine()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamEngine* SamEngine::instance(void)
{
	return m_instance;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::threadProcess(void)
{
	/*int numMessages = 0;
	int midiInputBusId = 0;
	SamChannel *channel = 0;

	while(isThreadRunning()) {
		m_waitForMidiMessage->wait();

		m_timer.start();

		numMessages = m_midiMessages->size();
		while(numMessages--) {
			CsMidiMessage midiMessage = m_midiMessages->get();
			midiInputBusId = midiMessage.user;
			
			updateState(ContextMidiInput, (midiMessage.data[2]*65536)+(midiMessage.data[1]*256)+(midiMessage.data[0]), 0, 0, (float)midiInputBusId);

	
			// Send the MIDI event to the appropriate Channels
			if(project()) {
				/*for(int i=0; i<m_project->numChannels(); i++) {
					SamChannel *channel = m_project->channel(i);
			

					//cDebug("channel[%d] bus = %d", i, channel->midiInputPort());
					//aDebug("midiEvent.channel = %d (%d)", midiEvent.channel, t->midiInputChannel());
					//aLog("channel[0].midiInputPort = %d", channel->midiInputPort());
					if(channel->midiInputPort()==midiInputBusId && (channel->midiInputChannel()==0 || channel->midiInputChannel()==midiMessage.channel+1)) {
						//aLog("Sending MIDI Event to channel %d", i);
						channel->processMidiMessage(midiMessage);
					}
				}*/


	/*			for(AListIterator<SamChannel*>item = m_project->channels()->begin(); item != m_project->channels()->end(); ++item) {
					channel = *item;
					if(channel->midiInputPort()==midiInputBusId && (channel->midiInputChannel()==0 || channel->midiInputChannel()==midiMessage.channel+1)) {
						//aLog("Sending MIDI Event to channel %d", i);
						channel->processMidiMessage(midiMessage);
					}
				}

			}
		}

		m_timer.stop();

		m_timerTotal += m_timer.duration();
		m_timerItems++;
		m_timerAverage = (m_timerTotal/m_timerItems)*1000000.0f;

	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEngine::init(AString audioDeviceuuid, AString audioDriverName, int numNetworkMidiInputs)
{
	// Initialise MIDI
	// This needs to happen before the audio )system is initialised, otherwise the built-in Windows synth will fail to open.
	// Although, having the Windows synth open causes big, big problems, so leaving it deactivated is preferable in any case.
	m_midiManager->init(numNetworkMidiInputs);

	// Set up the MIDI input buses
	// Note: should this be done everytime? Or should we store it and compare?
	// MIDI Input Bus = 0 implies "No MIDI Input"
	// aLog("m_midiManager->numDevices() = %d", m_midiManager->numDevices());
	for(int i=0; i<m_midiManager->numDevices(); i++) {
		CsMidiDevice *device = m_midiManager->device(i);
		for(int j=0; j<device->numPorts(CsMidiDevice::Input); j++) {
			SamMidiBus *midiBus = new SamMidiBus(this, m_midiManager, ((i+1)*1000)+j, device->portName(j, CsMidiDevice::Input), i, j);
			m_midiInputBuses.append(midiBus);
		}
	}

	// Initialise audio
	if(m_audioManager->init(audioDeviceuuid)) {
		aLog("m_audioServer->init() Ok");
		if(m_audioManager->open(audioDriverName)) {
			aLog("m_audioServer->open() Ok");
			aLog("Starting...");
			m_audioManager->start();
			aLog("Buffer size = %d", audioProcessInfo().bufferSize);
		}
	}

	//startThread();
	//setPriority(AThread::TimeCritical);
	
	//samEngine->executeRealTimeCommand(new RtcTransport(RtcTransport::Play));

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEngine::exit(void)
{
	aLog("SamEngine::exit()");

	//stopThread();

	// Deinitialise audio
	m_audioManager->stop();
	m_audioManager->close();
	m_audioManager->exit();

	// Clean up MIDI input buses
	for(int i=0; i<m_midiInputBuses.size(); i++) {
		delete m_midiInputBuses[i];
	}
	m_midiInputBuses.clear();

	// Deinitialise MIDI
	m_midiManager->exit();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::executeRealTimeCommand(CsRealTimeCommand *command)
{
	m_realTimeCommandQueue->add(command);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::audioStopped(void)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::processAudio(CsAudioBuffer *inputs, CsAudioBuffer *outputs, double nanos, double samples)
{
	//aLog("SamEngine::processAudio()");

	//APrecisionTimer t;
	

	m_previousTimestamp = m_currentTimestamp;
	m_currentTimestamp = nanos;

	// Handle playback preparation, if we're about to start playing
	// ------------------------------------------------------------
	if(m_aboutToStartPlaying) {
		// Where do we start playing from?
		// These values are needed for both audio and MIDI playback
		m_playStartPosition = 0;//project()->currentPosition();

		m_playStartTimestamp = nanos;

		m_playStartSample = samples;

		m_playing = true;
		m_aboutToStartPlaying = false;
	}

	// Calculate Ppqn Playback Times
	// -----------------------------
	/*CsNanoTime fromPosition = 0;
	CsNanoTime toPosition = 0;
	CsPpqnTime from = 0;
	CsPpqnTime to = 0;
	double nanosPerSample = (double)1000000000.0f/(double)44100.0f;
	double nanosPerBuffer = nanosPerSample*m_audioManager->bufferSize();
	//qDebug("nanosPerBuffer = %f (%lld)", nanosPerBuffer, (CsNanoTime)nanosPerBuffer);

	if(m_playing) {
		//fromPosition = (CsNanoTime)((double)nanosPerSample * (double)(samples-(double)m_playStartSample)) + m_playStartPosition;
		//toPosition = ((double)nanosPerSample * (double)(samples-(double)m_playStartSample)) + (double)nanosPerBuffer + m_playStartPosition;
		//qDebug("%f, %f", fromPosition, toPosition);
		//toPosition--;
		m_currentFromTime = CsBarBeatTime::ticksFromNanos(m_previousTimestamp-m_playStartTimestamp, ppqnResolution(), m_project->tempo()); //from = memory()->currentSong()->calculatePpqnPosition(fromPosition);
		m_currentToTime = CsBarBeatTime::ticksFromNanos(m_currentTimestamp-m_playStartTimestamp, ppqnResolution(), m_project->tempo()); // to = memory()->currentSong()->calculatePpqnPosition(toPosition);
		aLog("SamEngine: from=%lld to=%lld", m_currentFromTime, m_currentToTime);

		//for(int i=0; i<m_memory->currentSong()->numTracks(); i++) {
		//	m_memory->currentSong()->track(i)->processRange(from, to);
		//}
	}*/

	

	if(m_testTone) {
		for(int i=0; i<outputs->numStreams(); i++) {
			m_oscBuffer->buffer32[i] = outputs->stream(i);
		}
		m_osc->renderBuffer(m_oscBuffer);
		return;
	}

	

	if(m_project) {
		if(m_project->numChannels() > 0) {
			SamChannel *channel = 0;
			//t.start();
			for(AListIterator<SamChannel*>item = m_project->channels()->begin(); item != m_project->channels()->end(); ++item) {
			//for(int i=0; i<m_project->numChannels(); i++) {
				//aDebug("channel(%d)", i);
				channel = *item; //project->channel(i);
	//			channel->resetTask();
				////trackList[i]->setTimestampForNextBuffer(timestamp); // Do we ever need this?
				//m_project->channel(i)->setNextPlaybackRange(from, to);
				m_taskQueue->putTask(channel);
			}
			//t.stop();
			//aLog("t = %f", t.duration());
		
			m_taskQueue->start();
			m_taskQueue->waitForAllTasksToComplete();
		}//*/

		//m_taskList->clear();
		//for(int i=0; i<m_project->numChannels(); i++) {
			//m_taskList->push_back(*m_project->channel(i));
		//}


		/*if(m_project->numAudioBuses() > 0) {
			SamAudioBus *audioBus = 0;
			for(AListIterator<SamAudioBus*>item = m_project->audioBuses()->begin(); item != m_project->audioBuses()->end(); ++item) {
				audioBus = *item;
				m_taskQueue->putTask(audioBus);
			}

			m_taskQueue->start();
			m_taskQueue->waitForAllTasksToComplete();
		}//*/


		if(m_project->numAudioHardwareBuses() > 0) {
			SamAudioHardwareBus *audioHardwareBus = 0;
			for(AListIterator<SamAudioHardwareBus*>item = m_project->audioHardwareBuses()->begin(); item != m_project->audioHardwareBuses()->end(); ++item) {
				audioHardwareBus = *item;
			//for(int i=0; i<m_project->numAudioHardwareBuses(); i++) {
				//if(m_project->audioOutputBus(i)->isActive()){
	//				m_project->audioHardwareBus(i)->resetTask();
					audioHardwareBus->setAudioBuffer(outputs);
					m_taskQueue->putTask(audioHardwareBus);
				//}
			}//

		
			m_taskQueue->start();
			m_taskQueue->waitForAllTasksToComplete();
		}//*/

		/*if(m_multiCore) {
			m_taskQueue->run();
			m_taskQueue->waitForAllTasksToComplete();
		} else {
			m_taskQueue->runInCurrentThread();
		}*/
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::postProcessAudio(double nanos, double samples)
{
	if (m_realTimeCommandQueue->size()>0) {
		// Execute one command per buffer, so we don't overload the system
		CsRealTimeCommand *command = m_realTimeCommandQueue->get();
		command->execute(nanos, samples);
		delete command; // Should this be done here? Does it affect performance? Maybe add to a list instead, and delete in a non-critical moment.
	}

	m_totalVoicesPlayingTemp = 0;
	for(int i=0; i<m_project->numChannels(); i++) {
		m_totalVoicesPlayingTemp += m_project->channel(i)->numVoicesPlaying();
	}

	m_totalVoicesPlaying = m_totalVoicesPlayingTemp;
	if(m_totalVoicesPlayingTemp > m_totalVoicesPeak) {
		m_totalVoicesPeak = m_totalVoicesPlayingTemp;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::setAudioProcessConfiguration(CsAudioProcessInfo audioProcessInfo, int inputs, int outputs)
{
	aLog("*** SamEngine::setAudioProcessConfiguration()");

	//if(m_project) {

		// Update AudioHardwareBuses
		//for(int i=0; i<m_project->numAudioHardwareBuses(); i++) {
		//	m_project->audioHardwareBus(i)->setAudioProcessInfo(audioProcessInfo);
		//}

		// Update Channels
		//for(int i=0; i<m_project->numChannels(); i++) {
		//	m_project->channel(i)->setAudioProcessInfo(audioProcessInfo);
		//}

	//}

	m_osc->setSampleRate(audioProcessInfo.sampleRate);
	if(m_oscBuffer->buffer32) {
		aFree(m_oscBuffer->buffer32, sizeof(float*)*numOutputs());
	}
	m_oscBuffer->buffer32 = (float**)aMalloc(sizeof(float*)*outputs);
	m_oscBuffer->numStreams = outputs;
	m_oscBuffer->numFrames = audioProcessInfo.bufferSize;

	CsAudioManagerCallback::setAudioProcessConfiguration(audioProcessInfo, inputs, outputs);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::processMidiMessage(CsMidiMessage midiMessage)
{
	//m_timer.start();
	//aLog("SamEngine::processMidiEvent() %d", midiMessage.data[1]);

	// Identify the MIDI Bus from which the message was sent
	int midiInputBusId = 0;
	int midiBusIndex = 0;
	for(int i=0; i<m_midiInputBuses.size(); i++){
		//cDebug("%d, %d", midiInputBus(i)->deviceId(), midiInputBus(i)->portId());
		if(midiMessage.device == m_midiInputBuses[i]->deviceIndex() && midiMessage.port == m_midiInputBuses[i]->portIndex()){
			midiInputBusId = m_midiInputBuses[i]->id();
			break;
		}
		midiBusIndex++;
	}//*/

	// Identify the MIDI Bus from which the message was sent
	/*int midiInputBusId = 0;
	int midiBusIndex = 0;
	SamMidiBus *inputBus = 0;
	for(AListIterator<SamMidiBus*>item = m_midiInputBuses.begin(); item != m_midiInputBuses.end(); item++) {
		inputBus = *item;
		if (midiMessage.device == inputBus->deviceIndex() && midiMessage.port == inputBus->portIndex()) {
			midiInputBusId = inputBus->id();
			break;
		}
		midiBusIndex++;
	}//*/



	//aLog("midiInputBusId = %d", midiInputBusId);
	if(midiInputBusId == 0) {
		// MIDI Input Bus could not be determinded, so the message is ignored.
		return;
	}


	if(midiMessage.type == CsMidiMessage::Controller && m_midiControllerFloodgate && midiMessage.data[1] != 123) { // Added the 123 check so as not to block All Notes Off 
		if(m_lastControllerValue[midiBusIndex][midiMessage.channel][midiMessage.data[1]] == midiMessage.data[2]) {
			//aLog("Filtered");
			return;
		}
		m_lastControllerValue[midiBusIndex][midiMessage.channel][midiMessage.data[1]] = midiMessage.data[2];
	}

	//midiMessage.user = midiInputBusId;
	//m_midiMessages->add(midiMessage);
	//m_waitForMidiMessage->set();
		

	
	updateState(ContextMidiInput, (midiMessage.data[2]*65536)+(midiMessage.data[1]*256)+(midiMessage.data[0]), 0, 0, (float)midiInputBusId);

	
	// Send the MIDI event to the appropriate Channels
	if(project()) {
		for(int i=0; i<m_project->numChannels(); i++) {
			SamChannel *channel = m_project->channel(i);
			

			//cDebug("channel[%d] bus = %d", i, channel->midiInputPort());
			//aDebug("midiEvent.channel = %d (%d)", midiEvent.channel, t->midiInputChannel());
			//aLog("channel[0].midiInputPort = %d", channel->midiInputPort());
			if(channel->midiInputPort()==midiInputBusId && (channel->midiInputChannel()==0 || channel->midiInputChannel()==midiMessage.channel+1)) {
				//aLog("Sending MIDI Event to channel %d", i);
				channel->processMidiMessage(midiMessage);
			}
			
		}//*/
		/*for(AListIterator<SamChannel*>item = m_project->channels()->begin(); item != m_project->channels()->end(); ++item) {
			SamChannel *channel = *item;
			if(channel->midiInputPort()==midiInputBusId && (channel->midiInputChannel()==0 || channel->midiInputChannel()==midiMessage.channel+1)) {
				//aLog("Sending MIDI Event to channel %d", i);
				channel->processMidiMessage(midiMessage);
			}
		}*/

		/*int numAssignments = m_project->numChannelMidiAssignemnts(midiBusIndex, midiMessage.channel);
		int firstChannelIndex = m_project->firstChannelMidiAssignemnt(midiBusIndex, midiMessage.channel);
		if(firstChannelIndex > -1) {
			if(numAssignments > 1) {
				for(int i=firstChannelIndex; i<m_project->numChannels(); i++) {
					SamChannel *channel = m_project->channel(i);
					if(channel->midiInputPort()==midiInputBusId && (channel->midiInputChannel()==0 || channel->midiInputChannel()==midiMessage.channel+1)) {
						channel->processMidiMessage(midiMessage);
						numAssignments--;
						if(numAssignments == 0) break;
					}
				}
			} else {
				project()->channel(firstChannelIndex)->processMidiMessage(midiMessage);
			}
		} //else aLog("No Assignement [%d], %d, %d", firstChannelIndex, midiBusIndex, midiMessage.channel);
		//*/
	}//*/
	
	/*m_timer.stop();

	m_timerTotal += m_timer.duration();
	m_timerItems++;
	m_timerAverage = (m_timerTotal/m_timerItems)*1000000.0f;//*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::processTempoMessage(double timestamp, float tempo)
{
	if(m_project) {
		m_project->setTempo(tempo);
		//SamEvent ev;
		//ev.type = SamEvent::Tempo;
		//ev.timestamp = timestamp;
		//ev.fValue = tempo;
		//for(int i=0; i<m_project->numChannels(); i++) {
		//	m_project->channel(i)->addEvent(ev);
		//}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstIntPtr VSTCALLBACK SamEngine::HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	//aLog("SamEngine::HostCallback(%d)", opcode);

	VstIntPtr result = 0;

	switch (opcode) {
		case audioMasterCanDo:
			aLog("audioMasterCanDo");
			if(strcmp((char*)ptr, "sizeWindow") == 0) {
				result = 1;
			}
			break;

		case audioMasterAutomate: 
			{
				//aLog("audioMasterAutomate");
				SamAudioToolInsert *ati = insertFromEffect(effect);
				if(ati && ati->numInstances() > 1) {
					ati->parameterChanged(index, opt, effect);
				}
			}
			break;
		case audioMasterVersion:
			aLog("audioMasterVersion %d", kVstVersion);
			result = kVstVersion;
			break;
		case audioMasterCurrentId:
			//aLog("audioMasterCurrentId");
			break;
		case audioMasterIdle:
			//aLog("audioMasterIdle");
			break;
		case audioMasterGetTime:
			//aLog("audioMasterGetTime");
			m_vstTimeInfo.tempo = m_project->tempo();
			return (VstIntPtr)&m_vstTimeInfo;
			break;
		case audioMasterProcessEvents:
			//aLog("audioMasterProcessEvents");
			break;

		case audioMasterGetVendorString: // 32
			aLog("audioMasterGetVendorString");
			strcpy((char*)ptr, "MW");
			break;
		case audioMasterGetProductString: // 33
			aLog("audioMasterGetProductString");
			strcpy((char*)ptr, "Sam");
			break;
		case audioMasterSizeWindow:
			aLog("audioMasterSizeWindow %d, %d, %d, %f", index, value, ptr, opt);
			updateState(ContextAudioTool, SamAudioToolInsert::ItemEditorSizeUpdate, (void*)index, (void*)value);
			result = 1;
			break;
	}

	return result;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamEngine::getSampleRate(void)
{
	return m_audioManager->sampleRate();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEngine::getBufferSize(void)
{
	return m_audioManager->bufferSize();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamEngine::midiInputBusIndex(int midiInputBusId)
{
	for(int i=0; i<m_midiInputBuses.size(); i++) {
		if(m_midiInputBuses[i]->id() == midiInputBusId) {
			return i;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::setTestTone(bool testTone, void *from)
{
	m_testTone = testTone;

	//stateManager()->updateState(SamEngine::ContextEngine, ItemTestTone, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamInstrument* SamEngine::instrumentFromUuid(AString uuid)
{
	aLog("SamEngine::instrumentFromUuid()");
	for(int i=0; i<m_instruments.size(); i++) {
		aLog("%s,%s", uuid.toChar(), m_instruments[i]->uuid().toChar());
		if(uuid == m_instruments[i]->uuid()) {
			return m_instruments[i];
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::addInstrument(SamInstrument *instrument)
{
	m_instruments.append(instrument);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::removeInstrument(SamInstrument *instrument)
{
	for(int i=0; i<m_instruments.size(); i++) {
		if(m_instruments[i] == instrument) {
			SamInstrument::Ptr p = m_instruments[i];
			m_instruments.removeAt(i);
			p = 0;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::resetVoicesPeak(void)
{
	m_totalVoicesPeak = 0;
}

void SamEngine::stop(double timestamp, double samples)
{
	aLog("SamEngine::stop()");

	if(m_playing) {
		//project()->setCurrentPosition(m_playStartPosition+(timestamp-m_playStartTime));

		m_playing = false;
		//m_recording = false;
		//setRecordArmed(false);
		//messageQueue()->sendMessage(msgTransportPlay, msgContextTransport, 0.0);
		//messageQueue()->sendMessage(msgTransportRecord, msgContextTransport, 0.0);

		//stateManager()->updateState(ContextTransport, ItemStop, 0);
	} else {
		//project()->setCurrentPosition(0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamEngine::play(double timestamp, double samples)
{
	aLog("SamEngine::play()");

	if(!m_playing) {
		m_aboutToStartPlaying = true;	
	} else {
		stop(timestamp, samples);
	}

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsPpqnTime SamEngine::calculatePpqnPosition(double time)
{
	return CsBarBeatTime::ticksFromNanos(time, ppqnResolution(), project()->tempo());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
double SamEngine::calculateNanoPosition(CsPpqnTime time)
{
	return CsBarBeatTime::nanosFromTicks(time, ppqnResolution(), project()->tempo());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamEngine::centsToFrequency(float cents)
{
	int c = (int)cents;
	float fraction = cents - c;
	
	return m_centFreqTable[c] + fraction * (m_centFreqTable[c+1] - m_centFreqTable[c]);
}