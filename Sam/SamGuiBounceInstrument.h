#ifndef SAMGUIBOUNCEINSTRUMENT_H
#define SAMGUIBOUNCEINSTRUMENT_H

#include <QWidget>
//#include "ui_SamGuiBounceInstrument.h"

#include "SamGuiBounceGroup.h"
#include "SamGuiBounceRows.h"

#include "UiButton.h"

#include "SamChannel.h"

#include <QScrollArea>

class SamGuiBounceInstrument : public QWidget
{
	Q_OBJECT

public:
	SamGuiBounceInstrument(QWidget *parent = 0);
	~SamGuiBounceInstrument();

	void setChannel(SamChannel *channel);

public slots:
	void setActive(int id);
	void bounce(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiBounceInstrumentClass ui;
	SamChannel::Ptr m_channel;
	SamGuiBounceGroup *m_bounceGroups[8];
	SamGuiBounceRows *m_bounceRows;
	UiButton *m_buttonBounce;
	UiButton *m_buttonCancel;
	QScrollArea *m_scrollArea;
};

#endif // SAMGUIBOUNCEINSTRUMENT_H
