#ifndef __SamBus_h__
#define __SamBus_h__

#include "AModel.h"
#include "AString.h"
#include "AXmlDocument.h"
using namespace Atom;

class SamBus : public AModel
{
public:
	SamBus(AModel *parent, int id, AString name)
		: AModel(parent)
		, m_id(id)
		, m_name(name)
		, m_active(false)
	{
	}

    ~SamBus()
	{
	}

	int id(void) { return m_id; }

	AString name(void) { return m_name; }
	void setName(AString name) { m_name = name; }

	bool isActive(void) { return m_active; }
	void setActive(bool active = true) { m_active = active; }

	virtual int readElement(AXmlDocument *document, AXmlElement *element)
	{
		return 1;
	}

	virtual int writeElement(AXmlDocument *document, AXmlElement *element)
	{
		element->setAttribute("name", name());
		element->setAttribute("id", id());

		return 1;
	}

protected:
	int m_id;
	AString m_name;
	bool m_active;

private:
};

#endif // __SamBus_h__