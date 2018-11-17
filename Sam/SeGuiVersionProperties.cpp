#include "SeGuiVersionProperties.h"

#include "SamGuiEditor.h"
#include "SamVersionCommand.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiVersionProperties::SeGuiVersionProperties(QWidget *parent)
	: QWidget(parent)
	, m_version(0)
	, m_channel(0)
{
	//ui.setupUi(this);

	m_list = new UiList(this);
	m_list->setSelectable(false);
	m_list->setMovable(false);

	m_name = new SeGuiItemText(this, SamVersionCommand::Name, "Name");
	//m_name->setTint(true, QColor(120,120,200));
	connect(m_name, SIGNAL(textChanged(int, QString)), this, SLOT(setText(int, QString)));
	m_list->addItem(m_name);

	m_defaultVoices = new SeGuiItemValue(this, SamVersionCommand::DefaultVoices, "Default Voices", "", 1, 256, 64, 1, 8);
	//dynamic->setTint(true, QColor(120,120,200));
	//dynamic->setExtraInfo(2);
	connect(m_defaultVoices, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_defaultVoices);

	m_reference = new SeGuiItemValue(this, SamVersionCommand::Reference, "Reference", "", 0, 999, 0, 1, 8);
	connect(m_reference, SIGNAL(valueChanged(int, float)), this, SLOT(setValue(int, float)));
	m_list->addItem(m_reference);

	m_list->addItem(new SeGuiItemHeading(this, ""));
	m_list->addItem(new SeGuiItemHeading(this, ""));

	m_articulation = new SeGuiItemSelectArticuation(this);
	m_list->addItem(m_articulation);

	//m_instrumentMusicTools = new SeGuiItemHeading(this, "Instrument Music Tools");
	//m_list->addItem(m_instrumentMusicTools);
	
	/*for(int i=0; i<SamChannel::maxMusicTools; i++) {
		m_musicTools[i] = new SeGuiItemSelectMusicTool(this, i, "Tool "+QString::number(i+1));
		m_list->addItem(m_musicTools[i]);
	}*/

	setFixedSize(296, m_list->height());
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SeGuiVersionProperties::~SeGuiVersionProperties()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiVersionProperties::setVersion(SamVersion *version, SamChannel *channel)
{
	m_version = version;
	m_channel = channel;

	if(!m_version) return;

	m_name->setText(m_version->name().toChar());
	m_defaultVoices->setValue(m_version->numVoices());
	m_reference->setValue(m_version->reference());
	m_articulation->setChannel(channel);

	/*for(int i=0; i<SamChannel::maxMusicTools; i++) {
		m_musicTools[i]->setVersion(m_version, m_channel);
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiVersionProperties::setValue(int propertyId, float value)
{
	samEditor->commandManager()->execute(new SamVersionCommand(m_version, propertyId, value));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiVersionProperties::setText(int propertyId, QString text)
{
	QByteArray ba = text.toAscii();
	samEditor->commandManager()->execute(new SamVersionCommand(m_version, propertyId, AString(ba.data())));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SeGuiVersionProperties::resizeEvent(QResizeEvent *ev)
{
	m_list->setFixedSize(width(),height());
}