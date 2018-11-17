#include "SeGuiZoneDisplayItem.h"

#include <QPainter>
#include <QEvent>
#include <QMouseEvent>

#include "SeGuiZoneDisplay.h"

SeGuiZoneDisplayItem::SeGuiZoneDisplayItem(QWidget *parent, SamZone *zone)
	: QWidget(parent)
	, m_zone(zone)
	, m_showNumCells(true)
	, m_hoverMode(0)
{
	//ui.setupUi(this);
	setMouseTracking(true);
}

SeGuiZoneDisplayItem::~SeGuiZoneDisplayItem()
{

}

void SeGuiZoneDisplayItem::enterEvent(QEvent *ev)
{
	//qDebug("SeGuiZoneDisplayItem::enterEvent()");

	//setCursor(QCursor(Qt::SizeHorCursor));
}

void SeGuiZoneDisplayItem::leaveEvent(QEvent *ev)
{
	//qDebug("SeGuiZoneDisplayItem::leaveEvent()");
	//setCursor(QCursor(Qt::ArrowCursor));
	unsetCursor();
	m_hoverMode = 0;
}

void SeGuiZoneDisplayItem::mouseMoveEvent(QMouseEvent *ev)
{
	//qDebug("SeGuiZoneDisplayItem::mouseMoveEvent()");

	if(ev->buttons() & Qt::LeftButton) { 
		ev->setAccepted(false);
	} else { 
		if(ev->x() > 0 && ev->x() < 3) { 
			setCursor(QCursor(Qt::SizeHorCursor));
			m_hoverMode = 2;
		} else if(ev->x() > width()-3 && ev->x() < width()) {
			setCursor(QCursor(Qt::SizeHorCursor));
			m_hoverMode = 3;
		} else {
			unsetCursor();
			m_hoverMode = 1;
		}
	}
}

void SeGuiZoneDisplayItem::paint(QPainter &p)
{
	p.setClipRect(0,0,width(),height());

	if (m_zone->isSelected()) {
		p.setPen (QColor(100,100,255,100)); // 225,205,54
		p.setBrush (QColor(100,100,255,120)); //225,205,54,120
		p.drawRect (0,0,width()-1, height()-1);
	} else {
		p.setPen (QColor(0,0,0,50));
		if(m_zone->isCreatedByCopy()) {
			p.setBrush(QColor(100,0,0,60));
		} else {
			p.setBrush (QColor(0,0,0,60));
		}

		p.drawRect (0,0,width()-1, height()-1);

		p.setPen(QColor(60,60,60,100));
		p.drawLine (0,0,width()-1,0);
		p.drawLine (0,height()-1,0,0);
		p.setPen (QColor(120,120,120,100));
		p.drawLine (width()-1,0,width()-1, height()-1);
		p.drawLine (width()-1,height()-1,0,height()-1);
	
	}

	SeGuiZoneDisplay *zd = (SeGuiZoneDisplay*)parent();

	if(zd->zdiInfo() > 0) {
		p.setPen (QColor(0,0,0));
		p.setFont (QFont("Calibri",8));

		switch(zd->zdiInfo()) {
			case 1: // Num Cells
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->numCells()));
				break;
			case 2: // Velocity
				p.drawText (0,0,width()-1,height()/2,Qt::AlignHCenter|Qt::AlignTop,QString::number(m_zone->midiVelocityHigh()));
				p.drawText (0,height()/2,width()-1,height()/2,Qt::AlignHCenter|Qt::AlignBottom,QString::number(m_zone->midiVelocityLow()));
				break;
			case 3: // Sample Start
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->sampleStart()));
				break;
			case 4: // Gain
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->gain()));
				break;
			case 5: // Release Gain
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->releaseGain()));
				break;
			case 6: // Pre-loop Release Gain
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->preLoopReleaseGain()));
				break;
			case 7: // Pitch
				//p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->numCells()));
				break;
			case 8: // Fine Tune
				p.drawText (0,0,width()-1,height()-1,Qt::AlignCenter,QString::number(m_zone->fineTune()));
				break;
			
		}
	}
}