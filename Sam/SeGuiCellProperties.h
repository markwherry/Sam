#ifndef SEGUICELLPROPERTIES_H
#define SEGUICELLPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiCellProperties.h"

#include "AStateManager.h"

#include "UiList.h"
//#include "SeGuiItemText.h"
#include "SeGuiItemHeading.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemBool.h"

#include "SamZone.h"

class SeGuiCellProperties : public QWidget, public AStateListener
{
	Q_OBJECT

public:
	SeGuiCellProperties(QWidget *parent = 0);
	~SeGuiCellProperties();

	void onState(AStateEvent *ev);

	void setZone(SamZone *zone, int numZonesSelected);
	void setCell(SamCell *cell);

	//void setZone(SamZone *zone);
	//void setCell(SamCell *cell);

public slots:
	void updateCellSelection(void);
	void setValue(int propertyId, float value);
	void setState(int propertyId, bool state);
	//void setText(int propertyId, QString text);

protected:
	//void changeEvent(QEvent *ev);
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);
	void showEvent(QShowEvent *ev);

signals:
	void cellSelected(void);

private:
	//Ui::SeGuiCellPropertiesClass ui;
	UiList *m_list;
	UiList *m_cellList;

	SamZone *m_zone;
//	SamCell *m_cell; 


	SeGuiItemValue *m_sampleRate;
	SeGuiItemValue *m_length;
	SeGuiItemValue *m_lengthRelease;
	SeGuiItemValue *m_baseNote;
	SeGuiItemValue *m_fineTune;
	SeGuiItemValue *m_sourceStart;
	SeGuiItemValue *m_releaseStart;
	SeGuiItemValue *m_releaseAttenuation;
	SeGuiItemBool *m_releaseAssigned;

	SeGuiItemHeading *m_loop;
	SeGuiItemBool *m_loopEnable;
	SeGuiItemValue *m_loopStart;
	SeGuiItemValue *m_loopEnd;

	SeGuiItemHeading *m_slices;
	SeGuiItemValue *m_tempo;
	SeGuiItemValue *m_numSlices;
};

#endif // SEGUICELLPROPERTIES_H
