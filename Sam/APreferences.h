#ifndef __APreferences_h__
#define __APreferences_h__

#include "AXmlDocument.h"

namespace Atom {

class APreferences
{
public:
	APreferences(AString filename);
	~APreferences();

	void setPreference(AString name, AString value);
	AString preference(AString name, AString defaultValue = "");
	bool hasPreference(AString name);

protected:
private:
	void read(void);
	void write(void);
	AString m_filename;
	AXmlDocument m_document;
};

}

#endif