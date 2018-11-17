#ifndef SAMGUILAYERSTRIP_H
#define SAMGUILAYERSTRIP_H

#include <QWidget>
//#include "ui_SamGuiLayerStrip.h"

#include "SamChannel.h"

class SamGuiLayerStrip : public QWidget
{
	Q_OBJECT

public:
	SamGuiLayerStrip(QWidget *parent, SamChannel *channel);
	~SamGuiLayerStrip();

protected:
	void paintEvent(QPaintEvent *ev);

private:
	SamChannel *m_channel;
	//Ui::SamGuiLayerStripClass ui;
};

#endif // SAMGUILAYERSTRIP_H
