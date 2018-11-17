#ifndef SAMGUICHANNELLIST_H
#define SAMGUICHANNELLIST_H

#include "UiList.h"
//#include "ui_SamGuiChannelList.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamChannel.h"

class SamGuiChannelList : public UiList, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelList(QWidget *parent = 0);
	~SamGuiChannelList();

	void onState(AStateEvent *ev);

	void updateLabels(void);
	void updateArticulations(void);

public slots:
	void addChannel(SamChannel *channel);
	void removeChannel(SamChannel *channel);

signals:
	void channelAdded(SamChannel *channel);
	void channelRemoved(SamChannel *channel);
	void updated(void);
	

protected:
	void timerEvent(QTimerEvent *ev);

private:
	//Ui::SamGuiChannelListClass ui;
};

#endif // SAMGUICHANNELLIST_H
