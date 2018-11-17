#include "SamPreferences.h"

//#include "APlatform.h"
#include "Shlobj.h"

#include <QApplication>
#include "ALog.h"

SamPreferences* SamPreferences::m_instance = 0;

SamPreferences::SamPreferences()
{
	m_instance = this;

	//PWSTR p = NULL;
	//SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &p); // Windows 7 / 2008 Only
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);
	AString ps(szPath);
	ps.append("\\Sam");
	aLog("SHGetKnownFolderPath[%s]", ps.toChar());
	//CoTaskMemFree(p);

	if(!CreateDirectory(ps.data(), 0)) {
		aLog("CreateDirectory failed.");
		DWORD err = GetLastError();
		if(err == ERROR_ALREADY_EXISTS) {
			aLog("ERROR_ALREADY_EXISTS");
		} else if(err == ERROR_PATH_NOT_FOUND) {
			aLog("ERROR_PATH_NOT_FOUND");
		}
	}

	// Need to do this with Qt because Windows call return incorrect path
	//QByteArray path = QString(qApp->applicationDirPath()+"//engine.prf").toLatin1();
	//m_engine = new APreferences(path.data());

	//AString ep = ps + "\\engine.prf";
	//m_engine = new APreferences(ep);

	//path = QString(qApp->applicationDirPath()+"//recent.prf").toLatin1();
	//m_recentFiles = new APreferences(path.data());

	m_engine = new APreferences(ps + "\\engine.prf");
	m_recentFiles = new APreferences(ps + "\\recent.prf");
	m_app = new APreferences(ps + "\\app.prf");
	m_editor = new APreferences(ps + "\\editor.prf");
}

SamPreferences::~SamPreferences()
{
	delete m_engine;
	delete m_recentFiles;
	delete m_app;
	delete m_editor;
}

SamPreferences* SamPreferences::instance(void)
{
	return m_instance;
}