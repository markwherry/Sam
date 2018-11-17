#include "SeGuiParameterProperties.h"

#include "SamGuiEditor.h"
#include "SamParameterCommand.h"

SeGuiParameterProperties::SeGuiParameterProperties(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	pheading = new SeGuiItemHeading(this, "Parameter");
	m_list->addItem(pheading);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	source = new SeGuiItemValue(this, SamParameterCommand::Source, "Target", "", -1, 127, -1, 1, 2);
	source->setExtraInfo(10);
	source->setFixedWidth(120);
	connect(source, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(source);

	type = new SeGuiItemValue(this, SamParameterCommand::Type, "Type", "", 0, 127, 0, 1, 2);
	type->setExtraInfo(8);
	connect(type, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(type);

	midiRemoteControl = new SeGuiItemHeading(this, "MIDI Remote Control");
	//levelsAndOutputs->setBackgroundColor(QColor(120,0,0));
	m_list->addItem(midiRemoteControl);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	midiType = new SeGuiItemValue(this, SamParameterCommand::MidiType, "MIDI Type", "", 0, 127, 0, 1, 2);
	midiType->setExtraInfo(9);
	connect(midiType, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(midiType);

	midiController = new SeGuiItemValue(this, SamParameterCommand::MidiController, "MIDI Controller", "", 0, 127, 0, 1, 2);
	//midiController->setExtraInfo(1);
	connect(midiController, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(midiController);

	typeTrigger = new SeGuiItemHeading(this, "Type: Trigger");
	//levelsAndOutputs->setBackgroundColor(QColor(120,0,0));
	m_list->addItem(typeTrigger);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	triggerValue = new SeGuiItemValue(this, SamParameterCommand::TriggerValue, "Trigger Value", "", 0, 127, 0, 1, 2);
	//source->setExtraInfo(5);
	connect(triggerValue, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(triggerValue);

	triggerMidiValue = new SeGuiItemValue(this, SamParameterCommand::TriggerMidiValue, "Trigger MIDI Value", "", 0, 127, 0, 1, 2);
	triggerMidiValue->setExtraInfo(1);
	connect(triggerMidiValue, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(triggerMidiValue);

	typeRange = new SeGuiItemHeading(this, "Type: Range");
	//levelsAndOutputs->setBackgroundColor(QColor(120,0,0));
	m_list->addItem(typeRange);
	m_list->addItem(new SeGuiItemHeading(this, ""));

	rangeCurve = new SeGuiItemCurve(this, "Range Curve");
	rangeCurve->setPropertyIdShape(SamParameterCommand::RangeCurveShape);
	rangeCurve->setPropertyIdDepth(SamParameterCommand::RangeCurveDepth);
	rangeCurve->setPropertyIdStart(SamParameterCommand::RangeCurveStart);
	rangeCurve->setPropertyIdEnd(SamParameterCommand::RangeCurveEnd);
	connect(rangeCurve, SIGNAL(shapeChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(rangeCurve, SIGNAL(startChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(rangeCurve, SIGNAL(endChanged(int, float)), this, SLOT(setValue(int, float)));
	connect(rangeCurve, SIGNAL(depthChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(rangeCurve);

	inverted = new SeGuiItemBool(this, SamParameterCommand::RangeInverted, "Range Invert");
	//inverted->setTint(true, QColor(255,0,0));
	connect(inverted, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(inverted);

	defaultValue = new SeGuiItemValue(this, SamParameterCommand::DefaultValue, "Default Value", "%", 0, 100, 100, 1, 2);
	//defaultValue->setExtraInfo(5);
	connect(defaultValue, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(defaultValue);

	setFixedSize(296, m_list->height());
}

SeGuiParameterProperties::~SeGuiParameterProperties()
{

}

void SeGuiParameterProperties::setParameter(SamParameter *parameter)
{
	qDebug("SeGuiParameterProperties::setParameter()");

	m_parameter = parameter;

	if(m_parameter) { qDebug("Here");
		source->setValue(m_parameter->source());
		defaultValue->setValue(m_parameter->defaultValue()*100.0f);
		type->setValue(m_parameter->type());
		midiType->setValue(m_parameter->midiType());
		midiController->setValue(m_parameter->midiController());
		triggerValue->setValue(m_parameter->triggerValue());
		triggerMidiValue->setValue(m_parameter->midiValue());
		rangeCurve->setShape(m_parameter->curve()->shape());
		rangeCurve->setStart(m_parameter->curve()->start());
		rangeCurve->setEnd(m_parameter->curve()->end());
		rangeCurve->setDepth(m_parameter->curve()->depth());
		inverted->setChecked(m_parameter->isInverted());
	}
}


void SeGuiParameterProperties::setValue(int propertyId, float value)
{
	if(m_parameter) {
		samEditor->commandManager()->execute(new SamParameterCommand(m_parameter, propertyId, value));
	}
}

void SeGuiParameterProperties::setState(int propertyId, bool state)
{
	if(m_parameter) {
		samEditor->commandManager()->execute(new SamParameterCommand(m_parameter, propertyId, state));
	}
}

void SeGuiParameterProperties::setText(int propertyId, QString text)
{
	if(m_parameter) {
		QByteArray ba = text.toAscii();
		samEditor->commandManager()->execute(new SamParameterCommand(m_parameter, propertyId, ba.data()));
	}
}

void SeGuiParameterProperties::resizeEvent(QResizeEvent *event)
{
	m_list->setFixedSize(width(),height());
}