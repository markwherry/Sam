#ifndef SAMGUICOPYLAYERFROMINSTRUMENT_H
#define SAMGUICOPYLAYERFROMINSTRUMENT_H

#include <QWidget>
//#include "ui_SamGuiCopyLayerFromInstrument.h"

#include <QPushButton>
#include "UiSelectMenu.h"

#include "SamInstrument.h"

class SamGuiCopyLayerFromInstrument : public QWidget
{
	Q_OBJECT

public:
	SamGuiCopyLayerFromInstrument(QWidget *parent = 0);
	~SamGuiCopyLayerFromInstrument();

	void setInstrument(SamInstrument *instrument);

public slots:
	void ok(void);
	void cancel(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SamGuiCopyLayerFromInstrumentClass ui;
	SamInstrument *m_instrument;
	UiSelectMenu *m_selectInstrument;
	QPushButton *m_ok;
	QPushButton *m_cancel;
};

#endif // SAMGUICOPYLAYERFROMINSTRUMENT_H
