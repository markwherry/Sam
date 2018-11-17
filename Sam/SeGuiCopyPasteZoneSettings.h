#ifndef SEGUICOPYPASTEZONESETTINGS_H
#define SEGUICOPYPASTEZONESETTINGS_H

#include <QWidget>
//#include "ui_SeGuiCopyPasteZoneSettings.h"

#include "UiButton.h"
#include "UiCheckBox.h"

#include "SamInstrument.h"

class SeGuiCopyPasteZoneSettings : public QWidget
{
	Q_OBJECT

public:
	SeGuiCopyPasteZoneSettings(QWidget *parent = 0);
	~SeGuiCopyPasteZoneSettings();

public slots:
	void copy(void);
	void paste(void);

protected:
	void paintEvent(QPaintEvent *ev);
	void showEvent(QShowEvent *ev);

private:
	//Ui::SeGuiCopyPasteZoneSettings ui;
	SamInstrument *m_instrument;

	UiCheckBox *m_check[11];
	UiButton   *m_buttonCopy;
	UiButton   *m_buttonPaste;

	SamZone *m_zone;
};

#endif // SEGUICOPYPASTEZONESETTINGS_H
