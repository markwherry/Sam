#ifndef SAMGUIMIXERPRESETSECTION_H
#define SAMGUIMIXERPRESETSECTION_H

#include "UiSection.h"
#include "ui_SamGuiMixerPresetSection.h"

#include "SamGuiSelectPreset.h"
#include "SamGuiPresetDialog.h"
#include "UiStateButton.h"

#include "SamChannel.h"

class SamGuiStreamMixer;

class SamGuiMixerPresetSection : public UiSection
{
	Q_OBJECT

public:
	SamGuiMixerPresetSection(QWidget *parent);
	~SamGuiMixerPresetSection();

	void setChannel(SamChannel *channel);

public slots:
	void buttonClicked(int id);
	void addPreset(QString uuid, QString name);
	void selectPreset(int index);

private:
	//Ui::SamGuiMixerPresetSectionClass ui;
//	SamGuiSelectPreset *m_selectPreset;
//	UiStateButton *m_buttonAddPreset;
//	UiStateButton *m_buttonRemovePreset;
	UiStateButton *m_buttonSaveToInst;
	SamGuiPresetDialog *m_dialog;
	SamChannel::Ptr m_channel;
};

#endif // SAMGUIMIXERPRESETSECTION_H
