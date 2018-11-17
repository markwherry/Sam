#include "EQuickInfoButton.h"

#include <QPainter>
#include <QApplication>
#include <QToolTip>
#include <QMouseEvent>

EQuickInfoButton::EQuickInfoButton(QWidget *parent)
    : QWidget(parent)
{
	//ui.setupUi(this);

	setFixedSize(25,25);

	QString imgPath = qApp->applicationDirPath() + "/Resources/img/";
	pixInfo = new QPixmap(imgPath+"info.png");

	mouseOver = false;
}

EQuickInfoButton::~EQuickInfoButton()
{

}

void EQuickInfoButton::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	if(mouseOver){
		p.setPen(QColor(100,100,255,90));
		p.setBrush(QColor(100,100,255,90));
		p.drawRect(0,0,width()-1,height()-1);
	}

	p.drawPixmap(0,-1,*pixInfo);
}

void EQuickInfoButton::mousePressEvent(QMouseEvent *event)
{
	//timerId = startTimer(100);

	QToolTip::showText (mapToGlobal(QPoint(event->x(),event->y())),"Blah", this);
}

void EQuickInfoButton::enterEvent(QEvent *event)
{
	mouseOver = true;
	update();
}

void EQuickInfoButton::leaveEvent(QEvent *event)
{
	//if(!menu->isVisible()) {
		mouseOver = false;
		update();
	//}
}
/*
void EClickMenu::timerEvent(QTimerEvent *event)
{
	if(!menu->isVisible() && !underMouse()) {
		mouseOver = false;
		update();
		killTimer(timerId);
	}
}*/