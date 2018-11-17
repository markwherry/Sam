#include "SeGuiSampleDisplay.h"

#include <qpainter.h>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiSampleDisplay::SeGuiSampleDisplay(QWidget *parent)
	: QWidget(parent)
	, m_cell(0)
	, m_colorWaveform(QColor(200,200,200))
	, m_colorBackground(QColor(90,90,90))
	, m_colorLine(QColor(160,160,160))
{
	//ui.setupUi(this);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiSampleDisplay::~SeGuiSampleDisplay()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleDisplay::setCell(SamCell *cell)
{
	m_cell = cell;

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleDisplay::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.setPen(QColor(0,0,0));
	//p.setBrush(QColor(255,255,255));
	//p.drawRect(0,0,width()-1, height()-1);
	p.fillRect(0,0,width(),height(),m_colorBackground);
	
	p.setPen(m_colorLine);
	p.drawLine(0, height()/2, width(), height()/2);
	
	p.setPen(m_colorWaveform);

	if(!m_cell) return;

	SamElement *el = m_cell->element(0);

	m_samplesPerPixel = el->lengthFrames() / width();

	//float *rmsBuffer = (float*)malloc(sizeof(float)*(m_samplesPerPixel+1));
	//int rbp = 0;
	int counter = 0;

	float rms = 0.0f;
	float max = 0.0f;
	float min = 0.0f;
	int x = 0;

	float buffer[8][2048];

	QPolygon polyMax;
	QPolygon polyMin;
	
	for(int i=0; i<el->numBlocks(); i++) {
		for(int k = 0; k<el->numStreams(); k++) {
			SamElementBlock *block = el->block(k, i);
			el->decompressBlock(block, buffer[k]);
		}

		 
		
		for(int j=0; j<2048; j++) {
			float value = 0.0f;
			for(int k=0; k<el->numStreams(); k++) {
				value += buffer[k][j];
			}
			//rms += (buffer[j] * buffer[j]);
			if(value > max) {
				max = value;
			}
			if(value < min) {
				min = value;
			}
			counter++;
			if(counter >= m_samplesPerPixel) {
				//rms = sqrt(rms / m_samplesPerPixel); //aLog("rms = %f", rms);
				//float y = (height()/2.0f) * rms;
				float y = (height()/2.0f) * max;
				p.drawLine(x, height()/2 + (int)y, x, height()/2 - (int)y);
				//qDebug("max = %f, min = %f", max, min);
				polyMax << QPoint(x, height()/2 + (int)y);
				y = (height()/2.0f) * min;
				polyMin << QPoint(x, height()/2 + (int)y);
				x++;
				counter = 0;
				rms = 0.0f;
				max = 0.0f;
				min = 0.0f;
			}
		}
	}

	//p.setBrush(QColor(200,200,200));
	//p.drawPolygon(polyMax);
	//p.drawPolygon(polyMin);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleDisplay::resizeEvent(QResizeEvent *ev)
{
	qDebug("SeGuiSampleDisplay::resizeEvent() %d, %d", width(), height());

	update();
}