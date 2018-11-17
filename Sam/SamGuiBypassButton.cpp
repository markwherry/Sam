#include "SamGuiBypassButton.h"

#include <QMouseEvent>
#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiBypassButton::SamGuiBypassButton(QWidget *parent)
	: QWidget(parent)
	, m_insert(0)
{
	//ui.setupUi(this);

	setFixedSize(60,20);

	m_menuLearn = new QMenu(0);
	m_actionLearn   = m_menuLearn->addAction("Learn MIDI Controller");
	m_actionUnlearn = m_menuLearn->addAction("Unlearn MIDI Controller");
	connect(m_actionLearn,   SIGNAL(triggered()), this, SLOT(learn()));
	connect(m_actionUnlearn, SIGNAL(triggered()), this, SLOT(unlearn()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiBypassButton::~SamGuiBypassButton()
{

}

void SamGuiBypassButton::setToolInsert(SamToolInsert *insert)
{
	//qDebug("SamGuiCsToolEditor::setMusicToolInsert()");

	m_insert = insert;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiBypassButton::learn(void)
{
	if(m_insert) { qDebug("Set Active Learn");
		m_insert->setActiveLearn(true);
		update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiBypassButton::unlearn(void)
{
	if(m_insert) {
		m_insert->setActiveLearn(false);
		m_insert->setActiveController(-1);
		update();
		if(parentWidget())parentWidget()->update();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiBypassButton::mousePressEvent(QMouseEvent *ev)
{
	if(!m_insert) return;

	if(ev->button() == Qt::LeftButton) {
		emit pressed();
	} else if(ev->button() == Qt::RightButton) {
		if(m_insert->activeController() > -1 || m_insert->isActiveLearn()) {
			m_actionUnlearn->setEnabled(true);
		} else {
			m_actionUnlearn->setEnabled(false);
		}
		m_menuLearn->exec(ev->globalPos());
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiBypassButton::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(118,103,88));
	if(m_insert) {
		if(!m_insert->isActive()) {
			//p.setPen(QColor(118,103,88));
			p.setBrush(QColor(255,130,0));
		}
	
		if(m_insert->activeController() > -1) {
			p.setPen(QColor(0,255,255));
		}
		if(m_insert->isActiveLearn()) {
			p.setPen(QColor(255,0,0));
		}
	
	}
	
	p.drawRect(0,0,width()-1,height()-1);

	p.setFont (QFont("Calibri",10,QFont::Bold));
	p.setPen(QColor(0,0,0));
	p.drawText(0,0,width()-1,height()-1, Qt::AlignCenter, "Bypass");
}