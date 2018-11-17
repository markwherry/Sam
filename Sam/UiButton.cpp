#include "UiButton.h"

#include <QPainter>

UiButton::UiButton(QWidget *parent)
	: UiObject(parent)
	, m_default(false)
{
	//ui.setupUi(this);

	setFixedSize(90,30);

	setDrawFill(true);
	setLineColor(QColor(30,30,30));
	setFillColor(QColor(60,60,60));
	//setDrawTint(false);
}

UiButton::~UiButton()
{

}

void UiButton::setDefault(bool def)
{ 
	m_default = def;
	if(m_default) {
		setLineWidth(3);
	}
}

void UiButton::mousePressEvent(QMouseEvent *ev)
{
	//qDebug("UiButton::mousePressEvent()");
}

void UiButton::mouseReleaseEvent(QMouseEvent *ev)
{
	//qDebug("UiButton::mousePressEvent()");

	emit clicked();
}

void UiButton::paint(QPainter &p)
{



	int bold = 0;
	if(m_default) {
		bold = QFont::Bold;
	}
	p.setFont(QFont("Calibri", 12, bold));
	if(isEnabled()) {
		p.setPen(QColor(255,255,255));
	} else {
		p.setPen(QColor(100,100,100));
	}
	
	//p.drawText(5,0,width()-10,height()-3, Qt::AlignRight|Qt::AlignBottom, text());
	p.drawText(5,0,width()-10,height()-3, Qt::AlignCenter, text());

	p.setPen(QColor(90,90,90));
	p.setBrush(QColor(0,0,0,0));
	p.drawRect(1,1,width()-3,height()-3);
}