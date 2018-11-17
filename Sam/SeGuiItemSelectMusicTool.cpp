#include "SeGuiItemSelectMusicTool.h"

#include <QPainter>
#include <QEvent>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiItemSelectMusicTool::SeGuiItemSelectMusicTool(QWidget *parent, int propertyId, QString label)
	: UiListItem(parent, 20)
	, m_propertyId(propertyId)
	, m_label(label)
	, m_version(0)
	, m_channel(0)
{
	//ui.setupUi(this);
	m_selectMusicTool = new SamGuiSelectMusicTool(this, true);
	m_selectMusicTool->move(130,0);
	m_selectMusicTool->setTextColor(QColor(0,0,0));
	m_selectMusicTool->setDoubleClick(true);
	m_selectMusicTool->setInsertIndex(propertyId);
	//selectMusicTool[i]->setDrawFill(false);
	//selectMusicTool[i]->setDrawTint(false);
	//selectMusicTool[i]->setDrawLine(false);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiItemSelectMusicTool::~SeGuiItemSelectMusicTool()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiItemSelectMusicTool::setVersion(SamVersion *version, SamChannel *channel)
{
	m_version = version;
	m_channel = channel;
	m_selectMusicTool->setChannel(m_channel);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiItemSelectMusicTool::changeEvent(QEvent *event)
{
	if(event->type()==QEvent::EnabledChange) {
		if(isEnabled()) {
			m_selectMusicTool->show();
		} else {
			m_selectMusicTool->hide();
		}
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiItemSelectMusicTool::paintEvent(QPaintEvent *event)
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