#include "SeGuiZoneVelocityRuler.h"

#include <QPainter>
#include <QMouseEvent>

#include "SamPreferences.h"

SeGuiZoneVelocityRuler::SeGuiZoneVelocityRuler(QWidget *parent)
	: QWidget(parent)
	, m_velocityHeight(4)
	, m_velocitySplitDisplay(0)
	, m_channel(0)
	, m_mouseVelocity(0)
{
	//ui.setupUi(this);
	setFixedSize(40, m_velocityHeight*127);

	m_velocitySplitDisplay = samPreferences->editor()->preference("velocitySplitDisplay", "0").toInt();

	m_popupMenu = new QMenu(0);
	QString vsd[9] = { "Quarters", "Zimmer" };
	for(int i=0; i<2; i++) {
		m_velocitySplitDisplayActions[i] = m_popupMenu->addAction(vsd[i]);
		m_velocitySplitDisplayActions[i]->setCheckable(true);
	}
	m_velocitySplitDisplayActions[m_velocitySplitDisplay]->setChecked(true);
	connect(m_popupMenu, SIGNAL(triggered(QAction*)), this, SLOT(setVelocitySplitDisplay(QAction*)));
}

SeGuiZoneVelocityRuler::~SeGuiZoneVelocityRuler()
{

}

void SeGuiZoneVelocityRuler::setChannel(SamChannel *channel)
{
	m_channel = channel;
}

void SeGuiZoneVelocityRuler::setVelocityHeight(float velocityHeight)
{
	m_velocityHeight = velocityHeight;
	if(m_velocityHeight<3) {
		m_velocityHeight = 3;
	}
	setFixedHeight(m_velocityHeight*127);
	update();

	//emit sizeChanged();
}

void SeGuiZoneVelocityRuler::setVelocitySplitDisplay(QAction *action)
{
	int item = -1;
	for(int i=0; i<2; i++) {
		if(action == m_velocitySplitDisplayActions[i]) {
			item = i;
			m_velocitySplitDisplayActions[i]->setChecked(true);
		} else {
			m_velocitySplitDisplayActions[i]->setChecked(false);
		}
	}
	
	if(item > -1) {
		m_velocitySplitDisplay = item;
		update();
		samPreferences->editor()->setPreference("velocitySplitDisplay", AString::number(m_velocitySplitDisplay));
		emit velocitySplitDisplayChanged(item);
	}
}

void SeGuiZoneVelocityRuler::setMouseVelocity(int velocity)
{
	//qDebug("SeGuiZoneVelocityRuler::setMouseVelocity(%d)", velocity);

	m_mouseVelocity = velocity;
}

void SeGuiZoneVelocityRuler::mouseMoveEvent(QMouseEvent *ev)
{
	
}

void SeGuiZoneVelocityRuler::mousePressEvent(QMouseEvent *ev)
{
	qDebug("SeGuiZoneVelocityRuler::mousePressEvent()");

	if(ev->button() == Qt::RightButton) {
		m_popupMenu->exec(ev->globalPos());
	}
}

void SeGuiZoneVelocityRuler::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect (0,0,width(), height(), Qt::white);


	p.setPen (QColor(168,199,208,255));
	QLinearGradient linearGrad(QPointF(0, 0), QPointF(0, height()-1));
    linearGrad.setColorAt(0, QColor(168,199,208,220));
    linearGrad.setColorAt(1, QColor(168,199,208,0));

	p.setBrush (linearGrad);
	p.drawRect (0,0,width()-1, height()-1);

	p.setFont (QFont("Calibri",12));
	if(m_velocitySplitDisplay == 0) {
		int velocities[3] = { 31, 63, 95 };
		for (int i=0; i<3; i++) {
			//p.setPen (QColor(0,150,200,80));
			int vh = height()-(velocities[i]*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
			p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,150,200,80));
			p.setPen (QColor(0,150,200));
			p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(velocities[i]));
		}
	} else {
		
		int velocities[6] = { 51, 71, 91, 101, 111, 121 };
		for (int i=0; i<6; i++) {
			//p.setPen (QColor(0,150,200,80));
			int vh = height()-(velocities[i]*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
			p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,150,200,80));
			p.setPen (QColor(0,150,200));
			p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(velocities[i]));
		}
	}
	
	if(m_channel) {
		int controller = m_channel->currentZoneCrossfade();
		int ch = height()-(controller*m_velocityHeight);
		p.fillRect(0, ch, width(), m_velocityHeight, QColor(150,0,150));
		p.fillRect(0, ch+m_velocityHeight, width(), 20, QColor(255,200,255));
		p.setPen (QColor(150,0,150));
		p.drawText (0,ch+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(controller));

		int vh = height()-(m_channel->velocity(m_channel->currentPitch())*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
		p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,150,0));
		p.fillRect(0, vh+m_velocityHeight, width(), 20, QColor(200,255,200));
		p.setPen (QColor(0,150,0));
		p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(m_channel->velocity(m_channel->currentPitch())));


		
	}

	/*if(m_mouseVelocity > 0) {
		int vh = height()-(m_mouseVelocity*m_velocityHeight);
			//p.drawLine (0,vh,width()-1,vh);
		p.fillRect(0, vh, width(), m_velocityHeight, QColor(0,0,0));
		p.fillRect(0, vh+m_velocityHeight, width(), 20, QColor(200,200,200));
		p.setPen (QColor(0,0,0));
		p.drawText (0,vh+m_velocityHeight, width(), 20, Qt::AlignCenter, QString::number(m_mouseVelocity));
	}//*/
	
	//p.drawRect (0,0,width()-1, height()-1);
}