#include "SamGuiCellAnalyser.h"

#include <QPainter>

#include "CsMidiName.h"

SamGuiCellAnalyser::SamGuiCellAnalyser(QWidget *parent)
	: QWidget(parent)
	, m_channel(0)
	, m_timerId(0)
	, m_currentPitch(-1)
	, m_totalCells(0)
{
	//ui.setupUi(this);

	setWindowTitle("Cell Analyser");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	int x = 10;
	for(int i=0; i<16; i++) {
		m_velocities[i] = new SamGuiCellAnalyserVelocity(this);
		m_velocities[i]->move(x += (10+m_velocities[i]->width()), 10); 
		m_velocities[i]->setEnabled(false);
	}

	setFixedSize(x, m_velocities[0]->height()+20);
}

SamGuiCellAnalyser::~SamGuiCellAnalyser()
{

}

void SamGuiCellAnalyser::setChannel(SamChannel *channel)
{
	m_channel = channel;
	m_currentPitch = -1;

	//update();
}

void SamGuiCellAnalyser::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//
	//p.setPen(QColor(0,0,0,140));
	//p.setBrush(QColor(0,0,0,140));
	//p.drawRect(0,0,width()-1,height()-1);

	if(m_currentPitch > -1) {
		p.setFont (QFont("Calibri",18));	
		p.setPen(QColor(200,200,200));
		p.drawText(10, 10+16, m_velocities[0]->width()-1,  m_velocities[0]->height()/2-16, Qt::AlignCenter, CsMidiName::pitchToName(m_currentPitch).toChar());

		p.setFont (QFont("Calibri",18, QFont::Bold));	
		p.setPen(QColor(255,255,255));
		p.drawText(10, 10+(m_velocities[0]->height()/2+16), m_velocities[0]->width()-1,  m_velocities[0]->height()/2-16, Qt::AlignCenter, QString::number(m_totalCells));
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCellAnalyser::showEvent(QShowEvent *ev)
{
	//qDebug("SamGuiCellAnalyser::showEvent()");

	m_timerId = startTimer(50);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCellAnalyser::hideEvent(QHideEvent *ev)
{
	//qDebug("SamGuiCellAnalyser::hideEvent()");

	killTimer(m_timerId);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiCellAnalyser::timerEvent(QTimerEvent *ev)
{
	//qDebug("SamGuiCellAnalyser::timerEvent()");

	if(!m_channel) {
		return;
	}

	int p = m_channel->currentPitch();
	if(p == m_currentPitch) {
		return;
	}

	m_currentPitch = p;
	//qDebug("m_currentPitch = %d", m_currentPitch);

	QList<SamZone*> zones;
	m_totalCells = 0;
	int maxCells = 0;

	SamLayer *layer = m_channel->layer();
	for(int i=0; i<layer->numZones(); i++) {
		if(layer->zone(i)->midiNoteLow() <= m_currentPitch && layer->zone(i)->midiNoteHigh() >= m_currentPitch) {
			SamZone *zone = layer->zone(i);
			m_totalCells += zone->numCells();
			if(zone->numCells() > maxCells) {
				maxCells = zone->numCells();
			}

			zones.append(zone);
			int origPos = zones.size()-1;
			for(int j=zones.size()-1; j>0; j--) {
				if(zones[j-1]->midiVelocityLow() <= zone->midiVelocityLow() && zones[j]->midiVelocityLow() > zone->midiVelocityLow()) {
					zones.move(origPos, j-1);
				}
			}

			
		}
	}

	int n = zones.size();
	if(n >= 16) n = 15;
	//qDebug("n = %d", n);
	for(int i=0; i<16; i++) {
		if(i < zones.size()) {
			m_velocities[i]->setVelocityRange(zones[i]->midiVelocityLow(), zones[i]->midiVelocityHigh());
			m_velocities[i]->setNumCells(zones[i]->numCells(), maxCells);
			m_velocities[i]->setEnabled(true);
		} else {
			m_velocities[i]->setEnabled(false);
		}
	}


	update();
}