#ifndef __SamInstrument_h__
#define __SamInstrument_h__

#include "AModel.h"
#include "AString.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "SamDescription.h"
#include "SamElementPool.h"
#include "SamStreamMix.h"
#include "SamVersion.h"
#include "SamBounceGroup.h"

class SamInstrument : public AModel
{
public:
	SamInstrument(AModel *parent);
	~SamInstrument();

	SamDescription* description(void) { return m_description; }

	SamElementPool* elementPool(void) { return m_elementPool; }

	AString filename(void) { return m_filename; }
	void setFilename(AString filename);
	AString path(void) { return m_path; }

	AString name(void) { return m_name; }
	void setName(AString name);

	AString uuid(void) { return m_uuid; }
	void setUuid(AString uuid);

	int numStreamMixes(void) { return m_streamMixes.size(); }
	SamStreamMix* streamMix(int index) const { return m_streamMixes[index]; }

	void addVersion(SamVersion *version);
	int numVersions(void) { return m_versions.size(); }
	SamVersion* version(int index = 0) const { return m_versions[index]; }

	void create(int mode, bool allMonoStreams, bool createVersion, AString instrumentPath);
	void createFromBounceGroups(AString instrumentPath, AList<SamBounceGroup*> *bounceGroups);
	void createFromStreamMix(AString instrumentPath, SamElementPool *elementPool, SamStreamMix *streamMix);
	void recreateUuid(void);

	int numCells(void);

	int readElement(AXmlDocument *document, AXmlElement *element, ACommand *command);
	int writeElement(AXmlDocument *document, AXmlElement *element, ACommand *command);

	typedef AObjectPtr<SamInstrument> Ptr;

protected:
private:
	AString m_filename;
	AString m_path;
	AString m_name;
	AString m_uuid;

	SamDescription *m_description;
	SamElementPool *m_elementPool;
	AList<SamStreamMix*> m_streamMixes;
	AList<SamVersion*> m_versions;
};

#endif