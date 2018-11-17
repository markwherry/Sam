#ifndef __CsVstPluginHost_h__
#define __CsVstPluginHost_h__

#include "Vst/aeffectx.h"

#include "CsVstPlugin.h"
#include "SamAudioToolInsert.h"

class CsVstPluginHost
{
public:
	CsVstPluginHost();

	static VstIntPtr VSTCALLBACK InternalHostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
	virtual VstIntPtr VSTCALLBACK HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

	virtual float getSampleRate(void) = 0;
	virtual int   getBufferSize(void) = 0;

	void addPluginToInsert(AEffect *effect, SamAudioToolInsert *insert);
	SamAudioToolInsert* insertFromEffect(AEffect *effect);

protected:
	VstTimeInfo m_vstTimeInfo;

private:
	static CsVstPluginHost *m_host;

	struct VstPluginToInsert {
		AEffect *effect;
		SamAudioToolInsert *insert;
	};
	AList<VstPluginToInsert> m_plugsToInserts;
};

#endif __CsVstPluginHost_h__