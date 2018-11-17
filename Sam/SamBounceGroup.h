#ifndef __SamBounceGroup_h__
#define __SamBounceGroup_h__

#include "AString.h"
using namespace Atom;

#include "CsSpeakerConfig.h"
using namespace Cs;

class SamBounceGroup
{
public:
	SamBounceGroup(AString prefix, CsSpeakerConfig config)
		: m_prefix(prefix)
		, m_config(config)
	{
		for(int i=0; i<128; i++) {
			m_streams[i] = false;
		}
	}

	AString prefix(void) { return m_prefix; }

	void setStreamEnabled(int streamIndex, bool enabled) { m_streams[streamIndex] = enabled; }
	bool isStreamEnabled(int streamIndex) { return m_streams[streamIndex]; }

	CsSpeakerConfig config(void) { return m_config; }

private:
	AString m_prefix;
	CsSpeakerConfig m_config;
	bool m_streams[128];
};

#endif