#include "SamGuiCellAnalyserVelocity.h"

#include <QPainter>

SamGuiCellAnalyserVelocity::SamGuiCellAnalyserVelocity(QWidget *parent)
	: QWidget(parent)
	, m_velLo(100)
	, m_velHi(127)
	, m_numCells(5)
	, m_totalCells(10)
{
	//ui.setupUi(this);
	setFixedSize(50, 100);
}

SamGuiCellAnalyserVelocity::~SamGuiCellAnalyserVelocity()
{

}

void SamGuiCellAnalyserVelocity::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if(isEnabled()) {
		p.setPen(QColor(255,255,255));
		p.setBrush(QColor(60,60,60));
		p.drawRect(0, height()/2, width()-1, height()/2-1);

		p.setFont (QFont("Calibri",8));	
		p.setPen(QColor(200,200,200));
		p.drawText(0, height()/2, width()-1, 20, Qt::AlignCenter, QString::number(m_velLo)+QString(" - ")+QString::number(m_velHi));

		p.setFont (QFont("Calibri",18));	
		p.setPen(QColor(255,255,255));
		p.drawText(0, height()/2+16, width()-1, height()/2-16, Qt::AlignCenter, QString::number(m_numCells));

		float range = (height()/2-1) / m_totalCells;
		float h = range * (float)m_numCells;
		p.setPen(QColor(200,200,200));
		p.setBrush(QColor(200,200,200));
		p.drawRect(0,(height()/2-1)-h,width()-1,h);
	} else {
		p.setPen(QColor(100,100,100));
		p.setBrush(QColor(70,70,70));
		p.drawRect(0, height()/2, width()-1, height()/2-1);
	}
}