#include "SamGuiVstPluginManager.h"

#include <QPainter>

#include "SamPreferences.h"

#include <QMessageBox>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiVstPluginManager::SamGuiVstPluginManager(QWidget *parent)
	: QWidget(parent)
	, m_initialState(false)
{
	//ui.setupUi(this);

	setWindowTitle("VST Plug-in Manager");
	setWindowFlags(Qt::Tool|Qt::WindowStaysOnTopHint);

	m_pixmap = new QPixmap(":/Resources/VSTLogoWhite30.png");
	
	int x = 8;
	int y = 11;

	m_checkUse = new UiCheckBox(this);
	m_checkUse->move(x, y);
	m_checkUse->setText("Use VST Plug-ins");
	m_checkUse->setChecked(true);
	connect(m_checkUse, SIGNAL(changed(bool)), this, SLOT(setUse(bool)));

	y+=36;

	y+= 200;

	setFixedSize(300, y);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiVstPluginManager::~SamGuiVstPluginManager()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiVstPluginManager::setUse(bool state)
{
	//qDebug("SamGuiVstPluginManager::setUse(%d)", (int)state);

	samPreferences->engine()->setPreference("UseVstPlugins", AString::number((int)state));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiVstPluginManager::closeEvent(QCloseEvent *ev)
{
	if(m_initialState != m_checkUse->isChecked()) {
		QMessageBox::warning(this, "Sam", "<b>You will need to restart the application</b><br>for changes made in the VST Plug-in Manager to take effect.","OK");
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiVstPluginManager::paintEvent(QPaintEvent *ev)
{
	QPainter p(this);

	p.fillRect(0, 0, width(), height(), QColor(70,70,70));
	p.fillRect(0, 0, width(), 36, QColor(50,50,50));

	p.drawPixmap(width()-m_pixmap->width()-8, 8, *m_pixmap);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiVstPluginManager::showEvent(QShowEvent *ev)
{
	if(samPreferences->engine()->preference("UseVstPlugins", "0").toInt()) {
		m_checkUse->setChecked(true);
	} else {
		m_checkUse->setChecked(false);
	}

	m_initialState = m_checkUse->isChecked();
}