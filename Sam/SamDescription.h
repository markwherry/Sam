#ifndef __SamDescription_h__
#define __SamDescription_h__

#include "AModel.h"
#include "AString.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamDescription : public AModel
{
public:
	SamDescription(AModel *parent);
	~SamDescription();

	AString metaDataName(int id);
	int metaDataContentId(AString tag);

	AString metaData(int itemId);
	bool setMetaData(int itemId, AString data);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

private:
	static const int numMetaDataItems = 12;
	AString m_metaData[numMetaDataItems];
};

#endif // SAMDESCRIPTION_H
