#include "CsVstPluginManager.h"

#include "ALog.h"

#include <strsafe.h>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
CsVstPluginManager::CsVstPluginManager()
	: m_callback(0)
{
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginManager::addPath(AString path)
{
	m_paths.append(path);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginManager::scanPlugins(void)
{
	aLog("CsVstPluginManager::scanPlugins()");

	for(int i=0; i<m_paths.size(); i++) {
		TCHAR szDir[MAX_PATH];

		StringCchCopy(szDir, MAX_PATH, m_paths[i].data());
		parseDirectory(szDir);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginManager::debugPluginEntries(void)
{
	aLog("---------- CsVstPluginManager::debugPluginEntries() ----------");

	aLog("Number of entries = %d", m_entries.size());

	char data[5];
	memset(data, 0, 5);

	for(int i=0; i<m_entries.size(); i++) {
		data[0] = (char)m_entries[i].uniqueId;
		data[1] = (char)(m_entries[i].uniqueId>>8);
		data[2] = (char)(m_entries[i].uniqueId>>16);
		data[3] = (char)(m_entries[i].uniqueId>>24);
		aLog("[%s] %s (%s)", data, m_entries[i].productName, AString(m_entries[i].filename).toChar());
	}

	aLog("--------------------------------------------------------------");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
TCHAR* CsVstPluginManager::filenameFromUniqueId(VstInt32 id)
{
	aLog("CsVstPluginManager::filenameFromUniqueId(%d)", id);
	for(int i=0; i<m_entries.size(); i++) { aLog("Entry %d/%d %d = %d", i+1, m_entries.size(), m_entries[i].uniqueId, id);
		if(m_entries[i].uniqueId == id) { aLog("filename = %s", m_entries[i].filename);
			return m_entries[i].filename;
		}
	}

	return NULL;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginManager::parseDirectory(TCHAR *szDir)
{
	// Directory parsing code adapted from: http://msdn.microsoft.com/en-us/library/windows/desktop/aa365200(v=vs.85).aspx

	WIN32_FIND_DATA ffd;

	TCHAR szDirFind[MAX_PATH];
	StringCchCopy(szDirFind, MAX_PATH, szDir);
	StringCchCat(szDirFind, MAX_PATH, TEXT("\\*"));

	HANDLE hFind = hFind = FindFirstFile(szDirFind, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			//aLog("Entry: %s", AString(ffd.cFileName));

			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				AString dir(ffd.cFileName);
				aLog("<dir> %s", dir.toChar());

				if(!dir.isEqualTo(".") && !dir.isEqualTo("..")) {
					TCHAR szDirNext[MAX_PATH];
					StringCchCopy(szDirNext, MAX_PATH, szDir);

					StringCchCat(szDirNext, MAX_PATH, TEXT("\\"));
					StringCchCat(szDirNext, MAX_PATH, ffd.cFileName);
					parseDirectory(szDirNext);
				}

			} else {
				AString filename(ffd.cFileName);
				aLog("%s",filename.toChar());
				
				if(filename.length() > 4 &&(filename.last(4).isEqualTo(".dll") || filename.last(4).isEqualTo(".DLL"))) {
					TCHAR filename[MAX_PATH];
					StringCchCopy(filename, MAX_PATH, szDir);
					StringCchCat(filename, MAX_PATH, TEXT("\\"));
					StringCchCat(filename, MAX_PATH, ffd.cFileName);
					addPlugin(filename);
				}
			}
		} while (FindNextFile(hFind, &ffd) != 0);
			
		FindClose(hFind);

	} else {
		aLog("CsVstPluginManager::parseDirectory() INVALID_HANDLE_VALUE %s", szDir);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CsVstPluginManager::addPlugin(TCHAR *filename)
{
	aLog("CsVstPluginManager::addPlugin(%s)", filename);

	CsVstPlugin *plug = new CsVstPlugin(samEngine, filename);

	if(plug->open()) {
		char text[256]; 
		plug->getHostProductString(text); 
		if(m_callback) {
			m_callback->scanningPlugin(text);
		}

		if(!plug->isSynth() && plug->getVstVersion() >= 2300) { // We only support VST 2.3 and 2.4 plug-ins
			CsVstPluginEntry entry;
			StringCchCopy(entry.filename, MAX_PATH, filename);
			strcpy(entry.productName, text);
			entry.uniqueId = plug->uniqueId();
			m_entries.append(entry);
		}
	}

	plug->close(); aLog("After plug->close()");

	delete plug;
}