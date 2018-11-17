#include "EClickMenu.h"

#include <QPainter>
#include <QApplication>

EClickMenu::EClickMenu(QWidget *parent, QString title)
    : QWidget(parent)
{

	//ui.setupUi(this);
	m_title = title;

	menu = new QMenu();

	QString imgPath = qApp->applicationDirPath() + "/Resources/img/";
	pixMenuArrowDown = new QPixmap(imgPath+"menu_arrow_down.png");

	mouseOver = false;

	m_active = false;
}

EClickMenu::~EClickMenu()
{

}

void EClickMenu::addAction(QAction *action)
{
	menu->addAction(action);
}

void EClickMenu::addSeparator(void)
{
	menu->addSeparator();
}

void EClickMenu::setActive(bool active)
{
	m_active = active;
	update();
}

void EClickMenu::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	if(mouseOver){
		p.setPen(QColor(100,100,255,90));
		p.setBrush(QColor(100,100,255,90));
		p.drawRect(0,0,width()-1,height()-1);
		//p.setPen(QColor(230,230,230,100));	
		//p.drawLine(0,0,width()-1,0);
		//p.drawLine(width()-1,0,width()-1,height()-1);
		//p.drawLine(width()-1,height()-1,0,height()-1);
		//p.drawLine(0,height()-1,0,0);
	} else if (m_active) {
		p.setPen(QColor(100,255,100,50));
		p.setBrush(QColor(100,255,100,50));
		p.drawRect(0,0,width()-1,height()-1);
	}

	p.setPen(QColor(230,230,230));
	p.setFont(QFont("",8,QFont::Bold));
	
	p.drawText(4, 0, width()-4, height()-1, Qt::AlignLeft|Qt::AlignVCenter, m_title);

	p.drawPixmap((width()-17)-4,0,*pixMenuArrowDown);

	
}

void EClickMenu::mousePressEvent(QMouseEvent *event)
{
	timerId = startTimer(100);
	menu->popup(mapToGlobal(QPoint(0,height())));
}

void EClickMenu::enterEvent(QEvent *event)
{
	mouseOver = true;
	update();
}

void EClickMenu::leaveEvent(QEvent *event)
{
	if(!menu->isVisible()) {
		mouseOver = false;
		update();
	}
}

void EClickMenu::timerEvent(QTimerEvent *event)
{
	if(!menu->isVisible() && !underMouse()) {
		emit menuClosed();
		mouseOver = false;
		update();
		killTimer(timerId);
	}
}