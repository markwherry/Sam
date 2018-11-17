#include "SamGuiMidiChannelMapList.h"

#include <QPainter>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapList::SamGuiMidiChannelMapList(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedSize(150, samEngine->numMidiInputBuses() * 25);

	for(int i=0; i<128; i++) {
		m_highlights[i] = false;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapList::~SamGuiMidiChannelMapList()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapList::setHighlight(int port)
{
	for(int i=0; i<128; i++) {
		m_highlights[i] = false;
	}

	if(port >= 0 && port < 128) {
		m_highlights[port] = true;
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapList::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int numInputs = samEngine->numMidiInputBuses();

	p.setFont(QFont("Calibri",8,QFont::Bold));

	QColor cols[2] = { QColor(90,90,90), QColor(75,75,75) };

	int y = 0;
	for(int i=0; i<numInputs; i++) {
		p.fillRect(0, y, width()-1, y+25, cols[i%2]);

		p.setPen(QColor(255,255,255));
		p.drawText(5, y, width()-10, 25, Qt::AlignLeft|Qt::AlignVCenter, samEngine->midiInputBus(i)->name().toChar());
		
		y += 25;

		p.setPen(QColor(150,150,150));
		p.drawLine(0, y-1, width(), y-1);

		if(m_highlights[i]) {
			p.fillRect(0, y-25, width()-2, 24, (QColor(255,255,255,100)));
		}
	}
}