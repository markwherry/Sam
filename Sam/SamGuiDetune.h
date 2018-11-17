#ifndef SAMGUIDETUNE_H
#define SAMGUIDETUNE_H

#include <QWidget>
//#include "ui_SamGuiDetune.h"

#include "SamTool.h"
#include "SamGuiSteps.h"
//#include "SamGuiCsToolParameter.h"

class SamGuiDetune : public QWidget
{
	Q_OBJECT

public:
	SamGuiDetune(QWidget *parent, SamTool *tool);
	~SamGuiDetune();

public slots:
	void updateParameter(int id, float value);
	void updateValue(int step, float value);

private:
	//Ui::SamGuiDetuneClass ui;
	SamTool *m_tool;
	SamGuiSteps *m_steps;
};

#endif // SAMGUIDETUNE_H
