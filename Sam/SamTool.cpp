#include "SamTool.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamTool::SamTool(SamToolMaster *toolMaster, AString name, AString uuid)
	: m_toolMaster(toolMaster)
	, m_name(name)
	, m_uuid(uuid)
	//, m_editor(0)
	, m_bypassed(0)
	, m_learningParameter(-1)
{
	//addParameter(new SamToolParameter(pmrBypass, "Bypass", "", 0, 0, 1));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamTool::addParameter(SamToolParameter *parameter)
{
	parameterList.append(parameter);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamToolParameter* SamTool::parameterFromId(int id)
{
	for(int i=0; i<numParameters(); i++) {
		if(id == parameter(i)->id()) {
			return parameter(i);
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamTool::parameterIndexFromId(int id)
{
	for(int i=0; i<numParameters(); i++) {
		if(id == parameter(i)->id()) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamToolParameter* SamTool::parameterFromMidiController(int controller)
{
	for(int i=0; i<numParameters(); i++) {
		if(controller == parameter(i)->midiController()) {
			return parameter(i);
		}
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamTool::parameterIdFromIndex(int index)
{
	return parameter(index)->id();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
float SamTool::parameterValueNormalised(int id)
{
	//switch(id) {
	//	case pmrBypass: return parameterFromId(id)->normalisedFromValue(m_bypassController); break;
	//}

	return 0.0f;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamTool::setParameterValueNormalised(int id, float value)
{
	//switch(id) {
	//	case pmrBypass:
	//		m_bypassController = parameterFromId(pmrBypass)->valueFromNormalised(value);
	//		break;
	//}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamTool::setParameterMidiController(int parameterId, int midiController)
{
	SamToolParameter *tp = parameterFromId(parameterId);
	if(tp) {
		tp->setMidiController(midiController);
	}
	m_learningParameter = -1;
	m_toolMaster->parameterAssigned(parameterId);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamTool::readState(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<element->numChildElements(); i++) {
		AXmlElement *el = element->childElement(i);
		if(el->tag() == "Parameter") {
			int id = el->attribute("id", "-1").toInt();
			float value = el->attribute("value", "0").toFloat();
			if(id > -1) {
				setParameterValueNormalised(id, value);
				SamToolParameter *param = parameterFromId(id);
				if(param) {
					param->setMidiController(el->attribute("midiController", "-1").toInt());
					param->setMidiMin(el->attribute("midiMin", "0.0").toFloat());
					param->setMidiMax(el->attribute("midiMax", "1.0").toFloat());
				}
				//setParameterMidiController(id, el->attribute("midiController", "-1").toInt());
				
			}
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamTool::writeState(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<numParameters(); i++) {
		AXmlElement *elementParameter = document->createElement("Parameter");
		elementParameter->setAttribute("id", parameterList[i]->id());
		elementParameter->setAttribute("name", parameterList[i]->name());
		elementParameter->setAttribute("value", parameterValueNormalised(parameterList[i]->id()));
		elementParameter->setAttribute("midiController", parameterList[i]->midiController());
		elementParameter->setAttribute("midiMin", parameterList[i]->midiMin());
		elementParameter->setAttribute("midiMax", parameterList[i]->midiMax());
		element->addChildElement(elementParameter);
	}

	return 1;
}