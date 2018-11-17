#include "UiRect.h"

#include <QPainter>

UiRect::UiRect(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_penColor = QColor(0,0,0);
	m_brushColor = QColor(0,0,0);
}

UiRect::~UiRect()
{

}

void UiRect::setPenColor(QColor color)
{
	m_penColor = color;
	
	update();
}

void UiRect::setBrushColor(QColor color)
{
	m_brushColor = color;
	
	update();
}

void UiRect::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.setPen(m_penColor);
	p.setBrush(m_brushColor);
	p.drawRect(0,0,width()-1,height()-1);
}