#ifndef SAMGUIAUDIOBUSPROPERTIES_H
#define SAMGUIAUDIOBUSPROPERTIES_H

#include <QWidget>
//#include "ui_SamGuiAudioBusProperties.h"

#include "SamGuiSend.h"
#include "SamGuiInsert.h"

class SamGuiAudioBusProperties : public QWidget
{
	Q_OBJECT

public:
	SamGuiAudioBusProperties(QWidget *parent = 0);
	~SamGuiAudioBusProperties();

public slots:
	void setAudioBus(SamChannel *channel, int audioBusIndex);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	void paintSection(QPainter &p, int x, int y, int w, int h, QString title);
	//Ui::SamGuiAudioBusPropertiesClass ui;

	SamGuiSend *m_sends[SamAudioBus::maxSends];
	SamGuiInsert *m_inserts[SamAudioBus::maxInserts];
};

#endif // SAMGUIAUDIOBUSPROPERTIES_H
