#ifndef SEGUILAYERPROPERTIES_H
#define SEGUILAYERPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiLayerProperties.h"

#include "UiList.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemBool.h"

#include "SamLayer.h"

class SeGuiLayerProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiLayerProperties(QWidget *parent, int category);
	~SeGuiLayerProperties();

	void setLayer(SamLayer *layer);

public slots:
	void setValue(int propertyId, float value);
	void setText(int propertyId, QString text);
	void setState(int propertyId, bool state);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	void initGeneral(void);
	void initScope(void);
	void initDownbeat(void);
	void initInterval(void);
	void initTime(void);
	//Ui::SeGuiLayerPropertiesClass ui;
	int m_category;
	SamLayer *m_layer;
	UiList *m_list;

	SeGuiItemText *m_name;
	SeGuiItemValue *m_zoneSelectMode;
	SeGuiItemBool *m_shortNoteMode;
	SeGuiItemValue *m_releaseVoices;
	SeGuiItemValue *m_fineTune;
	SeGuiItemValue *m_pitchBendUp;
	SeGuiItemValue *m_pitchBendDn;
	SeGuiItemValue *m_gain;

	SeGuiItemValue *m_velocityHi;
	SeGuiItemValue *m_velocityLo;
	SeGuiItemValue *m_pitchHi;
	SeGuiItemValue *m_pitchLo;
	SeGuiItemValue *m_pedalMode;

	SeGuiItemValue *m_downbeatSampleOffset;
	SeGuiItemValue *m_downbeatEnvelopeOffset;

	SeGuiItemValue *m_intervalLow;
	SeGuiItemValue *m_intervalHigh;
	SeGuiItemValue *m_intervalRecorded;

	SeGuiItemValue *m_timeMin;
	SeGuiItemValue *m_timeMax;
	SeGuiItemBool *m_timeMaxInf;
};

#endif // SEGUILAYERPROPERTIES_H
