#ifndef SAMGUICHANNELINPUT_H
#define SAMGUICHANNELINPUT_H

#include <QWidget>
//#include "ui_SamGuiChannelInput.h"

#include "UiLabel.h"
#include "UiSelectMidiChannel.h"
#include "SamGuiSelectMidiInputBus.h"
#include "UiStateButton.h"

#include "AStateManager.h"

#include "SamChannel.h"

class SamGuiChannelInput : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelInput(QWidget *parent = 0);
	~SamGuiChannelInput();

	void onState(AStateEvent *ev);

public slots:
	void setMidiChannel(int itemIndex);
	void updateSelectedChannel(void);
	void updateMidiChannel(void);
	void updateMidiMuted(void);
	void setMidiMuted(int id, int flags);

signals:
	void selectedChannelUpdated(void);
	void midiChannelUpdated(void);
	void midiMutedUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiChannelInputClass ui;
	SamChannel::Ptr m_channel;
	UiLabel *labelMidiPort;
	UiLabel *labelMidiChannel;
	UiStateButton *buttonMidiMuted;
	//UiSelectMidiPort *selectMidiPort;
	SamGuiSelectMidiInputBus *selectMidiInput;
	UiSelectMidiChannel *selectMidiChannel;
	QPixmap *m_pixBackground;
};

#endif // SAMGUICHANNELINPUT_H
