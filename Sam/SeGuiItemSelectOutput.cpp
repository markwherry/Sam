#include "SeGuiItemSelectOutput.h"
#include "SamOutput.h"

#include <QPainter>
#include <QEvent>

SeGuiItemSelectOutput::SeGuiItemSelectOutput(QWidget *parent, int propertyId, QString label)
	: UiListItem(parent, 20)
	, m_propertyId(propertyId)
	, m_label(label)
	, m_version(0)
{
	//ui.setupUi(this);

	m_selectOutput = new UiSelectMenu(this, 150);
	m_selectOutput->move(130,0);
	m_selectOutput->setTextColor(QColor(0,0,0));
	connect(m_selectOutput, SIGNAL(itemSelected(int)), this, SLOT(internalSetOutput(int)));
}

SeGuiItemSelectOutput::~SeGuiItemSelectOutput()
{

}

void SeGuiItemSelectOutput::setVersion(SamVersion *version)
{
	//aLog("SeGuiItemSelectOutput::setVersion(%d)", version);
	m_version = version;

	updateOutputList();
}

void SeGuiItemSelectOutput::setOutput(int outputListId)
{
	aLog("SeGuiItemSelectOutput::setOutput(%d)", outputListId);
	if(m_version) {
		aLog("Setting");
		m_selectOutput->setSelectedItem(outputListId);
	}
}

void SeGuiItemSelectOutput::updateOutputList(void)
{
	if(!m_version) {
		return;
	}

	m_selectOutput->clear();

	for(int i=0; i<m_version->numOutputs(); i++) {
		SamOutput *output = m_version->output(i);
		m_selectOutput->addItem(output->name().toChar());
	}
}

void SeGuiItemSelectOutput::internalSetOutput(int output)
{
	int o = 0;

	if(m_version) {
		o = m_version->output(output)->id();
	}

	emit outputChanged(m_propertyId, (float)o);
}

void SeGuiItemSelectOutput::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			m_selectOutput->show();
		} else {
			m_selectOutput->hide();
		}
		update();
	}
}

void SeGuiItemSelectOutput::paintEvent(QPaintEvent *event)
{
	UiListItem::paintEvent(event);

	QPainter p(this);

	QFont font("", 8);
	font.setBold(true);
	p.setFont(font);
	int alpha = 255;
	if(!isEnabled()) {
		alpha = 100;
	}
	p.setPen(QColor(60,60,60,alpha));
	p.drawText(10,0,115,20,Qt::AlignLeft|Qt::AlignVCenter, m_label);
}