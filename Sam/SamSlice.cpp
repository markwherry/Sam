#include "SamSlice.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamSlice::SamSlice(int samplePos, int ppqnPos, int length)
	: m_samplePos(samplePos)
	, m_ppqnPos(ppqnPos)
	, m_length(length)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamSlice::readElement(AXmlDocument *document, AXmlElement *element)
{
	m_samplePos = element->attribute("sample", "0").toInt();
	m_ppqnPos = element->attribute("ppqn", "0").toInt();
	m_length = element->attribute("length", "0").toInt();

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamSlice::writeElement(AXmlDocument *document, AXmlElement *element)
{
	element->setAttribute("sample", m_samplePos);
	element->setAttribute("ppqn", m_ppqnPos);
	element->setAttribute("length", m_length);

	return 1;
}