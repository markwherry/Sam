#include "AXmlDocument.h"
#include "AStack.h"

#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlDocument::AXmlDocument()
: m_element(0)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlDocument::setSource(AString source)
{
	//aLog("XmlDocument::setSource()");
	m_source = source;
	parse();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AXmlElement* AXmlDocument::createElement(AString tag)
{
	AXmlElement *element = new AXmlElement(tag);

	return element;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlDocument::debugElementTree(AXmlElement *element)
{
	aLog("Element <%s> has %d attribute(s) and %d child(ren).", element->tag().toChar(), element->numAttributes(), element->numChildElements());

	for(int i=0; i<element->numChildElements(); i++) {
		debugElementTree(element->childElement(i));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AXmlDocument::toString(void)
{
	AString str;

	if(m_element) {	
		str = elementToString(m_element);
	}

	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString AXmlDocument::elementToString(AXmlElement *element, int level)
{
	AString str;

	for(int i=0; i<level; i++) {
		str.append(' ');
	}

	str.append('<');
	str.append(element->tag());
	for(int i=0; i<element->numAttributes(); i++) {
		str.append(' ');
		str.append(element->attribute(i)->name());
		str.append('=');
		str.append('"');
		str.append(element->attribute(i)->value());
		str.append('"');
	}

	if(element->numChildElements() < 1 && !element->hasInnerText()) {
		str.append(' ');
		str.append('/');
	} else {
		str.append('>');
		if(element->hasInnerText()) {
			str.append(element->innerText());
		}
		str.append(0x0d);
		str.append(0x0a);
		
		for(int i=0; i<element->numChildElements(); i++) {	
			AString strElement = elementToString(element->childElement(i), level+1);
			if(strElement.length() > 0) {
				str.append(strElement);
				str.append(0x0d);
				str.append(0x0a);
			}
		}

		for(int i=0; i<level; i++) {
			str.append(' ');
		}

		str.append('<');
		str.append('/');
		str.append(element->tag());
	}

	str.append('>');

	return str;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AXmlDocument::parse(void)
{
	//aLog("XmlDocument::parse()");

	enum {
		outsideElement   = 0,
		inElement        = 1,
		inTag            = 2,
		inAttributeName  = 3,
		inAttributeValue = 4
	};

	int where = outsideElement;
	bool isEndTag = false;
	bool isStartEndTag = false;

	AString currentTag;
	AString currentAttributeName;
	AString currentAttributeValue;
	AString currentInnerText;

	AXmlElement *firstElement = 0;
	AXmlElement *currentElement = 0;
	//XmlElement *parentElement = 0;
	AStack<AXmlElement*> elementStack;

	wchar_t lastChar = 0;

	for(int i=0; i<m_source.length(); i++) {
		//aLog("[%d] %c", i, m_source[i]);
		if (m_source[i] == '<') {
			
			if(where >= inElement) {
				// We're already in a tag -- reading another '<' before an '>' means an error.
				aLog("Already in a tag");
				return;
			} else if(where == outsideElement) {
				if(currentElement && currentInnerText.length() > 0) {
					aLog("Writing innerText %d [%s]", currentInnerText.length(), currentInnerText.toChar());
					currentElement->setInnerText(currentInnerText);
					currentInnerText.clear();
				}
			}
			where = inTag;

			currentElement = new AXmlElement();
			if(!firstElement) {
				firstElement = currentElement;
				//parentElement = m_element;
			}

			isEndTag = false;
			isStartEndTag = false;
			
		} else if(m_source[i] == '>') {
			
			if(where == outsideElement) {
				// If we're not in a tag, we should be to read '>' -- therefore, it's an error.
				aLog("Not in a tag");
				return;
			}
			where = outsideElement;

			//aLog("Tag: <%s> has %d attribute(s)", currentTag.toChar(), currentElement->numAttributes());

			currentElement->setTag(currentTag);
			
			if(isEndTag && !isStartEndTag) {
				if(elementStack.isEmpty()) {
					aLog("Stack is empty: unbalanced start-end tags.");
					m_element = 0; // Need to clear properly
					return;
				}
				AXmlElement *element = elementStack.pop();
				//aLog("End tag for </%s>", element->tag().toChar());
				if(!element->tag().isEqualTo(currentTag)) {
					aLog("Start and end tags do not match.");
					m_element = 0; // Need to clear properly
					return;
				} else {
					if(elementStack.isEmpty()) {
						aLog("!!!Should only get here at the end!!!");
						m_element = firstElement;
					} else {
						elementStack.top()->addChildElement(element);
					}
					

				}

				delete currentElement;
				currentElement = 0;

				

				
			} else {
				if(!isStartEndTag) {
					elementStack.push(currentElement);
				} else {
					if(elementStack.isEmpty()) {
						aLog("!!!Should never get here!!!");
					} else {
						elementStack.top()->addChildElement(currentElement);
					}
				}
					
			}

			currentTag.clear();

		} else if(m_source[i] == '/') {

			//aLog("m_source[i] == '/' where = %d", where);
			if(where == inAttributeValue) {
				currentAttributeValue.append(m_source[i]);
			} else {
				isEndTag = true;
				if(lastChar != '<') {
					//aLog("StartEndTag found.");
					isStartEndTag = true;
				}
			}

		} else if(m_source[i] == ' ') {

			if(where > outsideElement && where != inAttributeValue) {
				where = inElement;
			} else if(where == inAttributeValue) {
				currentAttributeValue.append(m_source[i]);
			}

		} else if(m_source[i] == '=') {


		} else if(m_source[i] == '"') {

			if(where != inAttributeValue) {
				//aLog("where = inAttributeValue");
				where = inAttributeValue;
			} else {
				//aLog("closing value");
				where = inElement;
				//aLog("attribute[%s]=[%s] in <%s>", currentAttributeName.toChar(), currentAttributeValue.toChar(), currentElement->tag().toChar());
				if(currentAttributeName.length() < 1 || currentAttributeValue.length() < 1) {
					aLog("*** attribute[%d]=[%d]", currentAttributeName.length(), currentAttributeValue.length());
				}
				//aLog("Here!");

				currentElement->setAttribute(currentAttributeName, currentAttributeValue);
				//aLog("Here!2");
				currentAttributeName.clear();
				currentAttributeValue.clear();
			}
			
		} else {
			//aLog("Adding");
			//if(m_source[i] == '/' && lastChar == '<') {
			//	isEndTag = true;
			//} else {
				if(m_source[i] >= 33 && m_source[i] <= 126) { // Was 48, changed to include hyphen, then 45, then changed again (122 < then <=)
					if(where == outsideElement) { //aLog("***** APPENDING INNER TEXT [%c] *****", m_source[i]);
						currentInnerText.append(m_source[i]);
					} else if(where == inTag) {
						currentTag.append(m_source[i]);
					} else if(where == inAttributeValue) {
						currentAttributeValue.append(m_source[i]);
					} else {
						currentAttributeName.append(m_source[i]);
						where = inAttributeName;
						//aLog("Appending attribute name");
					}
				}
			//}

		}

		lastChar = m_source[i];

		
	}
}

}