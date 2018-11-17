#ifndef __SamChannel_h__
#define __SamChannel_h__

#include "AModel.h"
#include "ATaskOld.h"
#include "AList.h"
#include "ARingBuffer.h"
#include "APrecisionTimer.h"
using namespace Atom;

#include "CsMidiMessage.h"
#include "CsOscillator.h"
#include "CsCurve.h"
//#include "CsSmoothValue.h"

#include "SamAudioBus.h"
#include "SamEvent.h"
#include "SamInstrument.h"
#include "SamVoice.h"
#include "SamParameter.h"
#include "SamPerformanceControls.h"

#include "SamMusicToolInsert.h"

//#include "tbb/task.h"

//#define USE_VOICE_TIMER

class SamChannel : public AModel, public ATask, public AStateListener// public task// 
{
friend SamOscillator;

public:
	SamChannel(AModel *parent);
	~SamChannel();

	void main(void);
	//task* execute(void);

	void onState(AStateEvent *ev);

	void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);
	CsAudioProcessInfo audioProcessInfo(void) { return m_audioProcessInfo; }

	void processMidiMessage(CsMidiMessage midiMessage);
	void processEvent(SamEvent *ev);
	void addEvent(SamEvent ev);
	void queueEvent(SamEvent ev);

	void doNoteOn(SamEvent *ev);
	void playNote(SamEvent *ev);
	void playNoteOnLayer(SamEvent *ev, int layerIndex);
	void doNoteOff(SamEvent *ev);
	void doController(SamEvent *ev);
	void doParameter(SamEvent *ev);
	void doProgram(SamEvent *ev);

	void allNotesOff(void);

	void updateStreamMixer(void); 
	void assignInstrument(SamInstrument *instrument, int versionIndex, int voices = -1);

	// Channel
	void setPositionInList(int position) { m_positionInList = position; }
	int positionInList(void) { return m_positionInList; }

	void setSelected(bool selected, void *from = 0, bool shouldUpdate = true);
	bool isSelected(void) { return m_selected; }

	// Input
	void setMidiInputPort(int port, void *from = 0, bool shouldUpdate = true);
	int midiInputPort(void) { return m_midiInputPort; }

	void setMidiInputChannel(int channel, void *from = 0, bool shouldUpdate = true);
	int midiInputChannel(void) { return m_midiInputChannel; }

	// Audio
	inline int numAudioOutputs(void) { return m_audioOutputBuses.size(); }
	SamAudioBus* audioOutputBus(int outputBusIndex) { return m_audioOutputBuses[outputBusIndex]; }

	void setMute(bool mute, void *from = 0, bool shouldUpdate = true);
	bool isMute(void) { return m_mute; }

	void setSolo(bool solo, void *from = 0, bool shouldUpdate = true);
	bool isSolo(void) { return m_solo; }

	void setPreSoloMuteState(bool state) { m_preSoloMuteState = state; }
	bool preSoloMuteState() { return m_preSoloMuteState; }
	

	// Controls
	//bool isMuted(void) { return m_muted; }
	//void setMuted(bool muted, void *from = 0, bool shouldUpdate = true);

	inline bool isMidiMuted(void) const { return m_midiMuted; }
	void setMidiMuted(bool muted, void *from = 0, bool shouldUpdate = true);

	bool isArticulationLock(void) { return m_articulationLock; }
	void setArticulationLock(bool lock, void *from = 0, bool shouldUpdate = true);

	bool isArticulationMode(void) { return m_articulationMode; }
	void setArticulationMode(bool mode, void *from = 0, bool shouldUpdate = true);

	bool isDownbeatOffset(void) { return m_downbeatOffset; }
	void setDownbeatOffset(bool downbeatOffset, void *from = 0, bool shouldUpdate = true);

	inline bool isLayerMutesEnabled(void) const { return m_layerMutesEnabled; }
	void setLayerMutesEnabled(bool enabled, void *from = 0, bool shouldUpdate = true);


	//
	int currentArticulationIndex(void) { return m_currentArticulationIndex; }
	void setCurrentArticulationIndex(int articulationIndex, void *from = 0, bool shouldUpdate = true);

	int currentDefaultArticulationIndex(void) { return m_currentDefaultArticulationIndex; }
	void setCurrentDefaultArticulationIndex(int articulationIndex, void *from = 0, bool shouldUpdate = true);

	int defaultCfArticulationIndex(void) { return m_defaultCfArticulationIndex; }
	int defaultCfDefaultArticulationIndex(void) { return m_defaultCfDefaultArticulationIndex; }
	void setDefaultCfArticulations(int articulationIndex, int defaultArticulationIndex);


	int currentVersionIndex(void) { return m_currentVersionIndex; }
	void setCurrentLayerIndex(int layerIndex, void *from = 0, bool shouldUpdate = true);
	int currentLayerIndex(int articulationIndex) { return m_currentLayerIndex[articulationIndex]; }
	SamLayer* layer(void);

	int layerSelectMode(int articulationIndex) { return m_layerSelectMode[articulationIndex]; }
	void setLayerSelectMode(int mode, void *from = 0, bool shouldUpdate = true);

	// Items
	enum ChannelItems {
		ItemSelected          =  0,
		ItemMidiInput         =  1,
		ItemMidiInputPort     = 10,
		ItemMidiInputChannel  = 11,
		ItemMidiMuted         = 12,
		ItemArticulation      = 20,
		ItemArticulationLock  = 21,
		ItemArticulationMode  = 22,
		ItemDownbeatOffset    = 23,
		ItemLayerMutesEnabled = 24,
		ItemSendAudioBus      = 31,
		ItemSendGain          = 32,
		ItemSendActive        = 33,
		ItemSendPreFader      = 34,
		ItemLayer             = 40,
		ItemConfig            = 50,
		ItemMute              = 60,
		ItemSolo              = 61,
		ItemPitchBend         = 62
	};

	static const int maxOutputs    = 8;
	static const int maxVoices     = 512;
	static const int overVoices    = 4;
	static const int maxMusicTools = 8;

	SamInstrument* instrument(void) { return m_instrument; }
	SamVersion* version(void) { return m_version; }
	SamStreamMix* streamMix(void) { return m_streamMix; }
	void setStreamMix(SamStreamMix *streamMix);

	inline CsSpeakerConfig speakerConfig(void) const { return m_streamMix->speakerConfig(); }
	void setSpeakerConfig(CsSpeakerConfig config, void *from = 0, bool shouldUpdate = true);

	/*AString perVoiceFxToolUuid(void) { return m_perVoiceFxToolUuid; }
	void setPerVoiceFxTool(AString toolUuid);

	char currentPerVoiceGroup(void) { return m_currentPerVoiceGroup; }
	void setCurrentPerVoiceGroup(char group) { m_currentPerVoiceGroup = group; }

	char perVoiceGroup(int pitch, int velocityZone) { return m_perVoiceGroup[pitch][velocityZone]; }
	void setPerVoiceGroup(int pitch, int velocityZone, char group);

	SamToolParameter* perVoiceParameter(char group, int index) { return m_parameterList[group][index]; }

	void setPerVoiceParameter(char group, int id, float value);

	int perVoiceVelocitySplit(void) { return m_perVoiceVelocitySplit; }
	void setPerVoiceVelocitySplit(int velocity) { m_perVoiceVelocitySplit = velocity; }*/

	float averageVoiceTime(void) { return m_averageVoiceTime; }

	void setNumVoices(int numVoices) { m_updateNumVoices = numVoices; }
	int numVoices(void) { return m_numVoices; }
	int numVoicesPlaying(void) { return m_numVoicesPlaying; }
	SamVoice *voice(int index) { return m_voices[index]; }

	float totalSetGain(int setIndex);
	bool isSetMuted(int setIndex);

	//CsModulator* zoneCrossfadeModulator(void) { return m_zoneCrossfadeModulator; }
	CsModulator* expressionModulator(void) { return m_expressionModulator; }
	CsModulator* articulationCrossfadeModulator(void) { return m_articulationCrossfadeModulator; }

	int currentZoneCrossfade(void) { return m_currentZoneCrossfade; }

	SamPerformanceControls* performanceControls(int articulationIndex) { return m_performanceControls[articulationIndex]; }

	SamMusicToolInsert* musicToolInsert(int index, bool instrument = false);

	CsCurve* curveArticulationCrossfade(void) { return m_curveArticulationCrossfade; } 

	float currentPitchBend(void) { return m_currentPitchBend; }
	int currentPitch(void) { return m_currentPitch; }

	int velocity(int pitch) { return m_notes[pitch].velocity; }
	int aftertouch(int pitch) { return m_notes[pitch].aftertouch; }

	inline float midiNoteGain(int pitch) const { return m_midiNoteGain[pitch]; }
	void setAttenuationForMidiNotes(int attenuationStart, int attenuationEnd, int attenuationFadeIn, int attenuationFadeOut);
	void updateAttenuationsForMidiNotes(void);
	int attenuationStart(void) { return m_attenuationStart; }
	void setAttenuationStart(int attenuationStart) { m_attenuationStart = attenuationStart; }
	int attenuationEnd(void) { return m_attenuationEnd; }
	void setAttenuationEnd(int attenuationEnd) { m_attenuationEnd = attenuationEnd; }
	int attenuationFadeIn(void) { return m_attenuationFadeIn; }
	void setAttenuationFadeIn(int attenuationFadeIn) { m_attenuationFadeIn = attenuationFadeIn; }
	int attenuationFadeOut(void) { return m_attenuationFadeOut; }
	void setAttenuationFadeOut(int attenuationFadeOut) { m_attenuationFadeOut = attenuationFadeOut; }
	float attenuationFadeInQ(void) { return m_curveAttenuationFadeIn->depth(); }
	void setAttenuationFadeInQ(float q) { m_curveAttenuationFadeIn->setDepth(q); }
	float attenuationFadeOutQ(void) { return m_curveAttenuationFadeOut->depth(); }
	void setAttenuationFadeOutQ(float q) { m_curveAttenuationFadeOut->setDepth(q); }

	inline AString label(void) const { return m_label; }
	inline void setLabel(AString label) { m_label = label; }

	inline bool isPitchBend(void) const { return m_pitchBend; }
	void setPitchBend(bool pitchBend, void *from = 0, bool shouldUpdate = true);

	inline int pitchBendUp(void) { return m_pitchBendUp; }
	void setPitchBendUp(int pitchBendUp, void *from = 0, bool shouldUpdate = true);

	inline int pitchBendDown(void) { return m_pitchBendDown; }
	void setPitchBendDown(int pitchBendDown, void *from = 0, bool shouldUpdate = true);

	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	typedef AObjectPtr<SamChannel> Ptr;

protected:
private:
	void voiceOff(int voiceIndex, SamEvent *ev);
	void updateStreamCoefficients(void);
	void allocateVoices(int voices);
	void freeVoices(void);
	SamVoice* nextVoice(SamLayer *layer, int pitch);
	SamVoice* nextReleaseVoice(SamLayer *layer, int pitch);
	int nextCellId(SamZone *zone, int velocity);

	SamVoice *m_voices[maxVoices+overVoices];
	int m_numVoices;
	int m_numVoicesPlaying;
	int m_numVoicesPlayingTemp;
	int m_updateNumVoices;

	int m_positionInList;
	bool m_selected;

	int m_midiInputPort;
	int m_midiInputChannel;
	
	//Core::List<SamAudioBus*> m_audioOutputBuses;

	//bool m_muted;
	bool m_midiMuted;
	bool m_articulationLock;
	bool m_articulationMode;
	bool m_downbeatOffset;
	bool m_mute;
	bool m_solo;
	bool m_preSoloMuteState;

	int m_currentVersionIndex;
	int m_currentArticulationIndex;
	int m_currentDefaultArticulationIndex;
	int m_defaultCfArticulationIndex;
	int m_defaultCfDefaultArticulationIndex;
	int m_lastArticulationMidiValue;
	int m_currentLayerIndex[128];

	CsAudioProcessInfo m_audioProcessInfo;
	//int m_numOutputs;
	AList<SamAudioBus*> m_audioOutputBuses;
	//Core::AudioBuffer *m_audioBuffer[maxOutputs];
	//int m_outputIds[MAX_CHANNEL_OUTPUTS];
	ARingBuffer<SamEvent> *m_events;


	SamInstrument::Ptr m_instrument;
	//int m_versionIndex;
	SamVersion *m_version;
	SamStreamMix *m_streamMix;
	//SamLayer *m_layer;

	CsModulator *m_pitchBendModulator;
	//CsModulator *m_zoneCrossfadeModulator;
	//CsModulator *m_volumeModulator;
	CsModulator *m_expressionModulator;
	CsModulator *m_articulationCrossfadeModulator;
	//CsSmoothValue *m_smoothVolume;

	CsCurve *m_filterCurve;
	CsCurve *m_curveArticulationCrossfade;

	CsOscillator *m_osc;
	//CsSampleBuffer *m_oscBuffer;

	bool m_sustain;

	CsSampleBuffer *m_voiceBuffer;

	float m_streamCoefficients[64][64][16];
	//int m_numAllocatedSets;
	//int *m_numAllocatedStreams;
	//int *m

	APrecisionTimer m_voiceTimer;
	float m_averageVoiceTime;
	float m_voiceTimeTotal;
	unsigned int m_voicesRendered;

	double m_nanosPerSample;

	int m_lastCellId[128];

	int m_currentZoneCrossfade;

	int m_currentFilterCutOff;
	bool m_filterCutOffUpdated;

	int m_layerSelectMode[128];
	int m_previousLayerSelectMode[128];
	int m_previousLayerIndex[128];

	double m_lastNoteOnTimeStamp;
	double m_lastNoteOffTimeStamp;

	SamPerformanceControls *m_performanceControls[128];

	SamMusicToolInsert *m_musicToolInserts[maxMusicTools];
	SamMusicToolInsert *m_musicToolInternals[maxMusicTools];

	float m_currentPitchBend;

	int m_pitchCounter[128];
	int m_releaseCounter;
	int m_lastPitch;
	int m_currentPitch;
	int m_numPitchesDown;


	struct SamNote {
		double timestamp;
		int velocity;
		int articulationIndex;
		int aftertouch;
	};

	SamNote m_notes[128];


	float m_midiNoteGain[128];
	int m_attenuationStart;
	int m_attenuationEnd;
	int m_attenuationFadeIn;
	int m_attenuationFadeOut;
	CsCurve *m_curveAttenuationFadeIn;
	CsCurve *m_curveAttenuationFadeOut;

	bool m_crossfadeUpdated;

	int m_cfCellCounter[128];

	bool m_layerMutesEnabled;
	bool m_layerMute[128][128];

	int m_nextVoiceId;

	float m_midiStreamMixGain[128];
	float m_midiSendGain[128];
	
	char m_lastControllerValue[128];

	AString m_label;

	bool m_pitchBend;
	int m_pitchBendUp;
	int m_pitchBendDown;

	SamEvent m_audioToolEventBuffer[1024];
	int m_audioToolEventBufferCount;
	//
	/*AString m_perVoiceFxToolUuid;
	int m_perVoiceVelocitySplit;
	char m_perVoiceGroup[128][2];
	char m_currentPerVoiceGroup;
	AList<SamToolParameter*> m_parameterList[8];*/
};

#endif // __SamEngine_h__