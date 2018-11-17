#include "SamPresetManager.h"

#include "ALog.h"
#include "AFile.h"

#include <QApplication>
#include <QDir>

#include "Shlobj.h"

SamPresetManager::SamPresetManager()
: AModel(0)
{
	aLog("========== Initialising SamPresetManager ==========");

	//PWSTR p = NULL;
	//SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &p);  // Windows 7 / 2008 Only
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath);
	m_presetPath = AString(szPath);
	m_presetPath.append("\\Sam\\Presets");
	aLog("SHGetKnownFolderPath[%s]", m_presetPath.toChar());
	//CoTaskMemFree(p);

	
	if(!CreateDirectory(m_presetPath.data(), 0)) {
		aLog("CreateDirectory failed.");
		DWORD err = GetLastError();
		if(err == ERROR_ALREADY_EXISTS) {
			aLog("ERROR_ALREADY_EXISTS");
		} else if(err == ERROR_PATH_NOT_FOUND) {
			aLog("ERROR_PATH_NOT_FOUND");
		}
	}

	/*QDir pd(m_presetPath.toChar());
	if(!pd.exists()) {
		aLog("Presets directory does not exist... Creating...");
		QDir adp(qApp->applicationDirPath());
		adp.mkdir("Presets");
	} else {
		aLog("Presets directory exists.");
	}*/

	aLog("===================================================");
}

SamPresetManager::~SamPresetManager()
{
}

int SamPresetManager::numPresets(QString uuid)
{
	int count = 0;

	for(int i=0; i<m_presets.size(); i++) {
		if(m_presets[i]->uuid == uuid) {
			count++;
		}
	}

	return count;
}

QString SamPresetManager::presetName(QString uuid, int index)
{
	int count = 0;
	
	for(int i=0; i<m_presets.size(); i++) {
		if(m_presets[i]->uuid == uuid) {
			if(index == i) {
				return m_presets[i]->name;
			}
			count++;
		}
	}

	return "";
}

void SamPresetManager::addPreset(QString uuid, QString name, AXmlElement *preset)
{
	qDebug("SamPresetManager::addPreset(%s, %s)", qPrintable(uuid), qPrintable(name));

	QByteArray baUuid = uuid.toAscii();
	QByteArray baName = name.toAscii();
	preset->setAttribute("bankUuid", baUuid.data());
	preset->setAttribute("presetName", baName.data());

	AXmlDocument doc;
	doc.setRootElement(preset);

	AString str = doc.toString();
	aLog("[%s]", str.toChar());

	/*QDir pd(qApp->applicationDirPath()+"//Presets//"+uuid);
	if(!pd.exists()) {
		aLog("Preset directory does not exist... Creating...");
		pd.mkpath(qApp->applicationDirPath()+"//Presets//"+uuid);
	} else {
		aLog("Preset directory exists.");
	}*/

	AString presetPath = m_presetPath + AString("\\") + AString(baUuid.data());
	if(!CreateDirectory(presetPath.data(), 0)) {
		aLog("CreateDirectory failed.");
		DWORD err = GetLastError();
		if(err == ERROR_ALREADY_EXISTS) {
			aLog("ERROR_ALREADY_EXISTS");
		} else if(err == ERROR_PATH_NOT_FOUND) {
			aLog("ERROR_PATH_NOT_FOUND");
		}
	}

	//QString filename = qApp->applicationDirPath()+"//Presets//"+uuid+"//"+name+".xml";
	//QByteArray baFn = filename.toAscii();
	
	AString filename = presetPath+AString("\\")+AString(baName.data())+AString(".xml");
	AFile file(filename);

	if(!file.create()) {
		return;
	}

	file.writeBytes(str.length(), str.toChar());
	file.close();

	SamPresetInfo *info = new SamPresetInfo;
	info->filename = filename.toChar();
	info->name     = name;
	info->uuid     = uuid;
	m_presets.append(info);

	updateState(0, 1000, 0, 0, 0, baUuid.data());
}

void SamPresetManager::removePreset(QString uuid, QString name)
{
	for(int i=0; i<m_presets.size(); i++) {
		if(m_presets[i]->uuid == uuid && m_presets[i]->name == name) {
			m_presets.removeAt(i);
			break;
		}
	}

	QByteArray baUuid = uuid.toAscii();
	updateState(0, 1001, 0, 0, 0, baUuid.data());
}

bool SamPresetManager::doesPresetExist(QString uuid, QString name)
{
	for(int i=0; i<m_presets.size(); i++) {
		if(m_presets[i]->uuid == uuid && m_presets[i]->name == name) {
			return true;
		}
	}

	return false;
}