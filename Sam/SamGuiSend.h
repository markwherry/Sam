#ifndef SAMGUISEND_H
#define SAMGUISEND_H

#include <QWidget>
//#include "ui_SamGuiSend.h"

#include "UiFader.h"
#include "UiStateButton.h"

#include "SamGuiSelectAudioOutputBus.h"

#include "SamChannel.h"

class SamGuiSend : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiSend(QWidget *parent, int index);
	~SamGuiSend();

	void onState(AStateEvent *ev);

	void setChannel(SamChannel *channel, int index);

public slots:
	void sendFaderMoved(int faderId, float value, int flags);
	void updateSendGain(void *updatedObject, void *fromObject);
	void updateSendActive(void);
	void updateSendPreFader(void);
	void setSendActive(int id, int flags);
	void setSendPreFader(int id, int flags);

signals:
	void sendGainUpdated(void *updatedObject, void *fromObject);
	void sendActiveUpdated(void);
	void sendPreFaderUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	void drawSendGain(QPainter &p, int x, int y, float gain);
	//Ui::SamGuiSendClass ui;
	SamChannel::Ptr m_channel;
	int m_index;

	UiStateButton              *buttonSend;
	UiStateButton              *buttonPreFader;
	SamGuiSelectAudioOutputBus *selectAudioSendBus;
	UiFader                    *faderSends;
};

#endif // SAMGUISEND_H
