#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "UiObject.h"
//#include "ui_UiButton.h"

class UiButton : public UiObject
{
	Q_OBJECT

public:
	UiButton(QWidget *parent = 0);
	~UiButton();

	void setDefault(bool def);

signals:
	void clicked(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paint(QPainter &p);

private:
	//Ui::UiButtonClass ui;
	bool m_default;
};

#endif // UIBUTTON_H
