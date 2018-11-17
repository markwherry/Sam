#include "SamGuiInsert.h"

SamGuiInsert::SamGuiInsert(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	setFixedSize(150, 40);

	int y = 0;

	m_buttonInsert = new UiStateButton(this, 3000, 2);
	m_buttonInsert->move(2,y+2);
	m_buttonInsert->setFixedSize(16, 16);
	//m_buttonInsert->setStateImage(0, ":/Resources/send16_3.png");
	//m_buttonInsert->setStateImage(1, ":/Resources/send16_3.png");
	m_buttonInsert->setStateFillColor(0,QColor(127, 64, 0)); //0, 75, 75    75, 38, 0
	m_buttonInsert->setStateFillColor(1,QColor(255, 127, 0)); //0, 255, 255
	m_buttonInsert->setStateLineColor(1,QColor(75, 38, 0));
	//m_buttonInsert(buttonSend, SIGNAL(clickedWithFlags(int, int)), this, SLOT(setSendActive(int, int)));
}

SamGuiInsert::~SamGuiInsert()
{

}
