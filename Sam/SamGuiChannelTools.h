#ifndef SAMGUICHANNELTOOLS_H
#define SAMGUICHANNELTOOLS_H

#include <QWidget>
//#include "ui_SamGuiChannelTools.h"

#include "AStateManager.h"

#include "SamGuiMusicToolInsert.h"

class SamGuiChannelTools : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelTools(QWidget *parent = 0);
	~SamGuiChannelTools();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel(void);

signals:
	void selectedChannelUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiChannelToolsClass ui;
	SamChannel::Ptr m_channel;
	SamGuiMusicToolInsert *m_insert[SamChannel::maxMusicTools];
	QPixmap *m_pixBackground;
};

#endif // SAMGUICHANNELTOOLS_H
