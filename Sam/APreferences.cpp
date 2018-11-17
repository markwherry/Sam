#include "APreferences.h"

#include "AFile.h"
#include "ALog.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
APreferences::APreferences(AString filename)
: m_filename(filename)
{
	read();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
APreferences::~APreferences()
{
	write();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void APreferences::setPreference(AString name, AString value)
{
	if(!m_document.rootElement()) {
		AXmlElement *element = m_document.createElement("Preferences");
		m_document.setRootElement(element);
	}

	AXmlElement *element = 0;
	for(int i=0; i<m_document.rootElement()->numChildElements(); i++) {
		if(m_document.rootElement()->childElement(i)->tag().isEqualTo(name)) {
			element = m_document.rootElement()->childElement(i);
			element->setAttribute("value", value);
			break;
		}
	}

	if(!element) {
		element = m_document.createElement(name);
		element->setAttribute("value", value);
		m_document.rootElement()->addChildElement(element);
	}

	write();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString APreferences::preference(AString name, AString defaultValue)
{
	if(m_document.rootElement()) {
		for(int i=0; i<m_document.rootElement()->numChildElements(); i++) {
			if(m_document.rootElement()->childElement(i)->tag().isEqualTo(name)) {
				return m_document.rootElement()->childElement(i)->attribute("value", defaultValue);
			}
		}
	}

	return defaultValue;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool APreferences::hasPreference(AString name)
{
	if(m_document.rootElement()) {
		for(int i=0; i<m_document.rootElement()->numChildElements(); i++) {
			if(m_document.rootElement()->childElement(i)->tag().isEqualTo(name)) {
				return true;
			}
		}
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void APreferences::read(void)
{
	AFile file(m_filename);
	
	if(!file.open()) {
		return;
	}

	char *data = (char*)malloc(file.size()+1);
	file.readBytes(file.size(), data);
	data[file.size()] = 0;

	file.close();

	m_document.setSource(data);
	if(!m_document.rootElement()) {
		aLog("!doc.element()");
		return;
	}
}

void APreferences::write(void)
{
	//aLog("APreferences::write()");

	AFile file(m_filename);

	if(!file.create()) {
		aLog("Could not write preferences file [%s]", m_filename.toChar());
		return;
	}

	AString str = m_document.toString();

	file.writeBytes(str.length(), str.toChar());
	file.close();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------

}