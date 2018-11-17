#ifndef SAMGUICHANNELPITCHBENDREMOTESETUP_H
#define SAMGUICHANNELPITCHBENDREMOTESETUP_H

#include <QWidget>
//#include "ui_SamGuiChannelPitchBendRemoteSetup.h"

#include "UiCheckBox.h"
#include "UiLabel.h"
#include "UiValue.h"

#define NRC 12

class SamGuiChannelPitchBendRemoteSetup : public QWidget
{
	Q_OBJECT

public:
	SamGuiChannelPitchBendRemoteSetup(QWidget *parent = 0);
	~SamGuiChannelPitchBendRemoteSetup();

public slots:
	void setActive(bool state);
	void setActivateOnMidi(bool state);
	void setController(float value);
	void setUp(float value);
	void setDown(float value);

protected:
	void closeEvent(QCloseEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SamGuiChannelPitchBendRemoteSetup ui;
	
	UiCheckBox *m_checkActive;
	UiCheckBox *m_checkActivateOnMidi;
	
	UiLabel *m_labelController;
	UiLabel *m_labelControllerName;
	UiLabel *m_labelValue[NRC];
	UiLabel *m_labelTrigger;
	UiLabel *m_labelUp;
	UiLabel *m_labelDown;
	
	UiValue *m_valueController;
	UiValue *m_valueValueUp[NRC];
	UiValue *m_valueValueDown[NRC];
};

#endif // SAMGUICHANNELPITCHBENDREMOTESETUP_H
