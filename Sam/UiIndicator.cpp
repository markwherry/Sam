#include "UiIndicator.h"

#include <QPainter>

UiIndicator::UiIndicator(QWidget *parent, int width, int height, bool held, bool vertical)
	: QWidget(parent)
	, m_vertical(vertical)
	, m_value(0)
	, m_minimumValue(0)
	, m_maximumValue(127) 
	, m_timerId(-1)
	, m_held(held)
{
	//ui.setupUi(this);
	setFixedSize(width, height);

	m_color = QColor(150,0,0,255);
	m_backgroundColor = Qt::black;
}

UiIndicator::~UiIndicator()
{

}

void UiIndicator::setValue(int value, bool falling)
{
	if (value > m_maximumValue) {
		m_value = m_maximumValue;
	} else if (value<m_minimumValue) {
		m_value = m_minimumValue;
	} else {
		if (falling || m_held)
			m_value = value;
		else if (value>m_value)
			m_value = value;
	}

	update ();
}

void UiIndicator::paintEvent(QPaintEvent *event)
{
	QPainter p(this);


	int range = m_maximumValue - m_minimumValue;
	float unit = (float)height()/(float)range;

	float h = unit * m_value;

	p.fillRect(0, height() - (int)h, width(), (int)h, m_color);
	


	/*int span;

	if (m_vertical)
		span = height()-1;
	else
		span = width()-1;

	int offset;

	if (m_value == m_minimumValue) {
		offset = span;
	} else {
		int range = (m_maximumValue-m_minimumValue)+1;
		float unit = (float)span/range;
		offset = (m_maximumValue-m_value)*unit;
	}

	p.setPen(m_color);
	p.setBrush (m_color);        

	QRectF rectangle(0, 0, width()-1, height()-1);
    p.drawRect(rectangle);

	// Edge Tint
	//p.setPen(QColor(255,255,255,60));
	//p.drawLine(0,0,0,height()-1);
	//p.setPen(QColor(0,0,0,60));
	//p.drawLine(width()-1,0,width()-1,height()-1);

	if(m_value < m_maximumValue)
	{
		p.setPen(m_backgroundColor);
		p.setBrush (m_backgroundColor);        

		QRectF rectangle2;
		if (m_vertical)
			rectangle2.setCoords(0, 0, width()-1, offset);
		else
			rectangle2.setCoords(width()-1, 0, (width()-1)-offset, height()-1);
		p.drawRect(rectangle2);
	}*/

	//p.setPen(QColor(30,30,30));
	//for(int i=1; i<height()-1; i+=2) {
	//	p.drawLine(0,i,width()-1,i);
	//}

	/*QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	color.setAlpha(40);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	p.drawRect(0,0,358,height()/2);*/

}

//void UiIndicator::timerEvent(QTimerEvent *event)
//{
	//qDebug ("CsIndicator::timerEvent ()");
	//timerId = event->timerId();
	//value--;

	//if (value<minimumValue)
	//{
	//	value = minimumValue;
	//	killTimer (timerId);
	//	timerId = -1;
	//}

	//repaint ();
	

	/*m_value--;

	if (m_value<m_minimumValue)
	{
		m_value = m_minimumValue;
		//timer->stop();
	}*/

//	update();
//}

void UiIndicator::timerUpdate (void)
{
	//qDebug ("Timer!");

	m_value--;

	if (m_value<m_minimumValue)
	{
		m_value = m_minimumValue;
		//timer->stop();
	}

	repaint ();
}

void UiIndicator::updateTimer(void)
{
	if (m_value!=0 && !m_held)
		setValue (m_value-4, true);

	repaint();
}