#ifndef __CsEngine_h__
#define __CsEngine_h__

#include "AModel.h"
#include "AList.h"
#include "AString.h"
#include "ARingBuffer.h"
#include "ATaskQueueOld.h"
#include "APrecisionTimer.h"
using namespace Atom;

#include "tbb/task_scheduler_init.h"
#include "tbb/task.h"
using namespace tbb;

#include "CsAudioManager.h"
#include "CsMidiManager.h"
#include "CsOscillator.h"
#include "CsRealTimeCommand.h"
#include "CsCurve.h"
#include "CsBarBeatTime.h"
#include "CsVstPluginManager.h"
#include "CsVstPluginHost.h"
using namespace Cs;

#include "SamMidiBus.h"
#include "SamInstrument.h"

#include "SamToolManager.h"
#include "SamDatabase.h"

class SamProject;

class SamEngine : public AModel, public CsAudioManagerCallback, public CsMidiManagerCallback, public AThread, public CsVstPluginHost
{
public:
	SamEngine(AModel *parent = 0);
	~SamEngine();

	static SamEngine* instance(void);

	void threadProcess(void);

	int init(AString audioDeviceuuid, AString audioDriverName, int numNetworkMidiInputs = 8);
	int exit(void);

	CsAudioManager *audioManager(void) { return m_audioManager; }

	void executeRealTimeCommand(CsRealTimeCommand *command);

	void audioStopped(void);
	void processAudio(CsAudioBuffer *inputs, CsAudioBuffer *outputs, double nanos, double samples);
	void postProcessAudio(double nanos, double samples);

	void setAudioProcessConfiguration(CsAudioProcessInfo audioProcessInfo, int inputs, int outputs);

	void processMidiMessage(CsMidiMessage midiMessage);
	void processTempoMessage(double timestamp, float tempo);

	// VstHost Overrides
	VstIntPtr VSTCALLBACK HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
	float getSampleRate(void);
	int   getBufferSize(void);

	int numMidiInputBuses(void) const { return m_midiInputBuses.size(); }
	SamMidiBus* midiInputBus(int index) const { return m_midiInputBuses[index]; }
	int midiInputBusIndex(int midiInputBusId);

	void setTestTone(bool testTone, void* from = 0);
	bool isTestTone(void) { return m_testTone; }

	SamInstrument* instrumentFromUuid(AString uuid);
	void addInstrument(SamInstrument *instrument);
	SamInstrument* instrument(int index) { return m_instruments[index]; }
	int numInstruments(void) { return m_instruments.size(); }
	void removeInstrument(SamInstrument *instrument);

	SamProject* project(void) { return m_project; }

	double previousTimestamp(void) const { return m_previousTimestamp; }
	double currentTimestamp(void) const { return m_currentTimestamp; }

	enum SamEngineStateContexts {
		ContextEngine     = 0,
		ContextProject    = 1,
		ContextChannel    = 2,
		ContextAudioBus   = 3,
		ContextStream     = 4,
		ContextSet        = 5,
		ContextMusicTool  = 6,
		ContextTool       = 7,
		ContextAudioTool  = 8,

		ContextInstrument   = 10,
		ContextVersion      = 11,
		ContextArticulation = 12,
		ContextLayer        = 13,
		ContextZone         = 14,
		ContextCell         = 15,
		ContextElement      = 16,

		ContextCommand      = 20,

		ContextMidiInput    = 30
	};

	int totalVoicesPlaying(void) { return m_totalVoicesPlaying; }
	int totalVoicesPeak(void) { return m_totalVoicesPeak; }
	void resetVoicesPeak(void);

	inline float midiFrequencyTable(int pitch) { return m_midiFrequencyTable[pitch]; }
	inline float midiVolumeToGain(int midiValue) const { return m_midiVolumeGain[midiValue]; }
	//float rootPitchFrequencyTable(int pitch) { return m_rootPitchFrequencyTable[pitch]; }
	CsCurve* expressionCurve(void) { return m_expressionCurve; }
	float centsToFrequency(float cents);

	//inline float midiStreamMixGain(int midiValue) { return m_midiStreamMixGain[midiValue]; }
	//inline float midiSendGain(int midiValue) { return m_midiSendGain[midiValue]; }


	SamToolManager* musicToolManager(void) { return m_musicToolManager; }
	SamToolManager* audioToolManager(void) { return m_audioToolManager; }

	CsVstPluginManager *vstPluginManager(void) { return m_vstPluginManager; }

	SamDatabase* database(void) { return m_database; }

	bool isMidiControllerFloodgate(void) { return m_midiControllerFloodgate; }
	void setMidiControllerFloodgate(bool enabled) { m_midiControllerFloodgate = enabled; }

	inline bool isLoadSampleDataWithInstrument(void) const { return m_loadSampleDataWithInstrument; }
	inline void setLoadSampleDataWithInstrument(bool enabled) { m_loadSampleDataWithInstrument = enabled; }

	inline bool isChannelPitchBendRemoteActive(void) const { return m_channelPitchBendRemoteActive; }
	void setChannelPitchBendRemoteActive(bool active) { m_channelPitchBendRemoteActive = active; }

	inline bool isChannelPitchBendRemoteActiveOnMidi(void) const { return m_channelPitchBendRemoteActiveOnMidi; }
	void setChannelPitchBendRemoteActiveOnMidi(bool active) { m_channelPitchBendRemoteActiveOnMidi = active; }

	inline int channelPitchBendRemoteController(void) const { return m_channelPitchBendRemoteController; }
	void setChannelPitchBendRemoteController(int controller) { m_channelPitchBendRemoteController = controller; }
	
	inline int channelPitchBendRemoteUp(int index) const { return m_channelPitchBendRemoteUp[index]; }
	void setChannelPitchBendRemoteUp(int index, int range) { m_channelPitchBendRemoteUp[index] = range; }
	
	inline int channelPitchBendRemoteDown(int index) const { return m_channelPitchBendRemoteDown[index]; }
	void setChannelPitchBendRemoteDown(int index, int range) { m_channelPitchBendRemoteDown[index] = range; }


	// ---
	bool isPlaying(void) { return m_playing; }

	void stop(double timestamp, double samples);
	void play(double timestamp, double samples);

	int ppqnResolution(void) { return 960; }

	CsPpqnTime currentFromTime(void) { return m_currentFromTime; }
	CsPpqnTime currentToTime(void) { return m_currentToTime; }

	double playStartPosition(void) { return m_playStartPosition; }
	double playStartTimestamp(void) { return m_playStartTimestamp; }

	CsPpqnTime calculatePpqnPosition(double time);
	double calculateNanoPosition(CsPpqnTime time);

	float timerAverage(void) { return m_timerAverage; }
	// ---

protected:
private:
	static SamEngine *m_instance;

	CsAudioManager *m_audioManager;
	CsMidiManager *m_midiManager;
	AList<SamMidiBus*> m_midiInputBuses;
	ARingBuffer<CsRealTimeCommand*> *m_realTimeCommandQueue;
	ATaskQueue *m_taskQueue;
	//task_scheduler_init *tsi;
	//task_list* m_taskList;
	bool m_multiCore;

	bool m_testTone;
	CsOscillator *m_osc;
	CsSampleBuffer *m_oscBuffer;

	AList<SamInstrument::Ptr> m_instruments;
	SamProject *m_project;

	double m_previousTimestamp;
	double m_currentTimestamp;

	int m_totalVoicesPlaying;
	int m_totalVoicesPlayingTemp;
	int m_totalVoicesPeak;

	float m_midiFrequencyTable[128];
	float m_midiVolumeGain[128];
	//float m_rootPitchFrequencyTable[128];
	//float m_midiStreamMixGain[128];
	//float m_midiSendGain[128];

	CsCurve *m_expressionCurve;

	SamToolManager *m_musicToolManager;
	SamToolManager *m_audioToolManager;
	CsVstPluginManager *m_vstPluginManager;

	SamDatabase *m_database;

	// ---
	bool m_aboutToStartPlaying;
	bool m_playing;
	CsNanoTime m_playStartPosition;
	CsNanoTime m_playStartTimestamp;
	CsSampleTime m_playStartSample;

	CsPpqnTime m_currentFromTime;
	CsPpqnTime m_currentToTime;
	// ---

	float m_centFreqTable[128 * 100];

	bool m_midiControllerFloodgate;
	bool m_loadSampleDataWithInstrument;

	char m_lastControllerValue[128][16][128]; 

	APrecisionTimer m_timer;
	unsigned int    m_timerItems;
	float           m_timerTotal;
	float           m_timerAverage;

	ARingBuffer<CsMidiMessage> *m_midiMessages;
	AWaitEvent *m_waitForMidiMessage;

	bool m_channelPitchBendRemoteActive;
	bool m_channelPitchBendRemoteActiveOnMidi;
	int m_channelPitchBendRemoteController;
	int m_channelPitchBendRemoteUp[128];
	int m_channelPitchBendRemoteDown[128];
};

#define samEngine SamEngine::instance()

#endif