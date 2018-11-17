#ifndef SAMGUICHANNELCROSSFADE_H
#define SAMGUICHANNELCROSSFADE_H

#include <QWidget>
//#include "ui_SamGuiChannelCrossfade.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamChannel.h"

#include "UiStateButton.h"
#include "SeGuiItemCurve.h"

class SamGuiChannelCrossfade : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelCrossfade(QWidget *parent = 0);
	~SamGuiChannelCrossfade();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel(void);
	void setCrossfadeMode(int id, int flags);
	void updateCrossfadeMode(void);
	void setValue(int propertyId, float value);

signals:
	void selectedChannelUpdated(void);
	void crossfadeUpdated(void);

private:
	//Ui::SamGuiChannelCrossfade ui;
	SamChannel::Ptr m_channel;

	UiStateButton  *m_buttonCrossfade;
	SeGuiItemCurve *m_curve;
};

#endif // SAMGUICHANNELCROSSFADE_H
