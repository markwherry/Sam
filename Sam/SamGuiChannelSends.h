#ifndef SAMGUICHANNELSENDS_H
#define SAMGUICHANNELSENDS_H

#include <QWidget>
//#include "ui_SamGuiChannelSends.h"

#include "SamGuiMusicToolInsert.h"

#include "SamGuiSend.h"

#include "SamChannel.h"

class SamGuiChannelSends : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelSends(QWidget *parent = 0);
	~SamGuiChannelSends();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel(void);

signals:
	void selectedChannelUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiChannelSendsClass ui;
	SamChannel::Ptr m_channel;

	SamGuiSend *m_sends[SamAudioBus::maxSends];

	QPixmap *m_pixBackground;
};

#endif // SAMGUICHANNELSENDS_H
