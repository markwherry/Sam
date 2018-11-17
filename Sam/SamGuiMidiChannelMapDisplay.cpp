#include "SamGuiMidiChannelMapDisplay.h"

#include "SamEngine.h"
#include "SamProject.h"

#include <QPainter>
#include <QMouseEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapDisplay::SamGuiMidiChannelMapDisplay(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedSize(16 * 25, samEngine->numMidiInputBuses() * 25);

	//updateMap();

	setMouseTracking(true);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiMidiChannelMapDisplay::~SamGuiMidiChannelMapDisplay()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapDisplay::updateMap(void)
{
	
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapDisplay::leaveEvent(QEvent *ev)
{
	emit highlighted(-1,-1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapDisplay::mouseMoveEvent(QMouseEvent *ev)
{
	int buses = samEngine->numMidiInputBuses();

	for(int i=0; i<buses; i++) {
		if(ev->y() >= i*25 && ev->y()<(i+1)*25) {
			for(int j=0; j<16; j++) {
				if(ev->x() >= j*25 && ev->x() < (j+1)*25) {
					emit highlighted(i, j);
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapDisplay::mouseDoubleClickEvent(QMouseEvent *ev)
{
	int port = -1;
	int channel = -1;

	int buses = samEngine->numMidiInputBuses();

	for(int i=0; i<buses; i++) {
		if(ev->y() >= i*25 && ev->y()<(i+1)*25) {
			for(int j=0; j<16; j++) {
				if(ev->x() >= j*25 && ev->x() < (j+1)*25) {
					port = i;
					channel = j;
				}
			}
		}
	}

	//qDebug("port = %d, channel = %d", port, channel);

	if(port > -1 && channel > -1) {
		int bus = samEngine->midiInputBus(port)->id();	

		for(int i=0; i<samEngine->project()->numChannels(); i++) {
			SamChannel *ch = samEngine->project()->channel(i);
			if(ch->isSelected()) {
				ch->setMidiInputPort(bus, this);
				ch->setMidiInputChannel(channel+1, this);
			}
		}
	}

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiMidiChannelMapDisplay::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int numInputs = samEngine->numMidiInputBuses();

	p.setFont(QFont("Calibri",8,QFont::Bold));

	QColor cols[2] = { QColor(90,90,90), QColor(75,75,75) };

	int y = 0;
	for(int i=0; i<numInputs; i++) {
		p.fillRect(0, y, width()-1, y+25, cols[i%2]);

		//p.setPen(QColor(255,255,255));
		//p.drawText(5, y, width()-10, 25, Qt::AlignLeft|Qt::AlignVCenter, samEngine->midiInputBus(i)->name().toChar());
		
		y += 25;

		p.setPen(QColor(150,150,150));
		p.drawLine(0, y-1, width(), y-1);

		for(int j=0; j<16; j++) {
			p.drawLine(((j+1)*25)-1, y-25, ((j+1)*25)-1, y-1);
			//int uses = numUses(i, j);
			int uses = samEngine->project()->numChannelMidiAssignemnts(i, j); 
			if(uses == 1) {
				p.fillRect((j*25)+4, y-25+4, 16, 16, QColor(0,255,0,200));
			} else if(uses > 1) {
				p.fillRect((j*25)+4, y-25+4, 16, 16, QColor(255,255,0,200));
				p.setPen(QColor(0,0,0));
				p.drawText((j*25)+4, y-25+4+1, 16, 16, Qt::AlignCenter, QString::number(uses));
				p.setPen(QColor(150,150,150));
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiMidiChannelMapDisplay::numUses(int port, int channel)
{
	channel++;
	int uses = 0;

	int bus = samEngine->midiInputBus(port)->id();
	
	int numChannels = samEngine->project()->numChannels();
	for(int i=0; i<numChannels; i++) {
		SamChannel *ch = samEngine->project()->channel(i);
		if(ch->midiInputPort() == bus && ch->midiInputChannel() == channel) {
			uses++;
		}
	}
	
	return uses;
}