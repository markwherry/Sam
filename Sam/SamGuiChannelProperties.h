#ifndef SAMGUICHANNELPROPERTIES_H
#define SAMGUICHANNELPROPERTIES_H

#include <QWidget>
//#include "ui_SamGuiChannelProperties.h"

#include "UiLabel.h"
//#include "UiSelectMidiPort.h"
#include "UiSelectMidiChannel.h"
#include "SamGuiSelectAudioOutputBus.h"
#include "SamGuiSelectMidiInputBus.h"
#include "SamGuiSelectMusicTool.h"
#include "UiFader.h"
#include "UiStateButton.h"

#include "AStateManager.h"

#include "SamChannel.h"

class SamGuiChannelProperties : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelProperties(QWidget *parent = 0);
	~SamGuiChannelProperties();

	void onState(AStateEvent *ev);

	void showAll(void);
	void showAllForMultiple(void);
	void hideAll(void);

public slots:
	//void setMidiPort(int itemIndex);
	void setMidiChannel(int itemIndex);
	void updateSelectedChannel(void);
	//void updateMidiPort(void);
	void updateMidiChannel(void);
	void updateMidiMuted(void);
	void setMidiMuted(int id, int flags);
	void sendFaderMoved(int faderId, float value, int flags);
	void updateSendGain(void *updatedObject, void *fromObject);
	void setDownbeatOffset(int id, int flags);
	void setLayerMutesEnabled(int id, int flags);
	void updateDownbeatOffset(void);
	void updateLayerMutesEnabled(void);
	void setMusicToolActive(int id, int flags);
	void updateMusicToolActive(void);
	void showMusicToolEditor(int id, int flags);
	void updateSendActive(void);
	void setSendActive(int id, int flags);

signals:
	void selectedChannelUpdated(void);
	//void midiPortUpdated(void);
	void midiChannelUpdated(void);
	void midiMutedUpdated(void);
	void downbeatOffsetUpdated(void);
	void sendGainUpdated(void *updatedObject, void *fromObject);
	void layerMutesEnabledUpdated(void);
	void musicToolActiveUpdated(void);
	void sendActiveUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	void drawSendGain(QPainter &p, int x, int y, float gain);
	//Ui::SamGuiChannelPropertiesClass ui;
	SamChannel::Ptr m_channel;
	UiLabel *labelInput;
	UiLabel *labelMidiPort;
	UiLabel *labelMidiChannel;
	UiStateButton *buttonMidiMuted;
	//UiSelectMidiPort *selectMidiPort;
	SamGuiSelectMidiInputBus *selectMidiInput;
	UiSelectMidiChannel *selectMidiChannel;

	UiLabel *labelChannelSettings;
	UiStateButton *m_buttonDownbeatOffset;
	UiStateButton *m_buttonLayerMutesEnabled;
	

	UiLabel *labelMusicTools;
	//UiLabel *labelMusicTool[SamChannel::maxMusicTools];
	UiStateButton *buttonMusicTool[SamChannel::maxMusicTools];
	UiStateButton *buttonMusicToolEdit[SamChannel::maxMusicTools];
	SamGuiSelectMusicTool* selectMusicTool[SamChannel::maxMusicTools];

	UiLabel *labelOutput;
	UiLabel *labelChannelOutput[SamChannel::maxOutputs];
	SamGuiSelectAudioOutputBus *selectAudioBus[SamChannel::maxOutputs];

	UiLabel *labelSends;
	//UiLabel *labelChannelSend[SamChannel::maxSends];
	UiStateButton *buttonSend[SamAudioBus::maxSends];
	SamGuiSelectAudioOutputBus *selectAudioSendBus[SamAudioBus::maxSends];
	UiFader *faderSends[SamAudioBus::maxSends];

	//static const int yAudioOffset = 84;
};

#endif // SAMGUICHANNELPROPERTIES_H
