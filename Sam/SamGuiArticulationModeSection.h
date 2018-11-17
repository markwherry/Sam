#ifndef SAMGUIARTICULATIONMODESECTION_H
#define SAMGUIARTICULATIONMODESECTION_H

#include "UiSection.h"
//#include "ui_SamGuiArticulationModeSection.h"

#include "UiStateButton.h"

class SamGuiArticulationModeSection : public UiSection
{
	Q_OBJECT

public:
	SamGuiArticulationModeSection(QWidget *parent = 0);
	~SamGuiArticulationModeSection();

public slots:
	void buttonClicked(int id);

private:
	//Ui::SamGuiArticulationModeSectionClass ui;
	UiStateButton *m_buttonEnable;
};

#endif // SAMGUIARTICULATIONMODESECTION_H
