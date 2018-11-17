#ifndef SAMGUIMIXERSTRIP_H
#define SAMGUIMIXERSTRIP_H

#include <QWidget>
//#include "ui_SamGuiMixerStrip.h"

#include "SamGuiMixerInsert.h"
#include "UiFader.h"

#include "SamChannel.h"

class SamGuiMixerStrip : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiMixerStrip(QWidget *parent, SamChannel *channel, int audioBusIndex);
	~SamGuiMixerStrip();

	void onState(AStateEvent *ev);

	SamChannel* channel(void) { return m_channel; }
	int         audioBusIndex(void) { return m_audioBusIndex; }

	void setSelected(bool selected);

signals:
	void needToUpdate(void);
	void selected(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiMixerStripClass ui;
	SamChannel::Ptr m_channel;
	int             m_audioBusIndex;

	//bool m_selected;

	SamGuiMixerInsert *m_inserts[4];
	//UiFader *m_fader;
};

#endif // SAMGUIMIXERSTRIP_H
