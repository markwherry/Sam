#ifndef __SamStreamMix_h__
#define __SamStreamMix_h__

#include "AModel.h"
#include "AString.h"
#include "AList.h"
#include "AXmlDocument.h"
using namespace Atom;

#include "CsSpeakerConfig.h"
using namespace Cs;

#include "SamStream.h"
#include "SamBounceGroup.h"

class SamStreamMix : public AModel
{
public:
	SamStreamMix(AModel *parent);
	SamStreamMix(AModel *parent, const SamStreamMix& streamMix);
    ~SamStreamMix();

	void createMonoStreamMix(int streams);
	void createStereoStreamMix(void);
	void createQuadStreamMix(int stereoPairs);
	void createFromBounceGroups(AList<SamBounceGroup*> *bounceGroups);

	int numStreams(void) { return m_streams.size(); }
	SamStream* stream(int index) { return m_streams[index]; }
	SamStream* getStream(int setId, int streamId);
	int indexFromStream(SamStream *stream);

	bool isSolo(void);

	void setStreamMix(SamStreamMix *streamMix);

	// Accessors
	AString name(void) { return m_name; }
	void setName(AString name) { m_name = name; }

	AString description(void) { return m_description; }
	void setDescription(AString description) { m_description = description; }

	AString mixer(void) { return m_mixer; }
	void setMixer(AString mixer) { m_mixer = mixer; }

	inline CsSpeakerConfig speakerConfig(void) const { return m_speakerConfig; }
	void setSpeakerConfig(CsSpeakerConfig speakerConfig) { m_speakerConfig = speakerConfig; }

	// Load and save
	int readElement(AXmlDocument *document, AXmlElement *element);
	int writeElement(AXmlDocument *document, AXmlElement *element);

private:
	AString m_name;
	AString m_description;
	AString m_mixer;
	CsSpeakerConfig m_speakerConfig;
	AList<SamStream*> m_streams;
};

#endif // __SamStreamMix_h__
