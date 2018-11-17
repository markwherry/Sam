#ifndef SAMGUISELECTPRESET_H
#define SAMGUISELECTPRESET_H

#include "UiSelectMenu.h"
//#include "ui_SamGuiSelectPreset.h"

#include "AStateManager.h"
using namespace Atom;

class SamGuiSelectPreset : public UiSelectMenu, public AStateListener
{
	Q_OBJECT

public:
	SamGuiSelectPreset(QWidget *parent, QString uuid);
	~SamGuiSelectPreset();

	void onState(AStateEvent *ev);

	void setUuid(QString uuid);

public slots:
	void updatePresets(int item = 0);
	void internalSelectItem(int index);

signals:
	void presetsUpdated(int item);
	void presetSelected(int index);

private:
	//Ui::SamGuiSelectPresetClass ui;
	QString m_uuid;
};

#endif // SAMGUISELECTPRESET_H
