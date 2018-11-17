#include "SamLayer.h"

#include "SamEngine.h"

#include "ALog.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamLayer::SamLayer(AModel *parent)
	: AModel(parent)
	, m_name("Layer")
	, m_zoneSelectMode(0)
	, m_shortNoteMode(false)
	, m_groupPlaybackMode(0)
	, m_groupPlaybackGroup(0)
	, m_pitchBendUp(200)
	, m_pitchBendDown(200)
	, m_fineTune(0)
	, m_downbeatSampleOffset(0)
	, m_downbeatEnvelopeOffset(0)
	, m_midiVelocityLow(1)
	, m_midiVelocityHigh(127)
	, m_midiPitchLow(0)
	, m_midiPitchHigh(127)
	, m_intervalLow(0)
	, m_intervalHigh(0)
	, m_intervalRecorded(0)
	, m_timeFromLastNoteMin(0)
	, m_timeFromLastNoteMax(0)
	, m_timeFromLastNoteMaxInf(false)
	, m_maximumCells(0)
	, m_releaseVoices(23)
	, m_gain(0.0f)
	, m_gainLinear(1.0f)
	, m_pedalMode(0)
	, m_sliceMode(false)
{
	for(int i=0; i<128; i++) {
		m_aliasNote[i] = -1;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamLayer::SamLayer(AModel *parent, const SamLayer& layer, int elementIdOffset, bool parametersOnly)
	: AModel(parent)
	, m_maximumCells(layer.m_maximumCells)
	, m_releaseVoices(layer.m_releaseVoices)
	, m_gain(layer.m_gain)
	, m_gainLinear(layer.m_gainLinear)
	, m_pedalMode(layer.m_pedalMode)
{
	m_name = layer.m_name;
	m_zoneSelectMode = layer.m_zoneSelectMode;
	m_shortNoteMode = layer.m_shortNoteMode;
	m_groupPlaybackMode = layer.m_groupPlaybackMode;
	m_groupPlaybackGroup = layer.m_groupPlaybackGroup;
	m_pitchBendUp = layer.m_pitchBendUp;
	m_pitchBendDown = layer.m_pitchBendDown;
	m_fineTune = layer.m_fineTune;
	m_downbeatSampleOffset = layer.m_downbeatSampleOffset;
	m_downbeatEnvelopeOffset = layer.m_downbeatEnvelopeOffset;
	m_midiVelocityLow = layer.m_midiVelocityLow;
	m_midiVelocityHigh = layer.m_midiVelocityHigh;
	m_midiPitchLow = layer.m_midiPitchLow;
	m_midiPitchHigh = layer.m_midiPitchHigh;
	m_intervalLow = layer.m_intervalLow;
	m_intervalHigh = layer.m_intervalHigh;
	m_intervalRecorded = layer.m_intervalRecorded;
	m_timeFromLastNoteMin = layer.m_timeFromLastNoteMin;
	m_timeFromLastNoteMax = layer.m_timeFromLastNoteMax;
	m_timeFromLastNoteMaxInf = layer.m_timeFromLastNoteMaxInf;
	

	/*for(int i=0; i<layer.groupList.size(); i++) {
		SamVoiceGroup *group = new SamVoiceGroup(this, *layer.groupList[i]);
		groupList.append(group);
	}*/

	if(!parametersOnly) {
		for(int i=0; i<layer.zoneList.size(); i++) {
			SamZone *zone = new SamZone(this, *layer.zoneList[i], elementIdOffset);
			zoneList.append(zone);
		}
	}

	for(int i=0; i<128; i++) {
		m_aliasNote[i] = layer.m_aliasNote[i];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamLayer::~SamLayer()
{
	for(int i=0; i<zoneList.size(); i++) {
		delete zoneList[i];
	}
	zoneList.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::addZone(SamZone *zone, bool shouldUpdate)
{
	zoneList.append(zone);

	if(zone->numCells() > m_maximumCells) {
		m_maximumCells = zone->numCells();
	}

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemZones, this, 0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setName(AString name, void *from, bool shouldUpdate)
{
	m_name = name;

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemName, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setMidiVelocityLow(int velocity, void *from, bool shouldUpdate)
{
	if(velocity < 1) velocity = 1;
	if(velocity > 127) velocity = 127;
	
	m_midiVelocityLow = velocity;

	//if(m_midiVelocityLow >= m_midiVelocityHigh) {
	//	m_midiVelocityLow = m_midiVelocityHigh;
	//}

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemScope, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setMidiVelocityHigh(int velocity, void *from, bool shouldUpdate)
{
	if(velocity < 1) velocity = 1;
	if(velocity > 127) velocity = 127;

	m_midiVelocityHigh = velocity;

	//if(m_midiVelocityHigh <= m_midiVelocityLow) {
	//	m_midiVelocityHigh = m_midiVelocityLow;
	//}

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemScope, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setMidiPitchLow(int pitch, void *from, bool shouldUpdate)
{
	if(pitch < 0) pitch = 0;
	if(pitch > 127) pitch = 127;

	m_midiPitchLow = pitch;

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemScope, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setMidiPitchHigh(int pitch, void *from, bool shouldUpdate)
{
	if(pitch < 0) pitch = 0;
	if(pitch > 127) pitch = 127;

	m_midiPitchHigh = pitch;

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemScope, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::setGain(float gain)
{
	m_gain = gain;
	m_gainLinear = (float)pow(10.0,(float)m_gain/20.0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamLayer::numSelectedZones(void)
{
	int selectedZones = 0;

	for (int i=0; i<zoneList.size(); i++) {
		if (zoneList[i]->isSelected()) {
			selectedZones++;
		}
	}

	return selectedZones;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamZone* SamLayer::getZone(int pitch, int velocity)
{
	//aLog ("SamLayer::getZone(%d, %d)", pitch, velocity);

	/*for (int i=0; i<zoneList.size(); i++) {
		if (zoneList[i]->isMidiNoteInRange (pitch) && zoneList[i]->isMidiVelocityInRange (velocity))
			return zoneList[i];
	}*/

	SamZone *zone = 0;
	for(AListIterator<SamZone*>item = zoneList.begin(); item != zoneList.end(); item++) {
		zone = *item;
		if (zone->isMidiNoteInRange(pitch) && zone->isMidiVelocityInRange(velocity))
			return zone;

	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*SamZone* SamLayer::getZoneByCrossfadeRange(int pitch, int crossfadeValue)
{
	//aLog("Choosing Zone to play for release...");
	for (int i=zoneList.size()-1; i>-1; i--) {
		//aLog("Zone[%d] %d-%d, %d-%d (cf = %d)", i, zoneList[i]->cfInStart(), zoneList[i]->cfOutStart(), zoneList[i]->cfInEnd(), zoneList[i]->cfOutEnd(), crossfadeValue);
		if (zoneList[i]->isMidiNoteInRange(pitch)) {
			((0-0)/2)+0;
			int pointA = ((zoneList[i]->cfInEnd()-zoneList[i]->cfInStart())/2)+zoneList[i]->cfInStart();
			int pointB = ((zoneList[i]->cfOutEnd()-zoneList[i]->cfOutStart())/2)+zoneList[i]->cfOutEnd();
			//aLog("A=%d, B=%d", pointA, pointB);
			if(crossfadeValue >= pointA && crossfadeValue <= pointB) {
			//if (crossfadeValue >=zoneList[i]->cfInStart() && crossfadeValue <= zoneList[i]->cfOutEnd()) { // This was lumpy on cf=64
			//if (crossfadeValue >=zoneList[i]->cfInEnd() && crossfadeValue <= zoneList[i]->cfOutEnd())
				//aLog("Returning Zone");
				return zoneList[i];
			}
		}
	}

	return 0;
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamLayer::getZonesByPitch(SamZone **zones, int pitch)
{
	//aLog ("SamLayer::getZonesByPitch()");

	int count = 0;

	for (int i=0; i<zoneList.size(); i++) {
		if (zoneList[i]->isMidiNoteInRange(pitch)) {
			zones[count] = zoneList[i];
			count++;
		}
	}

	return count;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamLayer::numZonesByPitch(int pitch)
{
	int count = 0;

	/*for (int i=0; i<zoneList.size(); i++) {
		if (zoneList[i]->isMidiNoteInRange(pitch)) {
			count++;
		}
	}*/

	SamZone *zone = 0;
	for(AListIterator<SamZone*>item = zoneList.begin(); item != zoneList.end(); item++) {
		zone = *item;
		if (zone->isMidiNoteInRange(pitch)) {
			count++;
		}

	}

	return count;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::removeZone(SamZone *zone, bool shouldUpdate)
{
	for(int i=0; i<zoneList.size(); i++) {
		if(zoneList[i] == zone) {
			aLog("!!!!!!!!!!!!!!!!!! Removing Zone");
			zoneList.removeAt(i);
			break;
		}
	}

	if(shouldUpdate) {
		updateState(SamEngine::ContextLayer, ItemZones, this, 0);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamLayer::createVoiceGroup(int id, Core::String name)
{
	SamVoiceGroup *group = new SamVoiceGroup(this);
	group->setId(id);
	group->setName(name);
	groupList.append(group);
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamZone* SamLayer::existingZone(int noteLo, int noteHi, int velLo, int velHi)
{
	SamZone *zone = 0;

	for(int i=0; i<zoneList.size(); i++) {
		if(zoneList[i]->midiNoteLow() == noteLo && zoneList[i]->midiNoteHigh() == noteHi && zoneList[i]->midiVelocityLow() == velLo && zoneList[i]->midiVelocityHigh() == velHi) {
			return zoneList[i];
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamLayer::readElement(AXmlDocument *document, AXmlElement *element)
{
	float releaseGainCompatibility = 0;
	float preLoopReleaseGainCompatibility = 0;

	// Read <Layer> attributes
	// -----------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Layer> does not have 'name' attribute.");
		return false;
	}
	if(m_name.length() < 1) {
		m_name = "Layer";
	}

	if(element->hasAttribute("zoneSelectMode")) {
		setZoneSelectMode(element->attribute("zoneSelectMode").toInt());
	} else {
		aLog("<Layer> does not have 'zoneSelectMode' attribute.");
		//return false;
	}

	m_shortNoteMode = (bool)element->attribute("shortNoteMode", "0").toInt();

	if(element->hasAttribute("groupPlaybackScope")) {
		setGroupPlaybackMode(element->attribute("groupPlaybackScope").toInt());
	} else {
		aLog("Warning: <Layer> does not have 'groupPlaybackScope' attribute. 0 is assumed.");
		//return false;
	}

	if(element->hasAttribute("groupPlaybackScopeValue")) {
		setGroupPlaybackGroup(element->attribute("groupPlaybackScopeValue").toInt());
	} else {
		aLog("Warning: <Layer> does not have 'groupPlaybackScopeValue' attribute. 0 is assumed.");
		//return false;
	}

	if(element->hasAttribute("pitchBendUp")) {
		setPitchBendUp(element->attribute("pitchBendUp").toInt());
	} else {
		aLog("Warning: <Layer> does not have 'pitchBendUp' attribute. 0 is assumed.");
		//return false;
	}

	if(element->hasAttribute("pitchBendDown")) {
		setPitchBendDown(element->attribute("pitchBendDown").toInt());
	} else {
		aLog("Warning: <Layer> does not have 'pitchBendDown' attribute. 0 is assumed.");
		//return false;
	}

	if(element->hasAttribute("releaseGain")) {
		aLog("releaseGain");
		//setReleaseGain(element->attribute("releaseGain").toFloat());
		releaseGainCompatibility = element->attribute("releaseGain").toFloat();
		
	} else {
		aLog("Warning: <Layer> does not have 'releaseGain' attribute.");
	}

	if(element->hasAttribute("preLoopReleaseGain")) {
		aLog("preLoopReleaseGain");
		//setPreLoopReleaseGain(element->attribute("preLoopReleaseGain").toFloat());
		preLoopReleaseGainCompatibility = element->attribute("preLoopReleaseGain").toFloat();
		
	} else {
		aLog("Warning: <Layer> does not have 'preLoopReleaseGain' attribute.");
	}

	if(element->hasAttribute("fineTune")) {
		setFineTune(element->attribute("fineTune").toFloat());
	} else {
		aLog("Warning: <Zone> does not have 'fineTune' attribute. 0 will be assumed.");
		//return false;
	}

	if(element->hasAttribute("downbeatSampleOffset")) {
		setDownbeatSampleOffset(element->attribute("downbeatSampleOffset").toInt());
	} else {
	}

	if(element->hasAttribute("downbeatEnvelopeOffset")) {
		setDownbeatEnvelopeOffset(element->attribute("downbeatEnvelopeOffset").toInt());
	} else {
	}

	m_releaseVoices = element->attribute("releaseVoices", "23").toInt();

	m_midiVelocityLow = element->attribute("midiVelocityLow", "1").toInt();
	m_midiVelocityHigh = element->attribute("midiVelocityHigh", "127").toInt();
	m_midiPitchLow = element->attribute("midiPitchLow", "0").toInt();
	m_midiPitchHigh = element->attribute("midiPitchHigh", "127").toInt();
	m_pedalMode = element->attribute("pedalMode", "0").toInt();
	m_sliceMode = (bool)element->attribute("sliceMode", "0").toInt();

	setIntervalLow(element->attribute("intervalLow", "0").toInt());
	setIntervalHigh(element->attribute("intervalHigh", "0").toInt());
	setIntervalRecorded(element->attribute("intervalRecorded", "0").toInt());

	m_timeFromLastNoteMin = element->attribute("timeFromLastNoteMin", "0").toInt();
	m_timeFromLastNoteMax = element->attribute("timeFromLastNoteMax", "0").toInt();
	m_timeFromLastNoteMaxInf = (bool)element->attribute("timeFromLastNoteMaxInf", "0").toInt();


	setGain(element->attribute("gain", "0").toFloat());


	// Read <Groups> and <Zone>s
	// -------------------------
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);

		// Read <Groups>
		// -------------
		/*if(el->tagName().contains("Groups", Qt::CaseInsensitive)) {
			aLog("Reading <Groups>");

			
			for(QDomNode nodeGroup = el->firstChild(); !nodeGroup.isNull(); nodeGroup = nodeGroup.nextSibling()) {
				QDomElement elementGroup = nodeGroup.toElement();
				if(elementGroup.tagName().contains("Group", Qt::CaseInsensitive)) {
					SamVoiceGroup *group = new SamVoiceGroup(this);
					aLog("Reading <Group> %d", groupCounter);
					if(!group->load(document, &elementGroup)) {
						aLog("Error reading <Group>");
						delete group;
						return false;
					}
					groupList.append(group);
				} else {
					aLog(qPrintable("Unrecognised child <"+elementGroup.tagName()+"> in <Groups>"));
					return false;
				}
				groupCounter++;
			}

		// Read <Zone>
		// -----------
		} else*/ if(el->tag() == "Zone") {
			SamZone *zone = new SamZone(this);
			aLog("Reading <Zone> %d", zoneList.size());

			if(!zone->readElement(document, el)) {
				aLog("Error reading <Zone>");
				delete zone;
				return false;
			}
			zoneList.append(zone);

		// Read <AliasNote>
		// ----------------
		} else if(el->tag() == "AliasNote") {
			int pitch = el->attribute("pitch").toInt();
			int alias = el->attribute("alias").toInt();
			m_aliasNote[pitch] = alias;

		// Unknown
		// -------
		} else {
			aLog("Unrecognised child <%s> in <Layer>", el->tag().toChar());
			//return false;
		}
	}

	/*if(groupCounter<1) {
		aLog("Error: Layer must contain at least one group.");
		return false;
	}*/

	if(releaseGainCompatibility!=0) {
		for(int i=0; i<numZones(); i++) {
			zone(i)->setReleaseGain(releaseGainCompatibility);
		}
	}

	if(preLoopReleaseGainCompatibility!=0) {
		for(int i=0; i<numZones(); i++) {
			zone(i)->setPreLoopReleaseGain(preLoopReleaseGainCompatibility);
		}
	}

	calculateMaximumCells();

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamLayer::writeElement(AXmlDocument *document, AXmlElement *element)
{
	aLog("SamLayer::saveElement()");

	element->setAttribute("name", m_name);
	element->setAttribute("zoneSelectMode", m_zoneSelectMode);
	if(m_shortNoteMode) {
		element->setAttribute("shortNoteMode", (int)m_shortNoteMode);
	}
	element->setAttribute("groupPlaybackScope", m_groupPlaybackMode);
	element->setAttribute("groupPlaybackScopeValue", m_groupPlaybackGroup);

	if(m_pitchBendUp != 0) {
		element->setAttribute("pitchBendUp", m_pitchBendUp);
	}

	if(m_pitchBendDown != 0) {
		element->setAttribute("pitchBendDown", m_pitchBendDown);
	}

	if(m_fineTune!=0) {
		element->setAttribute("fineTune", m_fineTune);
	}

	element->setAttribute("releaseVoices", m_releaseVoices);

	if(m_downbeatSampleOffset!=0) {
		element->setAttribute("downbeatSampleOffset", m_downbeatSampleOffset);
	}
	
	if(m_downbeatEnvelopeOffset!=0) {
		element->setAttribute("downbeatEnvelopeOffset", m_downbeatEnvelopeOffset);
	}

	if(m_midiVelocityLow != 1) {
		element->setAttribute("midiVelocityLow", m_midiVelocityLow);
	}

	if(m_midiVelocityHigh != 127) {
		element->setAttribute("midiVelocityHigh", m_midiVelocityHigh);
	}

	if(m_midiPitchLow != 0) {
		element->setAttribute("midiPitchLow", m_midiPitchLow);
	}

	if(m_midiPitchHigh != 127) {
		element->setAttribute("midiPitchHigh", m_midiPitchHigh);
	}

	if(m_pedalMode > 0) {
		element->setAttribute("pedalMode", m_pedalMode);
	}

	if(m_sliceMode) {
		element->setAttribute("sliceMode", (int)m_sliceMode);
	}

	if(m_intervalLow != 0) {
		element->setAttribute("intervalLow", m_intervalLow);
	}

	if(m_intervalHigh != 0) {
		element->setAttribute("intervalHigh", m_intervalHigh);
	}

	if(m_intervalRecorded != 0) {
		element->setAttribute("intervalRecorded", m_intervalRecorded);
	}

	if(m_timeFromLastNoteMin != 0) {
		element->setAttribute("timeFromLastNoteMin", m_timeFromLastNoteMin);
	}

	if(m_timeFromLastNoteMax != 0) {
		element->setAttribute("timeFromLastNoteMax", m_timeFromLastNoteMax);
	}

	if(m_timeFromLastNoteMaxInf) {
		element->setAttribute("timeFromLastNoteMaxInf", (int)m_timeFromLastNoteMaxInf);
	}

	if(m_gain != 0.0f) element->setAttribute("gain", m_gain);

	// Groups
	/*aLog("Saving <VoiceGroups>");
	QDomElement elementGroups = document->createElement("VoiceGroups");
	for(int i=0; i<numVoiceGroups(); i++) {
		aLog("Saving <VoiceGroup> #%d", i);
		QDomElement elementGroup = document->createElement("VoiceGroup");
		voiceGroup(i)->save(document, &elementGroup);
		elementGroups.appendChild(elementGroup);
	}
	element->appendChild(elementGroups);*/

	// Zones
	aLog("Saving <Zones>");
	for(int i=0; i<numZones(); i++) {
		aLog("Saving <Zone> #%d", i);
		AXmlElement *elementZone = document->createElement("Zone");
		zone(i)->writeElement(document, elementZone);
		element->addChildElement(elementZone);
	}

	// Alias Notes
	aLog("Saving <AliasNote>s");
	for(int i=0; i<128; i++) {
		if(m_aliasNote[i]>-1) {
			AXmlElement *elementAliasNote = document->createElement("AliasNote");
			elementAliasNote->setAttribute("pitch", i);
			elementAliasNote->setAttribute("alias", m_aliasNote[i]);
			element->addChildElement(elementAliasNote);
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamLayer::calculateMaximumCells(void)
{
	m_maximumCells = 0;
	
	for(int i=0; i<zoneList.size(); i++) {
		if(zoneList[i]->numCells() > m_maximumCells) {
			m_maximumCells = zoneList[i]->numCells();
		}
	}
}