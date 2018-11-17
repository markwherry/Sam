#include "SamAudioTool.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioTool::SamAudioTool(SamToolMaster *toolMaster, AString name, AString uuid)
	: SamTool(toolMaster, name, uuid)
{
	m_audioProcessInfo.bufferSize = 0;
	m_audioProcessInfo.sampleRate = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioTool::~SamAudioTool()
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamAudioTool::setSpeakerConfig(CsSpeakerConfig inputConfig, CsSpeakerConfig outputConfig)
{
	m_speakerConfig = inputConfig;

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsSpeakerConfig SamAudioTool::outputSetup(int outputId)
{
	if(outputId >= 0 && outputId <outputSetupList.size()) {
		return outputSetupList[outputId];
	}

	return CsSpeakerConfig();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamAudioTool::addAudioOutput(CsSpeakerConfig speakerConfig)
{
	outputSetupList.append(speakerConfig);
}