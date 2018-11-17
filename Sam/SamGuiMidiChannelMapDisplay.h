#ifndef SAMGUIMIDICHANNELMAPDISPLAY_H
#define SAMGUIMIDICHANNELMAPDISPLAY_H

#include <QWidget>
//#include "ui_SamGuiMidiChannelMapDisplay.h"

class SamGuiMidiChannelMapDisplay : public QWidget
{
	Q_OBJECT

public:
	SamGuiMidiChannelMapDisplay(QWidget *parent = 0);
	~SamGuiMidiChannelMapDisplay();

	void updateMap(void);

signals:
	void highlighted(int port, int channel);

protected:
	void leaveEvent(QEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	

private:
	int numUses(int port, int channel);
	//Ui::SamGuiMidiChannelMapDisplay ui;
};

#endif // SAMGUIMIDICHANNELMAPDISPLAY_H
