#include "SamGuiOutputHeader.h"

#include <QPainter>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputHeader::SamGuiOutputHeader(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	int numOutputs = samEngine->numOutputs();
	setFixedSize(numOutputs * 25, 148);

	for(int i=0; i<256; i++) {
		m_highlights[i] = false;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputHeader::~SamGuiOutputHeader()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputHeader::setHighlight(int output)
{
	for(int i=0; i<128; i++) {
		m_highlights[i] = false;
	}

	if(output >= 0 && output<128) {
		m_highlights[output] = true;
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputHeader::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(90,90,90));

	//p.drawLine(0,height()-25,width()-1,height()-25);
	
	p.fillRect(0,height()-25,width()-1,25, QColor(60,60,60));

	

	int numOutputs = samEngine->numOutputs();
	setFixedSize(numOutputs * 25, 148);

	p.setRenderHint(QPainter::TextAntialiasing);

	for(int i=0; i<numOutputs; i++) {
		p.setPen(QColor(150,150,150));
		p.drawLine(((i+1)*25)-1,0,((i+1)*25)-1,height()-1);
		
		p.setPen(QColor(255,255,255));
		p.setFont(QFont("Calibri",10,QFont::Bold));
		p.drawText(i*25,height()-25,25,25,Qt::AlignCenter|Qt::AlignVCenter,QString::number(i+1));

		int angle = -90;
		int offset = 118;
		p.rotate(angle);
		p.setFont(QFont("Calibri", 10));
		p.drawText(0-offset,i*25,100,25,Qt::AlignLeft|Qt::AlignVCenter, samEngine->audioManager()->channelName(i, false).toChar());
		p.rotate(-angle);

		if(m_highlights[i]) {
			p.fillRect((i*25), 0, 24, height(), (QColor(255,255,255,100)));
		}
	}
}