#ifndef SAMGUIMIDICHANNELMAPHEADER_H
#define SAMGUIMIDICHANNELMAPHEADER_H

#include <QWidget>
//#include "ui_SamGuiMidiChannelMapHeader.h"

class SamGuiMidiChannelMapHeader : public QWidget
{
	Q_OBJECT

public:
	SamGuiMidiChannelMapHeader(QWidget *parent = 0);
	~SamGuiMidiChannelMapHeader();

	void setHighlight(int channel);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiMidiChannelMapHeader ui;
	bool m_highlights[16];
};

#endif // SAMGUIMIDICHANNELMAPHEADER_H
