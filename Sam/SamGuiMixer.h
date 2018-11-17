#ifndef SAMGUIMIXER_H
#define SAMGUIMIXER_H

#include <QWidget>
//#include "ui_SamGuiMixer.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamGuiMixerStrip.h"

#include "SamChannel.h"

class SamGuiMixer : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMixer(QWidget *parent = 0);
	~SamGuiMixer();

	void onState(AStateEvent *ev);

public slots:
	void addChannel(SamChannel *channel, bool exclusive);
	void removeChannel(SamChannel *channel);
	void updateSelection(void);

signals:
	void channelAdded(SamChannel *channel);
	void channelRemoved(SamChannel *channel);
	void audioBusSelected(SamChannel *channel, int audioBusIndex);

private:
	//Ui::SamGuiMixerClass ui;
	//SamChannel::Ptr m_channel;
	QList<SamGuiMixerStrip*> m_strips;
};

#endif // SAMGUIMIXER_H
