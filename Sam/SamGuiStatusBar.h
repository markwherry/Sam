#ifndef SAMGUISTATUSBAR_H
#define SAMGUISTATUSBAR_H

#include <QWidget>
//#include "ui_SamGuiStatusBar.h"

#include "UiLevel.h"
#include "UiValue.h"

class SamGuiStatusBar : public QWidget
{
	Q_OBJECT

public:
	SamGuiStatusBar(QWidget *parent = 0);
	~SamGuiStatusBar();

public slots:
	void reset(void);
	void setTempo(float bpm);

protected:
	void mouseDoubleClickEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void timerEvent(QTimerEvent *ev);

private:
	//Ui::SamGuiStatusBarClass ui;
	UiLevel *m_systemLevel;
	UiLevel *m_peakLevel;
	UiLevel *m_memoryLevel;
	int m_usage;
	int m_peak;
	int m_memory;
	unsigned long long m_storage;
	UiValue *m_bpmValue;
};

#endif // SAMGUISTATUSBAR_H
