#include "SamGuiAudioBusProperties.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiAudioBusProperties::SamGuiAudioBusProperties(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedSize(200, 200);

	int y = 45;

	for(int i=0; i<SamAudioBus::maxSends; i++) {
		m_sends[i] = new SamGuiSend(this, i);
		m_sends[i]->move(154+2, y);
		y += (m_sends[i]->height() + 2);
	}

	y = 45;

	for(int i=0; i<SamAudioBus::maxInserts; i++) {
		m_inserts[i] = new SamGuiInsert(this);
		m_inserts[i]->move(2, y);
		y += (m_sends[i]->height() + 2);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiAudioBusProperties::~SamGuiAudioBusProperties()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioBusProperties::setAudioBus(SamChannel *channel, int audioBusIndex)
{
	for(int i=0; i<SamAudioBus::maxSends; i++) {
		m_sends[i]->setChannel(channel, i);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioBusProperties::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.setPen(QColor(255,255,255));
	//p.setBrush(QColor(0,0,0,0));
	//p.drawRect(0, 0, width()-1, height()-1);

	p.fillRect(0, 0, width()-1, 20, QColor(100,0,0,100));
	p.setPen(QColor(240,240,240));
	p.setFont(QFont("Calibri", 10, QFont::Bold));
	p.drawText(4, 0, width()-8, 20, Qt::AlignLeft|Qt::AlignVCenter, "1 Simple Piano - Output 1");

	paintSection(p, 0, 24, 150, 300, "Inserts");
	paintSection(p, 154, 24, 150, 300, "Sends");

	for(int i=0; i<SamAudioBus::maxInserts; i++) {
		p.setPen(QColor(0,0,0,50));
		p.drawLine(0, m_inserts[i]->y() + m_inserts[i]->height()-1, 148, m_inserts[i]->y() + m_inserts[i]->height()-1);
		p.setPen(QColor(255,255,255,25));
		p.drawLine(0, m_inserts[i]->y() + m_inserts[i]->height(), 148, m_inserts[i]->y() + m_inserts[i]->height());
	}

	for(int i=0; i<SamAudioBus::maxSends; i++) {
		p.setPen(QColor(0,0,0,50));
		p.drawLine(154, m_sends[i]->y() + m_sends[i]->height()-1, 154+148, m_sends[i]->y() + m_sends[i]->height()-1);
		p.setPen(QColor(255,255,255,25));
		p.drawLine(154, m_sends[i]->y() + m_sends[i]->height(), 154+148, m_sends[i]->y() + m_sends[i]->height());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiAudioBusProperties::paintSection(QPainter &p, int x, int y, int w, int h, QString title)
{
	p.fillRect(x, y, w-1, h-1, QColor(0,0,0,30));
	p.fillRect(x, y, w-1, 20, QColor(0,0,0,60));

	p.setPen(QColor(240,240,240));
	p.setFont(QFont("Calibri", 10, QFont::Bold));
	p.drawText(x+4, y, w-8, 20, Qt::AlignLeft|Qt::AlignVCenter, title);

	
	
}