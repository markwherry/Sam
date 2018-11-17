#include "SamGuiToolEditorToolbar.h"

#include <QPainter>

SamGuiToolEditorToolbar::SamGuiToolEditorToolbar(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	setFixedHeight(41);

	m_presetMenu = new UiSelectMenu(this, 150, 20);
	m_presetMenu->move(60,10);
	m_presetMenu->addItem("Default Settings");
	m_presetMenu->setTextBold(false);

	m_buttonAddPreset = new UiStateButton(this, -1, 1);
	m_buttonAddPreset->setFixedSize(20,20);
	m_buttonAddPreset->move(10,10);
	m_buttonAddPreset->setStateImage(0, ":/Resources/add16.png");
	m_buttonAddPreset->setDrawLine(false);
	m_buttonAddPreset->setDrawFill(false);
	m_buttonAddPreset->setDrawTint(false);

	m_buttonRemovePreset = new UiStateButton(this, -1, 1);
	m_buttonRemovePreset->setFixedSize(20,20);
	m_buttonRemovePreset->move(35,10);
	m_buttonRemovePreset->setStateImage(0, ":/Resources/remove16.png");
	m_buttonRemovePreset->setDrawLine(false);
	m_buttonRemovePreset->setDrawFill(false);
	m_buttonRemovePreset->setDrawTint(false);
}

SamGuiToolEditorToolbar::~SamGuiToolEditorToolbar()
{

}

void SamGuiToolEditorToolbar::paintEvent(QPaintEvent *event)
{
	QPainter p(this);

	p.setPen(QColor(72,72,72));
	p.setBrush(QColor(72,72,72));
	p.drawRect(0,0,width()-1,height()-1);

	//EStyle::paintBackground(&p,this);
	/*p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//*/

	p.setPen(QColor(50,50,50));
	p.drawLine(0,height()-1, width()-1, height()-1);

	/*p.setPen(QColor(0,0,0,50));
	p.drawLine(80,0,80,height()-2);

	p.setPen(QColor(255,255,255,25));
	p.drawLine(81,0,81,height()-2);*/
}