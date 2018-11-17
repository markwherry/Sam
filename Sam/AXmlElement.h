#ifndef __AXmlElement_h__
#define __AXmlElement_h__

#include "AString.h"
#include "AList.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AXmlAttribute
{
public:
	AXmlAttribute(AString name = "", AString value = "");
	AXmlAttribute(const AXmlAttribute& attribute);

	AString name(void) { return m_name; }

	void setValue(AString value) { m_value = value; }
	AString value(void) { return m_value; }

protected:
private:
	AString m_name;
	AString m_value;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AXmlElement
{
public:
	AXmlElement(AString tag = "");
	AXmlElement(const AXmlElement& element);

	AString tag(void) { return m_tag; }
	void setTag(AString tag) { m_tag = tag; }

	void setAttribute(AString name, AString value);
	void setAttribute(AString name, int value);
	void setAttribute(AString name, float value);

	int numAttributes(void) { return m_attributes.size(); }
	AXmlAttribute* attribute(int index) { return m_attributes[index]; }
	AString attribute(AString name, AString defaultValue = "");
	bool hasAttribute(AString name);

	AString innerText(void) { return m_innerText; }
	void setInnerText(AString text) { m_innerText = text; }
	bool hasInnerText(void) { return (m_innerText.length() > 0); }

	void addChildElement(AXmlElement *element);
	int numChildElements(void) { return m_elements.size(); }
	AXmlElement* childElement(int index) { return m_elements[index]; }

protected:
private:
	AString m_tag;
	AString m_innerText;
	AList<AXmlAttribute*> m_attributes;
	AList<AXmlElement*> m_elements;
};

}

#endif