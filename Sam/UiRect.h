#ifndef UIRECT_H
#define UIRECT_H

#include <QWidget>
//#include "ui_UiRect.h"

class UiRect : public QWidget
{
	Q_OBJECT

public:
	UiRect(QWidget *parent = 0);
	~UiRect();

	void setPenColor(QColor color);
	void setBrushColor(QColor color);

protected:
	void paintEvent(QPaintEvent *event);

private:
	//Ui::UiRectClass ui;
	QColor m_penColor;
	QColor m_brushColor;
};

#endif // UIRECT_H
