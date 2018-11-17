#include "SamZone.h"

#include <math.h>

#include "ALog.h"

//#include "SamLayer.h"
//#include "SamVersion.h"

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamZone::SamZone(AModel *parent)
	: AModel(parent)
	// Filter
	, m_filterEnabled(false)
	, m_cutOff(2000.0f)
	//, m_cutOffController(0)
	, m_minCutOff(100.0f)
	//, m_maxCutOff(22049.0f)
	, m_resonance(0.0f)
	, m_filterVelocityScaling(false)
	, m_filterVelocityDepth(1.0f)
	, m_filterPitchTracking(false)
	, m_filterEnvelope(false)
	, m_filterPitchTrackOffset(0)
	, m_filterVelocityFullRange(false)
	, m_filterParameter(true)
	, m_createdByCopy(false)
{
	m_name = "";
	m_description = "";
	m_midiNoteLow = 0;
	m_midiNoteHigh = 0;
	m_midiVelocityLow = 0;
	m_midiVelocityHigh = 0;
	m_dynamicRef = DynamicUnassigned;
	m_triggerMode = Random; //kSamTriggerFirstCellOnly;
	m_singleCellId = 0;
	//m_attenuation = 0;
	//m_attenuationLinear = 1.0;
	m_gain = 0;
	m_gainLinear = 1.0;
	m_releaseGain = 0.0f;
	m_releaseGainLinear = 1.0f;

	m_preLoopReleaseGain = -6.0f;
	m_preLoopReleaseGainLinear = 0.5f;
	m_predictableVelocity = false;
	m_predictableVelocityCellIndex = 0;
	//m_fixedVelocity = false;
	//m_fixedVelocityValue = 100;
	m_oneShot = false;
	m_trackPitch = true;
	m_fineTune = 0;

	m_cfInStart = 0;
	m_cfInEnd = 0;
	m_cfOutStart = 0;
	m_cfOutEnd = 0;
	m_cfInCurveDepth = 0.0f;
	m_cfOutCurveDepth = 0.0f;

	m_groupId = 0;

	//lastCellId = 0;
	//lastGroupId = 0;
	//lastGroupCellId = 0;

	// Random number seed for trigger mode stuff
	//srand(time(NULL));

	//m_selected = false;

	m_output = 0;
	m_outputListId = 0;
	m_releaseOutput = 0;
	m_releaseOutputListId = 0;
	m_enableZoneOutput = false;

	for(int i=0; i<numEnvelopes; i++) {
		envelopeList[i] = new SamEnvelope(this);
	}

	m_velocitySampleStart = 0;
	m_velocitySampleStartThreshold = 0;
	m_sampleStart = 0;

	m_attackOffset = 0;
	m_attackOffsetMode = 0;

	m_tempoReleaseAdjustment = false;
	m_tempoThreshold = 120;
	m_tempoReleaseDivisor = 2;

	//buildCellGroupMatrix();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamZone::SamZone(AModel *parent, const SamZone& zone, int elementIdOffset, bool parametersOnly)
	: AModel(parent)
	, m_filterEnabled(zone.m_filterEnabled)
	, m_cutOff(zone.m_cutOff)
	//, m_cutOffController(zone.m_cutOffController)
	, m_minCutOff(zone.m_minCutOff)
	//, m_maxCutOff(22049.0f)
	, m_resonance(zone.m_resonance)
	, m_filterVelocityScaling(zone.m_filterVelocityScaling)
	, m_filterVelocityDepth(zone.m_filterVelocityDepth)
	, m_filterPitchTracking(zone.m_filterPitchTracking)
	, m_filterEnvelope(zone.m_filterEnvelope)
	, m_filterPitchTrackOffset(zone.m_filterPitchTrackOffset)
	, m_filterVelocityFullRange(zone.m_filterVelocityFullRange)
	, m_filterParameter(zone.m_filterParameter)
	, m_createdByCopy(false)
{
	m_name = zone.m_name;
	m_description = zone.m_description;
	m_midiNoteLow = zone.m_midiNoteLow;
	m_midiNoteHigh = zone.m_midiNoteHigh;
	m_midiVelocityLow = zone.m_midiVelocityLow;
	m_midiVelocityHigh = zone.m_midiVelocityHigh;
	m_dynamicRef = zone.m_dynamicRef;
	m_triggerMode = zone.m_triggerMode;
	m_singleCellId = zone.m_singleCellId;
	//m_attenuation = zone.m_attenuation;
	//m_attenuationLinear = zone.m_attenuationLinear;
	m_gain = zone.m_gain;
	m_gainLinear = zone.m_gainLinear;
	m_releaseGain = zone.m_releaseGain;
	m_releaseGainLinear = zone.m_releaseGainLinear;

	m_preLoopReleaseGain = zone.m_preLoopReleaseGain;
	m_preLoopReleaseGainLinear = zone.m_preLoopReleaseGainLinear;
	m_predictableVelocity = zone.m_predictableVelocity;
	m_predictableVelocityCellIndex = zone.m_predictableVelocityCellIndex;
	//m_fixedVelocity = zone.m_fixedVelocity;
	//m_fixedVelocityValue = zone.m_fixedVelocityValue;
	m_oneShot = zone.m_oneShot;
	m_trackPitch = zone.m_trackPitch;
	m_fineTune = zone.m_fineTune;

	m_output = zone.m_output;
	m_outputListId = zone.m_outputListId;
	m_releaseOutput = zone.m_releaseOutput;
	m_releaseOutputListId = zone.m_releaseOutputListId;
	m_enableZoneOutput = zone.m_enableZoneOutput;

	m_cfInStart = zone.m_cfInStart;
	m_cfInEnd = zone.m_cfInEnd;
	m_cfOutStart = zone.m_cfOutStart;
	m_cfOutEnd = zone.m_cfOutEnd;
	m_cfInCurveDepth = zone.m_cfInCurveDepth;
	m_cfOutCurveDepth = zone.m_cfOutCurveDepth;

	m_groupId = zone.m_groupId;

	//lastCellId = zone.lastCellId;
	//lastGroupId = zone.lastGroupId;
	//lastGroupCellId = zone.lastGroupCellId;

	// Random number seed for trigger mode stuff
	//srand(time(NULL));

	if(!parametersOnly) {
		for(int i=0; i<zone.cellList.size(); i++) {
			SamCell *cell = new SamCell(this, *zone.cellList[i], elementIdOffset);
			cellList.append(cell);
		}
	}

	for(int i=0; i<numEnvelopes; i++) {
		envelopeList[i] = new SamEnvelope(this, *zone.envelopeList[i]);
	}

	m_velocitySampleStart = zone.m_velocitySampleStart;
	m_velocitySampleStartThreshold = zone.m_velocitySampleStartThreshold;
	m_sampleStart = zone.m_sampleStart;

	m_attackOffset = zone.m_attackOffset;
	m_attackOffsetMode = zone.m_attackOffsetMode;

	m_tempoReleaseAdjustment = zone.m_tempoReleaseAdjustment;
	m_tempoThreshold = zone.m_tempoThreshold;
	m_tempoReleaseDivisor = zone.m_tempoReleaseDivisor;

	m_filterVelocityFullRange = zone.m_filterVelocityFullRange;

	//buildCellGroupMatrix();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamZone::~SamZone()
{
	for(int i=0; i<numEnvelopes; i++) {
		delete envelopeList[i];
	}

	for(int i=0; i<cellList.size(); i++) {
		delete cellList[i];
	}
	cellList.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setSelected(bool selected, void *from, bool shouldUpdate)
{
	//aLog("SamZone::setSelected()");

	//if(selected != isSelected()) {
		AModel::setSelected(selected);
		if(shouldUpdate) {
			updateState(SamEngine::ContextZone, SamZone::ItemSelected, this, 0, (float)isSelected());
		}
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiNoteLow(int note, void *from, bool shouldUpdate)
{
	m_midiNoteLow = validateMidiValue(note);

	if(shouldUpdate) {
		updateState(SamEngine::ContextZone, ItemPitch, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiNoteHigh(int note, void *from, bool shouldUpdate)
{
	m_midiNoteHigh = validateMidiValue(note, midiNoteLow());

	if(shouldUpdate) {
		updateState(SamEngine::ContextZone, ItemPitch, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiNoteRange(int midiNoteLow, int midiNoteHigh, void *from, bool shouldUpdate)
{
	setMidiNoteLow(midiNoteLow, from, shouldUpdate);
	setMidiNoteHigh(midiNoteHigh, from, shouldUpdate);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiVelocityLow(int velocity, void *from, bool shouldUpdate)
{
	m_midiVelocityLow = validateMidiValue(velocity,1);

	if(shouldUpdate) {
		updateState(SamEngine::ContextZone, ItemVelocity, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiVelocityHigh(int velocity, void *from, bool shouldUpdate)
{
	m_midiVelocityHigh = validateMidiValue(velocity,midiVelocityLow());

	if(shouldUpdate) {
		updateState(SamEngine::ContextZone, ItemVelocity, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMidiVelocityRange(int midiVelocityLow, int midiVelocityHigh, void *from, bool shouldUpdate)
{
	setMidiVelocityLow(midiVelocityLow, from, shouldUpdate);
	setMidiVelocityHigh(midiVelocityHigh, from, shouldUpdate);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamZone::buildCellGroupMatrix(void)
{
	aLog ("SamZone::buildCellGroupMatrix ()");

	highestGroupId = 0;

	for (int i=0; i<16; i++) {
		m_numCellsInGroup[i] = 0;
		for (int j=0; j<32; j++)
			m_cellGroupMatrix[i][j] = 0;
	}

	for (int i=0; i<numCells(); i++) {
		int group = cellList[i]->cellGroup();
		if (group>highestGroupId)
			highestGroupId = group;
		//aLog ("group=%d", group);
		int count = m_numCellsInGroup[group];
		m_cellGroupMatrix[group][count]=i;
		//aLog ("m_cellGroupMatrix[%d][%d]=%d",group,count,i);
		m_numCellsInGroup[group]++;
	}


}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::addCell(SamCell *cell)
{
	cellList.append(cell);
	//buildCellGroupMatrix();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamZone::isMidiNoteInRange(int midiNote)
{
	if (midiNote>=m_midiNoteLow && midiNote <=m_midiNoteHigh)
		return true;

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamZone::isMidiVelocityInRange(int midiVelocity)
{
	if (midiVelocity>=m_midiVelocityLow && midiVelocity <=m_midiVelocityHigh)
		return true;

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setGain(float gain)
{
	m_gain = gain;
	m_gainLinear = (float)pow(10.0,(float)m_gain/20.0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setReleaseGain(float releaseGain)
{
	m_releaseGain = releaseGain;

	m_releaseGainLinear = (float)pow(10.0,(float)m_releaseGain/20.0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setPreLoopReleaseGain(float preLoopReleaseGain)
{
	m_preLoopReleaseGain = preLoopReleaseGain;

	m_preLoopReleaseGainLinear = (float)pow(10.0,(float)m_preLoopReleaseGain/20.0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setFilterEnabled(bool enabled)
{
	if(enabled != m_filterEnabled) {
		m_filterEnabled = enabled;
		updateState(SamEngine::ContextZone, SamZone::ItemProperty, this, 0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setCutOff(float cutOff)
{
	if(cutOff != m_cutOff) {
		m_cutOff = cutOff;
		if(m_cutOff < 10.0) {
			m_cutOff = 10.0f;
		}
		updateState(SamEngine::ContextZone, SamZone::ItemProperty, this, 0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//void SamZone::setCutOffController(int controller)
//{
//	m_cutOffController = controller;
//}
	
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setMinCutOff(float cutOff)
{
	m_minCutOff = cutOff;
	if(m_minCutOff < 10.0) {
		m_minCutOff = 10.0f;
	}
	if(m_cutOff < m_minCutOff) {
		m_minCutOff = m_cutOff;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamZone::setMaxCutOff(float cutOff)
{
	m_maxCutOff = cutOff;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setResonance(float resonance)
{
	m_resonance = resonance;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::setFilterVelocityScaling(bool scaling)
{
	m_filterVelocityScaling = scaling;
}

void SamZone::setFilterVelocityDepth(float depth)
{
	//aLog("SamZone::setFilterVelocityDepth(%f)", depth);
	m_filterVelocityDepth = depth;
}

void SamZone::setFilterPitchTracking(bool pitchTracking)
{
	m_filterPitchTracking = pitchTracking;
}

void SamZone::setFilterEnvelope(bool envelope)
{
	m_filterEnvelope = envelope;
}

void SamZone::setFilterPitchTrackOffset(float offset)
{
	m_filterPitchTrackOffset = offset;
}

void SamZone::setFilterParameter(bool parameter)
{
	m_filterParameter = parameter;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamZone::readElement(AXmlDocument *document, AXmlElement *element)
{
	// Read <Zone> attributes
	// ----------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		//aLog("Warning: <Layer> does not have 'name' attribute.");
		//return false;
	}

	if(element->hasAttribute("description")) {
		m_description = element->attribute("description");
	} else {
		//aLog("Warning: <Zone> does not have 'description' attribute.");
		//return false;
	}

	if(element->hasAttribute("noteLo")) {
		setMidiNoteLow(element->attribute("noteLo").toInt());
	} else {
		aLog("<Zone> does not have 'noteLo' attribute.");
		return false;
	}

	if(element->hasAttribute("noteHi")) {
		setMidiNoteHigh(element->attribute("noteHi").toInt());
	} else {
		aLog("<Zone> does not have 'noteHi' attribute.");
		return false;
	}

	if(element->hasAttribute("velocityLo")) {
		setMidiVelocityLow(element->attribute("velocityLo").toInt());
	} else {
		aLog("<Zone> does not have 'velocityLo' attribute.");
		return false;
	}

	if(element->hasAttribute("velocityHi")) {
		setMidiVelocityHigh(element->attribute("velocityHi").toInt());
	} else {
		aLog("<Zone> does not have 'velocityHi' attribute.");
		return false;
	}

	if(element->hasAttribute("dynamicRef")) {
		setDynamicRef(element->attribute("dynamicRef").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'dynamicRef' attribute. The dynamic reference will not be assigned.");
		//return false;
	}

	if(element->hasAttribute("triggerMode")) {
		setTriggerMode(element->attribute("triggerMode").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'triggerMode' attribute. 0 (First Cell Only) will be used.");
		//return false;
	}

	if(element->hasAttribute("singleCellId")) {
		setSingleCellId(element->attribute("singleCellId").toInt());
	} else {
		//aLog("Warning: <Zone> does not have 'singleCellId' attribute. 0 will be used.");
		//return false;
	}

	if(element->hasAttribute("attenuation")) {
		aLog("Warning: attenuation has been deprecated. Use gain instead.");
		//setAttenuation(element->attribute("attenuation").toFloat());
	} else {
		//aLog("Warning: <Zone> does not have 'attenuation' attribute. 0 dB will be assumed.");
		//return false;
	}

	if(element->hasAttribute("gain")) {
		setGain(element->attribute("gain").toFloat());
	} else {
		aLog("Warning: <Zone> does not have 'gain' attribute. 0dB will be assumed.");
		//return false;
	}

	if(element->hasAttribute("releaseGain")) {
		aLog("releaseGain");
		setReleaseGain(element->attribute("releaseGain").toFloat());
	} else {
		aLog("Warning: <Layer> does not have 'releaseGain' attribute.");
	}

	if(element->hasAttribute("preLoopReleaseGain")) {
		aLog("preLoopReleaseGain");
		setPreLoopReleaseGain(element->attribute("preLoopReleaseGain").toFloat());
	} else {
		aLog("Warning: <Layer> does not have 'preLoopReleaseGain' attribute.");
	}

	if(element->hasAttribute("predictableVelocity")) {
		setPredictableVelocity((bool)element->attribute("predictableVelocity").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'predictableVelocity' attribute. This will be disabled.");
		//return false;
	}

	m_predictableVelocityCellIndex = (char)element->attribute("predictableVelocityCellIndex", "0").toInt();

	/*if(element->hasAttribute("fixedVelocity")) {
		setFixedVelocity((bool)element->attribute("fixedVelocity").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'fixedVelocity' attribute. 0 will be assumed.");
		//return false;
	}

	if(element->hasAttribute("fixedVelocityValue")) {
		setFixedVelocityValue(element->attribute("fixedVelocityValue").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'fixedVelocityValue' attribute.");
		//return false;
	}*/

	if(element->hasAttribute("oneShot")) {
		setOneShot((bool)element->attribute("oneShot").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'oneShot' attribute.");
		//return false;
	}

	if(element->hasAttribute("trackPitch")) {
		setTrackPitch((bool)element->attribute("trackPitch").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'trackPitch' attribute.");
		//return false;
	}

	if(element->hasAttribute("fineTune")) {
		setFineTune(element->attribute("fineTune").toFloat());
	} else {
		aLog("Warning: <Zone> does not have 'fineTune' attribute. 0 will be assumed.");
		//return false;
	}

	if(element->hasAttribute("enableZoneOutput")) {
		setEnableZoneOutput((bool)element->attribute("enableZoneOutput").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'enableZoneOutput' attribute.");
		//return false;
	}

	if(element->hasAttribute("output")) {
		m_output = element->attribute("output").toInt();
	} else {
		aLog("<Articulation> does not have 'output' attribute.");
		//return false;
	}

	if(element->hasAttribute("releaseOutput")) {
		m_releaseOutput = element->attribute("releaseOutput").toInt();
	} else {
		aLog("<Articulation> does not have 'releaseOutput' attribute.");
		//return false;
	}

	if(element->hasAttribute("velocitySampleStart")) {
		m_velocitySampleStart = element->attribute("velocitySampleStart").toInt();
	} else {
		aLog("<Articulation> does not have 'velocitySampleStart' attribute.");
		//return false;
	}

	if(element->hasAttribute("velocitySampleStartThreshold")) {
		m_velocitySampleStartThreshold = element->attribute("velocitySampleStartThreshold").toInt();
	} else {
		aLog("<Articulation> does not have 'velocitySampleStartThreshold' attribute.");
		//return false;
	}

	if(element->hasAttribute("sampleStart")) {
		m_sampleStart = element->attribute("sampleStart").toInt();
	} else {
		aLog("<Articulation> does not have 'sampleStart' attribute.");
		//return false;
	}

	if(element->hasAttribute("attackOffset")) {
		m_attackOffset = element->attribute("attackOffset").toInt();
	} else {
		aLog("<Articulation> does not have 'attackOffset' attribute.");
		//return false;
	}

	if(element->hasAttribute("attackOffsetMode")) {
		m_attackOffsetMode = element->attribute("attackOffsetMode").toInt();
	} else {
		aLog("<Articulation> does not have 'attackOffsetMode' attribute.");
		//return false;
	}

	// -----
	if(element->hasAttribute("tempoReleaseAdjustment")) {
		setTempoReleaseAdjustment((bool)element->attribute("tempoReleaseAdjustment").toInt());
	} else {
		aLog("Warning: <Zone> does not have 'tempoReleaseAdjustment' attribute.");
		//return false;
	}

	if(element->hasAttribute("tempoThreshold")) {
		m_tempoThreshold = element->attribute("tempoThreshold").toInt();
	} else {
		aLog("<Zone> does not have 'tempoThreshold' attribute.");
		//return false;
	}

	if(element->hasAttribute("tempoReleaseDivisor")) {
		m_tempoReleaseDivisor = element->attribute("tempoReleaseDivisor").toInt();
	} else {
		aLog("<Zone> does not have 'tempoReleaseDivisor' attribute.");
		//return false;
	}

	m_createdByCopy = (bool)element->attribute("createdByCopy", "0").toInt();


	// Read <Cell>s and <Crossfade>s
	// -----------------------------
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);

		// Read <Cell>
		// -------------
		if(el->tag() == "Cell") {
			aLog("Reading <Cell> %d", cellList.size());
			SamCell *cell = new SamCell(this);

			if(!cell->readElement(document, el)) {
				aLog("Error reading <Cell>");
				delete cell;
				return false;
			}
			cellList.append(cell);

		// Read <Crossfade>
		// ----------------
		} else if(el->tag() == "Crossfade") {
			aLog("Reading <Crossfade>");

			if(el->hasAttribute("cfInStart") && el->hasAttribute("cfInEnd") && el->hasAttribute("cfOutStart") && el->hasAttribute("cfOutEnd")) {
				setCfInStart(el->attribute("cfInStart").toInt());
				setCfInEnd(el->attribute("cfInEnd").toInt());
				setCfOutStart(el->attribute("cfOutStart").toInt());
				setCfOutEnd(el->attribute("cfOutEnd").toInt());
				setCfInCurveDepth(el->attribute("cfInCurveDepth", "0.0").toFloat());
				setCfOutCurveDepth(el->attribute("cfOutCurveDepth", "0.0").toFloat());
			} else {
				aLog("Error reading <Crossfade>: Element does not contain all the necessary attributes.");
				return false;
			}

		// Read <Envelope>
		// ---------------
		} else if(el->tag() == "Envelope") {
			
			aLog("Reading <Envelope>");
			if(el->hasAttribute("destination")) {
				int destination = el->attribute("destination").toInt();
				if(destination>-1 && destination < numEnvelopes) {
					envelopeList[destination]->readElement(document, el);

					/*if(destination==1) {
						envelopeList[destination]->setAttackShape(1);
						envelopeList[destination]->setDecayShape(2);
						envelopeList[destination]->setReleaseShape(1);

						envelopeList[destination-1]->setAttackShape(0);
						envelopeList[destination-1]->setDecayShape(2);
						envelopeList[destination-1]->setReleaseShape(1);
					}*/

				} else {
					aLog("Error: <Envelope> doesn't have valid destination.");
				}
			}else {
				aLog("Error: <Envelope> doesn't have destination attribute.");
			}

		// Read <Filter>
		// ----------------
		} else if(el->tag() == "Filter") {
			aLog("Reading <Filter>");

			m_filterEnabled = (bool)el->attribute("enabled", "0").toInt();
			m_cutOff = el->attribute("cutOff").toFloat();
			//m_cutOffController = el->attribute("cutOffController").toFloat();
			m_minCutOff = el->attribute("minCutOff").toFloat();
			//m_maxCutOff = el->attribute("maxCutOff").toFloat();
			m_resonance = el->attribute("resonance").toFloat();
			m_filterVelocityScaling = (bool)el->attribute("velocityScaling").toInt();
			m_filterVelocityDepth = (float)el->attribute("velocityDepth", "1").toFloat();
			m_filterVelocityFullRange = (bool)el->attribute("velocityFullRange", "0").toInt();
			m_filterPitchTracking = (bool)el->attribute("pitchTracking").toInt();
			m_filterPitchTrackOffset = el->attribute("pitchTrackOffset", "0").toInt();
			m_filterEnvelope = (bool)el->attribute("envelope", "0").toInt();
			m_filterParameter = (bool)el->attribute("parameter", "1").toInt();

		}else {
			aLog("Unrecognised child <%s> in <Zone>", el->tag().toChar());
			return false;
		}

		
	}

	//buildCellGroupMatrix();
	calculateOutputListIds();

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamZone::writeElement(AXmlDocument *document, AXmlElement *element)
{
	aLog("SamZone::saveElement()");

	if(m_name.length()>0) {
		element->setAttribute("name", m_name);
	}

	if(m_description.length()>0) {
		element->setAttribute("description", m_description);
	}

	element->setAttribute("noteLo", m_midiNoteLow);
	element->setAttribute("noteHi", m_midiNoteHigh);

	element->setAttribute("velocityLo", m_midiVelocityLow);
	element->setAttribute("velocityHi", m_midiVelocityHigh);

	if(m_dynamicRef>DynamicUnassigned) {
		element->setAttribute("dynamicRef", m_dynamicRef);
	}

	//if(m_triggerMode>kSamTriggerFirstCellOnly) {
	element->setAttribute("triggerMode", m_triggerMode);
	//}

	if(singleCellId()!=0) { 
		element->setAttribute("singleCellId", singleCellId());
	}

	if(m_gain != 0.0f) {
		element->setAttribute("gain", m_gain);
	}

	element->setAttribute("releaseGain", releaseGain());
	element->setAttribute("preLoopReleaseGain", preLoopReleaseGain());

	if(m_predictableVelocity) {
		element->setAttribute("predictableVelocity", (int)m_predictableVelocity);
	}

	if(m_predictableVelocityCellIndex > 0) {
		element->setAttribute("predictableVelocityCellIndex", (int)m_predictableVelocityCellIndex);
	}

	/*if(m_fixedVelocity) {
		element->setAttribute("fixedVelocity", (int)m_fixedVelocity);
	}

	if(m_fixedVelocityValue != 100) {
		element->setAttribute("fixedVelocityValue", m_fixedVelocityValue);
	}*/

	if((m_cfInStart + m_cfInEnd + m_cfOutStart + m_cfOutEnd)>0) {
		AXmlElement *elementCrossfade = document->createElement("Crossfade");
		elementCrossfade->setAttribute("cfInStart", m_cfInStart);
		elementCrossfade->setAttribute("cfInEnd", m_cfInEnd);
		elementCrossfade->setAttribute("cfOutStart", m_cfOutStart);
		elementCrossfade->setAttribute("cfOutEnd", m_cfOutEnd);
		elementCrossfade->setAttribute("cfInCurveDepth", m_cfInCurveDepth);
		elementCrossfade->setAttribute("cfOutCurveDepth", m_cfOutCurveDepth);
		element->addChildElement(elementCrossfade);
	}

	if(m_oneShot) {
		element->setAttribute("oneShot", (int)m_oneShot);
	}

	if(!m_trackPitch) {
		element->setAttribute("trackPitch", (int)m_trackPitch);
	}

	if(m_fineTune!=0) {
		element->setAttribute("fineTune", m_fineTune);
	}

	if(m_enableZoneOutput) {
		element->setAttribute("enableZoneOutput", (int)m_enableZoneOutput);
	}

	if(m_output>0) {
		element->setAttribute("output", m_output);
	}

	if(m_releaseOutput>0) {
		element->setAttribute("releaseOutput", m_releaseOutput);
	}

	// Envelopes
	for(int i=0; i<numEnvelopes; i++) {
		//if(i==0 || (i==1)) {// && hasRelease())) {
			AXmlElement *elementEnvelope = document->createElement("Envelope");
			elementEnvelope->setAttribute("destination", i);
			envelopeList[i]->writeElement(document, elementEnvelope);
			element->addChildElement(elementEnvelope);
		//}
	}

	aLog("Saving Cells");
	for(int i=0; i<numCells(); i++) {
		aLog("Saving <Cell> #%d", i);
		AXmlElement *elementCell = document->createElement("Cell");
		cell(i)->writeElement(document, elementCell);
		element->addChildElement(elementCell);
	}

	if(m_velocitySampleStart>0) {
		element->setAttribute("velocitySampleStart", m_velocitySampleStart);
	}

	if(m_velocitySampleStartThreshold>0) {
		element->setAttribute("velocitySampleStartThreshold", m_velocitySampleStartThreshold);
	}

	if(m_sampleStart>0) {
		element->setAttribute("sampleStart", m_sampleStart);
	}

	if(m_attackOffset>0) {
		element->setAttribute("attackOffset", m_attackOffset);
		element->setAttribute("attackOffsetMode", m_attackOffsetMode);
	}

	element->setAttribute("tempoReleaseAdjustment", m_tempoReleaseAdjustment);
	element->setAttribute("tempoThreshold", m_tempoThreshold);
	element->setAttribute("tempoReleaseDivisor", m_tempoReleaseDivisor);

	// Filter
	AXmlElement *elementFilter = document->createElement("Filter");
	elementFilter->setAttribute("enabled", (int)m_filterEnabled);
	elementFilter->setAttribute("cutOff", m_cutOff);
	//elementFilter->setAttribute("cutOffController", m_cutOffController);
	elementFilter->setAttribute("minCutOff", m_minCutOff);
	//elementFilter->setAttribute("maxCutOff", m_maxCutOff);
	elementFilter->setAttribute("resonance", m_resonance);
	elementFilter->setAttribute("velocityScaling", (int)m_filterVelocityScaling);
	elementFilter->setAttribute("velocityDepth", m_filterVelocityDepth);
	elementFilter->setAttribute("velocityFullRange", (int)m_filterVelocityFullRange);
	elementFilter->setAttribute("pitchTracking", (int)m_filterPitchTracking);
	elementFilter->setAttribute("pitchTrackOffset", (int)m_filterPitchTrackOffset);
	if(m_filterEnvelope) elementFilter->setAttribute("envelope", (int)m_filterEnvelope);
	if(!m_filterParameter) elementFilter->setAttribute("parameter", (int)m_filterParameter);
	element->addChildElement(elementFilter);

	element->setAttribute("createdByCopy", (int)m_createdByCopy);

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamZone::validateMidiValue(int value, int lowestValue, int highestValue)
{
	if(value<lowestValue)
		value = lowestValue;
	
	if(value>highestValue)
		value = highestValue;

	return value;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamZone::calculateOutputListIds(void)
{
	aLog("SamZone::calculateOutputListIds()");

	SamVersion *version = (SamVersion*)parent()->parent()->parent();

	m_outputListId = 0;
	m_releaseOutputListId = 0;

	for(int i=0; i<version->numOutputs(); i++) {
		if(m_output==version->output(i)->id()) {
			m_outputListId = i;
		}
		if(m_releaseOutput==version->output(i)->id()) {
			m_releaseOutputListId = i;
		}
	}

	aLog("m_outputListId = %d", m_outputListId);
	aLog("m_releaseOutputListId = %d", m_releaseOutputListId);
}