#include "CsSpeakerConfig.h"
#include "ALog.h"

namespace Cs {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsSpeakerConfig::CsSpeakerConfig(CsSpeakerConfigType config)
: m_config(config)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsSpeakerConfig::CsSpeakerConfig(AString configName)
: m_config(Undefined)
{
	setConfig(configName);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsSpeakerConfig::CsSpeakerConfig(const CsSpeakerConfig& config)
{
	m_config = config.m_config;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsSpeakerConfig& CsSpeakerConfig::operator=(const CsSpeakerConfig& config)
{
	m_config = config.m_config;

	return *this;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsSpeakerConfig::operator==(const CsSpeakerConfig& config)
{
	return m_config == config.m_config;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsSpeakerConfig::operator!=(const CsSpeakerConfig& config)
{
	return !(m_config == config.m_config);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsSpeakerConfig::setConfig(AString configName)
{
	if(configName == "Mono") {
		m_config = Mono;
	} else if(configName == "Stereo") {
		m_config = Stereo;
	} else if(configName == "Quad") {
		m_config = Surround40;
	} else if(configName == "5.0") {
		m_config = Surround50;
	} else if(configName == "5.1") {
		m_config = Surround51;
	} else {
		m_config = Undefined;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsSpeakerConfig::speakerWithinConfig(CsSpeakerConfigType config, int stream)
{
	//aLog("ASpeakerConfig::getSpeakerWithinConfig(%d)", stream);

	switch(config) {
		case Undefined:
			return SpeakerUndefined;
			break;
		case Mono:
			if(stream==0) return SpeakerM;
			break;
		case Stereo:
			if(stream==0) return SpeakerL;
			if(stream==1) return SpeakerR;
			break;
		case Surround40:
			if(stream==0) return SpeakerL;
			if(stream==1) return SpeakerR;
			if(stream==2) return SpeakerLs;
			if(stream==3) return SpeakerRs;
			break;
		case Surround50:
			if(stream==0) return SpeakerL;
			if(stream==1) return SpeakerR;
			if(stream==2) return SpeakerLs;
			if(stream==3) return SpeakerRs;
			if(stream==4) return SpeakerC;
			break;
		case Surround51:
			if(stream==0) return SpeakerL;
			if(stream==1) return SpeakerR;
			if(stream==2) return SpeakerLs;
			if(stream==3) return SpeakerRs;
			if(stream==4) return SpeakerC;
			if(stream==5) return SpeakerLfe;
			break;
	}

	return SpeakerUndefined;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsSpeakerConfig::numSpeakersWithinConfig(CsSpeakerConfigType config)
{
	int numSpeakers = 0;

	switch(config) {
		case Undefined:
			numSpeakers = 0;
			break;
		case Mono:
			numSpeakers = 1;
			break;
		case Stereo:
			numSpeakers = 2;
			break;
		case Surround40:
			numSpeakers = 4;
			break;
		case Surround50:
			numSpeakers = 5;
			break;
		case Surround51:
			numSpeakers = 6;
			break;
	}

	return numSpeakers;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AString CsSpeakerConfig::speakerConfigName(CsSpeakerConfigType config)
{
	AString name = "Undefined";

	switch(config) {
		case Mono:
			name = "Mono";
			break;
		case Stereo:
			name = "Stereo";
			break;
		case Surround40:
			name = "Quad";
			break;
		case Surround50:
			name = "5.0";
			break;
		case Surround51:
			name = "5.1";
			break;
	}

	return name;
}

}