#include "SamGuiMidiChannelMapHeader.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapHeader::SamGuiMidiChannelMapHeader(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedSize(16 * 25, 35);

	for(int i=0; i<16; i++) {
		m_highlights[i] = false;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapHeader::~SamGuiMidiChannelMapHeader()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapHeader::setHighlight(int channel)
{
	for(int i=0; i<16; i++) {
		m_highlights[i] = false;
	}

	if(channel >= 0 && channel<16) {
		m_highlights[channel] = true;
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapHeader::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	QColor midiChannelColor[17] = { QColor(160,160,160),
									QColor(143,26,26),   QColor(143,26,84), QColor(143,26,143), QColor(84,26,143),
		                            QColor(26,26,143), QColor(26,84,143), QColor(26,143,143), QColor(26,143,84),
									QColor(89,153,0),QColor(74,143,26), QColor(143,143,26), QColor(143,113,26),
									QColor(143,84,26),QColor(143,55,26), QColor(215,24,0), QColor(215,98,0) };

	p.fillRect(0, 0, width()-1, height()-1, QColor(90,90,90));
	
	p.setFont(QFont("Calibri",8,QFont::Bold));

	for(int i=0; i<16; i++) {
		p.setPen(QColor(150,150,150));
		p.drawLine(((i+1)*25)-1,0,((i+1)*25)-1,height()-1);
		
		p.setPen(QColor(255,255,255));
		p.drawText(i*25,0,25,25,Qt::AlignCenter|Qt::AlignVCenter,QString::number(i+1));

		p.fillRect((i*25), height()-10, 24, 10, midiChannelColor[i+1]);

		if(m_highlights[i]) {
			p.fillRect((i*25), 0, 24, 25, (QColor(255,255,255,100)));
		}
	}
}