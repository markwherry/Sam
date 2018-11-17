#ifndef SAMGUICHANNELSETTINGS_H
#define SAMGUICHANNELSETTINGS_H

#include <QWidget>
//#include "ui_SamGuiChannelSettings.h"

#include "AStateManager.h"

#include "UiStateButton.h"
#include "UiLabel.h"
#include "UiValue.h"
#include "SamGuiAudioToolInsert.h"

#include "SamChannel.h"

class SamGuiChannelSettings : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiChannelSettings(QWidget *parent = 0);
	~SamGuiChannelSettings();

	void onState(AStateEvent *ev);

public slots:
	void updateSelectedChannel();
	void setDownbeatOffset(int id, int flags);
	void setLayerMutesEnabled(int id, int flags);
	void updateDownbeatOffset(void);
	void updateLayerMutesEnabled(void);
	void setPitchBend(int id, int flags);
	void setUp(float value);
	void setDown(float value);
	void updatePitchBend(void);
	void updateUp(void);
	void updateDown(void);

signals:
	void selectedChannelUpdated(void);
	void downbeatOffsetUpdated(void);
	void layerMutesEnabledUpdated(void);
	void pitchBendUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiChannelSettingsClass ui;
	
	SamChannel::Ptr m_channel;

	UiStateButton *m_buttonDownbeatOffset;
	UiStateButton *m_buttonLayerMutesEnabled;
	UiStateButton *m_buttonPitchBend;
	UiLabel *m_labelUp;
	UiLabel *m_labelDown;
	UiValue *m_valueUp;
	UiValue *m_valueDown;

	//SamGuiAudioToolInsert *m_audioToolInsert;

	QPixmap *m_pixBackground;
};

#endif // SAMGUICHANNELSETTINGS_H
