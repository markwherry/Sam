#ifndef UIPROGRESSBAR_H
#define UIPROGRESSBAR_H

#include <QWidget>
//#include "ui_UiProgressBar.h"

class UiProgressBar : public QWidget
{
	Q_OBJECT

public:
	UiProgressBar(QWidget *parent = 0);
	~UiProgressBar();

	void setValueNormalised(float valueNormalised);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::UiProgressBarClass ui;
	float m_valueNormalised;
};

#endif // UIPROGRESSBAR_H
