#ifndef SEGUISAMPLEEDITOR_H
#define SEGUISAMPLEEDITOR_H

#include <QWidget>
//#include "ui_SeGuiSampleEditor.h"

#include "AStateManager.h"
using namespace Atom;

#include "SeGuiSampleDisplay.h"

class SeGuiSampleEditor : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SeGuiSampleEditor(QWidget *parent = 0);
	~SeGuiSampleEditor();

	void onState(AStateEvent *ev);

public slots:
	void setCell(SamCell *cell);

signals:
	void cellSelected(SamCell *cell);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SeGuiSampleEditor ui;

	SamCell *m_cell;

	SeGuiSampleDisplay *m_sampleDisplay;

};

#endif // SEGUISAMPLEEDITOR_H
