#include "UiProgressBar.h"

#include <QPainter>

#include "UiObject.h"

UiProgressBar::UiProgressBar(QWidget *parent)
	: QWidget(parent)
	, m_valueNormalised(0)
{
	//ui.setupUi(this);
}

UiProgressBar::~UiProgressBar()
{

}

void UiProgressBar::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(30,30,30));
	p.setBrush(QColor(0,0,0));
	p.drawRect(0,0,width()-1,height()-1);

	float w = (width()-3) * m_valueNormalised;

	p.setPen(QColor(0,120,0));
	p.setBrush(QColor(0,150,0));
	p.drawRect(1,1,w,height()-3);

	QFont f("Calibri");
	f.setPixelSize((height()/2)+(height()/20));
	f.setBold(true);
	f.setStyleStrategy(QFont::PreferAntialias);
	p.setFont(f);
	p.setPen(QColor(255,255,255));
	p.drawText(1,1,w-4,height()-3, Qt::AlignRight|Qt::AlignVCenter, QString::number(m_valueNormalised*100.0,'f', 0)+"%");

	//UiObject::drawTint(p, this, 20, w, 0);
}

void UiProgressBar::setValueNormalised(float valueNormalised)
{
	m_valueNormalised = valueNormalised;

	update();
}