#ifndef SAMGUIOUTPUTLIST_H
#define SAMGUIOUTPUTLIST_H

#include <QWidget>
//#include "ui_SamGuiOutputList.h"

#include <QLineEdit>

class SamGuiOutputList : public QWidget
{
	Q_OBJECT

public:
	SamGuiOutputList(QWidget *parent = 0);
	~SamGuiOutputList();

	void updateOutputs(void);

	void setHighlight(int input);

	int selectedBus(void) { return m_selectedBus; }

public slots:
	void acceptEdit(void);

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiOutputList ui;
	int busFromY(int y);

	bool m_highlights[128];

	int m_selectedBus;

	QLineEdit *m_lineEdit;
	QWidget *previousKeyboardGrabber;
	QWidget *previousFocus;
	QWidget *previousMouseGrabber;
};

#endif // SAMGUIOUTPUTLIST_H
