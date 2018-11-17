#include "SamOutput.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOutput::SamOutput(AModel *parent)
	: AModel(parent)
	, m_id(-1)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOutput::SamOutput(AModel *parent, const SamOutput& output)
	: AModel(parent)
{
	m_id = output.m_id;
	m_name = output.m_name;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamOutput::~SamOutput()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamOutput::readElement(AXmlDocument *document, AXmlElement *element)
{
	if(element->hasAttribute("id")) {
		m_id = element->attribute("id","-1").toInt();
	} else {
		aLog("<Output> does not have 'id' attribute.");
		return 0;
	}

	if(element->hasAttribute("name")) {
		m_name = element->attribute("name");
	} else {
		aLog("<Output> does not have 'name' attribute.");
		return 0;
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamOutput::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("id", m_id);
	element->setAttribute("name", m_name);

	return 1;
}