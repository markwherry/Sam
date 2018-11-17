#include "SamGuiOutputList.h"

#include <QPainter>
#include <QMouseEvent>

#include "SamEngine.h"
#include "SamProject.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputList::SamGuiOutputList(QWidget *parent)
	: QWidget(parent)
	, m_selectedBus(0)
{
	//ui.setupUi(this);

	updateOutputs();

	for(int i=0; i<128; i++) {
		m_highlights[i] = false;
	}

	m_lineEdit = new QLineEdit(this);
	m_lineEdit->setFixedSize(200, 25);
	m_lineEdit->hide();
	connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(acceptEdit()));
	previousKeyboardGrabber = 0;
	previousFocus = 0;
	previousMouseGrabber = 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiOutputList::~SamGuiOutputList()
{
	
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::updateOutputs(void)
{
	setFixedSize(200, samEngine->project()->numAudioHardwareBuses() * 25);

	m_selectedBus = 0;

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::setHighlight(int input)
{
	for(int i=0; i<128; i++) {
		m_highlights[i] = false;
	}

	if(input >= 0 && input < 128) {
		m_highlights[input] = true;
	}

	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::acceptEdit(void)
{
	m_lineEdit->releaseKeyboard();

	if(previousKeyboardGrabber) {
		//qDebug("Grabbing old keyboard");
		previousKeyboardGrabber->grabKeyboard();
	}

	if(previousMouseGrabber) {
		previousMouseGrabber->grabMouse();
	}

	if(previousFocus) {
		previousFocus->setFocus();
	}
	
	//m_text = m_lineEdit->text();
	//emit textChanged(m_text);
	QByteArray baName = m_lineEdit->text().toLatin1();
	samEngine->project()->audioHardwareBus(m_selectedBus)->setName(baName.data());
	
	m_lineEdit->hide();
	
	update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::mouseDoubleClickEvent(QMouseEvent *ev)
{
	int bus = busFromY(ev->y());
	if(bus < 0) return;

	m_lineEdit->move(0, bus * 25);

	m_lineEdit->setText(samEngine->project()->audioHardwareBus(bus)->name().toChar());
	m_lineEdit->selectAll();
	m_lineEdit->show();

	previousFocus = QWidget::focusWidget();
	m_lineEdit->setFocus();

	previousMouseGrabber = QWidget::mouseGrabber();
	
	previousKeyboardGrabber = QWidget::keyboardGrabber();
	//qDebug("prviousKeyboardGrabber = %d", previousKeyboardGrabber);

	m_lineEdit->grabKeyboard();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::mousePressEvent(QMouseEvent *ev)
{
	int numBuses = samEngine->project()->numAudioHardwareBuses();

	for(int i=0; i<numBuses; i++) {
		if(ev->y() >= i*25 && ev->y()<(i+1)*25) {
			m_selectedBus = i;
			update();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiOutputList::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	int numOutputs = samEngine->project()->numAudioHardwareBuses();

	p.setFont(QFont("Calibri",8,QFont::Bold));

	QColor cols[2] = { QColor(90,90,90), QColor(75,75,75) };

	int y = 0;
	for(int i=0; i<numOutputs; i++) {
		if(m_selectedBus == i) {
			p.fillRect(0, y, width()-2, 25, (QColor(90,90,150))); //60,60,100
		} else {
			p.fillRect(0, y, width()-1, 25, cols[i%2]);
		}

		p.setPen(QColor(255,255,255));
		p.drawText(5, y, width()-10, 25, Qt::AlignLeft|Qt::AlignVCenter, samEngine->project()->audioHardwareBus(i)->name().toChar());
		
		y += 25;

		p.setPen(QColor(150,150,150));
		p.drawLine(0, y-1, width(), y-1);

		if(m_highlights[i]) {
			p.fillRect(0, y-25, width()-2, 24, (QColor(255,255,255,100)));
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
int SamGuiOutputList::busFromY(int y)
{
	int numBuses = samEngine->project()->numAudioHardwareBuses();

	for(int i=0; i<numBuses; i++) {
		if(y >= i*25 && y<(i+1)*25) {
			return i;
		}
	}

	return -1;
}