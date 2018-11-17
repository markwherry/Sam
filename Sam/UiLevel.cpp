#include "UiLevel.h"

#include <QPainter>

#include "ALog.h"

#include "UiObject.h"

UiLevel::UiLevel(QWidget *parent, bool vertical)
	: QWidget(parent)
	, m_vertical(vertical)
	, m_min(0)
	, m_max(100)
	, m_value(0)
	, m_color(QColor(60,180,60))
{
	//ui.setupUi(this);
}

UiLevel::~UiLevel()
{

}

void UiLevel::setValue(int value)
{
	//aLog("setValue(%d)");
	m_value = value;
	if(m_value < m_min) {
		m_value = m_min;
	} else if(m_value > m_max) {
		m_value = m_max;
	}
	//aLog("m_value = %d", m_value);

	update(); //*/
}

void UiLevel::setColor(QColor color)
{
	m_color = color;

	update();
}

void UiLevel::mousePressEvent(QMouseEvent *ev)
{
	emit clicked();
}

void UiLevel::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.setPen(QColor(0,0,0)); // was 30
	//p.setBrush(QColor(0,0,0));
	//p.drawRect(0,0,width()-1,height()-1);
	p.fillRect(0,0,width()-1,height()-1, QColor(0,0,0));

	//p.setPen(QColor(60,60,60));
	//for(int i=1; i<width(); i+=2) {
	//	p.drawLine(i,1,i,height()-2);
	//}

	float depth = (float)width()-2;
	if(m_vertical) {
		depth = (float)height();
	}
	float unit = (float)(m_max-m_min)/depth;

	float v = unit * (float)m_value;

	//p.setPen(m_color);
	p.fillRect(1, 1, v-2, height()-2, m_color);
	//for(int i=1; i<(int)v; i+=2) {
	//	p.drawLine(i,1,i,height()-2);
	//}

//	UiObject::drawTint(p, this, 0, width());

	/*

	p.fillRect(1,1,width()-2,height()-2, QColor(60,180,60));
	for(int i=1; i<width(); i+=2) {
		p.drawLine(i,1,i,height()-2);
	}

	float depth = (float)width()-2;
	if(m_vertical) {
		depth = (float)height();
	}
	float unit = (float)(m_max-m_min)/depth;

	float v = unit * (float)m_value;

	int w = width()-1-(int)v;
	p.fillRect((int)v, 1, w, height()-2, QColor(0,0,0));*/
}