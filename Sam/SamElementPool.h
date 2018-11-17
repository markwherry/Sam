#ifndef __SamElementPool_h__
#define __SamElementPool_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "ACommand.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamSet.h"
#include "SamElement.h"

//class SamLoadProgressCallback;

class SamElementPool : public AModel
{
public:
	SamElementPool(AModel *parent, AString instrumentPath);
    ~SamElementPool();

	void setInstrumentPath(AString instrumentPath) { m_instrumentPath = instrumentPath; }

	int numSets(void) { return setList.size(); }
	SamSet* set(int id) { return setList[id]; }
	int createSet(AString name, int streams);

	SamElement* element(int id);
	SamElement* createElement(AString filename, bool loadSampleData);
	int addElement(SamElement *element);
	int numElements(void) { return elementList.size(); }

	inline bool isAccelerated(void) const { return m_accelerated; }
	inline void setAccelerated(bool accelerated) { m_accelerated = accelerated; }

	unsigned long long size(void) { return m_size; }
	void recalculateSize(void);

	bool importElements(AString filename, ACommand *command);
	bool exportElements(AString filename, ACommand *command);

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element, ACommand *command);
	int writeElement(AXmlDocument *document, AXmlElement *element, ACommand *command);

private:
	AString m_instrumentPath;

	bool m_accelerated;
	
	AList<SamSet*> setList;
	AList<SamElement*> elementList;

	unsigned long long m_size;
};

#endif // __SamElementPool_h__
