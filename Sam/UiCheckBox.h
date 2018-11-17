#ifndef UICHECKBOX_H
#define UICHECKBOX_H

#include "UiObject.h"
//#include "ui_UiCheckBox.h"

class UiCheckBox : public UiObject
{
	Q_OBJECT

public:
	UiCheckBox(QWidget *parent, int w = 14, int h = 14);
	~UiCheckBox();

	bool isChecked(void) { return m_checked; }
	void setChecked(bool checked);

	void setText(QString text);

signals:
	void changed(bool checked);

protected:
	void mousePressEvent(QMouseEvent *event);
	void paint(QPainter &p);

private:
	//Ui::UiCheckBoxClass ui;
	bool m_checked;
};

#endif // UICHECKBOX_H
