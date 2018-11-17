#include "SamToolManager.h"

#include "Sam.h"

SamToolManager::SamToolManager()
{
	//registerAudioTool(AtTestTone::createInstance, AtTestTone::uuid(), AtTestTone::name());
	//registerAudioTool(AtS1::createInstance, AtS1::uuid(), AtS1::name());
	//registerAudioTool(Sam::createInstance, Sam::uuid(), Sam::name());
}

void SamToolManager::registerTool(SamTool* (*createFunction)(SamToolMaster*), AString uuid, AString name)
{
	SamToolInfo toolInfo;
	toolInfo.createFunction = createFunction;
	toolInfo.uuid = uuid;
	toolInfo.name = name;

	m_toolInfos.append(toolInfo);
}

SamTool* SamToolManager::createInstance(AString uuid, SamToolMaster *toolMaster)
{
	for(int i=0; i<m_toolInfos.size(); i++) {
		if(m_toolInfos[i].uuid.isEqualTo(uuid)) {
			return m_toolInfos[i].createFunction(toolMaster);
		}
	}

	return 0;
}

SamToolInfo SamToolManager::toolInfo(AString uuid)
{
	SamToolInfo toolInfo;
	toolInfo.createFunction = 0;
	toolInfo.uuid = uuid;

	for(int i=0; i<m_toolInfos.size(); i++) {
		if(m_toolInfos[i].uuid.isEqualTo(uuid)) {
			toolInfo.createFunction = m_toolInfos[i].createFunction;
			toolInfo.name = m_toolInfos[i].name;
			break;
		}
	}

	return toolInfo;
}

SamToolInfo SamToolManager::toolInfo(int index)
{
	return m_toolInfos[index];
}