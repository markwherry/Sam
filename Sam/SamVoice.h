#ifndef __SamVoice_h__
#define __SamVoice_h__

#include "CsModulator.h"
#include "CsAudioProcessInfo.h"
#include "CsSampleBuffer.h"
#include "CsEnvelope.h"
#include "CsFilter.h"
#include "CsAmplifier.h"
#include "CsLfo.h"

#include "SamOscillator.h"
#include "SamEvent.h"

#include "SamArticulation.h"

#include "SamAudioToolInsert.h"

class SamChannel;

class SamVoice
{
public:
	SamVoice(SamChannel* channel);
	~SamVoice();

	int id(void) { return m_id; }

	SamChannel* channel(void) { return m_channel; }

	void assign(int id, SamEvent *ev, SamCell *cell, bool release, float gainAdjustment = 1.0f, float releaseCutOff = 0.0f);//XeInstrument *instrument, XeZone *zone, CsSeqEvent *ev);//, float velocityAmplitude);
	void release(void);
	void end(int endFrame);
	bool render(CsSampleBuffer *buffer);

	void setAudioProcessInfo(CsAudioProcessInfo audioProcessInfo);

	//void updateStreamMixer(void);

	inline bool isPlaying (void) const { return m_playing; }
	inline bool isReleasing (void) const { return m_releasing; }
	inline bool isRelease(void) const { return m_release; }

	inline bool isSustained(void) { return m_sustained; }
	inline void setSustained(bool sustained = true) { m_sustained = sustained; }

	void setStolen(int count);
	inline int stolen(void) const { return m_stolen; }

	inline int pitch(void) { return m_pitch; }
	inline int velocity(void) { return m_velocity; }

	inline double timestamp(void) { return m_timestamp; }

	//void setPitchBendModulationBuffer(float *pitchBendModulationBuffer) { m_pitchBendModulationBuffer = pitchBendModulationBuffer; }
	void setPitchBendModulator(CsModulator *pitchBendModulator) { m_pitchBendModulator = pitchBendModulator; }

	SamCell* cell(void) { return m_cell; }
	SamZone* zone(void) { return m_zone; }
	SamLayer* layer(void) { return m_layer; }
	inline int layerIndex(void) const { return m_layerIndex; }
	inline int articulationIndex(void) const { return m_articulationIndex; }

	static const int maxOscillators = 64;

	int framePosition(void) const;
	void setReleaseInPlace(void);

	float setZoneCrossfade(int zoneCrossfade);
	int currentZoneCrossfadeMidiValue(void) { return m_currentZoneCrossfadeMidiValue; }
	void setZoneCrossfadeResponseTime(int time);
	void setZoneFilterResponseTime(int time);
	CsModulator* zoneCrossfadeModulator(void) { return m_zoneCrossfadeModulator; }

	void setFilterCutOff(int controllerValue);
	bool isFilterEnabled(void) { return m_filterEnabled; }

	float lastCutOff(void) { return m_lastCutOff; }

	int outputIndex(void) { return m_outputIndex; }

	bool isZoneCrossfade(void) { return m_zoneCrossfade; }

	bool isZoneCrossfadeSilent(void);
	
	float numInputSamplesRequired(int numOutputSamples);

	void setPitchMustUpdate(bool update = true) { m_pitchMustUpdate = update; }

	int triggeredPitch(void) { return m_triggeredPitch; }

	void setPitchModulation(float cents);

	//SamAudioToolInsert *audioToolInsert(void) { return m_audioToolInsert; }

protected:
private:
	int m_id;
	SamChannel* m_channel;
	CsAudioProcessInfo m_audioProcessInfo;

	SamOscillator *m_oscillators[maxOscillators];
	int m_numOscillators;

	CsEnvelopeAhdsr* m_envelope;
	CsEnvelopeAhdsr* m_filterEnvelope;
	CsSynthFilter* m_filter;
	CsAmplifier *m_amplifier;
	CsAmplifier *m_ampZoneCf;
	//CsList<CsSynthComponent*> synthComponents;

	SamZone* m_zone;
	SamCell* m_cell;
	SamLayer* m_layer;
	SamArticulation *m_articulation;
	int m_layerIndex;
	int m_articulationIndex;
	int m_defaultArticulationIndex;
	double m_sampleRate;
	int m_startFrame;
	int m_endFrame;
	int m_pitch;
	int m_velocity;
	float m_velocityAmplitude;
	bool m_playing;
	bool m_releasing;
	bool m_sustained;
	int m_stolen;
	float m_stolenGain;
	float m_stolenInc;
	float m_tuning;
	float *m_pitchModulationBuffer;
	CsModulator *m_pitchBendModulator;
	float m_rootPitchFrequency;
	double m_timestamp;
	int m_outOfRange;
	float m_cutOff;
	float m_lastCutOff;
	int m_startOffset;
	int m_envelopeOffset;

	float m_pitchBendUp;
	float m_pitchBendDn;

	bool m_release;
	
	bool m_filterEnabled;
	//int m_cutOffController;
	float m_lastExpressionValue;
	float m_lastZoneCrossfadeValue;
	int m_lastZoneCrossfade;

	CsModulator *m_zoneCrossfadeModulator;
	CsModulator *m_filterModulator;
	CsModulationSource *m_filterModulationSource;
	
	CsModulator *m_modPitch;
	CsModulationSource *m_mscPitch;
	CsLfo *m_lfoPitch;

	bool m_zoneCrossfade;
	bool m_articulationCrossfade;
	int m_currentZoneCrossfadeMidiValue;

	int m_outputIndex;
	float m_numInputSamplesRequired;

	float m_envAttack;
	float m_envHold;
	float m_emvDecay;
	float m_envSustain;
	float m_envReleaseT;

	float *m_pitchPowBuffer;
	float *m_twoBuffer;
	float *m_reqPitchBuffer;

	bool m_pitchMustUpdate;

	int m_triggeredPitch;

	float m_sampleRateRatio;

	//SamAudioToolInsert *m_audioToolInsert;

	
};
/*
class SamVoice
{
public:
	XeVoice(SamChannel* channel, int numStreams);
	~XeVoice();

	void setAudioProcessInfo(Core::AudioProcessInfo audioProcessInfo);

	//void assign(XeInstrument *instrument, XeZone *zone, CsSeqEvent *ev);//, float velocityAmplitude);
	void release(void);
	void end(int endFrame);
	void render(CsSampleBuffer *buffer);

	inline bool isPlaying (void) const { return m_playing; }
	inline bool isReleasing (void) const { return m_releasing; }

	inline bool isSustained(void) { return m_sustained; }
	inline void setSustained(bool sustained = true) { m_sustained = sustained; }

	void setStolen(bool stolen) { m_stolen = stolen; }
	bool isStolen(void) { return m_stolen; }

	inline int pitch(void) { return m_pitch; }

	inline double timestamp(void) { return m_timestamp; }

	//void setPitchBendModulationBuffer(float *pitchBendModulationBuffer) { m_pitchBendModulationBuffer = pitchBendModulationBuffer; }
	void setPitchBendModulator(CsModulator *pitchBendModulator) { m_pitchBendModulator = pitchBendModulator; }

	int bufferSize(void) { return m_audioProcessInfo.bufferSize; }
	double sampleRate(void) { return m_audioProcessInfo.sampleRate; }

protected:
private:
	SamChannel* m_channel;
	Core::AudioProcessInfo m_audioProcessInfo;
	int m_numStreams;

	XeOscillator* osc;
	CsEnvelopeAhdsr* env;
	CsSynthFilter* ftr;
	CsList<CsSynthComponent*> synthComponents;

	XeZone* m_zone;
	double m_sampleRate;
	int m_startFrame;
	int m_endFrame;
	int m_pitch;
	int m_velocity;
	float m_velocityAmplitude;
	bool m_playing;
	bool m_releasing;
	bool m_sustained;
	bool m_stolen;
	float *m_pitchModulationBuffer;
	CsModulator *m_pitchBendModulator;
	float m_rootPitchFrequency;
	CsNanoTime m_timestamp;

	
};*/

#endif // __XeVoice_h__