#include "SeGuiItemBool.h"

#include <QPainter>
#include <QEvent>

SeGuiItemBool::SeGuiItemBool(QWidget *parent, int propertyId, QString label)
	: UiListItem(parent, 24)
	, m_propertyId(propertyId)
	, m_label(label)
{
	//ui.setupUi(this);

	m_checkBox = new UiCheckBox(this);
	connect(m_checkBox, SIGNAL(changed(bool)), this, SLOT(internalSetChecked(bool)));
	m_checkBox->move(150, (height()/2) - (m_checkBox->height()/2));
}

SeGuiItemBool::~SeGuiItemBool()
{

}


void SeGuiItemBool::internalSetChecked(bool checked)
{
	//qDebug("SeGuiItemBool::internalSetChecked()");

	emit changed(m_propertyId, checked);
}

void SeGuiItemBool::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			m_checkBox->show();
		} else {
			m_checkBox->hide();
		}
	}
}

void SeGuiItemBool::paintEvent(QPaintEvent *event)
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
	p.setPen(QColor(200,200,200,alpha));
	p.drawText(10,0,125,height(),Qt::AlignRight|Qt::AlignVCenter, m_label);
}