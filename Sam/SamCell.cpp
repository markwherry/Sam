#include "SamCell.h"

//#include "SamZone.h"
//#include "SamLayer.h"
//#include "SamVersion.h"
#include "SamInstrument.h"
#include "SamEngine.h"

#include <math.h>

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamCell::SamCell(AModel *parent)
: AModel(parent)
, m_elementPool(0)
, m_noteBase(0)
, m_fineTune(0)
, m_loop(false)
, m_loopStart(0)
, m_loopEnd(0)
, m_loopCrossfadeLength(0)
, m_sourceStart(0)
, m_releaseStart(0)
, m_lengthFrames(0)
, m_releaseLengthFrames(0)
, m_sampleRate(44100.0f)
, m_tempo(120.f)
{
	// Get pointers to parents so we can find the Element Pool
	// (Maybe there is a better way of doing this...)

	// zone->layer->articulation->version->instrument
	SamInstrument *instrument = (SamInstrument*)parent->parent()->parent()->parent()->parent();
	m_elementPool = instrument->elementPool();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamCell::SamCell(AModel *parent, const SamCell& cell, int elementIdOffset, bool parametersOnly)
: AModel(parent)
{
	aLog("SamCell::SamCell() Copy Constructor");

	SamInstrument *instrument = (SamInstrument*)parent->parent()->parent()->parent()->parent();
	m_elementPool = instrument->elementPool();

	m_noteBase = cell.m_noteBase;
	m_fineTune = cell.m_fineTune;
	m_loop = cell.m_loop;
	m_loopStart = cell.m_loopStart;
	m_loopEnd = cell.m_loopEnd;
	m_loopCrossfadeLength = cell.m_loopCrossfadeLength;
	m_sourceStart = cell.m_sourceStart;
	m_releaseStart = cell.m_releaseStart;
	
	m_lengthFrames = 0;
	m_releaseLengthFrames = 0;

	if(!parametersOnly) {
		m_lengthFrames = cell.m_lengthFrames;
		m_releaseLengthFrames = cell.m_releaseLengthFrames;

		bool useRelease = false;
		if(cell.m_elements.size()==cell.m_releases.size()) {
			useRelease = true;
		}
	
		for(int i=0; i<cell.m_elements.size(); i++) {
			int releaseId = -1;
			if(useRelease) {
				releaseId = cell.m_releaseIds[i]+elementIdOffset;
			}
			addElement(cell.m_elementIds[i]+elementIdOffset, releaseId);
		}
	}

	aLog("EXIT: SamCell::SamCell() Copy Constructor");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamCell::~SamCell()
{
	m_elements.clear();
	m_releases.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCell::setSelected(bool selected, void *from, bool shouldUpdate)
{
	//aLog("SamCell::setSelected()");

	if(selected != isSelected()) {
		AModel::setSelected(selected);
		if(shouldUpdate) {
			updateState(SamEngine::ContextCell, SamCell::ItemSelected, this, 0, (float)isSelected());
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamCell::addElement(int sourceId, int releaseId)
{
	aLog("SamCell::addElement(%d,%d)", sourceId, releaseId);

	// Source
	SamElement *element = m_elementPool->element(sourceId); aLog("!!!");
	if(!element) {
		aLog("Source Element (id=%d) could not be found.", sourceId);
		return false;
	}
	aLog("Source Element found ok.");

	m_sampleRate = element->sampleRate();

	m_elements.append(element);
	m_elementIds.append(sourceId);
	
	// Release
	if(releaseId>-1) {
		SamElement *elementRelease = m_elementPool->element(releaseId);
		if(!elementRelease) {
			aLog("Release Element (id=%d) could not be found.", releaseId);
			//return false;
			releaseId = sourceId;
			elementRelease = m_elementPool->element(releaseId);
		}
		aLog("Release Element found ok.");

		m_releases.append(elementRelease);
		m_releaseIds.append(releaseId);

		if(m_releaseLengthFrames==0 || m_releaseLengthFrames>elementRelease->lengthFrames()) {
			m_releaseLengthFrames = elementRelease->lengthFrames();
		}
		
	} else {
		aLog("No release for this Element.");
	}

	// Length?
	//if (m_lengthFrames==0 || m_lengthFrames>element->lengthFrames()) {
	//	m_lengthFrames = element->lengthFrames();
	//}
	if(element->lengthFrames() > m_lengthFrames) {
		m_lengthFrames = element->lengthFrames();
	}
	

	aLog("EXIT: SamCell::addElement()");

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamCell::hasRelease(void)
{
	if (m_elements.size()==m_releases.size())
		return true;

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamCell::isSourceRelease(void)
{
	if(hasRelease()){ // This probably isn't necessary
		if(m_elements[0]==m_releases[0]) {
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCell::assignSourceToRelease(void)
{
	clearReleaseElements();

	for(int i=0; i<m_elementIds.size(); i++) {
		int elementId = m_elementIds[i];
		SamElement *elementRelease = m_elementPool->element(elementId);
		m_releases.append(elementRelease);
		m_releaseIds.append(elementId);

		if(m_releaseLengthFrames==0 || m_releaseLengthFrames > elementRelease->lengthFrames()) {
			m_releaseLengthFrames = elementRelease->lengthFrames();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCell::clearReleaseElements(void)
{
	m_releases.clear();
	m_releaseIds.clear();
	m_releaseLengthFrames = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamCell::setLoopCrossfadeLength (long loopCrossfadeLength)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamCell::readElement(AXmlDocument *document, AXmlElement *element)
{
	if(element->hasAttribute("noteBase")) {
		setNoteBase(element->attribute("noteBase").toInt());
	} else {
		aLog("<Cell> does not have 'noteBase' attribute.");
		return false;
	}

	m_fineTune = element->attribute("fineTune", "0.0").toFloat();
	m_loop = (bool)element->attribute("loop", "0").toInt();
	
	if(element->hasAttribute("loopStart")) {
		m_loopStart = element->attribute("loopStart").toFloat();
		if(m_loopStart<0) {
			m_loopStart = 0;
		}
	} else {
		//aLog("Warning: <Cell> does not have 'loopStart' attribute.");
		m_loop = false;
	}

	if(element->hasAttribute("loopEnd")) {
		int loopEnd = element->attribute("loopEnd").toInt(); // Nasty signed/unsigned workaround
		if(loopEnd < 0) {
			loopEnd = 0;
		}
		m_loopEnd = loopEnd;
	} else {
		aLog("Warning: <Cell> does not have 'loopEnd' attribute.");
		m_loopStart = 0.0f;
		m_loop = false;
	}

	m_loopCrossfadeLength = element->attribute("loopCrossfadeLength").toInt();

	m_sourceStart = element->attribute("sourceStart").toInt();
	m_releaseStart = element->attribute("releaseStart").toInt();

	m_releaseGain = element->attribute("releaseGain").toFloat();


	// Read <Element>s
	// ---------------
	for(int i=0; i<element->numChildElements(); i++) {

		AXmlElement *elementCell = element->childElement(i);

		// Read <Element>
		// --------------
		if(elementCell->tag() == "Element") {
			if(elementCell->hasAttribute("source")) {
				if(!addElement(elementCell->attribute("source", "-1").toInt(), elementCell->attribute("release", "-1").toInt())) {
					aLog("Error adding <Element> %d.", i);
					return 0;
				}
				aLog("<Element> %d successfully added.", i);
			} else {
				aLog("<Element> %d has no source", i);			
				return 0;
			}

		// Read <Slices
		// ------------
		} else if(elementCell->tag() == "Slices") {
			m_tempo = elementCell->attribute("tempo", "120.0").toFloat();

			for(int j = 0; j < elementCell->numChildElements(); j++) {
				AXmlElement *elementSlice = elementCell->childElement(j);
				if(elementSlice->tag() == "Slice") {
					SamSlice *slice = new SamSlice(elementSlice->attribute("sample", "0").toInt(),
						                           elementSlice->attribute("ppqn", "0").toInt(),
												   elementSlice->attribute("length", "0").toInt());
					m_slices.append(slice);
				}
			}

		// Read <Envelope>
		// ---------------
		// (This is here for very old backwards compatibility with certain instruments.)
		} else if(elementCell->tag() == "Envelope") {

			SamZone *zone = (SamZone*)parent();

			if(elementCell->hasAttribute("destination")) {
				int destination = elementCell->attribute("destination").toInt();
				if(destination>-1 && destination < 2) {
					zone->envelope(destination)->readElement(document, elementCell);
					if(destination == 1) {
						zone->envelope(destination)->setAttackShape(1);
						zone->envelope(destination)->setDecayShape(2);
						zone->envelope(destination)->setReleaseShape(1);

						zone->envelope(destination-1)->setAttackShape(0);
						zone->envelope(destination-1)->setDecayShape(2);
						zone->envelope(destination-1)->setReleaseShape(1);
					}
				} else {
					aLog("Error: <Envelope> doesn't have valid destination.");
				}
			}
			
		} else {
			aLog("Unrecognised child <%s> in <Cell>", elementCell->tag().toChar());
			//return 0;
		}
	} 

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamCell::writeElement(AXmlDocument *document, AXmlElement *element)
{
	//element->setAttribute("cellGroup", m_cellGroup);
	element->setAttribute("noteBase", m_noteBase);
	if(m_fineTune!=0) {
		element->setAttribute("fineTune", m_fineTune);
	}
	element->setAttribute("loop", (int)m_loop);
	element->setAttribute("loopStart", (int)m_loopStart);
	element->setAttribute("loopEnd", (int)m_loopEnd);
	element->setAttribute("loopCrossfadeLength", m_loopCrossfadeLength);
	element->setAttribute("sourceStart", (int)m_sourceStart);
	element->setAttribute("releaseStart", (int)m_releaseStart);
	//if(m_releaseAttenuation != 0.0f) {
	//	element->setAttribute("releaseAttenuation", m_releaseAttenuation);
	//}
	//if(m_noteOnCounter != 0) {
	//	element->setAttribute("noteOnCounter", m_noteOnCounter);
	//}

	// Envelopes
	//for(int i=0; i<2; i++) {
	//	if(i==0 || (i==1 && hasRelease())) {
	//		QDomElement elementEnvelope = document->createElement("Envelope");
	//		elementEnvelope.setAttribute("destination", i);
	//		envelopeList[i]->saveElement(document, &elementEnvelope);
	//		element->appendChild(elementEnvelope);
	//	}
	//}

	// Elements
	for(int i=0; i<m_elements.size(); i++) {
		AXmlElement *elementElement = document->createElement("Element");

		elementElement->setAttribute("source", m_elementIds[i]);

		if(hasRelease()) {
			elementElement->setAttribute("release", m_releaseIds[i]);
		} else {
			elementElement->setAttribute("release", -1);
		}

		element->addChildElement(elementElement);
	}

	// Slices
	if(m_slices.size() > 0) {
		AXmlElement *elementSlices = document->createElement("Slices");
		elementSlices->setAttribute("tempo", m_tempo);
		for(int i = 0; i < m_slices.size(); i++) {
			AXmlElement *elementSlice = document->createElement("Slice");
			elementSlice->setAttribute("samples", m_slices[i]->samplePos());
			elementSlice->setAttribute("ppqn", m_slices[i]->ppqnPos());
			if(m_slices[i]->length() > 0) {
				elementSlice->setAttribute("length", m_slices[i]->length());
			}
			elementSlices->addChildElement(elementSlice);
		}
		element->addChildElement(elementSlices);
	}

	return 1;
}