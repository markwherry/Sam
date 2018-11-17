#include "SamGuiArticulationModeSection.h"

#include "Sam.h"

SamGuiArticulationModeSection::SamGuiArticulationModeSection(QWidget *parent)
	: UiSection(parent, "Articulation Mode")
{
	//ui.setupUi(this);

	setFixedHeight(50); 

	int y = 4;

	m_buttonEnable = new UiStateButton(this, 110, 2);
	m_buttonEnable->setDrawFill(false);
	m_buttonEnable->setStateImage(0, ":/Resources/keyboardarticulation16_2.png");
	m_buttonEnable->setStateImage(1, ":/Resources/keyboardarticulation16_2_on.png");
	m_buttonEnable->setFixedSize(16,16);
	m_buttonEnable->move(4, y+=24);
	m_buttonEnable->setText("Show / Hide");

	connect(m_buttonEnable, SIGNAL(clicked(int)), this, SLOT(buttonClicked(int)));
}

SamGuiArticulationModeSection::~SamGuiArticulationModeSection()
{

}

void SamGuiArticulationModeSection::buttonClicked(int id)
{
	samGui->mainWidget()->pianoController()->setAttenuationMode(!samGui->mainWidget()->pianoController()->isAttenuationMode());
	m_buttonEnable->setState((int)samGui->mainWidget()->pianoController()->isAttenuationMode());
}