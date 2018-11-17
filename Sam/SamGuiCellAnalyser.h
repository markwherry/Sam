#ifndef SAMGUICELLANALYSER_H
#define SAMGUICELLANALYSER_H

#include <QWidget>
//#include "ui_SamGuiCellAnalyser.h"

#include "SamGuiCellAnalyserVelocity.h"

#include "SamChannel.h"

class SamGuiCellAnalyser : public QWidget
{
	Q_OBJECT

public:
	SamGuiCellAnalyser(QWidget *parent = 0);
	~SamGuiCellAnalyser();

	void setChannel(SamChannel *channel);

protected:
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);
	void hideEvent(QHideEvent *ev);
	void timerEvent(QTimerEvent *ev);

private:
	//Ui::SamGuiCellAnalyserClass ui;
	SamGuiCellAnalyserVelocity *m_velocities[20];
	SamChannel::Ptr m_channel;
	int m_timerId;
	int m_currentPitch;
	int m_totalCells;
};

#endif // SAMGUICELLANALYSER_H
