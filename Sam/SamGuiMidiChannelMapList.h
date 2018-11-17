#ifndef SAMGUIMIDICHANNELMAPLIST_H
#define SAMGUIMIDICHANNELMAPLIST_H

#include <QWidget>
//#include "ui_SamGuiMidiChannelMapList.h"

class SamGuiMidiChannelMapList : public QWidget
{
	Q_OBJECT

public:
	SamGuiMidiChannelMapList(QWidget *parent = 0);
	~SamGuiMidiChannelMapList();

	void setHighlight(int port);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiMidiChannelMapList ui;
	bool m_highlights[128];
};

#endif // SAMGUIMIDICHANNELMAPLIST_H
