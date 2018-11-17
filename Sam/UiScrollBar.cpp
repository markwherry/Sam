#include "UiScrollBar.h"

#include <QPainter>
#include <QMouseEvent>

UiScrollBar::UiScrollBar(QWidget *parent)
	: QWidget(parent)
	, m_mouseOver(false)
	, m_min(0)
	, m_max(0)
	, m_value(0)
	, m_pageStep(0)
	, m_screenValue(0)
	, m_barLength(0)
	, m_downScreenValue(0)
{
	//ui.setupUi(this);
	downY = 0;
}

UiScrollBar::~UiScrollBar()
{

}

void UiScrollBar::setMinimum(int min)
{
	m_min = min;
	update();
}

void UiScrollBar::setMaximum(int max)
{
	m_max = max;
	update();
}

void UiScrollBar::setValue(int value)
{
	m_value = value;

	if(m_value < m_min) {
		m_value = m_min;
	} else if(m_value > m_max) {
		m_value = m_max;
	}

	float unit = (float)height()/(float)((m_max-m_min)+m_pageStep);
	//qDebug("height()=%d r+ps=%d", height(), range+m_pageStep);
	//qDebug("unit=%f", unit);

	m_screenValue = m_value * unit;
	
	update();
}

void UiScrollBar::setPageStep(int pageStep)
{
	qDebug("ScrollBar::setPageStep(%d)", pageStep);
	m_pageStep = pageStep;

	float unit = (float)height()/(float)((m_max-m_min)+m_pageStep);
	m_barLength = (int)((float)(height())*unit);
	update();
}

void UiScrollBar::enterEvent(QEvent *ev)
{
	m_mouseOver = true;
	update();
}

void UiScrollBar::leaveEvent(QEvent *ev)
{
	m_mouseOver = false;
	update();
}

void UiScrollBar::mousePressEvent(QMouseEvent *ev)
{
	downY = ev->y();
	m_downScreenValue = m_screenValue;
}

void UiScrollBar::mouseMoveEvent(QMouseEvent *ev)
{
	m_screenValue = m_downScreenValue+(ev->y()-downY);

	if(m_screenValue < 0) {
		m_screenValue = 0;
	} else if(m_screenValue > height()-m_barLength) {
		m_screenValue = height()-m_barLength;
	}

	update();

	float screenUnit = (float)((m_max-m_min)+m_pageStep)/(float)height();

	m_value = screenUnit * m_screenValue;

	emit valueChanged(m_value);
}

void UiScrollBar::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int range = m_max-m_min;
	if(range > 0) {
		
		int color = 160;
		if(m_mouseOver) {
			color = 230;
		}
		p.setPen(QColor(color,color,color));
		p.setBrush(QColor(color,color,color));
		p.drawRect(0,m_screenValue,width()-1,m_barLength);

		color -= 40;
		p.setPen(QColor(color,color,color));
		p.drawLine(1,m_screenValue,1,m_screenValue+m_barLength);
		p.drawLine(width()-2,m_screenValue,width()-2,m_screenValue+m_barLength);

		color -= 80;
		p.setPen(QColor(color,color,color));
		p.drawLine(0,m_screenValue,0,m_screenValue+m_barLength);
		p.drawLine(width()-1,m_screenValue,width()-1,m_screenValue+m_barLength);
	}
	
	/*if(range < 1 || height() < 1) {
		return;
	}
	float unit = height()/range;
	qDebug("unit = %f", unit);
	if(unit < 1.0) {
		int color = 140;
		if(m_mouseOver) {
			color = 210;
		}
		p.setPen(QColor(color,color,color));
		p.setBrush(QColor(color,color,color));
		p.drawRect(0,0,width()-1,height()-1);
	}*/

}

void UiScrollBar::resizeEvent(QResizeEvent *ev)
{
	
}