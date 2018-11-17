#ifndef SAMGUISELECTAUDIOOUTPUTBUS_H
#define SAMGUISELECTAUDIOOUTPUTBUS_H

#include "UiSelectMenu.h"
//#include "ui_SamGuiSelectAudioBus.h"

#include "AStateManager.h"

#include "SamAudioBus.h"

class SamGuiSelectAudioOutputBus : public UiSelectMenu, public AStateListener
{
	Q_OBJECT

public:
	SamGuiSelectAudioOutputBus(QWidget *parent, SamAudioBus *audioBus, SamAudioSend *audioSend);
	~SamGuiSelectAudioOutputBus();

	void onState(AStateEvent *ev);

	void setParentAudioBus(SamAudioBus *audioBus, SamAudioSend *audioSend);

public slots:
	void refresh(void);
	void selectBus(QAction *action);
	void updateAudioOutput(void);
	
signals:
	void audioOutputsUpdated(void);
	void audioOutputUpdated(void);

private:
	//Ui::SamGuiSelectAudioBusClass ui;
	SamAudioBus *m_audioBus;
	SamAudioSend *m_audioSend;
	QAction *actionNoOutput;
	QList<QAction*> m_busActions;
	QList<QAction*> m_outActions;
};

#endif // SAMGUISELECTAUDIOOUTPUTBUS_H
