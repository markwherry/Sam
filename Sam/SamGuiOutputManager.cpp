#include "SamGuiOutputManager.h"

#include <QPainter>

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"

#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputManager::SamGuiOutputManager(QWidget *parent)
	: QWidget(parent)
	, m_sideBarRightWidth(170)
{
	//ui.setupUi(this);
	setWindowTitle("Outputs Manager");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	setFixedSize(800+m_sideBarRightWidth,400);

	QPalette p=palette();
    p.setColor(QPalette::Dark, QColor(45,45,45));
	p.setColor(QPalette::Foreground, QColor(150,150,150));
   
	m_areaHeader = new QScrollArea(this);
	m_areaHeader->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_areaHeader->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_areaHeader->setPalette(p);
	m_areaHeader->setBackgroundRole(QPalette::Dark);
	m_areaHeader->setForegroundRole(QPalette::Foreground);
	m_areaHeader->setFrameStyle(QFrame::Box | QFrame::Plain);

	m_outputHeader = new SamGuiOutputHeader(m_areaHeader->viewport());
	m_areaHeader->setWidget(m_outputHeader);

	m_areaList = new QScrollArea(this);
	m_areaList->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaList->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaList->setPalette(p);
	m_areaList->setBackgroundRole(QPalette::Dark);
	m_areaList->setForegroundRole(QPalette::Foreground);
	m_areaList->setFrameStyle(QFrame::Box | QFrame::Plain);

	m_outputList = new SamGuiOutputList(m_areaList->viewport());
	m_outputList->move(0,0);
	m_areaList->setWidget(m_outputList);

	m_areaEditor = new QScrollArea(this);
	m_areaEditor->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaEditor->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_areaEditor->setPalette(p);
	m_areaEditor->setBackgroundRole(QPalette::Dark);
	m_areaEditor->setForegroundRole(QPalette::Foreground);
	//m_areaEditor->setFrameShape (QFrame::NoFrame);
	m_areaEditor->setFrameStyle (QFrame::Box | QFrame::Plain);

	m_outputEditor = new SamGuiOutputEditor(m_areaEditor->viewport());
	m_outputEditor->move(0,0);
	m_areaEditor->setWidget(m_outputEditor);

	m_scrollH = new QScrollBar(Qt::Horizontal, this);
	m_scrollV = new QScrollBar(Qt::Vertical, this);

	connect(m_areaHeader->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doHScroll(int)));
	connect(m_areaEditor->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doHScroll(int)));
	connect(m_scrollH, SIGNAL(valueChanged(int)), this, SLOT(doHScroll(int)));

	connect(m_areaList->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doVScroll(int)));
	connect(m_areaEditor->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doVScroll(int)));
	connect(m_scrollV, SIGNAL(valueChanged(int)), this, SLOT(doVScroll(int)));

	// ----

	int x = width() - m_sideBarRightWidth + 5;
	int y = 9;
	m_button50 = new UiStateButton(this, -1, 2);
	m_button50->setDrawFill(false);
	m_button50->setStateImage(0, ":/Resources/sc_50_16.png");
	m_button50->setStateImage(1, ":/Resources/sc_50_16_on.png");
	m_button50->setFixedSize(16,16);
	m_button50->move(x, y); y+=24;
	m_button50->setText("Create 5.0 Output");
	m_button50->setDrawTint(false);
	connect(m_button50, SIGNAL(clicked()), samGui, SLOT(create50Output()));
	//y += 24;
	
	m_buttonQuad = new UiStateButton(this, -1, 2);
	m_buttonQuad->setDrawFill(false);
	m_buttonQuad->setStateImage(0, ":/Resources/sc_quad_16.png");
	m_buttonQuad->setStateImage(1, ":/Resources/sc_quad_16_on.png");
	m_buttonQuad->setFixedSize(16,16);
	m_buttonQuad->move(x, y); y+=24;
	m_buttonQuad->setText("Create Quad Output");
	m_buttonQuad->setDrawTint(false);
	connect(m_buttonQuad, SIGNAL(clicked()), samGui, SLOT(createQuadOutput()));

	m_buttonStereo = new UiStateButton(this, -1, 2);
	m_buttonStereo->setDrawFill(false);
	m_buttonStereo->setStateImage(0, ":/Resources/sc_stereo_16.png");
	m_buttonStereo->setStateImage(1, ":/Resources/sc_stereo_16_on.png");
	m_buttonStereo->setFixedSize(16,16);
	m_buttonStereo->move(x, y); y+=24;
	m_buttonStereo->setText("Create Stereo Output");
	m_buttonStereo->setDrawTint(false);
	connect(m_buttonStereo, SIGNAL(clicked()), samGui, SLOT(createStereoOutput()));

	m_buttonMono = new UiStateButton(this, -1, 2);
	m_buttonMono->setDrawFill(false);
	m_buttonMono->setStateImage(0, ":/Resources/sc_mono_16.png");
	m_buttonMono->setStateImage(1, ":/Resources/sc_mono_16_on.png");
	m_buttonMono->setFixedSize(16,16);
	m_buttonMono->move(x, y); y+=32;
	m_buttonMono->setText("Create Mono Output");
	m_buttonMono->setDrawTint(false);
	connect(m_buttonMono, SIGNAL(clicked()), samGui, SLOT(createMonoOutput()));

	m_buttonRemove = new UiStateButton(this, -1, 2);
	m_buttonRemove->setDrawFill(false);
	m_buttonRemove->setStateImage(0, ":/Resources/remove16.png");
	m_buttonRemove->setStateImage(1, ":/Resources/remove16.png");
	m_buttonRemove->setFixedSize(16,16);
	m_buttonRemove->move(x, y); y+=24;
	m_buttonRemove->setText("Remove Selected Output");
	m_buttonRemove->setDrawTint(false);
	connect(m_buttonRemove, SIGNAL(clicked()), this, SLOT(removeOutput()));


	//buttonAddOutput = new EButton("Add Output", this);
	//buttonAddOutput->setFixedSize(100,25);
	//buttonAddOutput->move(5,5);
	//connect(buttonAddOutput, SIGNAL(clicked()), this, SLOT(addOutput()));

	samEngine->registerListener(this, SamEngine::ContextProject);

	connect(this, SIGNAL(audioOutputsUpdated()), this, SLOT(updateOutputs()));

	updateVScrollBarRange();
	updateHScrollBarRange();

	connect(m_outputEditor, SIGNAL(highlighted(int, int)), this, SLOT(updateHighlight(int, int)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputManager::~SamGuiOutputManager()
{
	samEngine->unregisterListener(this, SamEngine::ContextProject);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::onState(AStateEvent *ev)
{
	//qDebug("SamGuiSelectAudioBus::onState()");

	if(ev->context() == SamEngine::ContextProject) {
		//qDebug("SamEngine::ContextProject");
		switch(ev->item()) {
			case SamProject::ItemAudioHardwareBusAdded:
			case SamProject::ItemAudioHardwareBusRemoved:
				//qDebug("SamProject::ItemAudioHardwareBusAdded:");
				emit audioOutputsUpdated();
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::updateOutputs(void)
{
	m_outputList->updateOutputs();
	m_outputEditor->updateOutputs();

	updateVScrollBarRange();
	updateHScrollBarRange();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::updateHighlight(int bus, int output)
{
	//aLog("SamGuiOutputManager::updateHighlight(%d, %d)", bus, output);

	m_outputHeader->setHighlight(output);
	m_outputList->setHighlight(bus);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::removeOutput(void)
{
	// samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(m_audioBus, 0, this));
	// samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(m_audioBus, samEngine->project()->audioHardwareBus(i)->id(), this));

	int busId = samEngine->project()->audioHardwareBus(m_outputList->selectedBus())->id();

	for(int i=0; i<samEngine->project()->numChannels(); i++) {
		
		// Unassign the output from any Channels that might have been using it... (including any sends)
		SamChannel *channel = samEngine->project()->channel(i);
		
		for(int j=0; j<channel->numAudioOutputs(); j++) {
		
			SamAudioBus *bus = channel->audioOutputBus(j);
		
			if(bus->audioOutputBusId() == busId) {
				samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(bus, 0, this));
			}

			for(int k=0; k<bus->numAudioSends(); k++) {
				SamAudioSend *send = bus->audioSend(k);
				if(send->audioBusId() == busId) {
					samEngine->executeRealTimeCommand(new RtcSetSendActive(send, false));
					samEngine->executeRealTimeCommand(new RtcSetSendOutput(bus, k, 0));
				}
			}
		}

		// Now, remove the output
		samEngine->executeRealTimeCommand(new RtcRemoveAudioHardwareBus(samEngine->project()->audioHardwareBus(m_outputList->selectedBus())));

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::updateHScrollBarRange(void)
{
	//qDebug("UiToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	m_scrollH->setMinimum(m_areaEditor->horizontalScrollBar()->minimum());
	m_scrollH->setMaximum(m_areaEditor->horizontalScrollBar()->maximum());
	m_scrollH->setSingleStep(m_areaEditor->horizontalScrollBar()->singleStep());
	m_scrollH->setPageStep(m_areaEditor->horizontalScrollBar()->pageStep());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiOutputManager::updateHScrollBarValue(int value)
{
	//qDebug("UiToolPanelContainer::updateScrollBarValue(%d)");
	m_scrollH->setValue(value);
	
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::doHScroll(int value)
{
	m_areaHeader->horizontalScrollBar()->setValue(value);
	m_areaEditor->horizontalScrollBar()->setValue(value);
	m_scrollH->setValue(value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::updateVScrollBarRange(void)
{
	//qDebug("UiToolPanelContainer::updateScrollBarRange(%d, %d)", min, max);
	m_scrollV->setMinimum(m_areaEditor->verticalScrollBar()->minimum());
	m_scrollV->setMaximum(m_areaEditor->verticalScrollBar()->maximum());
	m_scrollV->setSingleStep(m_areaEditor->verticalScrollBar()->singleStep());
	m_scrollV->setPageStep(m_areaEditor->verticalScrollBar()->pageStep());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
/*void SamGuiOutputManager::updateVScrollBarValue(int value)
{
	//qDebug("UiToolPanelContainer::updateScrollBarValue(%d)");
	m_scrollV->setValue(value);
	
}*/

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::doVScroll(int value)
{
	m_areaList->verticalScrollBar()->setValue(value);
	m_areaEditor->verticalScrollBar()->setValue(value);
	m_scrollV->setValue(value);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));

	//EStyle::paintBackground(&p,this);
	//p.setPen(QColor(70,70,70));
	//p.setBrush(QColor(70,70,70));
	//p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//
	p.setPen(QColor(0,0,0,130));
	p.setBrush(QColor(0,0,0,130));
	p.drawRect(0,0,width()-1,height()-1);

	 
	p.fillRect(width() - m_sideBarRightWidth - 1, 1, m_sideBarRightWidth, height()-2, QColor(60, 60, 60));
	p.setPen(QColor(75,75,75));
	p.drawLine(width() - m_sideBarRightWidth - 1, 1, width() - m_sideBarRightWidth - 1, height()-2);
	p.setPen(QColor(0,0,0));
	p.drawLine(width() - m_sideBarRightWidth - 1 -1, 1, width() - m_sideBarRightWidth - 1 -1, height()-2);

	p.setPen(QColor(75,75,75));
	p.drawLine(width() - m_sideBarRightWidth -1, m_buttonMono->y()+m_buttonMono->height() + 8, width(), m_buttonMono->y()+m_buttonMono->height() + 8);
	//p.setPen(QColor(0,0,0));
	//p.drawLine(width() - m_sideBarRightWidth -1, m_buttonMono->y()+m_buttonMono->height() + 8 +1, width(), m_buttonMono->y()+m_buttonMono->height() + 8 +1);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputManager::resizeEvent(QResizeEvent *ev)
{
	int margin = 10;

	m_areaHeader->move(margin+200, margin);
	m_areaHeader->setFixedSize(width()-200-(margin*2) - m_sideBarRightWidth - (margin*2),150);

	m_areaList->move(margin, margin+m_areaHeader->height());
	m_areaList->setFixedSize(200, height()-m_areaHeader->height()-(margin)-m_scrollV->height());
	
	m_scrollH->move(200+margin, margin+m_areaHeader->height()+m_areaList->height());
	m_scrollH->setFixedWidth(m_areaHeader->width());

	m_scrollV->move(margin+m_areaList->width()+m_areaHeader->width(),margin+m_areaHeader->height());
	m_scrollV->setFixedHeight(m_areaList->height());

	m_areaEditor->move(margin+m_areaList->width(),margin+m_areaHeader->height());
	m_areaEditor->setFixedSize(m_areaHeader->width(), m_areaList->height());
}