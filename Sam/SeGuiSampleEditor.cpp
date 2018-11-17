#include "SeGuiSampleEditor.h"

#include <qpainter.h>

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiSampleEditor::SeGuiSampleEditor(QWidget *parent)
	: QWidget(parent)
	, m_cell(0)
{
	//ui.setupUi(this);

	m_sampleDisplay = new SeGuiSampleDisplay(this);
	m_sampleDisplay->setFixedSize(800,150);
	m_sampleDisplay->move(0,0);

	samEngine->registerListener(this, SamEngine::ContextCell);

	connect(this, SIGNAL(cellSelected(SamCell*)), this, SLOT(setCell(SamCell*)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiSampleEditor::~SeGuiSampleEditor()
{
	samEngine->unregisterListener(this, SamEngine::ContextCell);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleEditor::onState(AStateEvent *ev)
{
	if(ev->context() == SamEngine::ContextCell) {
		switch(ev->item()) {
			case SamCell::ItemSelected:
				qDebug("SamCell::ItemSelected:");
				emit cellSelected((SamCell*)ev->updatedObject());
				break;
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleEditor::setCell(SamCell *cell)
{
	qDebug("SeGuiSampleEditor::setCell()");

	m_cell = cell;

	m_sampleDisplay->setCell(cell);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleEditor::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	//p.fillRect(0,0,width(),height(),QColor(255,255,255));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiSampleEditor::resizeEvent(QResizeEvent *ev)
{
	m_sampleDisplay->setFixedSize(width(), height());
}