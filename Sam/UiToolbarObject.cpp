#include "UiToolbarObject.h"

#include <QPainter>

UiToolbarObject::UiToolbarObject(UiToolBarObjectType type, int width)
	: QWidget(0)
	, m_type(type)
{
	//ui.setupUi(this);
	setFixedWidth(width);
	m_label[0] = "";
	m_label[1] = "";
	for(int i=0; i<2; i++) {
		m_pixmap[i] = 0;
	}
	m_mouseOver = false;
	m_state = 0;
}

UiToolbarObject::~UiToolbarObject()
{

}

void UiToolbarObject::setImage(QString filename, int state)
{
	m_pixmap[state] = new QPixmap(filename);
	//update();
}

void UiToolbarObject::enterEvent(QEvent *event)
{
	if(m_type != Separator) {
		m_mouseOver = true;
		update();
	}
}

void UiToolbarObject::leaveEvent(QEvent *event)
{
	if(m_type != Separator) {
		m_mouseOver = false;
		update();
	}
}

void UiToolbarObject::mousePressEvent(QMouseEvent *event)
{
	if(m_type != Separator) {
		emit clicked();
	}
}

void UiToolbarObject::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	if(m_type == Separator) {
		p.setPen(QColor(0,0,0,60));
		p.drawLine(0,0,0,height()-1);
		p.setPen(QColor(255,255,255,40));
		p.drawLine(1,0,1,height()-1);
	} else {

		if(m_mouseOver) {
			//p.setPen(QColor(255,255,255,0));
			//p.setBrush(QColor(255,255,255,20));
			//p.drawRect(0,0,width()-1, height()-1);
		}

		if(m_pixmap[m_state]) {
			p.drawPixmap(0, 0, *m_pixmap[m_state]);
		}

		QFont f("",0,QFont::Bold);
		f.setPixelSize(9); //9
		f.setStyleStrategy(QFont::PreferAntialias);

		p.setFont(f);
		int tc = 180;
		if(m_mouseOver) {
			tc = 255;
		}
		p.setPen(QColor(tc,tc,tc));

		p.drawText(28,1+0, width(), 12, Qt::AlignLeft|Qt::AlignVCenter, m_label[0]);
		p.drawText(28,1+12, width(), 12, Qt::AlignLeft|Qt::AlignVCenter, m_label[1]);

		/*

		if(m_mouseOver) {
			p.setPen(QColor(255,255,255,0));
			p.setBrush(QColor(255,255,255,20));
			p.drawRect(0,0,width()-1, height()-1);
		}
		
		QFont f("",0,QFont::Bold);
		f.setPixelSize(9); //9
		f.setStyleStrategy(QFont::PreferAntialias);

		p.setFont(f);
		int tc = 200;
		if(m_mouseOver) {
			tc = 255;
		}
		p.setPen(QColor(tc,tc,tc));

		if(m_label[1].length()==0) {
			p.drawText(2,height()-29,width()-2,22,Qt::AlignCenter|Qt::AlignVCenter, m_label[0]);
		} else {
			p.drawText(2,height()-27,width()-2,12,Qt::AlignCenter|Qt::AlignVCenter, m_label[0]);
			p.drawText(2,height()-15,width()-2,12,Qt::AlignCenter|Qt::AlignVCenter, m_label[1]);
		}

		if(m_pixmap[m_state]!=0) {
			//QPen pen(QColor(255,255,255));
			//pen.setWidth(2);
			//p.setPen(pen);
			//p.setRenderHint(QPainter::Antialiasing);
			//
			//p.drawEllipse((width()/2)-(m_pixmap[m_state]->width()/2),0,m_pixmap[m_state]->width(), m_pixmap[m_state]->height());
			////p.drawPixmap((width()/2)-(m_pixmap->width()/2),6,*m_pixmap);
			p.drawPixmap((width()/2)-(m_pixmap[m_state]->width()/2),0,*m_pixmap[m_state]);
		}

		if(!isEnabled()) {
			p.setPen(QColor(0,0,0,150));
			p.setBrush(QColor(0,0,0,150));
			p.drawRect(2,0,width()-3,height()-1);
		}

		*/

	}

}