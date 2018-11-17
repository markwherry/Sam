#ifndef UIAUDIOHARDWARE_H
#define UIAUDIOHARDWARE_H

#include <QWidget>
//#include "ui_UiAudioHardware.h"

#include "UiLabel.h"
#include "UiSelectMenu.h"
#include "UiValue.h"

#include "CsAudioManager.h"
using namespace Cs;

class UiAudioHardware : public QWidget
{
	Q_OBJECT

public:
	UiAudioHardware(QWidget *parent, CsAudioManager *audioManager);
	~UiAudioHardware();

public slots:
	void updateDevice(int deviceIndex);
	void updateDriver(int offsetDriverId);
	void updateInputs(float inputs);

protected:
	void closeEvent(QCloseEvent *ev);

private:
	void updateDriverList(void);

	//Ui::UiAudioHardwareClass ui;
	CsAudioManager *m_audioManager;
	
	UiLabel *m_labelAudioDevice;
	UiLabel *m_labelAudioDriver;
	UiLabel *m_labelNumInputs;

	UiSelectMenu *m_selectAudioDevice;
	UiSelectMenu *m_selectAudioDriver;
	UiValue *m_valueNumInputs;

	int m_originalPorts;
};

#endif // UIAUDIOHARDWARE_H
