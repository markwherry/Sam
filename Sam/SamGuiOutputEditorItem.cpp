#include "SamGuiOutputEditorItem.h"

#include <QPainter>

#include "SamEngine.h"

SamGuiOutputEditorItem::SamGuiOutputEditorItem(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	int numOutputs = samEngine->numOutputs();
	setFixedSize(numOutputs * 25, 25);
}

SamGuiOutputEditorItem::~SamGuiOutputEditorItem()
{

}

void SamGuiOutputEditorItem::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.fillRect(0, 0, width(), height(), QColor(0,0,0));
	//p.drawLine(0, height()-1, width(), height()-1);

	p.setPen(QColor(200,200,200));
	p.setBrush(QColor(200,200,200));
	p.drawRect(0,0,width()-1,height()-1);

	p.setPen(QColor(0,0,0));
	p.drawLine(0,height()-1,width()-1,height()-1);

	int numOutputs = samEngine->numOutputs();
	for(int i=0; i<numOutputs; i++) {
		p.drawLine(((i+1)*25)-1,0,((i+1)*25)-1,height()-1);
	}

	/*EOutput *output = e->currentProject()->output(m_outputListId);

	if(!output->isActive()) {
		p.setPen(QColor(255,0,0,50));
		p.setBrush(QColor(255,0,0,50));
		p.drawRect(0,0,width()-1,height()-1);
	}*/
}