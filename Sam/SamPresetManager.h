#ifndef __SamPresetManager_h__
#define __SamPresetManager_h__

//#include "AString.h"
//using namespace Atom;

#include <QString>
#include <QHash>
#include <QList>

//#include "AList.h"
#include "AModel.h"
#include "AXmlDocument.h"
using namespace Atom;

struct SamPresetInfo
{
	QString uuid;
	QString name;
	QString filename;
};

class SamPresetManager : public AModel
{
public:
	SamPresetManager();
	~SamPresetManager();

	int numPresets(QString uuid);
	QString presetName(QString uuid, int index);

	void addPreset(QString uuid, QString name, AXmlElement *preset);
	void removePreset(QString uuid, QString name);
	bool doesPresetExist(QString uuid, QString name);

protected:
private:
	AString m_presetPath;
	QList<SamPresetInfo*> m_presets;
};

#endif // __SamPresetManager_h__