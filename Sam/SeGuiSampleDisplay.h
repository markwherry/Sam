#ifndef SEGUISAMPLEDISPLAY_H
#define SEGUISAMPLEDISPLAY_H

#include <QWidget>
//#include "ui_SeGuiSampleDisplay.h"

#include "SamCell.h"

class SeGuiSampleDisplay : public QWidget
{
	Q_OBJECT

public:
	SeGuiSampleDisplay(QWidget *parent = 0);
	~SeGuiSampleDisplay();

	void setCell(SamCell *cell);
	
	void setWaveformColor(QColor color) { m_colorWaveform = color; }
	void setBackgroundColor(QColor color) { m_colorBackground = color; }
	void setLineColor(QColor color) { m_colorLine = color; }

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SeGuiSampleDisplay ui;
	
	SamCell *m_cell;
	int m_samplesPerPixel;
	QColor m_colorWaveform;
	QColor m_colorBackground;
	QColor m_colorLine;
};

#endif // SEGUISAMPLEDISPLAY_H
