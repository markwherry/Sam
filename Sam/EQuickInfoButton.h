#ifndef EQUICKINFOBUTTON_H
#define EQUICKINFOBUTTON_H

#include <QWidget>
//#include "ui_EQuickInfoButton.h"

#include <QPixmap>

class EQuickInfoButton : public QWidget
{
	Q_OBJECT

public:
	EQuickInfoButton(QWidget *parent = 0);
	~EQuickInfoButton();

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	//void timerEvent(QTimerEvent *event);

private:
	//Ui::EQuickInfoButtonClass ui;
	QPixmap *pixInfo;
	bool mouseOver;
	//int timerId;	
};

#endif // EQUICKINFOBUTTON_H