#include "SeGuiParameters.h"



#include "SamEngine.h"
#include "SamRealTimeCommands.h"
#include "CsCurve.h"

#include <QMessageBox>

#include "SamGuiEditor.h"
#include "SamParameterCommand.h"

SeGuiParameters::SeGuiParameters(QWidget *parent)
	: QWidget(parent)
	, m_articulation(0)
	, m_parameterId(-1)
{
	//ui.setupUi(this);

	m_listContainer = new UiListContainer(this);
	m_listContainer->list()->setMovable(false);

	m_parameterProperties = new SeGuiParameterProperties(this);
	m_parameterProperties->setFixedHeight(433);
	

	

	connect(m_listContainer->list(), SIGNAL(itemSelectedListPosition(int)), this, SLOT(setParameter(int)));

	connect(this, SIGNAL(parameterListUpdated()), this, SLOT(rebuild()));

	samEngine->registerListener(this, SamEngine::ContextArticulation);
}

SeGuiParameters::~SeGuiParameters()
{

}

void SeGuiParameters::onState(AStateEvent *ev)
{
	qDebug("SamGuiEditor::onState()");

	switch(ev->item()) {
		case SamArticulation::ItemParameterAdded:
			qDebug("SamArticulation::ItemParameterAdded:");
			emit parameterListUpdated();
			break;
		case SamArticulation::ItemParameterRemoved:
			qDebug("SamArticulation::ItemParameterRemoved:");
			emit parameterListUpdated();
			break;
	}
}

void SeGuiParameters::setArticulation(SamArticulation *articulation)
{
	m_articulation = articulation;

	rebuild();
}

void SeGuiParameters::rebuild(void)
{
	m_listContainer->list()->removeAllItems();

	if(!m_articulation) {
		return;
	}

	for(int i=0; i<m_articulation->numParameters(); i++) {
		//qDebug("%d - %s", i, qPrintable(m_version->output(i)->name()));
		SeGuiItemText *item = new SeGuiItemText(this, i, "Parameter "+QString::number(i+1), "");
		item->setText(m_articulation->parameter(i)->name().toChar());
		item->setIndent(false);
		item->setBackgroundColor(0, QColor(85,85,85));
		item->setBackgroundColor(1, QColor(80,80,80));
		
		m_listContainer->list()->addItem(item);
		item->show();
		connect(item, SIGNAL(textChanged(int, QString)), this, SLOT(setParameterName(int, QString)));
	}

	

	if(m_articulation->numParameters()>1) {
		m_listContainer->list()->setSelectable(true);
	} else {
		m_listContainer->list()->setSelectable(false);
		setParameter(0);
	}

	if(m_articulation->numParameters()<1) {
		m_parameterProperties->setEnabled(false);
	} else {
		m_parameterProperties->setEnabled(true);
	}

	UiListItem *item = m_listContainer->list()->itemFromPosition(0);
	if(item) {
		item->select();
	}
}

void SeGuiParameters::setParameterName(int id, QString name)
{
	qDebug("SeGuiParameters::setParameterName(%d, %s)", id, qPrintable(name));
	if(m_articulation) {
		if(id >= 0 && id < m_articulation->numParameters()) {
			QByteArray ba = name.toAscii();
			samEditor->commandManager()->execute(new SamParameterCommand(m_articulation->parameter(id), SamParameterCommand::Name, AString(ba.data())));
		}
	}
}

void SeGuiParameters::setParameter(int id)
{
	qDebug("SeGuiParameters::setParameter(%d)", id);
	m_parameterId = id;

	if(m_articulation) {
		if(id >= 0 && id < m_articulation->numParameters()) {
			m_parameterProperties->setParameter(m_articulation->parameter(id));
		}
	}
}

void SeGuiParameters::addParameter(void)
{
	if(m_articulation) {
		SamParameter *parameter = new SamParameter(m_articulation);
		samEngine->executeRealTimeCommand(new RtcAddParameter(m_articulation, parameter));
	}
}

void SeGuiParameters::removeParameter(void)
{
	if(m_parameterId > -1) {
		samEngine->executeRealTimeCommand(new RtcRemoveParameter(m_articulation, m_parameterId));
	}
}

void SeGuiParameters::createFilterCutOffParameter(void)
{
	qDebug("SeGuiParameters::createFilterCutOffParameter()");

	bool foundParameter = false;

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());

	for(int i=0; i<articulation->numParameters(); i++) {
		if(articulation->parameter(i)->name() == "Filter Cut-Off") {
			foundParameter = true;
			QMessageBox::warning(this, "Sam", "<b>The parameter you're trying to create already exists.</b><br><br>There's no need to create it again.","OK");
		}
	}

	if(!foundParameter) {
		qDebug("Creating parameter...");
		SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
		parameter->setName("Filter Cut-Off");
		parameter->setType(SamParameter::sptRange);
		parameter->setMidiType(SamParameter::spmtController);
		parameter->setMidiController(1);
		parameter->setSource(SamParameter::FilterCutOff);
		parameter->setSourceControl(0);
		parameter->setInverted(false);
		parameter->setCurveShape(CsCurve::Linear);
		parameter->setDefaultValue(0.0f);
		samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
	}
}

void SeGuiParameters::createMinimumVelocityParameter(void)
{
	qDebug("SeGuiParameters::createMinimumVelocityParameter()");

	bool foundParameter = false;

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());

	for(int i=0; i<articulation->numParameters(); i++) {
		if(articulation->parameter(i)->name() == "Minimum Velocity") {
			foundParameter = true;
			QMessageBox::warning(this, "Sam", "<b>The parameter you're trying to create already exists.</b><br><br>There's no need to create it again.","OK");
		}
	}

	if(!foundParameter) {
		qDebug("Creating parameter...");
		SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
		parameter->setName("Minimum Velocity");
		parameter->setType(SamParameter::sptRange);
		parameter->setMidiType(SamParameter::spmtController);
		parameter->setMidiController(1);
		parameter->setSource(SamParameter::MinimumVelocity);
		parameter->setSourceControl(0);
		parameter->setInverted(false);
		parameter->setCurveShape(CsCurve::Linear);
		parameter->setDefaultValue(0.0f);
		samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
	}
}

void SeGuiParameters::createSampleStartParameter(void)
{
	qDebug("SeGuiParameters::createSampleStartParameter()");

	bool foundParameter = false;

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());

	for(int i=0; i<articulation->numParameters(); i++) {
		if(articulation->parameter(i)->name() == "Sample Start") {
			foundParameter = true;
			QMessageBox::warning(this, "Sam", "<b>The parameter you're trying to create already exists.</b><br><br>There's no need to create it again.","OK");
		}
	}

	if(!foundParameter) {
		qDebug("Creating parameter...");
		SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
		parameter->setName("Sample Start");
		parameter->setType(SamParameter::sptRange);
		parameter->setMidiType(SamParameter::spmtController);
		parameter->setMidiController(2);
		parameter->setSource(SamParameter::SampleStart);
		parameter->setSourceControl(0);
		parameter->setInverted(false);
		parameter->setCurveShape(CsCurve::LinearSquared);
		parameter->setDefaultValue(0.0f);
		samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
	}
}

void SeGuiParameters::createAttackParameter(void)
{
	qDebug("Sam::createAttackParameter()");

	// Attack 73
	// Hold 75
	// Decay 76
	// Sustain 77
	// Release 72

	AString paramName[5] = { "Attack", "Hold", "Decay", "Sustain", "Release" };
	int paramId[5] = { SamParameter::EnvelopeAttack, SamParameter::EnvelopeHold, SamParameter::EnvelopeDecay, SamParameter::EnvelopeSustain, SamParameter::EnvelopeRelease };
	int paramCtrl[5] = { 73, 75, 76, 77, 72 };
	

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());

	for(int j=0; j<5; j++) {

		bool foundParameter = false;

		for(int i=0; i<articulation->numParameters(); i++) {
			if(articulation->parameter(i)->name()== paramName[j]) {
				foundParameter = true;
				QMessageBox::warning(this, "Sam", "<b>The parameter " + QString(paramName[j].toChar()) +" already exists.</b><br><br>There's no need to create it again.","OK");
			}
		}

		if(!foundParameter) {
			qDebug("Creating parameter...");
			SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
			parameter->setName(paramName[j]);
			parameter->setType(SamParameter::sptRange);
			parameter->setMidiType(SamParameter::spmtController);
			parameter->setMidiController(paramCtrl[j]);
			parameter->setSource(paramId[j]);
			parameter->setSourceControl(0);
			parameter->setInverted(false);
			parameter->setCurveShape(CsCurve::LinearSquared);
			parameter->setDefaultValue(1.0f);
			//parameter->curve()->setStart(0.5);
			samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
		}

	}
}

void SeGuiParameters::createReleaseEnvelopeParameters(void)
{
	qDebug("Sam::createReleaseEnvelopeParameters()");

	// RHold 78
	// RRelease 79

	AString paramName[2] = { "Release Hold", "Release Release" };
	int paramId[2] = { SamParameter::EnvelopeReleaseHold, SamParameter::EnvelopeReleaseRelease  };
	int paramCtrl[5] = { 78, 79 };
	

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());

	for(int j=0; j<2; j++) {

		bool foundParameter = false;

		for(int i=0; i<articulation->numParameters(); i++) {
			if(articulation->parameter(i)->name()== paramName[j]) {
				foundParameter = true;
				QMessageBox::warning(this, "Sam", "<b>The parameter " + QString(paramName[j].toChar()) +" already exists.</b><br><br>There's no need to create it again.","OK");
			}
		}

		if(!foundParameter) {
			qDebug("Creating parameter...");
			SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
			parameter->setName(paramName[j]);
			parameter->setType(SamParameter::sptRange);
			parameter->setMidiType(SamParameter::spmtController);
			parameter->setMidiController(paramCtrl[j]);
			parameter->setSource(paramId[j]);
			parameter->setSourceControl(0);
			parameter->setInverted(false);
			parameter->setCurveShape(CsCurve::LinearSquared);
			parameter->setDefaultValue(1.0f);
			//parameter->curve()->setStart(0.5);
			samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
		}

	}
}

void SeGuiParameters::createCellSampleStartAdjustParameter(void)
{
	qDebug("Sam::createCellSampleStartAdjustParameter()");

	bool foundParameter = false;

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());
	for(int i=0; i<articulation->numParameters(); i++) {
		if(articulation->parameter(i)->name() == "Cell Sample Start Adjust") {
			foundParameter = true;
			qDebug("Found parameter: Cell Sample Start Adjust");
			QMessageBox::warning(this, "Sam", "<b>The parameter you're trying to create already exists.</b><br><br>There's no need to create it again.","OK");
		}
	}

	if(!foundParameter) {
		qDebug("Creating parameter...");
		SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
		parameter->setName("Cell Sample Start Adjust");
		parameter->setType(SamParameter::sptRange);
		parameter->setMidiType(SamParameter::spmtController);
		parameter->setMidiController(16);
		parameter->setSource(SamParameter::CellSampleStart);
		parameter->setSourceControl(0);
		parameter->setInverted(true);
		parameter->setCurveShape(CsCurve::Linear);
		parameter->setDefaultValue(1.0f);
		samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
	}
}

void SeGuiParameters::createLayerSwitchParameters(void)
{
	qDebug("Sam::createLayerSwitchParameters()");

	bool foundParameter = false;

	SamArticulation *articulation = m_articulation; //editorWidget->instrument()->version(0)->articulation(editorWidget->articulationId());
	for(int i=0; i<articulation->numParameters(); i++) {
		if(articulation->parameter(i)->name() == "Alternate Layers") {
			foundParameter = true;
			qDebug("Found parameter: Alternate Layers");
			QMessageBox::warning(this, "Sam", "<b>The parameter you're trying to create already exists.</b><br><br>There's no need to create it again.","OK");
		}
	}

	if(!foundParameter) {
		qDebug("Creating parameters...");
		for(int i=0; i<articulation->numLayers(); i++) {
			SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
			parameter->setName(AString("Switch to Layer ")+AString::number(i));
			parameter->setType(SamParameter::sptTrigger);
			parameter->setMidiType(SamParameter::spmtController);
			parameter->setMidiController(3);
			parameter->setSource(11);
			parameter->setTriggerValue(i);
			parameter->setMidiValue(i);
			samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
		}

		SamParameter *parameter = new SamParameter(articulation); //articulation->createParameter();
		parameter->setName("Alternate Layers");
		parameter->setType(SamParameter::sptTrigger);
		parameter->setMidiType(SamParameter::spmtController);
		parameter->setMidiController(3);
		parameter->setSource(10);
		parameter->setTriggerValue(1);
		parameter->setMidiValue(127);
		samEngine->executeRealTimeCommand(new RtcAddParameter(articulation, parameter));
	}
/*
	<Parameter midiController="3" midiType="2" source="10" name="Alternate Bowing" >
    <Trigger midiValue="127" value="1" />
   </Parameter>
   <Parameter midiController="3" midiType="2" source="11" name="Down Bows Only" >
    <Trigger midiValue="0" value="0" />
   </Parameter>
   <Parameter midiController="3" midiType="2" source="11" name="Up Bows Only" >
    <Trigger midiValue="1" value="1" />
   </Parameter>*/
}

void SeGuiParameters::resizeEvent(QResizeEvent *ev)
{
	m_listContainer->setFixedSize(width(), 120);
	m_parameterProperties->move(0, m_listContainer->height()+1);
	m_parameterProperties->setFixedSize(width(), height()-m_listContainer->height());
}