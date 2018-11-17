#include "SeGuiItemSelectArticuation.h"

#include <QPainter>
#include <QEvent>

SeGuiItemSelectArticuation::SeGuiItemSelectArticuation(QWidget *parent)
	: UiListItem(parent, 20)
	, m_channel(0)
	//, m_propertyId(propertyId)
{
	//ui.setupUi(this);
	m_selectArticulation = new UiSelectMenu(this, 150);
	m_selectArticulation->move(150,0);
	m_selectArticulation->setTextColor(QColor(250,250,250));
	m_selectArticulation->setTextBold(false);
	connect(m_selectArticulation, SIGNAL(itemSelected(int)), this, SLOT(internalSetArticulation(int)));
}

SeGuiItemSelectArticuation::~SeGuiItemSelectArticuation()
{

}

void SeGuiItemSelectArticuation::setChannel(SamChannel *channel)
{
	m_channel = channel;

	m_selectArticulation->clear();

	if(m_channel) {
		if(m_channel->isArticulationLock()) {
			m_selectArticulation->addItem("*** LOCKED ***");
			m_selectArticulation->setEnabled(false);
		} else {
			m_selectArticulation->setEnabled(true);
			for(int i=0; i<m_channel->version()->numArticulations(); i++) {
				m_selectArticulation->addItem(m_channel->version()->articulation(i)->name().toChar());
			}

			if(m_channel->currentArticulationIndex() > -1) {
				m_selectArticulation->setSelectedItem(m_channel->currentArticulationIndex());
			}
		}
	}
}

void SeGuiItemSelectArticuation::internalSetArticulation(int index)
{
	if(m_channel) {
		SamEvent ev(SamEvent::Parameter);
		ev.subType = SamParameter::Articulation;
		ev.iValue = index;
		ev.from = this;
		m_channel->addEvent(ev);
	}
}

/*void SeGuiItemSelectArticuation::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			m_selectArticulation->show();
		} else {
			m_selectArticulation->hide();
		}
		update();
	}
}*/

void SeGuiItemSelectArticuation::paintEvent(QPaintEvent *event)
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
	p.drawText(10,0,125,height(),Qt::AlignRight|Qt::AlignVCenter, "Articulation");
}