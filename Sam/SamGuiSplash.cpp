#include "SamGuiSplash.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSplash::SamGuiSplash()
	: QSplashScreen()
{
	//ui.setupUi(this);
	setPixmap(QPixmap(":/Resources/splash.png"));

	showMessage("Scanning VST plug-ins...", Qt::AlignHCenter, QColor(255,255,255));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiSplash::~SamGuiSplash()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiSplash::scanningPlugin(AString name)
{
	//qDebug("SamGuiSplash::scanningPlugin(%s)", name.toChar());

	QString message = "";//Scanning VST plug-ins... ";
	message += name.toChar();
	showMessage(message, Qt::AlignHCenter, QColor(255,255,255));
}