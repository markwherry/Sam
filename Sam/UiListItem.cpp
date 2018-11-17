#include "UiListItem.h"

#include <QPainter>
#include <Quuid>

UiListItem::UiListItem(QWidget *parent, int initialHeight)
	: QWidget(parent)
	, m_selected(false)
	, m_positionInList(0)
	, m_label("")
	, m_indent(true)
{
	//ui.setupUi(this);

	m_uuid = QUuid::createUuid().toString();

	//int n = 55;
	m_backgroundColor[1] = QColor (55,55,55); //55,55,55 //200//80
	m_backgroundColor[0] = QColor (55,55,55); //50,50,50 //180//75

	setFixedHeight(initialHeight);
}

UiListItem::~UiListItem()
{

}

void UiListItem::setSelected(bool selected)
{
	m_selected = selected;

	update();
}

void UiListItem::setLabel(QString label)
{
	m_label = label;

	update();
}

void UiListItem::select(bool exclusive)
{
	//qDebug("UiListItem::select()");

	emit selected(m_uuid, exclusive);
}

void UiListItem::changeHeight(int h)
{
	setFixedHeight(h);

	emit heightChanged();
}

void UiListItem::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	// Draw the background
	// -------------------
	int cIndex = 0;
	if(positionInList()%2 == 0) {
		cIndex = 1;
	}

	if(m_selected) {
		p.setBrush(QColor(120,120,200));
		p.setPen(QColor(120,120,200));
	} else {
		
		p.setBrush(m_backgroundColor[cIndex]);
		p.setPen(m_backgroundColor[cIndex]);

	}
	p.drawRect(0,0,width()-1, height()-1);

	if(m_indent) {
		if(positionInList() != 0) {
			p.setPen(QColor(255,255,255,20));
			p.drawLine(0,0,width()-1,0);
		}
	
		p.setPen(QColor(0,0,0,40));
		p.drawLine(0,height()-1,width()-1,height()-1);
	}

	// Draw the label
	// --------------
	if(m_label.length() < 1) {
		return;
	}

	/*QFont font("", 8);
	font.setBold(true);
	if(m_labelBold) {
		p.setFont(font);
	}*/
	int alpha = 255;
	if(!isEnabled()) {
		alpha = 100;
	}

	QColor colorText = QColor(0,0,0);
	if(m_selected) {
		colorText = QColor(240,240,240, alpha);
	}
	p.setPen(colorText);
	p.drawText(10,0,width()-10,height(),Qt::AlignLeft|Qt::AlignVCenter, m_label);

	
}