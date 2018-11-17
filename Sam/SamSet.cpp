#include "SamSet.h"
#include "SamEngine.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamSet::SamSet(AModel *parent)
	: AModel(parent)
	, m_streams(0)
	, m_active(true)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamSet::~SamSet()
{	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamSet::setName(AString name, void *from)
{
	aLog("SamSet::setName(%s)", name.toChar());

	m_name = name;

	updateState(SamEngine::ContextSet, ItemName, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamSet::setActive(bool active, void *from)
{
	aLog("SamSet::setActive(%d)", (int)active);

	m_active = active;

	updateState(SamEngine::ContextSet, ItemActive, this, from);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamSet::readElement(AXmlDocument *document, AXmlElement *element)
{
	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<SamSet> does not have 'name' attribute.");
		return 0;
	}

	if(element->hasAttribute("streams")) {
		m_streams = element->attribute("streams","0").toInt();
	} else {
		aLog("<SamSet> does not have 'streams' attribute.");
		return 0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamSet::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("name", m_name);
	element->setAttribute("streams", m_streams);

	return 1;
}