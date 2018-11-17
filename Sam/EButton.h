#ifndef EBUTTON_H
#define EBUTTON_H

#include <QWidget>
//#include "ui_EButton.h"

class EButton : public QWidget
{
	Q_OBJECT

public:
	EButton(QWidget *parent, QString title);
	~EButton();

signals:
	void clicked(void);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	//Ui::EButtonClass ui;
	QString m_title;
	bool mouseOver;
	
};

#endif // EBUTTON_H
