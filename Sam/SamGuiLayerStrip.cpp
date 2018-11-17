#include "SamGuiLayerStrip.h"

#include <QPainter>

SamGuiLayerStrip::SamGuiLayerStrip(QWidget *parent, SamChannel *channel)
	: QWidget(parent)
	, m_channel(channel)
{
	//ui.setupUi(this);
}

SamGuiLayerStrip::~SamGuiLayerStrip()
{

}

void SamGuiLayerStrip::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(40,40,40));


	int ai = m_channel->currentArticulationIndex();
	SamArticulation *art = m_channel->version()->articulation(ai);

	int numLayers = art->numLayers();
	float lw = (float)width() - (float)numLayers - 3.0f;

	lw /= (float)numLayers;

	QColor lc = QColor(100,100,0);
	if(m_channel->layerSelectMode(m_channel->currentArticulationIndex()) == SamArticulation::SingleLayer || m_channel->layerSelectMode(m_channel->currentArticulationIndex()) == SamArticulation::AllLayers) {
		lc = QColor(0,100,0);
	}

	float x = 2.0f;
	for(int i=0; i<numLayers; i++) {
		QColor c = lc;
		if(m_channel->layerSelectMode(m_channel->currentArticulationIndex()) == SamArticulation::SingleLayer) {
			if(m_channel->currentLayerIndex(ai) != i) {
				c = QColor(80,80,80);
			}
		}
		p.fillRect(x, 2.0f, lw, (float)height() - 4.0f, c);
		p.setPen(QColor(255,255,255));
		p.drawText(x, 2.0f, lw, (float)height() - 4.0f, Qt::AlignCenter, art->layer(i)->name().toChar());
		x += (lw + 2.0f);
	}

	/*QColor color(255,255,255);
	QLinearGradient linearGradient(QPointF(0, 0), QPointF(0, height()));
	color.setAlpha(40+ 0);//90
	linearGradient.setColorAt(0, color);
	color.setAlpha(5+ 0);//50
	linearGradient.setColorAt(1, color);

	p.setBrush(linearGradient);
	p.setPen(QColor(0,0,0,0));
	
	p.drawRect(1,1,width()-2,(height()/2));//*/
}