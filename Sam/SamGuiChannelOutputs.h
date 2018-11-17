#ifndef SAMGUICHANNELOUTPUTS_H
#define SAMGUICHANNELOUTPUTS_H

#include <QWidget>
//#include "ui_SamGuiChannelOutputs.h"

#include "AStateManager.h"

#include "UiLabel.h"

#include "SamGuiSelectAudioOutputBus.h"

#include "SamChannel.h"

class SamGuiChannelOutputs : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelOutputs(QWidget *parent = 0);
	~SamGuiChannelOutputs();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel();

signals:
	void selectedChannelUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiChannelOutputsClass ui;
	SamChannel::Ptr m_channel;

	UiLabel *labelChannelOutput[SamChannel::maxOutputs];
	SamGuiSelectAudioOutputBus *selectAudioBus[SamChannel::maxOutputs];

	QPixmap *m_pixBackground;
};

#endif // SAMGUICHANNELOUTPUTS_H
