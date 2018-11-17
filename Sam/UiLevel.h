#ifndef UILEVEL_H
#define UILEVEL_H

#include <QWidget>
//#include "ui_UiLevel.h"

class UiLevel : public QWidget
{
	Q_OBJECT

public:
	UiLevel(QWidget *parent, bool vertical = true);
	~UiLevel();

	void setValue(int value);
	void setColor(QColor color);

signals:
	void clicked(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::UiLevelClass ui;
	bool m_vertical;
	int m_min;
	int m_max;
	int m_value;
	QColor m_color;
};

#endif // UILEVEL_H
