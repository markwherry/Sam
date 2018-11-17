#ifndef SAMGUISTREAMSTRIP_H
#define SAMGUISTREAMSTRIP_H

#include <QWidget>
//#include "ui_SamGuiStreamStrip.h"

#include "UiFader.h"
#include "UiPanner.h"
#include "UiSelectMenu.h"
#include "UiLabel.h"
#include "UiValue.h"
#include "UiStateButton.h"

#include "SamChannel.h"

class SamGuiStreamMixStrip : public QWidget
{
	Q_OBJECT

public:
	SamGuiStreamMixStrip(QWidget *parent, int id);
	~SamGuiStreamMixStrip();

	SamStream* stream(void) { return m_stream; }
	void setStream(SamChannel *channel, SamStream *stream);

	void updateFader(void);
	void updatePan(void);
	void updateMute(void);
	void updateSolo(void);
	void updateActive(void);

public slots:
	void faderValueChange(float value, int flags);
	void pannerValueChange(float x, float y);
	void setController(float value);
	void setGroup(int index);
	void setMute(int id, int flags);
	void setSolo(int id, int flags);
	void setSetName(QString name);
	void setActive(int id, int flags);

protected:
	void paintEvent(QPaintEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);

private:
	//Ui::SamGuiStreamStripClass ui;
	int m_id;
	SamChannel::Ptr m_channel;
	SamStream *m_stream;
	UiFader *m_fader;
	UiPanner *m_panner;
	UiSelectMenu *m_selectGroup;
	UiLabel *m_labelMidiPort;
	UiValue *m_valueController;
	UiStateButton *m_buttonMute;
	UiStateButton *m_buttonSolo;
	UiStateButton *m_buttonActive;
	
};

#endif // SAMGUISTREAMSTRIP_H
