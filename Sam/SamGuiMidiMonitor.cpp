#include "SamGuiMidiMonitor.h"
//#include "EStyle.h"

#include <QPainter>
#include <QApplication>

#include "SamEngine.h"

SamGuiMidiMonitor::Properties::Properties()
{
	reset();
}

void SamGuiMidiMonitor::Properties::reset(void)
{
	scrollToNewEvent = false;
}

SamGuiMidiMonitor::SamGuiMidiMonitor(QWidget *parent)
    : QWidget(parent)
{
	//ui.setupUi(this);

	setWindowTitle("MIDI Input Monitor");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	setMinimumHeight(200);

	scrollArea = new QScrollArea (this);
	scrollArea->move (0,35);
	scrollArea->setFrameShape (QFrame::NoFrame);
	scrollArea->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	scrollArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);

	QPalette p=palette();
    p.setColor(QPalette::Dark, QColor (180,180,180));
    scrollArea->setPalette(p);
	scrollArea->setBackgroundRole(QPalette::Dark);

	midiMonitorList = new SamGuiMidiMonitorList (scrollArea->viewport());
	midiMonitorList->move(0,0);
	scrollArea->setWidget (midiMonitorList);
	
	scrollArea->setFixedWidth(midiMonitorList->width());

	//scrollArea->setAutoFillBackground(true);

	scrollBar = new QScrollBar (Qt::Vertical, this);

	
	buttonClear = new EButton(this, "Clear Events");
	buttonClear->setFixedSize(100,25);
	buttonClear->move(5,5);

	connect(buttonClear, SIGNAL(clicked()), midiMonitorList, SLOT(clearList()));
	connect(buttonClear, SIGNAL(clicked()), this, SLOT(update()));
	

	connect (scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(doVerticalScroll(int)));
	connect (scrollBar, SIGNAL(valueChanged(int)), this, SLOT(doVerticalScroll(int)));


	menuFilter = new EClickMenu(this, "Filter");
	menuFilter->setFixedSize(55,25);
	menuFilter->move(115,5);

	menuTools = new EClickMenu(this, "Tools");
	menuTools->setFixedSize(55,25);
	menuTools->move(180,5);

	actionFilterNoteOn = new QAction("Note On", this);
	actionFilterNoteOff = new QAction("Note Off", this);
	actionFilterAftertouch = new QAction("Aftertouch", this);
	actionFilterController = new QAction("Controller", this);
	actionFilterProgramChange = new QAction("Program Change", this);
	actionFilterChannelPressure = new QAction("Channel Pressure", this);
	actionFilterPitchWheel = new QAction("Pitch Wheel", this);
	actionFilterSysEx = new QAction("System Exclusive", this);
	actionFilterShowAll = new QAction("Show All", this);
	actionFilterAll = new QAction("Filter All", this);

	actionFilterNoteOn->setCheckable(true);
	actionFilterNoteOff->setCheckable(true);
	actionFilterAftertouch->setCheckable(true);
	actionFilterController->setCheckable(true);
	actionFilterProgramChange->setCheckable(true);
	actionFilterChannelPressure->setCheckable(true);
	actionFilterPitchWheel->setCheckable(true);
	actionFilterSysEx->setCheckable(true);

	menuFilter->addAction(actionFilterNoteOn);
	menuFilter->addAction(actionFilterNoteOff);
	menuFilter->addAction(actionFilterAftertouch);
	menuFilter->addAction(actionFilterController);
	menuFilter->addAction(actionFilterProgramChange);
	menuFilter->addAction(actionFilterChannelPressure);
	menuFilter->addAction(actionFilterPitchWheel);
	menuFilter->addAction(actionFilterSysEx);
	menuFilter->addSeparator();
	menuFilter->addAction(actionFilterShowAll);
	menuFilter->addAction(actionFilterAll);

	connect(actionFilterAll, SIGNAL(triggered()), this, SLOT(filterAll()));
	connect(actionFilterShowAll,SIGNAL(triggered()), this, SLOT(filterShowAll()));

	actionScrollToNewEvent = new QAction("Scroll To New Events", this);
	actionShowNoteControllerNumbers = new QAction("Show MIDI Numbers for Notes and Controllers", this);
	actionDeleteFilteredEvents = new QAction("Delete Filtered Events", this);
	actionSaveEventsToLogFile = new QAction("Save Events To Log File...", this);
	actionScrollToNewEvent->setCheckable(true);
	actionScrollToNewEvent->setChecked(true);
	actionShowNoteControllerNumbers->setCheckable(true);
	menuTools->addAction(actionScrollToNewEvent);
	menuTools->addAction(actionShowNoteControllerNumbers);
	menuTools->addSeparator();
	menuTools->addAction(actionDeleteFilteredEvents);
	//menuTools->addAction(actionSaveEventsToLogFile);

	qDebug("here!");
	connect(actionShowNoteControllerNumbers, SIGNAL(triggered(bool)), midiMonitorList, SLOT(showNumbers(bool)));
	connect(actionDeleteFilteredEvents, SIGNAL(triggered()), midiMonitorList, SLOT(deleteFilteredEvents()));
	connect(actionDeleteFilteredEvents,SIGNAL(triggered()), this, SLOT(filterShowAll()));
	connect(actionDeleteFilteredEvents,SIGNAL(triggered()), this, SLOT(updateFilterMenuButton()));

	scrollBarUpdateFlag = false;

	connect(menuFilter, SIGNAL(menuClosed()), this, SLOT(updateFilterMenuButton()));

	samEngine->registerListener(this, SamEngine::ContextMidiInput);

	connect(this, SIGNAL(midiReceived(CsMidiMessage)), this, SLOT(midiEvent(CsMidiMessage)));
	
}

SamGuiMidiMonitor::~SamGuiMidiMonitor()
{
	samEngine->unregisterListener(this, SamEngine::ContextMidiInput);

	//qDebug("EMidiMonitorView::~EMidiMonitorView()");
	delete scrollArea; // Also calls delete midiMonitorList;
	delete scrollBar;
	delete buttonClear;
}

void SamGuiMidiMonitor::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannel::onState()");

	if(ev->context() == SamEngine::ContextMidiInput) {
		CsMidiMessage msg;
		msg.buildFromWinMme(ev->item());
		emit midiReceived(msg);
	}
}

void SamGuiMidiMonitor::midiEvent(CsMidiMessage midiMessage)
{
	if(isVisible()){
		bool filtered = false;

		if (midiMessage.data[0]>=0x80 && midiMessage.data[0]<=0x8F && actionFilterNoteOff->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0x90 && midiMessage.data[0]<=0x9F && actionFilterNoteOn->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xA0 && midiMessage.data[0]<=0xAF && actionFilterAftertouch->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xB0 && midiMessage.data[0]<=0xBF && actionFilterController->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xC0 && midiMessage.data[0]<=0xCF && actionFilterProgramChange->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xD0 && midiMessage.data[0]<=0xDF && actionFilterChannelPressure->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xE0 && midiMessage.data[0]<=0xEF && actionFilterPitchWheel->isChecked()) {
			filtered = true;
		} else if (midiMessage.data[0]>=0xF0 && midiMessage.data[0]<=0xFF && actionFilterSysEx->isChecked()) {
			filtered = true;
		}
		
		bool showNumbers = false;
		if(actionShowNoteControllerNumbers->isChecked())
			showNumbers = true;
		midiMonitorList->addItem(midiMessage, filtered, showNumbers);

		scrollBarUpdateFlag = true;
		
		
		update();
	}
	
}

void SamGuiMidiMonitor::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);

	midiMonitorList->clearList();
}

void SamGuiMidiMonitor::resizeEvent(QResizeEvent *event)
{
	//qDebug("EMidiMonitorView::resizeEvent()");

	scrollArea->setFixedHeight (height()-35);
	setFixedWidth(scrollArea->width()+scrollBar->width());

	//midiMonitorList->resetHeight();
	//if (midiMonitorList->height()<scrollArea->height())
	//	midiMonitorList->setFixedHeight(scrollArea->height());

	scrollBar->move (width()-scrollBar->width(),35);
	scrollBar->setFixedHeight(height()-35);
	
	updateScrollBars();

	if(midiMonitorList->height()>scrollArea->height() && actionScrollToNewEvent->isChecked() && scrollBarUpdateFlag) {
		doVerticalScroll(scrollBar->maximum());
		scrollBarUpdateFlag = false;
	}

	//quickInfoButton->move(width()-30,5);
}

void SamGuiMidiMonitor::paintEvent(QPaintEvent *event)
{
	resizeEvent(0);

	QPainter p(this);

	//EStyle::paintBackground(&p,this);
	p.setPen(QColor(70,70,70));
	p.setBrush(QColor(70,70,70));
	p.drawRect(0,0,width()-1, height()-1);

	QPen pen(QColor(63,63,63));
	pen.setWidth(2);
	p.setPen(pen);
	for (int i=0; i<height(); i+=4) {
		p.drawLine(0,i,width()-1,i);
	}
	//

	p.setPen(QColor(0,0,0,70));
	p.setBrush(QColor(0,0,0,70));
	p.drawRect(0,0,width()-1,35);
}

void SamGuiMidiMonitor::doVerticalScroll (int value)
{
	scrollArea->verticalScrollBar()->setValue(value);
	scrollBar->setValue (value);
	//qDebug("value=%d,maximum=%d",value,scrollBar->maximum());
}

void SamGuiMidiMonitor::updateScrollBars (void)
{
	scrollBar->setMinimum (scrollArea->verticalScrollBar()->minimum());
	scrollBar->setMaximum (scrollArea->verticalScrollBar()->maximum());
	scrollBar->setSingleStep (scrollArea->verticalScrollBar()->singleStep());
	scrollBar->setPageStep (scrollArea->verticalScrollBar()->pageStep());
}

void SamGuiMidiMonitor::filterAll(void)
{
	actionFilterNoteOn->setChecked(true);
	actionFilterNoteOff->setChecked(true);
	actionFilterAftertouch->setChecked(true);
	actionFilterController->setChecked(true);
	actionFilterProgramChange->setChecked(true);
	actionFilterChannelPressure->setChecked(true);
	actionFilterPitchWheel->setChecked(true);
	actionFilterSysEx->setChecked(true);
}

void SamGuiMidiMonitor::filterShowAll(void)
{
	actionFilterNoteOn->setChecked(false);
	actionFilterNoteOff->setChecked(false);
	actionFilterAftertouch->setChecked(false);
	actionFilterController->setChecked(false);
	actionFilterProgramChange->setChecked(false);
	actionFilterChannelPressure->setChecked(false);
	actionFilterPitchWheel->setChecked(false);
	actionFilterSysEx->setChecked(false);
}

void SamGuiMidiMonitor::updateFilterMenuButton(void)
{
	int count = 0;

	if(actionFilterNoteOn->isChecked())
		count++;
	else if(actionFilterNoteOff->isChecked())
		count++;
	else if(actionFilterAftertouch->isChecked())
		count++;
	else if(actionFilterController->isChecked())
		count++;
	else if(actionFilterProgramChange->isChecked())
		count++;
	else if(actionFilterChannelPressure->isChecked())
		count++;
	else if(actionFilterPitchWheel->isChecked())
		count++;
	else if(actionFilterSysEx->isChecked())
		count++;

	if(count>0)
		menuFilter->setActive(true);
	else
		menuFilter->setActive(false);

	midiMonitorList->updateFilteredList(
		actionFilterNoteOn->isChecked(),
		actionFilterNoteOff->isChecked(),
		actionFilterAftertouch->isChecked(),
		actionFilterController->isChecked(),
		actionFilterProgramChange->isChecked(),
		actionFilterChannelPressure->isChecked(),
		actionFilterPitchWheel->isChecked(),
		actionFilterSysEx->isChecked()
		);

	scrollBarUpdateFlag = true;
}