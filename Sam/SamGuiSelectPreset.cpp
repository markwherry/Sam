#include "SamGuiSelectPreset.h"

#include "Sam.h"

SamGuiSelectPreset::SamGuiSelectPreset(QWidget *parent, QString uuid)
	: UiSelectMenu(parent, 136)
{
	//ui.setupUi(this);

	setUuid(uuid);

	samGui->presetManager()->registerListener(this, 0);

	connect(this, SIGNAL(presetsUpdated(int)), this, SLOT(updatePresets(int)));
	connect(this, SIGNAL(itemSelected(int)), this, SLOT(internalSelectItem(int)));

	updatePresets();
}

SamGuiSelectPreset::~SamGuiSelectPreset()
{

}

void SamGuiSelectPreset::onState(AStateEvent *ev)
{
	qDebug("SamGuiSelectPreset::onState()");

	if(QString(ev->string().toChar()) == m_uuid) {
		emit presetsUpdated(ev->item());
	}
}

void SamGuiSelectPreset::setUuid(QString uuid)
{
	m_uuid = uuid;

	int numPresets = samGui->presetManager()->numPresets(m_uuid);

	if(numPresets > 0) {
		for(int i=0; i<numPresets; i++) {
			addItem(samGui->presetManager()->presetName(m_uuid, i));
		}
	} else {
		//addItem("No Presets");
		//setDisabled(true);
	}
}

void SamGuiSelectPreset::updatePresets(int item)
{
	qDebug("SamGuiSelectPreset::updatePresets()");

	clear();

	addItem("Default Preset");

	for(int i=0; i<samGui->presetManager()->numPresets(m_uuid); i++) {
		addItem(samGui->presetManager()->presetName(m_uuid, i));
	}

	if(item == 1000) {
		qDebug("item=1000");
		setSelectedItem(numItems()-1);
	}
}

void SamGuiSelectPreset::internalSelectItem(int index)
{
	qDebug("SamGuiSelectPreset::internalSelectItem(%d)", index);

	emit presetSelected(index);
}