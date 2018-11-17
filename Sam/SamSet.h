#ifndef __SamSet_h__
#define __SamSet_h__

#include "AModel.h"
#include "AString.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamSet : public AModel
{
public:
	SamSet(AModel *parent);
    ~SamSet();

	// Accessors
	inline AString name(void) const { return m_name; }
	void setName(AString name, void *from = 0);

	inline int streams(void) const { return m_streams; }
	void setStreams(int streams) { m_streams = streams; }

	inline bool isActive(void) const { return m_active; }
	void setActive(bool active, void *from = 0);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

	enum SamSetItem {
		ItemName       = 0,
		ItemNumStreams = 1,
		ItemActive     = 2
	};

private:
	AString m_name;
	int     m_streams;
	bool    m_active;
};

#endif // __SamSet_h__
