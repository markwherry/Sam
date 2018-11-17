#include "SamArticulation.h"
#include "SamVersion.h"
#include "SamEngine.h"

#include <math.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArticulation::SamArticulation(AModel *parent)
	: AModel(parent)
	, m_polyphonicModulation(0)
	, m_maxAftertouch(127)
	, m_velocitySetsCrossfade(false)
	, m_velocityCrossfadeDecay(false)
	, m_velocityCrossfadeDecayTime(0)
	, m_velocityCrossfadeDecayTo(0)
	//, m_minimumVelocityController(-1)
{
	m_name = "";
	m_id = -1;
	m_zoneCrossfadeResponseTime = 10;
	m_zoneFilterResponseTime = 100;
	m_output = 0;
	m_outputListId = 0;
	m_releaseOutput = 0;
	m_releaseOutputListId = 0;
	m_defaultStreamMix = 0;
	m_defaultLayer = 0;
	m_layerSelectMode = SamArticulation::SingleLayer;
	m_switchLayerTime = 12;
	m_legatoOverlap = 0;

	

	m_curveVelocity = new CsCurve(128, 2);
	m_curveVelocityFilter = new CsCurve(128, 2);
	m_curveCrossfadeGain = new CsCurve(128, 2, 0.5);
	//m_curveReleaseAttenuation = new SamCurve(this, 128, 2, 0.8);
	m_curveReleaseAttenuation = new CsCurve(128, 3, 0.8, 0.4);
	m_curveReleaseAttenuation->setDepth(0.4);

	

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArticulation::SamArticulation(AModel *parent, const SamArticulation& articulation, int elementIdOffset, bool parametersOnly)
: AModel(parent)
{
	m_name = articulation.m_name;
	m_id = articulation.m_id;
	m_zoneCrossfadeResponseTime = articulation.m_zoneCrossfadeResponseTime;
	m_zoneFilterResponseTime = articulation.m_zoneFilterResponseTime;
	m_output = articulation.m_output;
	m_releaseOutput = articulation.m_releaseOutput;
	m_defaultStreamMix = articulation.m_defaultStreamMix;
	m_defaultLayer = articulation.m_defaultLayer;
	m_layerSelectMode = articulation.m_layerSelectMode;
	m_velocityCrossfadeDecay = articulation.m_velocityCrossfadeDecay;
	m_velocityCrossfadeDecayTime = articulation.m_velocityCrossfadeDecayTime;
	m_velocityCrossfadeDecayTo = articulation.m_velocityCrossfadeDecayTo;

	m_curveVelocity = new CsCurve(*articulation.m_curveVelocity);
	m_curveVelocityFilter = new CsCurve(*articulation.m_curveVelocityFilter);
	m_curveCrossfadeGain = new CsCurve(*articulation.m_curveCrossfadeGain);
	m_curveReleaseAttenuation = new CsCurve(*articulation.m_curveReleaseAttenuation);

	m_switchLayerTime = articulation.m_switchLayerTime;
	m_legatoOverlap = articulation.m_legatoOverlap;

	m_polyphonicModulation = articulation.m_polyphonicModulation;
	m_maxAftertouch = articulation.m_maxAftertouch;
	m_velocitySetsCrossfade = articulation.m_velocitySetsCrossfade;

	//m_minimumVelocityController = articulation.m_minimumVelocityController;

	if(!parametersOnly) {
		for(int i=0; i<articulation.layerList.size(); i++) {
			SamLayer *layer = new SamLayer(this, *articulation.layerList[i], elementIdOffset);
			layerList.append(layer);
		}
	}

	for(int i=0; i<articulation.parameterList.size(); i++) {
		SamParameter *parameter = new SamParameter(this, *articulation.parameterList[i]);
		parameterList.append(parameter);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamArticulation::~SamArticulation()
{
	delete m_curveVelocity;
	delete m_curveVelocityFilter;
	delete m_curveCrossfadeGain;
	delete m_curveReleaseAttenuation;

	for(int i=0; i<layerList.size(); i++) {
		delete layerList[i];
	}
	layerList.clear();

	for(int i=0; i<parameterList.size(); i++) {
		delete parameterList[i];
	}
	parameterList.clear();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulation::setZoneCrossfadeResponseTime(int time, void *from, bool shouldUpdate)
{
	m_zoneCrossfadeResponseTime = time;

	if(shouldUpdate) {
		updateState(SamEngine::ContextArticulation, ItemZoneCrossfadeResponseTime, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulation::setZoneFilterResponseTime(int time, void *from, bool shouldUpdate)
{
	m_zoneFilterResponseTime = time;

	if(shouldUpdate) {
		updateState(SamEngine::ContextArticulation, ItemZoneFilterResponseTime, this, from);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamLayer* SamArticulation::createLayer(void)
{
	SamLayer *layer = new SamLayer(this);
	//layer->createVoiceGroup(0, "Default");
	layerList.append(layer);

	return layer;
}

int SamArticulation::layerIndex(SamLayer *layer)
{
	for(int i=0; i<layerList.size(); i++) {
		if(layerList[i] == layer) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamParameter* SamArticulation::createParameter(void)
{
	SamParameter *parameter = new SamParameter(this);
	addParameter(parameter);
	return parameter;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulation::addParameter(SamParameter *parameter)
{
	parameter->setParent(this);
	parameterList.append(parameter);
	
	updateState(SamEngine::ContextArticulation, ItemParameterAdded, this, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamArticulation::containsLoops(void)
{
	for(int i=0; i<layerList.size(); i++) {
		for(int j=0; j<layerList[i]->numZones(); j++) {
			for(int k=0; k<layerList[i]->zone(j)->numCells(); k++) {
				if(layerList[i]->zone(j)->cell(k)->isLoop()) {
					return true;
				}
			}

		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulation::removeParameter(int id)
{
	if(id >= 0  && id < parameterList.size()) {
		parameterList.removeAt(id);
		updateState(SamEngine::ContextArticulation, ItemParameterRemoved, this, 0);
	}
	/*std::vector<SamParameter*>::iterator iterator = parameterList.begin();

	for(int i=0; i < parameterList.size(); i++) {
		if(i==id) {
			delete parameterList[i];
			parameterList.erase(iterator);
			break;
		}
		iterator++;
	}

	emit parameterDeleted();*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamArticulation::calculateOutputListIds(void)
{
	aLog("SamArticulation::calculateOutputListIds()");

	SamVersion *version = (SamVersion*)parent();

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

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamArticulation::readElement(AXmlDocument *document, AXmlElement *element)
{
	float releaseGainCompatibility = 0;
	float preLoopReleaseGainCompatibility = 0;
	// Read <Articulation> attributes
	// ----------------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Articulation> does not have 'name' attribute.");
		return false;
	}

	if(element->hasAttribute("triggerId")) {
		m_id = element->attribute("triggerId").toInt();
	} else {
		aLog("<Articulation> does not have 'triggerId' attribute.");
		return false;
	}

	m_zoneCrossfadeResponseTime = element->attribute("zoneCrossfadeResponseTime", "10").toInt();
	m_zoneFilterResponseTime = element->attribute("zoneFilterResponseTime", "100").toInt();

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

	if(element->hasAttribute("defaultStreamMix")) {
		setDefaultStreamMix(element->attribute("defaultStreamMix").toInt());
	} else {
		aLog("Warning: <Articulation> does not have 'defaultStreamMix' attribute. This is optional.");
	}

	if(element->hasAttribute("defaultLayer")) {
		setDefaultLayer(element->attribute("defaultLayer").toInt());
	} else {
		aLog("Warning: <Articulation> does not have 'defaultLayer' attribute. This is optional.");
	}

	if(element->hasAttribute("layerSelectMode")) {
		setLayerSelectMode(element->attribute("layerSelectMode").toInt());
	} else {
		aLog("Warning: <Articulation> does not have 'layerSelectMode' attribute. This is optional.");
	}

	if(element->hasAttribute("switchLayerTime")) {
		setSwitchLayerTime(element->attribute("switchLayerTime").toInt());
	} else {
		aLog("Warning: <Articulation> does not have 'switchLayerTime' attribute. This is optional.");
	}

	m_legatoOverlap = element->attribute("legatoOverlap", "0").toInt();

	m_polyphonicModulation = element->attribute("polyphonicModulation", "0").toInt();

	m_maxAftertouch = element->attribute("maxAftertouch", "127").toInt();
	m_velocitySetsCrossfade = (bool)element->attribute("velocitySetsCrossfade").toInt();
	m_velocityCrossfadeDecay = (bool)element->attribute("velocityCrossfadeDecay").toInt();
	m_velocityCrossfadeDecayTime = element->attribute("velocityCrossfadeDecayTime").toInt();
	m_velocityCrossfadeDecayTo = element->attribute("velocityCrossfadeDecayTo").toInt();

	//m_minimumVelocityController = element->attribute("minimumVelocityController", "-1").toInt();

	// Read <Layer>s and <Parameter>s
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);
		if(el->tag() == "Layer") {
			SamLayer *layer = new SamLayer(this);
			//aLog("Reading <Layer> %d", layerCounter);
			if(!layer->readElement(document, el)) {
				aLog("Error reading <Layer>");
				delete layer;
				return false;
			}
			layerList.append(layer);

			
		} else if(el->tag() == "Parameter") {
			SamParameter *parameter = new SamParameter(this);
			aLog("Reading <Parameter>");
			if(!parameter->readElement(document, el)) {
				aLog("Error reading <Parameter>");
				delete parameter;
				return false;
			}
			parameterList.append(parameter);
			//parameterCounter++;
		} else if(el->tag() == "ReleaseGainCurve") {
			
			aLog(">>>>>>>>>> <ReleaseGainCurve>");

			if(el->hasAttribute("start")) {
				aLog("start");
				m_curveReleaseAttenuation->setStart(el->attribute("start").toFloat());
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'start' attribute.");
			}

			if(el->hasAttribute("end")) {
				aLog("end");
				m_curveReleaseAttenuation->setEnd(el->attribute("end").toFloat());
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'end' attribute.");
			}

			if(el->hasAttribute("q")) {
				aLog("q");
				m_curveReleaseAttenuation->setDepth(el->attribute("q").toFloat());
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'q' attribute.");
			}

			if(el->hasAttribute("shape")) {
				aLog("shape");
				m_curveReleaseAttenuation->setShape(el->attribute("shape").toInt());
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'shape' attribute.");
			}

			if(el->hasAttribute("gain")) {
				aLog("gain");
				releaseGainCompatibility = el->attribute("gain").toFloat();
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'gain' attribute.");
			}

			if(el->hasAttribute("preLoopGain")) {
				aLog("preLoopGain");
				preLoopReleaseGainCompatibility = el->attribute("preLoopGain").toFloat();
			} else {
				aLog("Warning: <ReleaseGainCurve> does not have 'preLoopGain' attribute.");
			}

		} else if(el->tag() == "VelocityCurve") {
			
			aLog(">>>>>>>>>> <VelocityCurve>");

			if(el->hasAttribute("start")) {
				aLog("start");
				m_curveVelocity->setStart(el->attribute("start").toFloat());
			} else {
				aLog("Warning: <VelocityCurve> does not have 'start' attribute.");
			}

			if(el->hasAttribute("end")) {
				aLog("end");
				m_curveVelocity->setEnd(el->attribute("end").toFloat());
			} else {
				aLog("Warning: <VelocityCurve> does not have 'end' attribute.");
			}

			if(el->hasAttribute("q")) {
				aLog("q");
				m_curveVelocity->setDepth(el->attribute("q").toFloat());
			} else {
				aLog("Warning: <VelocityCurve> does not have 'q' attribute.");
			}

			if(el->hasAttribute("shape")) {
				aLog("shape");
				m_curveVelocity->setShape(el->attribute("shape").toInt());
			} else {
				aLog("Warning: <VelocityCurve> does not have 'shape' attribute.");
			}

			/*if(el->hasAttribute("gain")) {
				aLog("gain");
				setReleaseGain(el->attribute("gain").toFloat());
			} else {
				aLog("Warning: <VelocityCurve> does not have 'shape' attribute.");
			}*/
		} else if(el->tag() == "VelocityFilterCurve") {
			
			aLog(">>>>>>>>>> <VelocityFilterCurve>");

			if(el->hasAttribute("start")) {
				aLog("start");
				m_curveVelocityFilter->setStart(el->attribute("start").toFloat());
			} else {
				aLog("Warning: <VelocityFilterCurve> does not have 'start' attribute.");
			}

			if(el->hasAttribute("end")) {
				aLog("end");
				m_curveVelocityFilter->setEnd(el->attribute("end").toFloat());
			} else {
				aLog("Warning: <VelocityFilterCurve> does not have 'end' attribute.");
			}

			if(el->hasAttribute("q")) {
				aLog("q");
				m_curveVelocityFilter->setDepth(el->attribute("q").toFloat());
			} else {
				aLog("Warning: <VelocityFilterCurve> does not have 'q' attribute.");
			}

			if(el->hasAttribute("shape")) {
				aLog("shape");
				m_curveVelocityFilter->setShape(el->attribute("shape").toInt());
			} else {
				aLog("Warning: <VelocityFilterCurve> does not have 'shape' attribute.");
			}

		} else if(el->tag() == "CrossfadeGainCurve") {
			
			aLog(">>>>>>>>>> <CrossfadeGainCurve>");

			if(el->hasAttribute("start")) {
				aLog("start");
				m_curveCrossfadeGain->setStart(el->attribute("start").toFloat());
			} else {
				aLog("Warning: <CrossfadeGainCurve> does not have 'start' attribute.");
			}

			if(el->hasAttribute("end")) {
				aLog("end");
				m_curveCrossfadeGain->setEnd(el->attribute("end").toFloat());
			} else {
				aLog("Warning: <CrossfadeGainCurve> does not have 'end' attribute.");
			}

			if(el->hasAttribute("q")) {
				aLog("q");
				m_curveCrossfadeGain->setDepth(el->attribute("q").toFloat());
			} else {
				aLog("Warning: <CrossfadeGainCurve> does not have 'q' attribute.");
			}

			if(el->hasAttribute("shape")) {
				aLog("shape");
				m_curveCrossfadeGain->setShape(el->attribute("shape").toInt());
			} else {
				aLog("Warning: <CrossfadeGainCurve> does not have 'shape' attribute.");
			}

			/*if(el->hasAttribute("gain")) {
				aLog("gain");
				setReleaseGain(el->attribute("gain").toFloat());
			} else {
				aLog("Warning: <CrossfadeGainCurve> does not have 'shape' attribute.");
			}*/
		}
	}

	calculateOutputListIds();

	if(releaseGainCompatibility!=0) {
		for(int i=0; i<layerList.size(); i++) {
			for(int j=0; j<layerList[i]->numZones(); j++) {
				layerList[i]->zone(i)->setReleaseGain(releaseGainCompatibility);
			}
		}
	}

	if(preLoopReleaseGainCompatibility!=0) {
		for(int i=0; i<layerList.size(); i++) {
			for(int j=0; j<layerList[i]->numZones(); j++) {
				layerList[i]->zone(i)->setPreLoopReleaseGain(preLoopReleaseGainCompatibility);
			}
		}
	}

	aLog("EXIT: SamArticulation::readElement()");

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamArticulation::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("name", m_name);

	element->setAttribute("triggerId", m_id);

	if(m_zoneCrossfadeResponseTime != 10) {
		element->setAttribute("zoneCrossfadeResponseTime", m_zoneCrossfadeResponseTime);
	}

	if(m_zoneFilterResponseTime != 100) {
		element->setAttribute("zoneFilterResponseTime", m_zoneFilterResponseTime);
	}

	element->setAttribute("output", m_output);
	element->setAttribute("releaseOutput", m_releaseOutput);

	element->setAttribute("defaultStreamMix", m_defaultStreamMix);

	element->setAttribute("defaultLayer", m_defaultLayer);

	element->setAttribute("layerSelectMode", m_layerSelectMode);
	element->setAttribute("switchLayerTime", m_switchLayerTime);
	if(m_legatoOverlap != 0) {
		element->setAttribute("legatoOverlap", m_legatoOverlap);
	}

	if(m_polyphonicModulation != 0) {
		element->setAttribute("polyphonicModulation", m_polyphonicModulation);
	}

	element->setAttribute("maxAftertouch", m_maxAftertouch);
	element->setAttribute("velocitySetsCrossfade", (int)m_velocitySetsCrossfade);
	element->setAttribute("velocityCrossfadeDecay", (int)m_velocityCrossfadeDecay);
	element->setAttribute("velocityCrossfadeDecayTime", m_velocityCrossfadeDecayTime);
	element->setAttribute("velocityCrossfadeDecayTo", m_velocityCrossfadeDecayTo);

	//if(m_minimumVelocityController > -1) {
	//	element->setAttribute("minimumVelocityController", m_minimumVelocityController);
	//}
	

	aLog("Saving <VelocityCurve>");
	AXmlElement *elementVelocityCurve = document->createElement("VelocityCurve");
	elementVelocityCurve->setAttribute("start", m_curveVelocity->start());
	elementVelocityCurve->setAttribute("end", m_curveVelocity->end());
	elementVelocityCurve->setAttribute("q", m_curveVelocity->depth());
	elementVelocityCurve->setAttribute("shape", m_curveVelocity->shape());
	element->addChildElement(elementVelocityCurve);

	aLog("Saving <VelocityFilterCurve>");
	AXmlElement *elementVelocityFilterCurve = document->createElement("VelocityFilterCurve");
	elementVelocityFilterCurve->setAttribute("start", m_curveVelocityFilter->start());
	elementVelocityFilterCurve->setAttribute("end", m_curveVelocityFilter->end());
	elementVelocityFilterCurve->setAttribute("q", m_curveVelocityFilter->depth());
	elementVelocityFilterCurve->setAttribute("shape", m_curveVelocityFilter->shape());
	element->addChildElement(elementVelocityFilterCurve);

	aLog("Saving <ReleaseGainCurve>");
	AXmlElement *elementReleaseGainCurve = document->createElement("ReleaseGainCurve");
	elementReleaseGainCurve->setAttribute("start", m_curveReleaseAttenuation->start());
	elementReleaseGainCurve->setAttribute("end", m_curveReleaseAttenuation->end());
	elementReleaseGainCurve->setAttribute("q", m_curveReleaseAttenuation->depth());
	elementReleaseGainCurve->setAttribute("shape", m_curveReleaseAttenuation->shape());
	element->addChildElement(elementReleaseGainCurve);

	aLog("Saving <CrossfadeGainCurve>");
	AXmlElement *elementCrossfadeGainCurve = document->createElement("CrossfadeGainCurve");
	elementCrossfadeGainCurve->setAttribute("start", m_curveCrossfadeGain->start());
	elementCrossfadeGainCurve->setAttribute("end", m_curveCrossfadeGain->end());
	elementCrossfadeGainCurve->setAttribute("q", m_curveCrossfadeGain->depth());
	elementCrossfadeGainCurve->setAttribute("shape", m_curveCrossfadeGain->shape());
	//elementReleaseGainCurve.setAttribute("gain", releaseGain());
	element->addChildElement(elementCrossfadeGainCurve);

	aLog("Saving <Parameter>s");
	for(int i=0; i<numParameters(); i++) {
		aLog("Saving <Parameter> %d", i);
		AXmlElement *elementParameter = document->createElement("Parameter");
		parameter(i)->writeElement(document, elementParameter);
		element->addChildElement(elementParameter);
	}

	aLog("Saving <Layer>s");
	for(int i=0; i<numLayers(); i++) {
		aLog("Saving <Layer> #%d", i);
		AXmlElement *elementLayer = document->createElement("Layer");
		layer(i)->writeElement(document, elementLayer);
		element->addChildElement(elementLayer);
	}

	return 1;
}