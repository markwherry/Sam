#include "SeGuiItemHeading.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiItemHeading::SeGuiItemHeading(QWidget *parent, QString heading)
	: UiListItem(parent, 1)
	, m_heading(heading)
	, m_backgroundColor(QColor(00,00,00))
{
	//ui.setupUi(this);

	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiItemHeading::~SeGuiItemHeading()
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiItemHeading::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	QColor bc = m_backgroundColor;
	/*if(!isEnabled()) {
		bc = QColor(60,60,60);
	}*/

	p.setPen(bc);
	p.setBrush(bc);
	p.drawRect(0,0,width()-1,height()-1);

	QFont font("", 8);
	//font.setBold(true);
	p.setFont(font);
	p.setPen(QColor(200,200,200));
	//p.drawText(10,0,width()-20,height(),Qt::AlignCenter, m_heading);
}