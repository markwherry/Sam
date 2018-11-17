#include "SeGuiCellItem.h"

#include <QPainter>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiCellItem::SeGuiCellItem(QWidget *parent, SamCell *cell)
	: UiListItem(parent, 20)
	, m_cell(cell)
{
	//ui.setupUi(this);
	m_sampleDisplay = new SeGuiSampleDisplay(this);
	m_sampleDisplay->move(29,0);
	m_sampleDisplay->setFixedSize(width()-30, height());
	m_sampleDisplay->setBackgroundColor(QColor(0,0,0,0));
	m_sampleDisplay->setWaveformColor(QColor(200,200,200));
	m_sampleDisplay->setLineColor(QColor(200,200,200));
	m_sampleDisplay->setCell(cell);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiCellItem::~SeGuiCellItem()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiCellItem::setSelected(bool selected)
{
	//qDebug("SeGuiCellItem::setSelected()");

	m_cell->setSelected(selected, this, true);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiCellItem::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	if (isSelected()) {
		p.setPen (QColor(120,120,120));//,100)); // 225,205,54
		p.setBrush (QColor(120,120,200));//,120)); //225,205,54,120
		p.drawRect (0,0,width()-1, height()-1);
	} else {
		int a = 180;
		if(positionInList()%2==0) {
			a = 160;
		}
		p.setPen (QColor(a,a,a));
		p.setBrush (QColor(a,a,a));

		p.drawRect (0,0,width()-1, height()-1);

		p.setPen(QColor(60,60,60,100));
		p.drawLine (0,0,width()-1,0);
		p.drawLine (0,height()-1,0,0);
		p.setPen (QColor(120,120,120,100));
		p.drawLine (width()-1,0,width()-1, height()-1);
		p.drawLine (width()-1,height()-1,0,height()-1);
	
	}

	p.setPen(QColor(255,255,255));
	p.setFont(QFont("", 10));//, QFont::Bold));
	p.drawText(2,2,20,height()-3,Qt::AlignRight|Qt::AlignVCenter,QString::number(positionInList()+1));
	p.setPen(QColor(255,255,255,50));
	p.drawLine(27,1,27, height()-2);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiCellItem::resizeEvent(QResizeEvent *ev)
{
	//qDebug("SeGuiCellItem::resizeEvent()");

	m_sampleDisplay->setFixedSize(width()-30, height());
}