#ifndef SAMGUICELLANALYSERVELOCITY_H
#define SAMGUICELLANALYSERVELOCITY_H

#include <QWidget>
//#include "ui_SamGuiCellAnalyserVelocity.h"

class SamGuiCellAnalyserVelocity : public QWidget
{
	Q_OBJECT

public:
	SamGuiCellAnalyserVelocity(QWidget *parent = 0);
	~SamGuiCellAnalyserVelocity();

	void setVelocityRange(int lo, int hi) { m_velLo = lo; m_velHi = hi; }
	void setNumCells(int numCells, int totalCells) { m_numCells = numCells; m_totalCells = totalCells; } 

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiCellAnalyserVelocityClass ui;
	int m_velLo;
	int m_velHi;
	int m_numCells;
	int m_totalCells;
};

#endif // SAMGUICELLANALYSERVELOCITY_H
