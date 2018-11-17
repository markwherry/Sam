#include "UiSection.h"

#include <QPainter>

UiSection::UiSection(QWidget *parent, QString title)
	: QWidget(parent)
	, m_title(title)
{
	//ui.setupUi(this);

	/*m_labelTitle = new UiLabel(this, title);
	m_labelTitle->move(4,1);
	//m_labelTitle->setDrawUnderline(true);
	m_labelTitle->setTextBold(true);
	m_labelTitle->setTextColor(QColor(240,240,240));
	m_labelTitle->setTextSize(10);*/
}

UiSection::~UiSection()
{

}

void UiSection::paintEvent(QPaintEvent *ev)
{
	//qDebug("+++++ UiSection::paintEvent()");
	QPainter p(this);

	//p.setPen(QColor(0,0,0));
	//p.setBrush(QColor(0,0,0));
	//p.drawRect(0, 0, width() - 1, height()-1);

	p.fillRect(0, 0, width() - 1, height()-1, QColor(0,0,0,30));
	p.fillRect(0, 0, width() - 1, 20, QColor(0,0,0,60));

	p.setPen(QColor(240,240,240));
	p.setFont(QFont("Calibri", 10, QFont::Bold));
	p.drawText(4, 0, width()-8, 20, Qt::AlignLeft|Qt::AlignVCenter, m_title);
}

void UiSection::resizeEvent(QPaintEvent *ev)
{
	//m_labelTitle->setFixedWidth(width()-8);
}