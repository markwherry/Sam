#include "AXmlElement.h"

#include "ALog.h"

#include <stdio.h>

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlAttribute::AXmlAttribute(AString name, AString value)
: m_name(name)
, m_value(value)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlAttribute::AXmlAttribute(const AXmlAttribute& attribute)
: m_name(attribute.m_name)
, m_value(attribute.m_value)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlElement::AXmlElement(AString tag)
: m_tag(tag)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlElement::AXmlElement(const AXmlElement& element)
: m_tag(element.m_tag)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlElement::setAttribute(AString name, AString value)
{
	//aLog("AXmlElement::setAttribute(%s,%s)", name.toChar(), value.toChar());

	AXmlAttribute *attribute = 0;

	for(int i=0; i<m_attributes.size(); i++) {
		//aLog("%s.isEqualTo(%s)", m_attributes[i]->name().toChar(), name.toChar());
		if(m_attributes[i]->name().isEqualTo(name)) {
			//aLog("Attribute found");
			attribute = m_attributes[i];
			attribute->setValue(value);
			break;
		}
	}

	if(!attribute) {
		//aLog("Attribute not found");
		attribute = new AXmlAttribute(name, value);
		m_attributes.append(attribute);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlElement::setAttribute(AString name, int value)
{
	setAttribute(name, AString::number(value));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlElement::setAttribute(AString name, float value)
{
	char number[256];
	sprintf(number, "%f", value);
	setAttribute(name, number);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AXmlElement::attribute(AString name, AString defaultValue)
{
	for(int i=0; i<m_attributes.size(); i++) {
		//aLog("Find attribute [%s,%s]", name.toChar(), m_attributes[i]->name().toChar());
		if(m_attributes[i]->name().isEqualTo(name)) {
			return m_attributes[i]->value();
		}
	}

	return defaultValue;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool AXmlElement::hasAttribute(AString name)
{
	for(int i=0; i<m_attributes.size(); i++) {
		if(m_attributes[i]->name().isEqualTo(name)) {
			return true;
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlElement::addChildElement(AXmlElement *element)
{
	m_elements.append(element);
}

}