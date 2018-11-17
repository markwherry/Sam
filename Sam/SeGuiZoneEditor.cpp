#include "SeGuiZoneEditor.h"

SeGuiZoneEditor::SeGuiZoneEditor(QWidget *parent)
	: QWidget(parent)
	, m_layer(0)
	, m_channel(0)
{
	//ui.setupUi(this);

	m_zoneDisplay = new SeGuiZoneDisplay(this);
	m_pianoKeyboard = new UiPianoKeyboard(this, 10, 50, 128, 0);
	m_velocityRuler = new SeGuiZoneVelocityRuler(0);

	hScrollBar = new QScrollBar(Qt::Horizontal, this);
	vScrollBar = new QScrollBar(Qt::Vertical, this);

	


	scrollPianoKeyboard = new QScrollArea(this);
	scrollPianoKeyboard->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollPianoKeyboard->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollPianoKeyboard->setFrameShape(QFrame::NoFrame);
	scrollPianoKeyboard->setWidget(m_pianoKeyboard);
	scrollPianoKeyboard->setFixedHeight(m_pianoKeyboard->height());
	scrollPianoKeyboard->move(40, 0);

	scrollZoneDisplay = new QScrollArea(this);
	scrollZoneDisplay->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollZoneDisplay->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollZoneDisplay->setFrameShape(QFrame::NoFrame);
	scrollZoneDisplay->setWidget(m_zoneDisplay);
	scrollZoneDisplay->move(40, scrollPianoKeyboard->height());

	scrollVelocityRuler = new QScrollArea(this);
	scrollVelocityRuler->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollVelocityRuler->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollVelocityRuler->setFrameShape(QFrame::NoFrame);
	scrollVelocityRuler->setWidget(m_velocityRuler);
	scrollVelocityRuler->setFixedHeight(m_velocityRuler->height());
	scrollVelocityRuler->move(0, scrollPianoKeyboard->height());

	connect(vScrollBar, SIGNAL(valueChanged(int)), scrollZoneDisplay->verticalScrollBar(), SLOT(setValue(int)));
	connect(hScrollBar, SIGNAL(valueChanged(int)), this, SLOT(setHorizontalScroll(int)));
	connect(scrollZoneDisplay->verticalScrollBar(), SIGNAL(valueChanged(int)), vScrollBar, SLOT(setValue(int)));

	//connect(m_zoneDisplay, SIGNAL(sizeChanged()), this, SLOT(updateScrollBars()));

	connect(m_pianoKeyboard, SIGNAL(pitchPressed(int, int, int)), this, SLOT(selectFromPianoKeyboard(int, int, int)));

	connect(m_velocityRuler, SIGNAL(velocitySplitDisplayChanged(int)), m_zoneDisplay, SLOT(setVelocitySplitDisplay(int)));
	connect(m_zoneDisplay, SIGNAL(mouseOverVelocity(int)), m_velocityRuler, SLOT(setMouseVelocity(int)));
}

SeGuiZoneEditor::~SeGuiZoneEditor()
{
	
}

void SeGuiZoneEditor::setLayer(SamLayer *layer, SamChannel *channel)
{
	qDebug("SeGuiZoneEditor::setLayer()");

	m_layer = layer;
	m_channel = channel;
	m_zoneDisplay->setLayer(layer, channel);
	m_velocityRuler->setChannel(channel);
}

void SeGuiZoneEditor::updateZoneDisplay(void)
{
	m_zoneDisplay->update();
}

void SeGuiZoneEditor::updateScrollBars(void)
{
	hScrollBar->setMinimum(scrollZoneDisplay->horizontalScrollBar()->minimum());
	hScrollBar->setMaximum(scrollZoneDisplay->horizontalScrollBar()->maximum());
	hScrollBar->setSingleStep(scrollZoneDisplay->horizontalScrollBar()->singleStep());
	hScrollBar->setPageStep(scrollZoneDisplay->horizontalScrollBar()->pageStep());

	vScrollBar->setMinimum(scrollZoneDisplay->verticalScrollBar()->minimum());
	vScrollBar->setMaximum(scrollZoneDisplay->verticalScrollBar()->maximum());
	vScrollBar->setSingleStep(scrollZoneDisplay->verticalScrollBar()->singleStep());
	vScrollBar->setPageStep(scrollZoneDisplay->verticalScrollBar()->pageStep());
}

void SeGuiZoneEditor::setHorizontalScroll(int value)
{
	scrollPianoKeyboard->horizontalScrollBar()->setValue(value);
	scrollZoneDisplay->horizontalScrollBar()->setValue(value);
}

void SeGuiZoneEditor::selectFromPianoKeyboard(int pitch, int velocity, int modifiers)
{
	//aLog("SeGuiZoneEditor::selectFromPianoKeyboard()");

	if(m_layer) {
		int alias = m_layer->aliasNote(pitch);
		if(alias > -1) {
			pitch = alias;
		}
	}

	if(modifiers == 0) {
		m_zoneDisplay->selectZonesByPitch(pitch, true);
	} else if(modifiers == Qt::ControlModifier) {
		m_zoneDisplay->selectZonesByPitch(pitch, false);
	}
}

void SeGuiZoneEditor::resizeEvent(QResizeEvent *ev)
{
	int wr = 40;

	scrollPianoKeyboard->setFixedWidth(width()-vScrollBar->width()-wr);
	
	scrollZoneDisplay->setFixedSize(width()-vScrollBar->width() - wr, height()-scrollPianoKeyboard->height()-hScrollBar->height());
	scrollVelocityRuler->setFixedHeight(height()-scrollPianoKeyboard->height()-hScrollBar->height());

	float vh = (float)((float)scrollZoneDisplay->height()-6.0f)/127.0f;
	//qDebug("============================================");
	//qDebug("%f = %f/%f", vh, (float)m_zoneDisplay->height(), 127.0f);
	//qDebug("============================================");
	m_zoneDisplay->setVelocityHeight(vh);
	m_velocityRuler->setVelocityHeight(vh);

	hScrollBar->move(0,height()-hScrollBar->height());
	hScrollBar->setFixedWidth(width()-vScrollBar->width());

	vScrollBar->move(width()-vScrollBar->width(),scrollPianoKeyboard->height());
	vScrollBar->setFixedHeight(height()-hScrollBar->height()-scrollPianoKeyboard->height());

	updateScrollBars();
	
}