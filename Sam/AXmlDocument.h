#ifndef __AXmlDocument_h__
#define __AXmlDocument_h__

#include "AString.h"
#include "AXmlElement.h"

namespace Atom {

	
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class AXmlDocument
{
public:
	AXmlDocument();

	void setSource(AString source);

	void setRootElement(AXmlElement *element) { m_element = element; }
	AXmlElement* rootElement(void) { return m_element; }
	AXmlElement* createElement(AString tag);

	void debugElementTree(AXmlElement *element);

	AString toString(void);

protected:
private:
	AString elementToString(AXmlElement *element, int level = 0);
	void parse(void);

	AString m_source;
	AXmlElement *m_element;
};

}

#endif