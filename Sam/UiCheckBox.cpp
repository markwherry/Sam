#include "UiCheckBox.h"

#include <QPainter>

UiCheckBox::UiCheckBox(QWidget *parent, int w, int h)
	: UiObject(parent)
	, m_checked(false)
{
	//ui.setupUi(this);
	setFixedSize(w, h);
	setFillColor(QColor(70,70,70));
	setLineColor(QColor(0,0,0));
}

UiCheckBox::~UiCheckBox()
{

}

void UiCheckBox::setChecked(bool checked)
{
	m_checked = checked;

	update();
}

void UiCheckBox::setText(QString text)
{
	UiObject::setText(text);

	QFontMetrics fm(m_font);
	m_textWidth = fm.width(m_text) + 5;

	setFixedWidth(width()+m_textWidth);

	update();
}

void UiCheckBox::mousePressEvent(QMouseEvent *ev)
{
	if(!isEditable()) {
		return;
	}

	if(m_checked) {
		m_checked = false;
	} else {
		m_checked = true;
	}

	emit changed(m_checked);

	update();
}

void UiCheckBox::paint(QPainter &p)
{
	if(m_checked) {
		p.fillRect(1,1,height()-2,height()-2, QColor(70,122,195)); //
	}
	

	int alpha = 255;
	if(!isEnabled()) {
		alpha = 100;
	}

	QPen pen(QColor(250,250,250,alpha));
	//p.setPen(pen);
	//p.setBrush(QColor(0,0,0,20));
	//p.drawRect(0, 0, width()-1, height()-1);
	if(m_checked) {
		pen.setWidth(2);
		p.setPen(pen);
		p.drawLine(4,4,height()-5,height()-5);
		p.drawLine(height()-5,4,4,height()-5);
	}

	if(m_text.size()>0) {
		int tc = 200;
		if(m_mouseOver) {
			tc = 255;
		}
		p.setPen(QColor(tc,tc,tc));
		p.setFont (m_font);

		p.drawText(width()-m_textWidth+5, 0, m_textWidth, height()-1, Qt::AlignLeft|Qt::AlignVCenter, m_text);
	}
}