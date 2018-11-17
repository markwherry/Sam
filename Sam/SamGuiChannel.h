#ifndef SAMGUICHANNEL_H
#define SAMGUICHANNEL_H

#include "UiListItem.h"
//#include "ui_SamGuiChannel.h"

#include "AStateManager.h"

#include "SamChannel.h"

#include "UiLabel.h"
//#include "Ui/UiSelectMidiPort.h"
#include "UiSelectMidiChannel.h"
#include "UiStateButton.h"
#include "UiValue.h"
#include "UiIndicator.h"

#include "SamGuiSelectMidiInputBus.h"

#include "SamGuiParameterControl.h"

#include "SamGuiIndicatorBlock.h"

#include "SamGuiLayerStrip.h"

class SamGuiChannel : public UiListItem, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannel(QWidget *parent, SamChannel *channel);
	~SamGuiChannel();

	void onState(AStateEvent *ev);

	SamChannel* channel(void) { return m_channel; }

	// UiListItem
	void setPositionInList(int position) { m_channel->setPositionInList(position); }
	int positionInList(void) { return m_channel->positionInList(); }

	void setSelected(bool selected) { m_channel->setSelected(selected); }
	bool isSelected(void) { return m_channel->isSelected(); }

	void updateNextFrame(void);
	void refreshLabel(void);

public slots:
	//void setMidiPort(int itemIndex);
	void setMidiChannel(int itemIndex);
	//void updateMidiPort(void);
	void updateMidiChannel(void);
	void updateMidiMuted(void);
	void setMidiMuted(int id, int flags);
	void updateDownbeatOffset(void);
	void setDownbeatOffset(int id, int flags);
	void toggleArticulationLock(int);
	void toggleArticulationMode(int);
	void setArticulation(int index);
	void setNumVoices(float value);
	void setMute(int id, int flags);
	void setSolo(int id, int flags);

	void updateArticulationLock(void);
	void updateArticulationMode(void);
	void updateArticulation(void);
	void updateArticulationList(void);
	void updateArticulationColor(void);
	void updateMute(void);
	void updateSolo(void);
	//void edit(int id, int flags);
	void midiEvent(int ev, int port);
	void updateLabel(QString text);

signals:
	void editInstrumentFromChannel(SamChannel *channel);
	//void midiPortUpdated(void);
	void midiChannelUpdated(void);
	//void updated(void);

	void midiMutedUpdated(void);
	void downbeatOffsetUpdated(void);

	void articulationLockUpdated(void);
	void articulationModeUpdated(void);
	void articulationUpdated(void);
	void articulationColorUpdated(void);
	void muteUpdated(void);
	void soloUpdated(void);
	void layerUpdated(void);
	void midiReceived(int ev, int port);

protected:
	void mousePressEvent(QMouseEvent *ev);
	//void mouseDoubleClickEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void timerEvent(QTimerEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiChannelClass ui;
	SamChannel::Ptr m_channel;
	UiIndicator *m_indicatorNote;
	UiLabel *m_label;
	UiLabel *labelMidiPort;
	UiLabel *labelMidiChannel;
	UiLabel *labelVoices;
	UiLabel *labelMaxVoices;
	UiValue *valueVoices;
	SamGuiSelectMidiInputBus *selectMidiInput;
	UiSelectMidiChannel *selectMidiChannel;

	//UiStateButton *buttonEdit;

	UiStateButton *buttonMidiMuted;
	UiStateButton *m_buttonDownbeatOffset;
	UiStateButton *m_buttonMute;
	UiStateButton *m_buttonSolo;

	UiStateButton *buttonLock;
	UiStateButton *buttonArt;
	UiSelectMenu *selectArticulation;

	QList<SamGuiParameterControl*> m_parameterControls;
	bool m_mouseOver;

	SamGuiIndicatorBlock *m_indicators[13];

	SamGuiLayerStrip *m_layerStrip;

	QLineEdit *m_lineEdit;
};

#endif // SAMGUICHANNEL_H
