#ifndef SAMGUIOUTPUTCONFIGSECTION_H
#define SAMGUIOUTPUTCONFIGSECTION_H

#include "UiSection.h"
//#include "ui_SamGuiOutputConfigSection.h"

//#include "AStateManager.h"

#include "UiStateButton.h"

#include "SamChannel.h"

#include "CsSpeakerConfig.h"
using namespace Cs;

class SamGuiOutputConfigSection : public UiSection//, public AStateListener
{
	Q_OBJECT

public:
	SamGuiOutputConfigSection(QWidget *parent = 0);
	~SamGuiOutputConfigSection();

	//void onState(AStateEvent *ev);

	void setSpeakerConfig(SamChannel *channel, CsSpeakerConfig config);

public slots:
	void outputClicked(int id);
	void refresh(void);

private:
	//Ui::SamGuiOutputConfigSectionClass ui;
	SamChannel::Ptr m_channel;
	CsSpeakerConfig m_config;
	UiStateButton *m_button50;
	UiStateButton *m_buttonQuad;
	UiStateButton *m_buttonStereo;
	UiStateButton *m_buttonMono;
};

#endif // SAMGUIOUTPUTCONFIGSECTION_H
