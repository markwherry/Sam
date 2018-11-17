#include "SeGuiItemValue.h"

#include <QEvent>
#include <QPainter>

SeGuiItemValue::SeGuiItemValue(QWidget *parent, int propertyId, QString label, QString unit, int min, int max, float def, float step, int pixels)
	: UiListItem(parent, 24)
	, m_propertyId(propertyId)
	//, m_editorFaderController(editorFaderController)
	, m_label(label)
	, m_extraInfo(-1)
{
	//ui.setupUi(this);
	guiValue = new UiValue(this, 130, 24, unit, min, max, def, step, pixels);
	//guiValue->setFixedSize(100,20);
	guiValue->move(150,0);
	guiValue->setValueTextColor(QColor(250,250,250));
	guiValue->setUnitTextColor(QColor(200,200,200));
	connect(guiValue, SIGNAL(valueChanged(float)), this, SLOT(internalSetValue(float)));
	//qDebug("EXIT: SeGuiItemValue::SeGuiItemValue()");
	
	editorFaderRegistered = false;
	pixEditorFader = new QPixmap(":/Resources/editorfader16.png");

	//guiValue->setValueTextColor(QColor(255,255,255));
	//guiValue->setValueTextColor(QColor(60,60,60));
}

SeGuiItemValue::~SeGuiItemValue()
{

}

void SeGuiItemValue::setValueNormalised(float normalisedValue)
{
	float range = (float)(guiValue->maxValue() - guiValue->minValue());
	float v = range * normalisedValue;
	setValue(guiValue->minValue()+(int)v);
	emit valueChanged(m_propertyId, guiValue->minValue()+(int)v);
}

void SeGuiItemValue::internalSetValue(float value)
{
	qDebug("SeGuiItemValue::internalSetValue(%f)", value);

	emit valueChanged(m_propertyId, value);
}

void SeGuiItemValue::editorFaderUnregistered(void)
{
	//editorFaderRegistered = false;
	//update();
}

void SeGuiItemValue::changeEvent(QEvent *event)
{
	//qDebug("SeGuiItemValue::changeEvent()");

	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			guiValue->show();
		} else {
			guiValue->hide();
		}
	}
}

void SeGuiItemValue::mouseDoubleClickEvent(QMouseEvent *event)
{
	qDebug("SeGuiItemValue::mouseDoubleClickEvent()");

	/*if(m_editorFaderController) {
		qDebug("m_editorFaderController");
		if(editorFaderRegistered) {
			m_editorFaderController->unregisterGuiItemValue();
		} else {
			m_editorFaderController->registerGuiItemValue(this);
			editorFaderRegistered = true;
			update();
		}
		
	}*/
}

void SeGuiItemValue::paintEvent(QPaintEvent *event)
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
	int offset = 0;
	if(editorFaderRegistered) {
		p.setPen(QColor(100,100,255,alpha));
		p.drawPixmap(6,2,*pixEditorFader);
		offset = 16;
	} else {
		p.setPen(QColor(200,200,200, alpha)); //60
	}
	p.drawText(10+offset,0,125-offset,height(),Qt::AlignRight|Qt::AlignVCenter, m_label);
}