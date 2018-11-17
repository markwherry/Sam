#ifndef SAMGUIOUTPUTMANAGER_H
#define SAMGUIOUTPUTMANAGER_H

#include <QWidget>
//#include "ui_SamGuiOutputManager.h"
#include "AStateManager.h"
using namespace Atom;

#include <QScrollArea>
#include <QScrollBar>

#include "SamGuiOutputHeader.h"
#include "SamGuiOutputList.h"
#include "SamGuiOutputEditor.h"

#include "UiStateButton.h"

class SamGuiOutputManager : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SamGuiOutputManager(QWidget *parent = 0);
	~SamGuiOutputManager();

	void onState(AStateEvent *ev);

public slots:
	void updateOutputs(void);
	void updateHighlight(int bus, int output);

	void removeOutput(void);

	void updateHScrollBarRange(void);
	//void updateVScrollBarValue(int value);
	void doHScroll(int value);

	void updateVScrollBarRange(void);
	//void updateVScrollBarValue(int value);
	void doVScroll(int value);

signals:
	void audioOutputsUpdated(void);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiOutputManager ui;
	SamGuiOutputHeader *m_outputHeader;
	SamGuiOutputList   *m_outputList;
	SamGuiOutputEditor *m_outputEditor;

	QScrollArea *m_areaHeader;
	QScrollArea *m_areaList;
	QScrollArea *m_areaEditor;

	QScrollBar *m_scrollH;
	QScrollBar *m_scrollV;

	UiStateButton *m_button50;
	UiStateButton *m_buttonQuad;
	UiStateButton *m_buttonStereo;
	UiStateButton *m_buttonMono;
	UiStateButton *m_buttonRemove;

	int m_sideBarRightWidth;
};

#endif // SAMGUIOUTPUTMANAGER_H
