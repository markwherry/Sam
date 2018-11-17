#ifndef SEGUIMOVECOPYZONES_H
#define SEGUIMOVECOPYZONES_H

#include <QWidget>
//#include "ui_SeGuiMoveCopyZones.h"

#include "UiButton.h"
#include "UiCheckBox.h"
#include <QSpinBox>

#include "SamInstrument.h"

class SeGuiMoveCopyZones : public QWidget
{
	Q_OBJECT

public:
	SeGuiMoveCopyZones(QWidget *parent = 0);
	~SeGuiMoveCopyZones();

	void setLayer(SamLayer *layer);

public slots:
	void move(void);
	void copy(void);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::SeGuiMoveCopyZonesClass ui;
	SamLayer *m_layer;
	QSpinBox *m_semitones;
	UiCheckBox *m_transposeRootNote;
	UiButton *m_buttonMove;
	UiButton *m_buttonCopy;
	UiButton *m_buttonCancel;
};

#endif // SEGUIMOVECOPYZONES_H
