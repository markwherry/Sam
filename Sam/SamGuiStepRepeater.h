#ifndef SAMGUISTEPREPEATER_H
#define SAMGUISTEPREPEATER_H

#include <QWidget>
//#include "ui_SamGuiStepRepeater.h"

#include "SamTool.h"
#include "SamGuiSteps.h"
#include "SamGuiCsToolParameter.h"

class SamGuiStepRepeater : public QWidget
{
	Q_OBJECT

public:
	SamGuiStepRepeater(QWidget *parent, SamTool *tool);
	~SamGuiStepRepeater();

public slots:
	void updateParameter(int id, float value);
	void updateValue(int step, float value);
	void updateMute(int step, bool mute);
	void updateAccent(int step, bool accent);


private:
	//Ui::SamGuiStepRepeaterClass ui;
	SamTool *m_tool;
	SamGuiSteps *m_steps;
	SamGuiCsToolParameter *m_numSteps;
};

#endif // SAMGUISTEPREPEATER_H
