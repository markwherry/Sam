#include "SamDescription.h"

#include "ALog.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamDescription::SamDescription(AModel *parent)
	: AModel(parent)
{
	for(int i=0; i<numMetaDataItems; i++) {
		m_metaData[i] = "";
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamDescription::~SamDescription()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString SamDescription::metaDataName(int id)
{
	if(id<0 || id>= numMetaDataItems) {
		return "";
	}

	AString metaDataTags[numMetaDataItems] = {
		"author",
		"creator",
		"creationDate",
		"instrumentGroup",
		"instrumentType",
		"film",
		"keywords",
		"description",
		"comments",
		"copyright",
		"library",
		"numberOfPlayers"
	};

	return metaDataTags[id];
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamDescription::metaDataContentId(AString tag)
{
	for(int i=0; i<numMetaDataItems; i++) {
		if(tag == metaDataName(i)) {
			return i;
		}
	}

	return -1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString SamDescription::metaData(int itemId)
{
	if(itemId<0 || itemId>= numMetaDataItems) {
		return "";
	}

	return m_metaData[itemId];
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool SamDescription::setMetaData(int itemId, AString data)
{
	if(itemId<0 || itemId>= numMetaDataItems) {
		return false;
	}

	m_metaData[itemId] = data;

	return true;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamDescription::readElement(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<element->numChildElements(); i++){
		AXmlElement *elementMeta = element->childElement(i);
		int contentId = metaDataContentId(elementMeta->attribute("name", ""));
		if(contentId>-1) {
			setMetaData(contentId, elementMeta->attribute("content", ""));
		} else {
			aLog("Unrecognised <Meta> name: %s", elementMeta->attribute("name", "").toChar());
		}
	}

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamDescription::writeElement(AXmlDocument *document, AXmlElement *element)
{
	for(int i=0; i<numMetaDataItems; i++) {
		if(metaData(i).length()>0) {
			AXmlElement *elementMeta = document->createElement("Meta");
			elementMeta->setAttribute("name", metaDataName(i));
			elementMeta->setAttribute("content", metaData(i));
			element->addChildElement(elementMeta);
		}
	}

	return 1;
}