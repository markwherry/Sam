#ifndef UISCROLLBAR_H
#define UISCROLLBAR_H

#include <QWidget>
//#include "ui_ScrollBar.h"

class UiScrollBar : public QWidget
{
	Q_OBJECT

public:
	UiScrollBar(QWidget *parent = 0);
	~UiScrollBar();

	void setMinimum(int min);
	void setMaximum(int max);
	void setValue(int value);
	void setPageStep(int pageStep);

signals:
	void valueChanged(int value);

protected:
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::ScrollBarClass ui;
	bool m_mouseOver;
	int m_min;
	int m_max;
	int m_value;
	int m_pageStep;
	int downY;
	int m_downScreenValue;
	int m_screenValue;
	int m_barLength;
};

#endif // SCROLLBAR_H
