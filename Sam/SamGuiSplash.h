#ifndef SAMGUISPLASH_H
#define SAMGUISPLASH_H

#include <QSplashScreen>
//#include "ui_SamGuiSplash.h"

#include "CsVstPluginManager.h"

class SamGuiSplash : public QSplashScreen, public CsVstPluginManagerScannerCallback
{
	Q_OBJECT

public:
	SamGuiSplash();
	~SamGuiSplash();

	void scanningPlugin(AString name);

private:
	//Ui::SamGuiSplash ui;
};

#endif // SAMGUISPLASH_H
