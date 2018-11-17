#include "SeGuiLayerProperties.h"

#include "SamGuiEditor.h"
#include "SamLayerCommand.h"

SeGuiLayerProperties::SeGuiLayerProperties(QWidget *parent, int category)
	: QWidget(parent)
	, m_category(category)
	, m_layer(0)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	switch(m_category) {
		case 0: // General
			initGeneral();
			break;
		case 1: // Scope
			initScope();
			break;
		case 2: // Downbeat Offset
			initDownbeat();
			break;
		case 3: // Interval
			initInterval();
			break;
		case 4: // Time
			initTime();
			break;
	}

	setFixedSize(296, m_list->height());
}

SeGuiLayerProperties::~SeGuiLayerProperties()
{

}

void SeGuiLayerProperties::setLayer(SamLayer *layer)
{
	m_layer = layer;
	if(!m_layer) return;

	switch(m_category) {
		case 0: // General
			m_name->setText(m_layer->name().toChar());
			m_zoneSelectMode->setValue(m_layer->zoneSelectMode());
			m_shortNoteMode->setChecked(m_layer->isShortNoteMode());
			m_releaseVoices->setValue(m_layer->releaseVoices());
			m_fineTune->setValue(m_layer->fineTune());
			m_pitchBendUp->setValue(m_layer->pitchBendUp());
			m_pitchBendDn->setValue(m_layer->pitchBendDown());
			m_gain->setValue(m_layer->gain());
			break;
		case 1: // Scope
			m_velocityLo->setValue(m_layer->midiVelocityLow());
			m_velocityHi->setValue(m_layer->midiVelocityHigh());
			m_pitchLo->setValue(m_layer->midiPitchLow());
			m_pitchHi->setValue(m_layer->midiPitchHigh());
			m_pedalMode->setValue(m_layer->pedalMode());
			break;
		case 2: // Downbeat Offset
			m_downbeatSampleOffset->setValue(m_layer->downbeatSampleOffset());
			m_downbeatEnvelopeOffset->setValue(m_layer->downbeatEnvelopeOffset());
			break;
		case 3: // Interval
			m_intervalLow->setValue(m_layer->intervalLow());
			m_intervalHigh->setValue(m_layer->intervalHigh());
			m_intervalRecorded->setValue(m_layer->intervalRecorded());
			break;
		case 4: // Time
			m_timeMin->setValue(m_layer->timeFromLastNoteMin());
			m_timeMax->setValue(m_layer->timeFromLastNoteMax());
			m_timeMaxInf->setChecked(m_layer->isTimeFromLastNoteMaxInf());
			break;
	}
}

void SeGuiLayerProperties::setValue(int propertyId, float value)
{
	samEditor->commandManager()->execute(new SamLayerCommand(m_layer, propertyId, value));
}

void SeGuiLayerProperties::setText(int propertyId, QString text)
{
	QByteArray ba = text.toAscii();
	samEditor->commandManager()->execute(new SamLayerCommand(m_layer, propertyId, AString(ba.data())));
}

void SeGuiLayerProperties::setState(int propertyId, bool state)
{
	samEditor->commandManager()->execute(new SamLayerCommand(m_layer, propertyId, state));

	/*if(propertyId == SamZoneCommand::OutputEnable) {
		if(state) {
			m_output->setEnabled(true);
			m_releaseOutput->setEnabled(true);
		} else {
			m_output->setEnabled(false);
			m_releaseOutput->setEnabled(false);
		}
	}*/
}

void SeGuiLayerProperties::resizeEvent(QResizeEvent *ev)
{
	m_list->setFixedSize(width(),height());
}

void SeGuiLayerProperties::initGeneral(void)
{
	m_name = new SeGuiItemText(this, SamLayerCommand::Name, "Name");
	connect(m_name, SIGNAL(textChanged(int, QString)), this, SLOT(setText(int, QString)));
	m_list->addItem(m_name);

	m_zoneSelectMode = new SeGuiItemValue(this, SamLayerCommand::ZoneSelectMode, "Zone Select Mode", "", 0, 2, 0, 1, 2);
	m_zoneSelectMode->setExtraInfo(4);
	connect(m_zoneSelectMode, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_zoneSelectMode);

	m_shortNoteMode = new SeGuiItemBool(this, SamLayerCommand::ShortNoteMode, "Short Note Mode");
	connect(m_shortNoteMode, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_shortNoteMode);

	m_releaseVoices = new SeGuiItemValue(this, SamLayerCommand::ReleaseVoices, "Release Voices", "", 1, 128, 23, 1, 2);
	connect(m_releaseVoices, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_releaseVoices);

	m_fineTune = new SeGuiItemValue(this, SamLayerCommand::FineTune, "Fine Tune", "cents", -99, 99, 0, 1, 2);
	connect(m_fineTune, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_fineTune);

	m_pitchBendUp = new SeGuiItemValue(this, SamLayerCommand::PitchBendUp, "Pitch Bend Up", "cents", 0, 1200, 0, 1, 2);
	connect(m_pitchBendUp, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchBendUp);

	m_pitchBendDn = new SeGuiItemValue(this, SamLayerCommand::PitchBendDown, "Pitch Bend Down", "cents", 0, 1200, 0, 1, 2);
	connect(m_pitchBendDn, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchBendDn);

	m_gain = new SeGuiItemValue(this, SamLayerCommand::Gain, "Gain", "dB", -12, 12, 0.0, 0.25, 2);
	connect(m_gain, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_gain);
}

void SeGuiLayerProperties::initScope(void)
{
	m_velocityLo = new SeGuiItemValue(this, SamLayerCommand::MidiVelocityLow, "Velocity Low", "", 1, 127, 1, 1, 2);
	//m_velocityLo->setTint(true, QColor(0,0,255));
	connect(m_velocityLo, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocityLo);

	m_velocityHi = new SeGuiItemValue(this, SamLayerCommand::MidiVelocityHigh, "Velocity High", "", 1, 127, 127, 1, 2);
	//m_velocityHi->setTint(true, QColor(0,0,255));
	connect(m_velocityHi, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_velocityHi);

	m_pitchLo = new SeGuiItemValue(this, SamLayerCommand::MidiPitchLow, "Pitch Low", "", 0, 127, 0, 1, 2);
	//m_velocityLo->setTint(true, QColor(0,0,255));
	connect(m_pitchLo, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchLo);

	m_pitchHi = new SeGuiItemValue(this, SamLayerCommand::MidiPitchHigh, "Pitch High", "", 0, 127, 127, 1, 2);
	//m_velocityHi->setTint(true, QColor(0,0,255));
	connect(m_pitchHi, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pitchHi);

	m_pedalMode = new SeGuiItemValue(this, SamLayerCommand::PedalMode, "Pedal Mode", "", 0, 2, 0, 1, 2);
	m_pedalMode->setExtraInfo(11);
	connect(m_pedalMode, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_pedalMode);

	m_velocityLo->setLiveUpdate();
	m_velocityHi->setLiveUpdate();
	m_pitchLo->setLiveUpdate();
	m_pitchHi->setLiveUpdate();
}

void SeGuiLayerProperties::initDownbeat(void)
{
	m_downbeatSampleOffset = new SeGuiItemValue(this, SamLayerCommand::DownbeatSampleOffset, "Downbeat Offset", "samples", 0, 44100, 0, 1, 2);
	connect(m_downbeatSampleOffset, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_downbeatSampleOffset);

	m_downbeatEnvelopeOffset = new SeGuiItemValue(this, SamLayerCommand::DownbeatEnvelopeOffset, "Downbeat Env Offset", "samples", -22050, 22050, 0, 1, 2);
	connect(m_downbeatEnvelopeOffset, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_downbeatEnvelopeOffset);
}

void SeGuiLayerProperties::initInterval(void)
{
	m_intervalLow = new SeGuiItemValue(this, SamLayerCommand::IntervalLow, "Interval Low", "semitones", -60, 60, 0, 1, 2);
	connect(m_intervalLow, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_intervalLow);

	m_intervalHigh = new SeGuiItemValue(this, SamLayerCommand::IntervalHigh, "Interval High", "semitones", -60, 60, 0, 1, 2);
	connect(m_intervalHigh, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_intervalHigh);

	m_intervalRecorded = new SeGuiItemValue(this, SamLayerCommand::IntervalRecorded, "Recorded Interval", "semitones", -60, 60, 0, 1, 2);
	connect(m_intervalRecorded, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_intervalRecorded);
}

void SeGuiLayerProperties::initTime(void)
{
	m_timeMin = new SeGuiItemValue(this, SamLayerCommand::TimeMin, "Time Range Min", "ms", 0, 1000, 0, 1, 2);
	connect(m_timeMin, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_timeMin);

	m_timeMax = new SeGuiItemValue(this, SamLayerCommand::TimeMax, "Time Range Max", "ms", 0, 1000, 0, 1, 2);
	connect(m_timeMax, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_timeMax);

	m_timeMaxInf = new SeGuiItemBool(this, SamLayerCommand::TimeMaxInf, "Maximum is Infinity");
	connect(m_timeMaxInf, SIGNAL(changed(int, bool)), this, SLOT(setState(int, bool)));
	m_list->addItem(m_timeMaxInf);
}