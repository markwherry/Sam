#ifndef __SamOutput_h__
#define __SamOutput_h__

#include "AModel.h"
#include "AString.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamOutput : public AModel
{
public:
	SamOutput(AModel *parent);
	SamOutput(AModel *parent, const SamOutput& output);
	~SamOutput();

	// Accessors
	int id(void) { return m_id; }
	void setId(int id) { m_id = id; }

	inline AString name (void) const { return m_name; }
	inline void setName (AString name) { m_name = name; }

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

private:
	int m_id;
	AString m_name;
};

#endif // __SamOutput_h__
