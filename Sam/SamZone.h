#ifndef __SamZone_h__
#define __SamZone_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamCell.h"
#include "SamEnvelope.h"

class SamZone : public AModel
{
public:
    SamZone(AModel *parent);
	SamZone(AModel *parent, const SamZone& zone, int elementIdOffset = 0, bool parametersOnly = false);
    ~SamZone();

	void setSelected(bool selected, void *from = 0, bool shouldUpdate = true);

	//void buildCellGroupMatrix(void);
	//int getNextCellId(int velocity, int groupPlaybackMode, int currentPlaybackGroup);

	int numCells(void) { return cellList.size(); }
	SamCell* cell(int id) { return cellList[id]; }
	void addCell(SamCell *cell);

	// Accessors
	inline AString name(void) const { return m_name; }
	inline void setName(AString name) { m_name = name; }

	inline AString description(void) const { return m_description; }
	inline void setDescription(AString description) { m_description = description; }

	inline int midiNoteLow(void) const { return m_midiNoteLow; }
	void setMidiNoteLow(int note, void *from = 0, bool shouldUpdate = true);

	inline int midiNoteHigh(void) const { return m_midiNoteHigh; }
	void setMidiNoteHigh(int note, void *from = 0, bool shouldUpdate = true);

	int midiNoteRange(void) { return m_midiNoteHigh - m_midiNoteLow; }
	void setMidiNoteRange(int midiNoteLow, int midiNoteHigh, void *from = 0, bool shouldUpdate = true);
	bool isMidiNoteInRange(int midiNote);

	inline int midiVelocityLow(void) const { return m_midiVelocityLow; }
	void setMidiVelocityLow(int velocity, void *from = 0, bool shouldUpdate = true);

	inline int midiVelocityHigh(void) const { return m_midiVelocityHigh; }
	void setMidiVelocityHigh(int velocity, void *from = 0, bool shouldUpdate = true);

	void setMidiVelocityRange(int midiVelocityLow, int midiVelocityHigh, void *from = 0, bool shouldUpdate = true);
	bool isMidiVelocityInRange(int midiVelocity);

	enum SamDynamicRef {
		DynamicUnassigned = 0,
		DynamicPppp       = 1,
		DynamicPpp        = 2,
		DynamicPp         = 3,
		DynamicP          = 4,
		DynamicMp         = 5,
		DynamicMf         = 6,
		DynamicF          = 7,
		DynamicFf         = 8,
		DynamicFff        = 9,
		DynamicFfff       = 10
	};

	inline int dynamicRef(void) { return m_dynamicRef; }
	inline void setDynamicRef(int dynamicRef) { m_dynamicRef = dynamicRef; }

	enum SamTriggerMode {
		FirstCellOnly = 0,
		RoundRobin    = 1,
		Random        = 2
	};

	inline int triggerMode(void) { return m_triggerMode; }
	inline void setTriggerMode(int triggerMode) { m_triggerMode = triggerMode; }

	inline int singleCellId(void) { return m_singleCellId; }
	inline void setSingleCellId(int cellId) { m_singleCellId = cellId; }

	//inline float attenuation(void) const { return m_attenuation; }
	//void setAttenuation(float attenuation);

	//inline float attenuationLinear(void) const { return m_attenuationLinear; }

	inline float gain(void) const { return m_gain; }
	inline void setGain(float gain);

	inline float gainLinear(void) const { return m_gainLinear; }

	inline float releaseGain(void) { return m_releaseGain; }
	void setReleaseGain(float releaseGain);

	inline float preLoopReleaseGain(void) { return m_preLoopReleaseGain; }
	void setPreLoopReleaseGain(float preLoopReleaseGain);

	inline float releaseGainLinear(void) { return m_releaseGainLinear; }
	inline float preLoopReleaseGainLinear(void) { return m_preLoopReleaseGainLinear; }
	
	inline bool isPredictableVelocity(void) { return m_predictableVelocity; }
	inline void setPredictableVelocity(bool predictableVelocity = true) { m_predictableVelocity = predictableVelocity; }

	inline char predictableVelocityCellIndex(void) const { return m_predictableVelocityCellIndex; }
	inline void setPredictableVelocityCellIndex(char index) { m_predictableVelocityCellIndex = index; }

	//inline bool isFixedVelocity(void) const { return m_fixedVelocity; };
	//inline void setFixedVelocity(bool fixedVelocity = true) { m_fixedVelocity = fixedVelocity; }

	//inline int fixedVelocityValue(void) const { return m_fixedVelocityValue; }
	//inline void setFixedVelocityValue(int velocity) { m_fixedVelocityValue = validateMidiValue(velocity,1); }

	// Crossfade accessors
	inline int cfInStart(void) const { return m_cfInStart; }
	inline void setCfInStart(int cfInStart) { m_cfInStart = cfInStart; }

	inline int cfInEnd(void) const { return m_cfInEnd; }
	inline void setCfInEnd(int cfInEnd) { m_cfInEnd = cfInEnd; }

	inline int cfOutStart(void) const { return m_cfOutStart; }
	inline void setCfOutStart(int cfOutStart) { m_cfOutStart = cfOutStart; }

	inline int cfOutEnd(void) const { return m_cfOutEnd; }
	inline void setCfOutEnd(int cfOutEnd) { m_cfOutEnd = cfOutEnd; }

	inline float cfInCurveDepth(void) const { return m_cfInCurveDepth; }
	inline void setCfInCurveDepth(float curveDepth) { m_cfInCurveDepth = curveDepth; }

	inline float cfOutCurveDepth(void) const { return m_cfOutCurveDepth; }
	inline void setCfOutCurveDepth(float curveDepth) { m_cfOutCurveDepth = curveDepth; }

	inline int groupId(void) { return m_groupId; }
	void setGroupId(int id) { m_groupId = id; }

	inline bool isOneShot(void) { return m_oneShot; }
	void setOneShot(bool oneShot) { m_oneShot = oneShot; }

	inline bool isTrackPitch(void) { return m_trackPitch; }
	void setTrackPitch(bool trackPitch) { m_trackPitch = trackPitch; }

	inline float fineTune(void) const { return m_fineTune; }
	inline void setFineTune(float fineTune) { m_fineTune = fineTune; }

	//void setSelected (bool selected) { m_selected = selected; }
	//inline bool isSelected (void) const { return m_selected; }

	inline bool isEnableZoneOutput(void) { return m_enableZoneOutput; }
	void setEnableZoneOutput(bool enable) { m_enableZoneOutput = enable; }

	inline int outputDebug(void) { return m_output; }
	inline int outputListId(void) { return m_outputListId; }
	void setOutput(int output) { m_output = output; calculateOutputListIds(); }

	//int releaseOutput(void) { return m_releaseOutput; }
	inline int releaseOutputListId(void) { return m_releaseOutputListId; }
	void setReleaseOutput(int releaseOutput) { m_releaseOutput = releaseOutput; calculateOutputListIds();}

	enum SamZoneEnvelope{
		EnvelopeAmp = 0,
		EnvelopeRel = 1,
		EnvelopeFtr = 2
	};
	SamEnvelope* envelope(int id) { return envelopeList[id]; }

	inline int velocitySampleStart(void) { return m_velocitySampleStart; }
	void setVelocitySampleStart(int velocitySampleStart) { m_velocitySampleStart = velocitySampleStart; }

	inline int velocitySampleStartThreshold(void) { return m_velocitySampleStartThreshold; }
	void setVelocitySampleStartThreshold(int velocitySampleStartThreshold) { m_velocitySampleStartThreshold = velocitySampleStartThreshold; }

	inline int sampleStart(void) { return m_sampleStart; }
	void setSampleStart(int sampleStart) { m_sampleStart = sampleStart; }

	inline int attackOffset(void) { return m_attackOffset; }
	void setAttackOffset(int attackOffset) { m_attackOffset = attackOffset; }

	inline int attackOffsetMode(void) { return m_attackOffsetMode; }
	void setAttackOffsetMode(int attackOffsetMode) { m_attackOffsetMode = attackOffsetMode; }

	inline bool isTempoReleaseAdjustment(void) { return m_tempoReleaseAdjustment; }
	void setTempoReleaseAdjustment(bool tempoReleaseAdjustment) { m_tempoReleaseAdjustment = tempoReleaseAdjustment; }

	inline int tempoThreshold(void) { return m_tempoThreshold; }
	void setTempoThreshold(int tempoThreshold) { m_tempoThreshold = tempoThreshold; }

	inline int tempoReleaseDivisor(void) { return m_tempoReleaseDivisor; }
	void setTempoReleaseDivisor(int divisor) { m_tempoReleaseDivisor = divisor; }

	// Filter
	inline bool isFilterEnabled(void) { return m_filterEnabled; }
	void setFilterEnabled(bool enabled);

	inline float cutOff(void) { return m_cutOff; }
	void setCutOff(float cutOff);

	//inline int cutOffController(void) { return m_cutOffController; }
	//void setCutOffController(int controller);
	
	inline float minCutOff(void) { return m_minCutOff; }
	void setMinCutOff(float cutOff);

	//float maxCutOff(void) { return m_maxCutOff; }
	//void setMaxCutOff(float cutOff);

	inline float resonance(void) { return m_resonance; }
	void setResonance(float resonance);

	inline bool isFilterVelocityScaling(void) { return m_filterVelocityScaling; }
	void setFilterVelocityScaling(bool scaling);

	inline float filterVelocityDepth(void) { return m_filterVelocityDepth; }
	void setFilterVelocityDepth(float depth);

	inline bool isFilterVelocityFullRange(void) { return m_filterVelocityFullRange; }
	void setFilterVelocityFullRange(bool fullRange) { m_filterVelocityFullRange = fullRange; }

	inline bool isFilterPitchTracking(void) { return m_filterPitchTracking; }
	void setFilterPitchTracking(bool pitchTracking);

	inline float filterPitchTrackOffset(void) { return m_filterPitchTrackOffset; }
	void setFilterPitchTrackOffset(float offset);

	inline bool isFilterEnvelope(void) { return m_filterEnvelope; }
	void setFilterEnvelope(bool envelope);

	inline bool isFilterParameter(void) { return m_filterParameter; }
	void setFilterParameter(bool parameter);

	inline bool isCreatedByCopy(void) const { return m_createdByCopy; }
	void setCreatedByCopy(bool copied = true) { m_createdByCopy = copied; }

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamZoneStateItem {
		ItemSelected = 0,
		ItemProperty = 1,
		ItemPitch    = 2,
		ItemVelocity = 3
	};

private:
	int validateMidiValue(int value, int lowestValue = 0, int highestValue = 127);
	void calculateOutputListIds(void);

	// Basic attributes
	AString m_name;
	AString m_description;
	int m_midiNoteLow;
	int m_midiNoteHigh;
	int m_midiVelocityLow;
	int m_midiVelocityHigh;
	int m_dynamicRef;
	int m_triggerMode;
	int m_singleCellId;
	//float m_attenuation;
	//float m_attenuationLinear;
	float m_gain;
	float m_gainLinear;
	float m_releaseGain;
	float m_releaseGainLinear;
	float m_preLoopReleaseGain;
	float m_preLoopReleaseGainLinear;
	bool m_predictableVelocity;
	char m_predictableVelocityCellIndex;
	//bool m_fixedVelocity;
	//int m_fixedVelocityValue;
	int m_groupId;
	bool m_oneShot;
	bool m_trackPitch;
	int m_fineTune;

	// Crossfade
	int m_cfInStart;
	int m_cfInEnd;
	int m_cfOutStart;
	int m_cfOutEnd;
	float m_cfInCurveDepth;
	float m_cfOutCurveDepth;

	// Trigger mode
	//int lastCellId; // Moved to SamChannel
	//int lastGroupId; // Moved to SamChannel
	//int lastGroupCellId; // Moved to SamChannel
	int m_numCellsInGroup[16];
	int m_cellGroupMatrix[16][32];
	int highestGroupId;

	//bool m_selected;

	int m_output;
	int m_outputListId;
	int m_releaseOutput;
	int m_releaseOutputListId;
	bool m_enableZoneOutput;

	// Envelopes
	static const int numEnvelopes = 3;
	SamEnvelope *envelopeList[numEnvelopes];

	int m_velocitySampleStart;
	int m_velocitySampleStartThreshold;

	int m_sampleStart;

	int m_attackOffset;
	int m_attackOffsetMode;

	bool m_tempoReleaseAdjustment;
	int m_tempoThreshold;
	int m_tempoReleaseDivisor;
	
	AList<SamCell*> cellList;

	// Filter
	bool m_filterEnabled;
	float m_cutOff;
	//int m_cutOffController;
	float m_minCutOff;
	//float m_maxCutOff;
	float m_resonance;
	bool m_filterVelocityScaling;
	float m_filterVelocityDepth;
	bool m_filterVelocityFullRange;
	bool m_filterPitchTracking;
	bool m_filterEnvelope;
	int m_filterPitchTrackOffset;
	bool m_filterParameter;

	bool m_createdByCopy;
};

#endif // __SamZone_h__
