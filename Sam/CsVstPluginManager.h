#ifndef __CsVstPluginManager_h__
#define __CsVstPluginManager_h__

#include "AList.h"
#include "AString.h"
using namespace Atom;

#include "CsVstPlugin.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CsVstPluginManagerScannerCallback
{
public:
	virtual void scanningPlugin(AString name) = 0;
};

// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CsVstPluginManager
{
public:
	CsVstPluginManager();

	void addPath(AString path);

	void scanPlugins(void);

	void setCallback(CsVstPluginManagerScannerCallback *callback) { m_callback = callback; }

	void debugPluginEntries(void);

	struct CsVstPluginEntry {
		TCHAR filename[MAX_PATH];
		char productName[256];
		VstInt32 uniqueId;
	};

	int numPlugins(void) { return m_entries.size(); }
	TCHAR* pluginFilename(int index) { return m_entries[index].filename; }
	char* pluginProductName(int index) { return m_entries[index].productName; }
	TCHAR* filenameFromUniqueId(VstInt32 id);

protected:
private:
	void parseDirectory(TCHAR *szDir);
	void addPlugin(TCHAR *filename);

	AList<AString> m_paths;
	CsVstPluginManagerScannerCallback *m_callback;

	AList<CsVstPluginEntry> m_entries;
};

#endif // __CsVstPluginManager_h__