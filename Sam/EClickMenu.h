#ifndef ECLICKMENU_H
#define ECLICKMENU_H

#include <QWidget>
//#include "ui_EClickMenu.h"

#include <QWidget>
#include <QMenu>
#include <QPixmap>

class EClickMenu : public QWidget
{
	Q_OBJECT

public:
	EClickMenu(QWidget *parent, QString title);
	~EClickMenu();

	void addAction(QAction *action);
	void addSeparator(void);
	void setActive(bool active);

signals:
	void menuClosed(void);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	//Ui::EClickMenuClass ui;
	QMenu *menu;
	QString m_title;
	bool m_active;
	bool mouseOver;
	int timerId;

	QPixmap *pixMenuArrowDown;
};

#endif // ECLICKMENU_H