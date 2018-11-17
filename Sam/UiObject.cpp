#include "UiObject.h"

#include <QPainter>

UiObject::UiObject(QWidget *parent)
	: QWidget(parent)
	, m_mouseOver(false)
	, m_drawLine(true)
	, m_drawFill(true)
	, m_drawTint(false)
	, m_bold(false)
	, m_textAlignment(Qt::AlignLeft|Qt::AlignVCenter)
	, m_textSize(9)
	, m_textWidth(0)
	, m_lineColor(QColor(60,60,60))
	, m_fillColor(QColor(120,120,120))
	, m_textColor(QColor(0,0,0))
	, m_lineWidth(1)
	, m_drawMouseOverFill(true)
	, m_editable(true)
{
	//ui.setupUi(this);
	m_font = QFont("Calibri", 9);
}

UiObject::~UiObject()
{

}

void UiObject::enterEvent(QEvent *event)
{
	if(!isEnabled()) return;

	m_mouseOver = true;
	update();
}

void UiObject::leaveEvent(QEvent *event)
{
	if(!isEnabled()) return;

	m_mouseOver = false;
	update();
}

void UiObject::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	QColor penColor = QColor(0,0,0,0);
	QColor brushColor = QColor(0,0,0,0);
	
	if(m_drawLine) {
		if(m_mouseOver && isEnabled() && m_editable) {
			penColor = QColor(225,225,225);
		} else {
			penColor = m_lineColor;
		}
	}

	if(m_drawFill) {
		brushColor = m_fillColor;
	}

	if(!isEnabled()) {
		if(m_drawLine) {
			penColor.setAlpha(100);
		}
		if(m_drawFill) {
			brushColor.setAlpha(100);
		}
	}

	QPen pen(penColor);
	pen.setWidth(m_lineWidth);
	p.setPen(pen);
	p.setBrush(brushColor);
	p.drawRect(0, 0, width()-1-m_textWidth, height()-1);

	if(m_mouseOver && isEnabled() && m_drawMouseOverFill && m_editable) {
		p.setPen(QColor(0,0,0,0));
		p.setBrush(QColor(255,255,255,50));
		p.drawRect(0, 0, width()-1-m_textWidth, height()-1);
	}

	paint(p);

	if(m_drawTint) {
		drawTint(p, this, 0, width()-2-m_textWidth);
	}
}

void UiObject::paint(QPainter &p)
{
}

void UiObject::drawTint(QPainter &p, QWidget *widget, int alphaAdjustment, int width, int xOffset)
{
	//if(widgetHeight<1) {
		int widgetHeight = widget->height();
	//}
	QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, widgetHeight));
	color.setAlpha(40+alphaAdjustment);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5+alphaAdjustment);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	int w = width;
	if(w<0) w = widget->width()-2;
	p.drawRect(xOffset+1,1,w,(widgetHeight/2));
}