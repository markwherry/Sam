#include "UiPanner.h"

#include <QPainter>
#include <QMouseEvent>

UiPanner::UiPanner(QWidget *parent, int size)
	: QWidget(parent)
	, m_x(512)
	, m_y(0)
	, m_xPos(0)
	, m_yPos(0)
	, m_border(4)
{
	//ui.setupUi(this);

	setFixedSize(size, size);

	updatePositionFromValues();
}

UiPanner::~UiPanner()
{

}

void UiPanner::setXNormalised(float x)
{
	x = x * 1023;
	m_x = (int)x;
	updatePositionFromValues();
	update();
}

void UiPanner::setYNormalised(float y)
{
	y = y * 1023;
	m_y = (int)y;
	updatePositionFromValues();
	update();
}

float UiPanner::x(bool normalised)
{
	if(normalised) {
		float normalised = 1.0/(float)1023;
		return normalised * (float)m_x;
	}

	return (float)m_x;
}

float UiPanner::y(bool normalised)
{
	if(normalised) {
		float normalised = 1.0/(float)1023;
		return normalised * (float)m_y;
	}

	return (float)m_y;
}

void UiPanner::mousePressEvent(QMouseEvent *ev)
{
	if(ev->modifiers() & Qt::ControlModifier) {
		setXNormalised(0.5f);
		setYNormalised(0.0f);
	} else if(ev->modifiers() & Qt::ShiftModifier) {
		setXNormalised(0.0f);
		setYNormalised(0.0f);
	} else if(ev->modifiers() & Qt::AltModifier) {
		setXNormalised(1.0f);
		setYNormalised(0.0f);
		

	} else {
		m_xPos = ev->x();
		m_yPos = ev->y();

		updateValuesFromPosition();
	}

	//executeAction();
	emit valueChanged(x(),y());

	update();
}

void UiPanner::mouseMoveEvent(QMouseEvent *ev)
{
	if(ev->modifiers() & Qt::ControlModifier) {
		return;
	}

	m_xPos = ev->x();
	m_yPos = ev->y();

	updateValuesFromPosition();

	//executeAction();
	emit valueChanged(x(),y());

	update();
}

void UiPanner::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//updatePositionFromValues(); // This should only happen on a resize, not on every paint.

	//g.setBrush(NeColor(0.0f, 0.0f, 0.0f));
	//g.fillRectangle(NeRect(0,0,width()-1, height()-1));
	p.fillRect(0,0,width()-1, height()-1, QColor(0,0,0));

	//g.setBrush(NeColor(1.0f, 1.0f, 1.0f,0.4));
	//g.drawRectangle(m_border, m_border, width()-1-(m_border*2), height()-1-(m_border*2));
	//g.drawLine(width()/2, m_border, width()/2, height()-1-m_border);
	//g.drawLine(m_border, height()/2, width()-1-m_border, height()/2);

	p.setPen(QColor(255, 255, 255, 102));
	p.setBrush(QColor(0,0,0));
	p.drawRect(m_border, m_border, width()-1-(m_border*2), height()-1-(m_border*2));
	p.drawLine(width()/2, m_border, width()/2, height()-1-m_border);
	p.drawLine(m_border, height()/2, width()-1-m_border, height()/2);

	p.fillRect(m_xPos-1, m_yPos-1, 3, 3, QColor(120, 255, 120));
	

	/*NeLinearBrush lb(NeColor(1.0f, 1.0f, 1.0f, 0.21f), NeColor(1.0f, 1.0f, 1.0f, 0.07f)); // 0.16, 0.02
	lb.setStartPoint(NePoint(0, 0));
	lb.setEndPoint(NePoint(0, height()));
	g.setBrush(lb);
	g.fillRectangle(1, 1, width()-1-2, height()/2);//*/

	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);
	p.fillRect(1, 1, width()-3, height()-3, linearGradient);

	
}

void UiPanner::updatePositionFromValues(void)
{
	float length = height()-1-3-(m_border*2); //aLog("length = %f", length);
	float x = length/(float)1023; //aLog("x = %f", x);
	x = x * (float)m_x; //aLog("x = %f (m_value = %d)", x, m_x);
	m_xPos = 1+x+m_border;//1+(length)-(int)x; //aLog("m_faderPosition = %d", m_faderPosition);

	float y = length/(float)1023; //aLog("x = %f", x);
	y = y * (float)m_y; //aLog("x = %f (m_value = %d)", x, m_value);
	m_yPos = 1+y+m_border;//1+(length)-(int)y; //aLog("m_faderPosition = %d", m_faderPosition);

	//aLog("m_xPos = %d, m_yPos = %d", m_xPos, m_yPos);
}

void UiPanner::updateValuesFromPosition(void)
{
	if(m_xPos<1+m_border) {
		m_xPos = 1+m_border;
	}

	if(m_xPos>height()-1-2-m_border) {
		m_xPos = height()-1-2-m_border;
	}

	//aLog("(height()-1-3-(m_border*2)) = %d", (height()-1-3));
	float unit = (float)1023/(height()-1-3-(m_border*2));
	//aLog("unit = %f", unit);
	float v = unit * (float)(m_xPos-1-m_border);
	m_x = v;//(1023-v);
	//aLog("m_x = %d", m_x);

	if(m_yPos<1+m_border) {
		m_yPos = 1+m_border;
	}

	if(m_yPos>height()-1-2-m_border) {
		m_yPos = height()-1-2-m_border;
	}

	v = unit * (float)(m_yPos-1-m_border);
	m_y = v;//(1023-v);
	//aLog("m_y = %d", m_y);
}