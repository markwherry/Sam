#include "SamGuiMixerPresetSection.h"

#include "Sam.h"
#include "SamCommands.h"
#include "SamEngine.h"
#include "SamProject.h"

#include <QMessageBox>

SamGuiMixerPresetSection::SamGuiMixerPresetSection(QWidget *parent)
	: UiSection(parent, "Stream Mixer Presets")
	, m_channel(0)
{
	//ui.setupUi(this);
	setFixedHeight(50); //125

	int y = 4;

	/*m_selectPreset = new SamGuiSelectPreset(this, "StreamMixer");
	m_selectPreset->move(4, y+=24);
	m_selectPreset->setDrawLine(true);
	m_selectPreset->setTextSize(8);
	m_selectPreset->setLineColor(QColor(90,90,90));

	m_buttonAddPreset = new UiStateButton(this, 100, 1);
	m_buttonAddPreset->setDrawFill(false);
	//m_buttonAddPreset->setStateImage(0, ":/Resources/sc_50_16.png");
	//m_buttonAddPreset->setStateImage(1, ":/Resources/sc_50_16_on.png");
	m_buttonAddPreset->setFixedSize(16,16);
	m_buttonAddPreset->move(4, y+=24);
	m_buttonAddPreset->setText("Add Preset");

	m_buttonRemovePreset = new UiStateButton(this, 101, 1);
	m_buttonRemovePreset->setDrawFill(false);
	//m_buttonRemovePreset->setStateImage(0, ":/Resources/sc_50_16.png");
	//m_buttonRemovePreset->setStateImage(1, ":/Resources/sc_50_16_on.png");
	m_buttonRemovePreset->setFixedSize(16,16);
	m_buttonRemovePreset->move(4, y+=24);
	m_buttonRemovePreset->setText("Remove Preset");*/

	m_buttonSaveToInst = new UiStateButton(this, 110, 1);
	m_buttonSaveToInst->setDrawFill(false);
	m_buttonSaveToInst->setStateImage(0, ":/Resources/savemixtoinstrument16.png");
	m_buttonSaveToInst->setStateImage(1, ":/Resources/savemixtoinstrument16.png");
	m_buttonSaveToInst->setFixedSize(16,16);
	m_buttonSaveToInst->move(4, y+=24);
	m_buttonSaveToInst->setText("Save Mix to Instrument");

//	connect(m_buttonAddPreset, SIGNAL(clicked(int)), this, SLOT(buttonClicked(int)));
//	connect(m_buttonRemovePreset, SIGNAL(clicked(int)), this, SLOT(buttonClicked(int)));
	connect(m_buttonSaveToInst, SIGNAL(clicked(int)), this, SLOT(buttonClicked(int)));

	m_dialog = new SamGuiPresetDialog(this, "StreamMixer");
	connect(m_dialog, SIGNAL(presetAdded(QString, QString)), this, SLOT(addPreset(QString, QString)));

//	connect(m_selectPreset, SIGNAL(presetSelected(int)), this, SLOT(selectPreset(int)));
	
}

SamGuiMixerPresetSection::~SamGuiMixerPresetSection()
{

}

void SamGuiMixerPresetSection::setChannel(SamChannel *channel)
{
	m_channel = channel;
}

void SamGuiMixerPresetSection::buttonClicked(int id)
{
	qDebug("SamGuiMixerPresetSection::buttonClicked(%d)", id);

	switch(id) {
		/*case 100: // Add
			{
				m_dialog->move(mapToGlobal(QPoint(width()-m_dialog->width(),m_buttonAddPreset->y()+m_buttonAddPreset->height())));
				m_dialog->show();
			}
			break;

		case 101: // Remove
			break;*/

		case 110:
			{
				SamChannel *channel = samEngine->project()->selectedChannel();
				if(channel) {
					samGui->commandManager()->executeInBackground(new SetInstrumentStreamMix(channel, false));
				}
			}
			break;
	}
}

void SamGuiMixerPresetSection::addPreset(QString uuid, QString name)
{
	qDebug("SamGuiMixerPresetSection::addPreset(%s, %s)", qPrintable(uuid), qPrintable(name));

	/*if(name.length() < 1) {
		return;
	}

	int ret = 0;

	if(samGui->presetManager()->doesPresetExist(uuid, name)) {
		ret = QMessageBox::question(this, "Sam", "<b>A preset with the name you've chosen already exists.</b><br><br>Would you like to replace the existing preset?","No", "Yes (Replace)");
		
	} else {
		ret = 1;
	}

	if(ret == 1) {
		qDebug("Yes, replace.");
		AXmlDocument document;

		AXmlElement *element = document.createElement("Preset");
		m_channel->streamMix()->writeElement(&document, element);
		samGui->presetManager()->addPreset(uuid, name, element);
	}*/
}

void SamGuiMixerPresetSection::selectPreset(int index)
{
	qDebug("SamGuiMixerPresetSection::selectPreset(%d)", index);

	//samGui->commandManager()->executeInBackground(new SetStreamMixPreset(m_channel, index));
}