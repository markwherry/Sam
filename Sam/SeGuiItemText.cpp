#include "SeGuiItemText.h"

#include <QEvent>
#include <QPainter>

SeGuiItemText::SeGuiItemText(QWidget *parent, int propertyId, QString label, QString tag)
	: UiListItem(parent, 24)
	, m_propertyId(propertyId)
	, m_tag(tag)
	, m_label(label)
{
	//ui.setupUi(this);

	m_labelData = new UiLabel(this, "", 24, 140); //150, 20);
	m_labelData->setTextColor(QColor(0,0,0));
	m_labelData->move(150,0);
	m_labelData->setEditable(true);
	//connect(m_labelData, SIGNAL(clicked()), this, SLOT(select()));
	connect(m_labelData, SIGNAL(textChanged(QString)), this, SLOT(internalSetText(QString)));
}

SeGuiItemText::~SeGuiItemText()
{

}

void SeGuiItemText::internalSetText(QString text)
{
	qDebug("SeGuiItemText::internalSetText()");

	emit textChanged(m_propertyId, text);
	emit textChanged(m_propertyId, text, m_tag);
}

void SeGuiItemText::changeEvent(QEvent *event)
{
	//qDebug("SeGuiItemText::changeEvent()");
	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			m_labelData->show();
		} else {
			m_labelData->hide();
		}
	}
	//qDebug("EXIT: SeGuiItemText::changeEvent()");
}

void SeGuiItemText::paintEvent(QPaintEvent *event)
{
	UiListItem::paintEvent(event);

	QPainter p(this);

	QFont font("Calibri", 10);
	//font.setBold(true);
	p.setFont(font);
	int alpha = 255;
	if(!isEnabled()) {
		alpha = 100;
	}
	int c = 200; //60
	int dc = 250; //220
	//if(isSelected()) {
	//	c = 60;
	//	dc = 0;
	//}
	m_labelData->setTextColor(QColor(dc,dc,dc));
	p.setPen(QColor(c,c,c,alpha));
	p.drawText(10,0,125,height(),Qt::AlignRight|Qt::AlignVCenter, m_label);
}