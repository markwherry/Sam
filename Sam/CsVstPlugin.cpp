#include "CsVstPlugin.h"

#include "CsVstPluginHost.h"

#include "ALog.h"

#include <strsafe.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsVstPlugin::CsVstPlugin(CsVstPluginHost *host, TCHAR *filename)
	: m_host(host)
	, m_library(NULL)
	, m_effect(NULL)
{
	StringCchCopy(m_filename, MAX_PATH, filename);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
//CsVstPlugin::~CsVstPlugin()
//{
//}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::resume(void)
{
	//aLog("CsVstPlugin::resume()");
	m_effect->dispatcher(m_effect, effMainsChanged, 0, 1, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::suspend(void)
{
	//aLog("CsVstPlugin::suspend()");
	m_effect->dispatcher(m_effect, effMainsChanged, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::startProcess(void)
{
	//aLog("CsVstPlugin::startProcess()");
	m_effect->dispatcher(m_effect, effStartProcess, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::stopProcess(void)
{
	//aLog("CsVstPlugin::stopProcess()");
	m_effect->dispatcher(m_effect, effStopProcess, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsVstPlugin::open(void)
{
	// Load the DLL
	// ------------
	m_library = LoadLibrary(m_filename);
	if (m_library == NULL) {
		aLog("CsVstPlugin::open() LoadLibrary(): Error %d", GetLastError());
		return 0;
	}

	// Get entry point
	// ---------------
	PluginEntryProc mainProc = (PluginEntryProc)GetProcAddress(m_library, "VSTPluginMain");
	if(!mainProc) {
		mainProc = (PluginEntryProc)GetProcAddress(m_library, "main");
	}

	if(mainProc == NULL) {
		aLog("CsVstPlugin::open() mainProc: Error");
		close();
		return 0;
	}

	// Create an instance
	// ------------------
	m_effect = mainProc(m_host->InternalHostCallback);

	if (m_effect == NULL) {
		aLog("effect: Plug-in instantiation error");
		close();
		return 0;
	}

	// Check the magic number
	// ----------------------
	if(m_effect->magic==kEffectMagic) {
		aLog("It's a valid VST plugin");
	} else {
		aLog("Not a VST plugin");
		close();
		return 0;
	}

	// Initialise the plug-in
	// ----------------------
	m_effect->dispatcher(m_effect, effOpen, 0, 0, 0, 0);
	m_effect->dispatcher(m_effect, effSetSampleRate, 0, 0, 0, m_host->getSampleRate()); aLog("m_host->getSampleRate() = %f", m_host->getSampleRate());
	m_effect->dispatcher(m_effect, effSetBlockSize, 0, m_host->getBufferSize(), 0, 0); aLog("m_host->getBufferSize()  = %d", m_host->getBufferSize());

	return 1;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::close(void)
{
	aLog("CsVstPlugin::close()");

	if(m_effect != NULL) { aLog("A");
		m_effect->dispatcher(m_effect, effClose, 0, 0, 0, 0); aLog("B");

		m_effect = NULL; aLog("");
	}

	if(m_library != NULL) { aLog("D");
		FreeLibrary(m_library); aLog("E");
		m_library = NULL; aLog("F");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	//aLog("CsVstPlugin::processReplacing()");

	m_effect->processReplacing(m_effect, inputs, outputs, sampleFrames);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsVstPlugin::getHostProductString(char *text)
{
	return (bool)m_effect->dispatcher(m_effect, effGetProductString, 0, 0, text, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsVstPlugin::getSpeakerArrangement(VstSpeakerArrangement **pluginInput, VstSpeakerArrangement **pluginOutput)
{
	return (bool)m_effect->dispatcher(m_effect, effGetSpeakerArrangement, 0, (VstIntPtr)pluginInput, pluginOutput, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsVstPlugin::setSpeakerArrangement(VstSpeakerArrangement *pluginInput, VstSpeakerArrangement *pluginOutput)
{
	return (bool)m_effect->dispatcher(m_effect, effSetSpeakerArrangement, 0, (VstIntPtr)pluginInput, pluginOutput, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsVstPlugin::getVstVersion(void)
{
	return m_effect->dispatcher(m_effect, effGetVstVersion, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsVstPlugin::programsAreChunks(void)
{
	return m_effect->flags & effFlagsProgramChunks;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
bool CsVstPlugin::isSynth(void)
{
	return m_effect->flags & effFlagsIsSynth;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsVstPlugin::editorOpen(HWND hWnd)
{
	//CSDEBUG (L"CsVSTPlugin::openEditor ()");

	return m_effect->dispatcher(m_effect, effEditOpen, 0, 0, hWnd, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsVstPlugin::editorClose (void)
{
	return m_effect->dispatcher(m_effect, effEditClose, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int CsVstPlugin::editorGetRect (RECT *rect)
{
	ERect* eRect = 0;
	m_effect->dispatcher(m_effect, effEditGetRect, 0, 0, &eRect, 0);
	rect->top = eRect->top;
	rect->left = eRect->left;
	rect->bottom = eRect->bottom;
	rect->right = eRect->right;
	return 1;		
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPlugin::editorIdle(void)
{
	m_effect->dispatcher(m_effect, effEditIdle, 0, 0, 0, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstInt32 CsVstPlugin::getChunk(void **data, bool isPreset)
{
	return m_effect->dispatcher(m_effect, effGetChunk, 0, 0, data, 0);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
VstInt32 CsVstPlugin::setChunk(void *data, VstInt32 size, bool isPreset)
{
	m_effect->dispatcher(m_effect, effBeginSetProgram, 0, 0, 0, 0);
	m_effect->dispatcher(m_effect, effSetChunk, 0, size, data, 0);
	m_effect->dispatcher(m_effect, effEndSetProgram, 0, 0, 0, 0);

	return 1;
}