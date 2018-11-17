#include "SamGuiBounceGroup.h"

#include <QPainter>
#include <QMouseEvent>

SamGuiBounceGroup::SamGuiBounceGroup(QWidget *parent, int id)
	: QWidget(parent)
	, m_id(id)
	, m_active(false)
{
	//ui.setupUi(this);

	setFixedSize(180, 30);

	m_selectMenu = new UiSelectMenu(this, 65);
	m_selectMenu->move(110, 5);
	m_selectMenu->addItem("Mono");
	m_selectMenu->addItem("Stereo");
	m_selectMenu->addItem("Quad");
	m_selectMenu->setEnabled(false);

	m_label = new UiLabel(this, "", 16, 65-6);
	m_label->move(35+3, 7);
	m_label->setEditable(true);
}

SamGuiBounceGroup::~SamGuiBounceGroup()
{

}

SamBounceGroup* SamGuiBounceGroup::createBounceGroup(void)
{
	AString prefix = AString("");
	if(m_label->text().length() > 0) {
		QByteArray bap = m_label->text().toAscii();
		prefix = bap.data();
	}

	CsSpeakerConfig config;
	switch(m_selectMenu->selectedItem()) {
		case 0:
			config.setConfig(CsSpeakerConfig::Mono);
			break;
		case 1:
			config.setConfig(CsSpeakerConfig::Stereo);
			break;
		case 2:
			config.setConfig(CsSpeakerConfig::Surround40);
			break;
	}
	
	SamBounceGroup *bg = new SamBounceGroup(prefix, config);

	return bg;
}

void SamGuiBounceGroup::setActive(bool active)
{
	m_active = active;
	if(m_active) {
		raise();
	} else {
		lower();
	}

	emit activated(m_id);

	update();
}

void SamGuiBounceGroup::mousePressEvent(QMouseEvent *ev)
{
	if(ev->x() >= 0 && ev->x() < 30) {
		setActive(!m_active);
		m_selectMenu->setEnabled(m_active);
		m_label->setEnabled(m_active);
	}
}

void SamGuiBounceGroup::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(m_active) {
		p.setPen(QColor(255,255,255));
	} else {
		p.setPen(QColor(100,100,100));
	}
	//p.setBrush(QColor(0,0,0,0));
	if(m_id%2==0) {
		p.setBrush(QColor(60,60,60));
	} else {
		p.setBrush(QColor(80,80,80));
	}
	p.drawRect(0,0,width()-1,height()-1);

	p.drawLine(30,0,30,height()-1);
	p.drawLine(105,0,105,height()-1);

	p.setFont(QFont("Calibri", 14, QFont::Bold));
	p.drawText(0,0,30,30,Qt::AlignCenter, QString::number(m_id+1));
}