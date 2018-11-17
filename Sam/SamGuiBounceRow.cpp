#include "SamGuiBounceRow.h"

#include <QPainter>
#include <QMouseEvent>

SamGuiBounceRow::SamGuiBounceRow(QWidget *parent, int id)
	: QWidget(parent)
	, m_id(id)
	, m_channel(0)
	, m_active(false)
{
	if(m_id < 0) {
		setFixedSize(0, 100);
	} else {
		setFixedSize(0, 30);
	}
	//ui.setupUi(this);

	for(int i=0; i<128; i++) {
		m_streamSelected[i] = false;
	}
}

SamGuiBounceRow::~SamGuiBounceRow()
{

}

int SamGuiBounceRow::numStreamsSelected(void)
{
	int numStreams = 0;

	for(int i=0; i<128; i++) {
		if(m_streamSelected[i]) {
			numStreams++;
		}
	}

	return numStreams;
}

void SamGuiBounceRow::setActive(bool active)
{
	m_active = active;

	if(m_active) {
		raise();
	} else {
		lower();
	}

	update();
}

void SamGuiBounceRow::setChannel(SamChannel *channel)
{
	m_channel = channel;

	if(m_channel) {
		setFixedWidth(30 * channel->streamMix()->numStreams());
	} else {
		setFixedWidth(0);
	}
}

void SamGuiBounceRow::mousePressEvent(QMouseEvent *ev)
{
	//qDebug("Click");
	if(m_channel && m_active) {
		//qDebug("m_channel");
		for(int i=0; i<m_channel->streamMix()->numStreams(); i++) {
			if(ev->x() >= (i*30) && ev->x() < (i*30)+30) {
				//qDebug("Click stream %d", i);
				m_streamSelected[i] = !m_streamSelected[i];
				update();
				break;
			}
		}
	}
}

void SamGuiBounceRow::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(m_active || m_id < 0) {
		p.setPen(QColor(255,255,255));
	} else {
		p.setPen(QColor(100,100,100));
	}
	if(m_id==-1) {
		p.setBrush(QColor(90,90,90));
	} else {
		if(m_id%2==0) {
			p.setBrush(QColor(60,60,60));
		} else {
			p.setBrush(QColor(80,80,80));
		}
	}
	p.drawRect(0,0,width()-1,height()-1);

	if(!m_channel) {
		return;
	}

	SamInstrument *instrument = m_channel->instrument();

	for(int i=0; i<m_channel->streamMix()->numStreams()+1; i++) {
		p.drawLine(i*30, 0, i*30, height()-1);
		
		if(m_streamSelected[i]) {
			if(m_active) {
				p.fillRect((i*30)+5, 5, 20, 20, QColor(255,255,255));
			} else {
				p.fillRect((i*30)+5, 5, 20, 20, QColor(100,100,100));
			}
		}

		if(m_id < 0 && i < m_channel->streamMix()->numStreams()) {
			
			int set = m_channel->streamMix()->stream(i)->setId();
			if(set >= 0 && set < instrument->elementPool()->numSets()) {
				//p.setFont(QFont("", 8));//, QFont::Bold));
				//p.setPen(QColor(255,255,255,200));
				//p.drawText(2, height()-21, width()-3, 20, Qt::AlignCenter|Qt::AlignVCenter, instrument->elementPool()->set(set)->name().toChar());

				QFont f("",0,QFont::Bold);
				f.setPixelSize(14); //9
				f.setStyleStrategy(QFont::PreferAntialias);
				p.setFont(f);
				//p.setPen(QColor(255,255,255,200));
				p.rotate(-90);
				p.drawText(-90, (i*30), 80, 30, Qt::AlignLeft|Qt::AlignVCenter, QString::number(i+1)+QString("  ")+QString(instrument->elementPool()->set(set)->name().toChar()));
				p.rotate(90);
			}
			
		}
	}
}