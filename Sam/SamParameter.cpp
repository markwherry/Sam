#include "SamParameter.h"

#include "ALog.h"

SamParameter::SamParameter(AModel *parent)
	: AModel(parent)
{
	m_name = "Parameter";
	m_source = -1;
	m_sourceControl = 0;
	m_type = 0;
	m_midiType = 0;
	m_midiController = -1;
	m_midiValue = 0;
	m_triggerValue = 0;
	m_inverted = false;
	m_defaultValue = 0.0f;

	m_curve = new CsCurve(128, 0);
}

SamParameter::SamParameter(AModel *parent, const SamParameter& parameter)
: AModel(parent)
{
	m_name = parameter.m_name;
	m_source = parameter.m_source;
	m_sourceControl = parameter.m_sourceControl;
	m_type = parameter.m_type;
	m_midiType = parameter.m_midiType;
	m_midiController = parameter.m_midiController;
	m_midiValue = parameter.m_midiValue;
	m_triggerValue = parameter.m_triggerValue;
	m_inverted = parameter.m_inverted;
	m_defaultValue = parameter.m_defaultValue;

	m_curve = new CsCurve(*parameter.m_curve);
}

SamParameter::~SamParameter()
{
	
}

void SamParameter::debug(void)
{
	aLog("SamParameter::debug()");
	aLog("name=%s", m_name.toChar());
	aLog("source=%d", m_source);
}

int SamParameter::readElement(AXmlDocument *document, AXmlElement *element)
{
	// Read <Parameter> attributes
	// ---------------------------
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Parameter> does not have 'name' attribute.");
		return false;
	}

	if(element->hasAttribute("source")) {
		m_source = element->attribute("source").toInt();
	} else {
		aLog("<Parameter> does not have 'source' attribute.");
		return false;
	}

	if(element->hasAttribute("sourceControl")) {
		m_sourceControl = element->attribute("sourceControl").toInt();
	} else {
		aLog("Warning: <Parameter> does not have 'sourceControl' attribute.");
		//return false;
	}

	if(element->hasAttribute("midiType")) {
		m_midiType = element->attribute("midiType").toInt();
	} else {
		aLog("Warning: <Parameter> does not have 'midiType' attribute.");
	}

	if(element->hasAttribute("midiController")) {
		m_midiController = element->attribute("midiController").toInt();
	} else {
		if(m_midiType == spmtController) {
			aLog("<Parameter> does not have 'midiController' attribute when midiType = 2.");
			return false;
		}
	}

	if(element->hasAttribute("defaultValue")) {
		m_defaultValue = element->attribute("defaultValue").toFloat();
	} else {
		aLog("<Parameter> does not have 'defaultValue' attribute.");
		//return false;
	}

	/*QDomNodeList nodeListParameterChildren = element->childNodes();
	aLog ("Number of <Parameter> children = %d", nodeListParameterChildren.size());
	if(nodeListParameterChildren.size()<1) {
		return false;
	}*/

	if(element->numChildElements() < 1) {
		aLog("Parameter has no children.");
		return 0;
	}

	/*for(QDomNode n = element->firstChild(); !n.isNull(); n = n.nextSibling()){
		QDomElement el = n.toElement();*/
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);
		if(el->tag() == "Trigger") {
			m_type = sptTrigger;

			if(el->hasAttribute("value")) {
				m_triggerValue = el->attribute("value").toInt();
			} else {
				aLog("<Trigger> does not have 'value' attribute.");
				return false;
			}

			if(el->hasAttribute("midiValue")) {
				m_midiValue = el->attribute("midiValue").toInt();
			} else {
				if(m_midiType>-1) {
					aLog("<Trigger> does not have 'midiValue' attribute when midiType is set for <Parameter>.");
					return false;
				}
			}
		} else if(el->tag() == "Range") {
			m_type = sptRange;

			if(el->hasAttribute("inverted")) {
				m_inverted = (bool)el->attribute("inverted").toInt();
			} else {
				aLog("<Range> does not have 'inverted' attribute.");
				//return false;
			}

			if(el->hasAttribute("curveShape")) {
				m_curve->setShape(el->attribute("curveShape").toInt());
			} else {
				aLog("<Range> does not have 'curveShape' attribute.");
				//return false;
			}

			if(el->hasAttribute("curveStart")) {
				m_curve->setStart(el->attribute("curveStart").toFloat());
			} else {
				aLog("<Range> does not have 'curveStart' attribute.");
				//return false;
			}

			if(el->hasAttribute("curveEnd")) {
				m_curve->setEnd(el->attribute("curveEnd").toFloat());
			} else {
				aLog("<Range> does not have 'curveEnd' attribute.");
				//return false;
			}

			if(el->hasAttribute("curveQ")) {
				m_curve->setDepth(el->attribute("curveQ").toFloat());
			} else {
				aLog("<Range> does not have 'curveQ' attribute.");
				//return false;
			}

		} else {
			aLog("Unrecognised <Parameter> child: %s", el->tag().toChar());
			return false;
		}
	}

	return true;
}

int SamParameter::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("name", m_name);
	element->setAttribute("source", m_source);
	//element->setAttribute("sourceControl", m_sourceControl);
	element->setAttribute("defaultValue", m_defaultValue);
	
	if(m_midiType>-1) element->setAttribute("midiType", m_midiType);
	if(m_midiController>-1) element->setAttribute("midiController", m_midiController);

	switch(m_type) {
		case sptTrigger: 
			{
				AXmlElement *elementTrigger = document->createElement("Trigger");
				elementTrigger->setAttribute("value", m_triggerValue);
				elementTrigger->setAttribute("midiValue", m_midiValue);
				element->addChildElement(elementTrigger);
			}
			break;
		case sptRange:
			{
				AXmlElement *elementRange = document->createElement("Range");
				elementRange->setAttribute("inverted", m_inverted);
				elementRange->setAttribute("curveShape", m_curve->shape());
				elementRange->setAttribute("curveStart", m_curve->start());
				elementRange->setAttribute("curveEnd", m_curve->end());
				elementRange->setAttribute("curveQ", m_curve->depth());
				element->addChildElement(elementRange);
			}
			break;
	}

	return false;
}