#ifndef __CsVstPlugin_h__
#define __CsVstPlugin_h__

#include <Windows.h>

#include "Vst/aeffectx.h"

class CsVstPluginHost;

class CsVstPlugin
{
public:
	CsVstPlugin(CsVstPluginHost *host, TCHAR *filename);
	//~CsVstPlugin();

	int open(void);
	void close(void);

	void resume(void);
	void suspend(void);

	void startProcess(void);
	void stopProcess(void);

	void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);
	void processEvents(VstEvents* events) { m_effect->dispatcher(m_effect, effProcessEvents, 0, 0, events, 0); }

	bool getHostProductString(char *text);
	bool getSpeakerArrangement(VstSpeakerArrangement **pluginInput, VstSpeakerArrangement **pluginOutput);
	bool setSpeakerArrangement(VstSpeakerArrangement *pluginInput, VstSpeakerArrangement *pluginOutput);
	int getVstVersion(void);

	VstInt32 getNumMidiInputChannels(void) { return m_effect->dispatcher(m_effect, effGetNumMidiInputChannels, 0, 0, 0, 0); }

	bool programsAreChunks(void);
	bool isSynth(void);

	int editorOpen(HWND hWnd);
	int editorClose(void);
	int editorGetRect(RECT *rect);
	void editorIdle(void);

	VstInt32 getChunk(void **data, bool isPreset = false);
	VstInt32 setChunk(void *data, VstInt32 size, bool isPreset = false);

	AEffect* effect(void) { return m_effect; }

	VstInt32 uniqueId(void) { return m_effect->uniqueID; }

	VstInt32 canDo(char* text) { return m_effect->dispatcher(m_effect, effCanDo, 0, 0, text, 0); }

protected:
private:
	CsVstPluginHost *m_host;
	TCHAR            m_filename[MAX_PATH];
	HINSTANCE        m_library;
	AEffect         *m_effect;
};

typedef AEffect* (*PluginEntryProc) (audioMasterCallback audioMaster);

#endif // __CsVstPlugin_h__