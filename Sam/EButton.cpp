#include "EButton.h"

#include <QPainter>

EButton::EButton(QWidget *parent, QString title)
    : QWidget(parent)
{
	//ui.setupUi(this);

	m_title = title;

	mouseOver = false;
}

EButton::~EButton()
{

}

void EButton::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.setPen(QColor(30,30,30));
	p.setBrush(QColor(90,90,90));
	p.drawRect(0,0,width()-1,height()-1);

	if(mouseOver) {
		p.setPen(QColor(100,100,255,90));
		p.setBrush(QColor(100,100,255,90));
		p.drawRect(0,0,width()-1,height()-1);
	}

	p.setPen(QColor(230,230,230));
	p.setFont(QFont("",8,QFont::Bold));
	
	p.drawText(4, 0, width()-8, height()-1, Qt::AlignCenter|Qt::AlignVCenter, m_title);
}

void EButton::mousePressEvent(QMouseEvent *event)
{
	emit clicked();
}

void EButton::enterEvent(QEvent *event)
{
	mouseOver = true;
	update();
}

void EButton::leaveEvent(QEvent *event)
{
	mouseOver = false;
	update();
}