#ifndef SAMGUISELECTMIDIINPUTBUS_H
#define SAMGUISELECTMIDIINPUTBUS_H

#include "UiSelectMenu.h"
//#include "ui_SamGuiSelectMidiInput.h"

#include "SamChannel.h"
#include "AStateManager.h"

class SamGuiSelectMidiInputBus : public UiSelectMenu, public AStateListener
{
	Q_OBJECT

public:
	SamGuiSelectMidiInputBus(QWidget *parent, SamChannel *channel);
	~SamGuiSelectMidiInputBus();

	void onState(AStateEvent *ev);

	void refresh(void);

	void setParentChannel(SamChannel *channel);

public slots:
	void selectInput(QAction *action);
	void updateMidiPort(void);

signals:
	void midiPortUpdated();

private:
	//Ui::SamGuiSelectMidiInputClass ui;
	SamChannel::Ptr m_channel;
	QAction *m_actionNoInput;
	QList<QAction*> m_inputActions;
};

#endif // SAMGUISELECTMIDIINPUTBUS_H
