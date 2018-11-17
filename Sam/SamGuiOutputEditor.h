#ifndef SAMGUIOUTPUTEDITOR_H
#define SAMGUIOUTPUTEDITOR_H

#include <QWidget>
//#include "ui_SamGuiOutputEditor.h"

//#include "SamGuiOutputEditorItem.h"

class SamGuiOutputEditor : public QWidget
{
	Q_OBJECT

public:
	SamGuiOutputEditor(QWidget *parent = 0);
	~SamGuiOutputEditor();

	void updateOutputs(void);
	void clear(void);

signals:
	void highlighted(int port, int channel);

protected:
	void leaveEvent(QEvent *ev);
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiOutputEditor ui;
	//QList<SamGuiOutputEditorItem*> m_items;

	int outputFromX(int x);
	int busFromY(int y);

	struct Pin {
		int output;
		int bus;
		int stream;
	};
	QList<Pin> m_pins;

	int m_clickedOnPin;
		

};

#endif // SAMGUIOUTPUTEDITOR_H
