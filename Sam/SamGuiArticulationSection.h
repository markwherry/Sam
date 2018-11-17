#ifndef SAMGUIARTICULATIONSECTION_H
#define SAMGUIARTICULATIONSECTION_H

#include "UiSection.h"
//#include "ui_SamGuiArticulationSection.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamChannel.h"

#include "UiStateButton.h"

class SamGuiArticulationSection : public UiSection, public AStateListener
{
	Q_OBJECT

public:
	SamGuiArticulationSection(QWidget *parent = 0);
	~SamGuiArticulationSection();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel(void);
	void setCrossfadeMode(int id, int flags);
	void updateCrossfadeMode(void);
	void setAsDefault(int id);

signals:
	void selectedChannelUpdated(void);
	void crossfadeUpdated(void);

private:
	//Ui::SamGuiArticulationSection ui;
	SamChannel::Ptr m_channel;

	UiStateButton  *m_buttonCrossfade;
	UiStateButton  *m_buttonStore;
};

#endif // SAMGUIARTICULATIONSECTION_H
