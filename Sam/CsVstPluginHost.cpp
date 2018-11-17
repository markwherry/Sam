#include "CsVstPluginHost.h"

#include "ALog.h"

CsVstPluginHost *CsVstPluginHost::m_host = NULL; 

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsVstPluginHost::CsVstPluginHost()
{
	m_host = this;

	m_vstTimeInfo.samplePos          = 0;     // current Position in audio samples (always valid) 
	m_vstTimeInfo.sampleRate         = 0;     // current Sample Rate in Herz (always valid) 
	m_vstTimeInfo.nanoSeconds        = 0;     // System Time in nanoseconds (10^-9 second). 
	m_vstTimeInfo.ppqPos             = 0;     // Musical Position, in Quarter Note (1.0 equals 1 Quarter Note). 
	m_vstTimeInfo.tempo              = 120.0; // current Tempo in BPM (Beats Per Minute)
	m_vstTimeInfo.barStartPos        = 0;     // last Bar Start Position, in Quarter Note
	m_vstTimeInfo.cycleStartPos      = 0;     // Cycle Start (left locator), in Quarter Note. 
	m_vstTimeInfo.cycleEndPos        = 0;     // Cycle End (right locator), in Quarter Note.
	m_vstTimeInfo.timeSigNumerator   = 4;     // Time Signature Numerator (e.g. 3 for 3/4). 
	m_vstTimeInfo.timeSigDenominator = 4;     // Time Signature Denominator (e.g. 4 for 3/4). 
	m_vstTimeInfo.smpteOffset        = 0;     // SMPTE offset (in SMPTE subframes (bits; 1/80 of a frame)). The current SMPTE position can be calculated using samplePos, sampleRate, and smpteFrameRate. 
	m_vstTimeInfo.smpteFrameRate     = 24;    // 
	m_vstTimeInfo.samplesToNextClock = 0;     // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest clock). 
	m_vstTimeInfo.flags              = 0;     //
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstIntPtr VSTCALLBACK CsVstPluginHost::InternalHostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	//aLog("CsVstPluginHost::InternalHostCallback()");

	return m_host->HostCallback(effect, opcode, index, value, ptr, opt);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstIntPtr VSTCALLBACK CsVstPluginHost::HostCallback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt)
{
	aLog("***> CsVstPluginHost::HostCallback(%d)", opcode);

	VstIntPtr result = 0;

	/*audioMasterAutomate = 0,	///< [index]: parameter index [opt]: parameter value  @see AudioEffect::setParameterAutomated
	audioMasterVersion,			///< [return value]: Host VST version (for example 2400 for VST 2.4) @see AudioEffect::getMasterVersion
	audioMasterCurrentId,		///< [return value]: current unique identifier on shell plug-in  @see AudioEffect::getCurrentUniqueId
	audioMasterIdle,			///< no arguments  @see AudioEffect::masterIdle
	
	audioMasterGetTime,				///< [return value]: #VstTimeInfo* or null if not supported [value]: request mask  @see VstTimeInfoFlags @see AudioEffectX::getTimeInfo
	audioMasterProcessEvents,		///< [ptr]: pointer to #VstEvents  @see VstEvents @see AudioEffectX::sendVstEventsToHost
	
	*/

	switch(opcode) {
		case audioMasterAutomate:
			aLog("audioMasterAutomate");
			break;
		case audioMasterVersion:
			aLog("audioMasterVersion");
			result = 2400;
			break;
		case audioMasterCurrentId:
			aLog("audioMasterCurrentId");
			break;
		case audioMasterIdle:
			aLog("audioMasterIdle");
			break;

		case audioMasterGetVendorString: // 32
			aLog("audioMasterGetVendorString");
			strcpy((char*)ptr, "MW");
			break;
		case audioMasterGetProductString: // 33
			aLog("audioMasterGetProductString");
			strcpy((char*)ptr, "Sam");
			break;
	}


	return result;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginHost::addPluginToInsert(AEffect *effect, SamAudioToolInsert *insert)
{
	VstPluginToInsert vpti;
	vpti.effect = effect;
	vpti.insert = insert;

	m_plugsToInserts.append(vpti);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamAudioToolInsert* CsVstPluginHost::insertFromEffect(AEffect *effect)
{
	for(int i=0; i<m_plugsToInserts.size(); i++) {
		if(m_plugsToInserts[i].effect == effect) {
			return m_plugsToInserts[i].insert;
		}
	}

	return 0;
}