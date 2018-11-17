#ifndef __CsSpeakerConfig_h__
#define __CsSpeakerConfig_h__

#include "AString.h"
using namespace Atom;

namespace Cs {

class CsSpeakerConfig
{
public:
	enum CsSpeakerConfigType {
		Undefined = 0,
		Mono = 1,
		Stereo = 2,
		Surround40 = 3,
		Surround50 = 4,
		Surround51 = 5
	};

	CsSpeakerConfig(CsSpeakerConfigType config = Undefined);
	CsSpeakerConfig(AString configName);
	CsSpeakerConfig(const CsSpeakerConfig& config);

	CsSpeakerConfig& operator=(const CsSpeakerConfig& config);
	bool operator==(const CsSpeakerConfig& config);
	bool operator!=(const CsSpeakerConfig& config);

	void setConfig(CsSpeakerConfigType config) { m_config = config; }
	void setConfig(AString configName);
	CsSpeakerConfigType config(void) { return m_config; }

	AString name(void) { return speakerConfigName(m_config); }

	int numSpeakers(void) { return numSpeakersWithinConfig(m_config); }

	enum SpeakerType {
		SpeakerUndefined = 0,
		SpeakerM = 1,
		SpeakerL = 2,
		SpeakerC = 3,
		SpeakerR = 4,
		SpeakerLs = 5,
		SpeakerRs = 6,
		SpeakerLfe = 7
	} ;

	static int speakerWithinConfig(CsSpeakerConfigType config, int stream);
	static int numSpeakersWithinConfig(CsSpeakerConfigType config);
	static AString speakerConfigName(CsSpeakerConfigType config);

protected:
private:
	CsSpeakerConfigType m_config;
};

}

#endif // __SpeakerConfig_h__