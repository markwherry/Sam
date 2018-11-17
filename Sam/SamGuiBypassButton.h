#ifndef SAMGUIBYPASSBUTTON_H
#define SAMGUIBYPASSBUTTON_H

#include <QWidget>
//#include "ui_SamGuiBypassButton.h"

#include <QMenu>

#include "SamToolInsert.h"

class SamGuiBypassButton : public QWidget
{
	Q_OBJECT

public:
	SamGuiBypassButton(QWidget *parent = 0);
	~SamGuiBypassButton();

	void setToolInsert(SamToolInsert *insert);

public slots:
	void learn(void);
	void unlearn(void);

signals:
	void pressed(void);

protected:
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiBypassButtonClass ui;
	SamToolInsert *m_insert;

	QMenu   *m_menuLearn;
	QAction *m_actionLearn;
	QAction *m_actionUnlearn;
};

#endif // SAMGUIBYPASSBUTTON_H
