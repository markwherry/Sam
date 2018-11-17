#include "SamVersion.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVersion::SamVersion(AModel *parent)
	: AModel(parent)
	, m_reference(0)
{
	m_name = "";
	m_defaultArticulation = 0;
	//m_numOutputs = 1;
	m_numVoices = 64;
	//for(int i=0; i<32; i++) {
	//	m_outputName[i] = QString::number(i+1);
	//}

	for(int i=0; i<2; i++) {
		m_musicToolElements[i] = 0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVersion::SamVersion(AModel *parent, const SamVersion& version, bool parametersOnly)
	: AModel(parent)
	, m_reference(version.m_reference)
{
	m_name = version.m_name;
	m_defaultArticulation = version.m_defaultArticulation;
	m_numVoices = version.m_numVoices; 

	for(int i=0; i<version.outputList.size(); i++) {
		SamOutput *output = new SamOutput(this, *version.outputList[i]);
		outputList.append(output);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamVersion::~SamVersion()
{
	for(int i=0; i<articulationList.size(); i++) {
		delete articulationList[i];
	}
	articulationList.clear();

	for(int i=0; i<outputList.size(); i++) {
		delete outputList[i];
	}
	outputList.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArticulation* SamVersion::createArticulation(void)
{
	SamArticulation *articulation = new SamArticulation(this);
	articulation->createLayer();
	articulationList.append(articulation);

	return articulation;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVersion::addArticulation(SamArticulation *articulation)
{
	articulationList.append(articulation);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::getArticulationTriggerId(int id)
{
	if(id >= 0 && id < numArticulations()) {
		return articulationList[id]->id();
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::getArticulationId(int triggerId)
{
	for(int i=0; i<articulationList.size(); i++) {
		if(articulationList[i]->id() == triggerId) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamVersion::isPitchValid(int articulationIndex, int layerIndex, int pitch)
{
	if(articulationIndex < articulationList.size() && articulationIndex >= 0) {

		SamArticulation *a = articulationList[articulationIndex];

		if(layerIndex < 0) {
			for(int i=0; i<a->numLayers(); i++) {
				SamLayer *l = a->layer(i);
				if(l->aliasNote(pitch)>-1) {
					return true;
				}
				
				SamZone *zone = 0;
				for(AListIterator<SamZone*>item = l->zones()->begin(); item != l->zones()->end(); item++) {
					zone = *item;
					if (zone->isMidiNoteInRange(pitch)) {
						return true;
					}

				}
			}
		} else {
			if(layerIndex < a->numLayers() && layerIndex >= 0) {

				SamLayer *l = a->layer(layerIndex);

				if(l->aliasNote(pitch)>-1) {
					return true;
				}

				/*for (int i=0; i<l->numZones(); i++) {
					if (l->zone(i)->isMidiNoteInRange(pitch)) {
						return true;
					}
				}*/
				SamZone *zone = 0;
				for(AListIterator<SamZone*>item = l->zones()->begin(); item != l->zones()->end(); item++) {
					zone = *item;
					if (zone->isMidiNoteInRange(pitch)) {
						return true;
					}

				}

			}
		}

	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVersion::addOutput(SamOutput *output)
{
	aLog("SamVersion::addOutput()");
	outputList.append(output);
	aLog("numOutputs = %d", outputList.size());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOutput* SamVersion::createOutput(AString name)
{
	SamOutput *output = new SamOutput(this);

	output->setId(createOutputId());
	output->setName(name);

	addOutput(output);

	return output;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamVersion::deleteOutput(int id)
{
	/*int idCounter = 0;
	for(std::vector<SamOutput*>::iterator i = outputList.begin(); i != outputList.end(); i++) {
		if(idCounter == id) {
			cDebug("!!!!!!!!!!!!!!!!!! Removing Output");
			outputList.erase(i);
			break;
		}
		idCounter++;
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::outputListId(int id)
{
	for(int i=0; i<numOutputs(); i++) {
		if(outputList[i]->id()==id) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::readElement(AXmlDocument *document, AXmlElement *element)
{
	// Read <Version> attributes
	// ----------------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Version> does not have 'name' attribute.");
		return false;
	}

	/*if(element->hasAttribute("numOutputs")) {
		m_numOutputs = element->attribute("numOutputs").toInt();
	} else {
		cDebug("<Version> does not have 'numOutputs' attribute.");
		//return false;
	}*/

	if(element->hasAttribute("numVoices")) {
		m_numVoices = element->attribute("numVoices").toInt();
		if(m_numVoices > 256) {
			m_numVoices = 256;
		}
		if(m_numVoices < 8) {
			m_numVoices = 8;
		}
	} else {
		//aLog("<Version> does not have 'numVoices' attribute.");
		//return false;
	}

	m_reference = element->attribute("reference", "0").toInt();

	if(element->hasAttribute("defaultArticulation")) {
		m_defaultArticulation = element->attribute("defaultArticulation").toInt();
	} else {
		//aLog("<Version> does not have 'defaultArticulation' attribute.");
		//return false;
	}

	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);
		if(el->tag() == "Articulation") {
			SamArticulation *articulation = new SamArticulation(this);
			//aLog("Reading <Articulation> %d", articulationCounter);
			if(!articulation->readElement(document, el)) {
				aLog("Error reading <Articulation>");
				delete articulation;
				return false;
			}
			articulationList.append(articulation);

			
		} else if(el->tag() == "OutputDescription" || el->tag() == "Output") {

			SamOutput *output = new SamOutput(this);
			//aLog("Reading <Output> %d", outputCounter);
			if(!output->readElement(document, el)) {
				aLog("Error reading <Output>");
				delete output;
				return false;
			}

			outputList.append(output);

		} else {
			aLog("<Version> contains unknown tag: %s", el->tag().toChar());
			return false;
		}
	}

	if(outputList.size() < 1) {
		createOutput("Output");
	}

	aLog("EXIT: SamVersion::readElement()");
	

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::writeElement(AXmlDocument *document, AXmlElement *element)
{
	aLog("SamVersion::saveElement()");

	element->setAttribute("name", m_name);
	//element->setAttribute("numOutputs", m_numOutputs);
	element->setAttribute("numVoices", m_numVoices);
	element->setAttribute("reference", m_reference);
	element->setAttribute("defaultArticulation", m_defaultArticulation);

	for(int i=0; i<numOutputs(); i++) {
		aLog("Saving <Output> #%d", i);
		AXmlElement *elementOutput = document->createElement("Output");
		output(i)->writeElement(document, elementOutput);
		element->addChildElement(elementOutput);
	}

	aLog("Saving <Articulation>s");
	for(int i=0; i<numArticulations(); i++) {
		aLog("Saving <Articulation> #%d", i);
		AXmlElement *elementArticulation = document->createElement("Articulation");
		articulation(i)->writeElement(document, elementArticulation);
		element->addChildElement(elementArticulation);
	}

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamVersion::createOutputId(void)
{
	int id = -1;
	int idCounter = 0;

	while(id<0) {
		bool idTaken = false;
		for(int i=0; i<outputList.size(); i++) {
			if(outputList[i]->id() == idCounter) {
				idTaken = true;
			}
		}
		if(idTaken) {
			idCounter++;
		} else {
			id = idCounter;
		}
	}

	return id;
}