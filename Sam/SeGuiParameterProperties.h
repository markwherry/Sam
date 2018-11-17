#ifndef SEGUIPARAMETERPROPERTIES_H
#define SEGUIPARAMETERPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiParameterProperties.h"

#include "SamParameter.h"
#include "UiList.h"
#include "SeGuiItemHeading.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"
#include "SeGuiItemBool.h"
#include "SeGuiItemCurve.h"


class SeGuiParameterProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiParameterProperties(QWidget *parent = 0);
	~SeGuiParameterProperties();

	void setParameter(SamParameter *parameter);

public slots:
	void setValue(int propertyId, float value);
	void setState(int propertyId, bool state);
	void setText(int propertyId, QString text);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::SeGuiParameterPropertiesClass ui;
	UiList *m_list;
	SamParameter *m_parameter;
	
	SeGuiItemHeading *pheading;
	SeGuiItemValue *source;
	SeGuiItemValue *type;
	
	SeGuiItemHeading *midiRemoteControl;
	SeGuiItemValue *midiType;
	SeGuiItemValue *midiController;

	SeGuiItemHeading *typeTrigger;
	SeGuiItemValue *triggerValue;
	SeGuiItemValue *triggerMidiValue;

	SeGuiItemHeading *typeRange;
	SeGuiItemCurve *rangeCurve;
	SeGuiItemBool *inverted;
	SeGuiItemValue *defaultValue;
};

#endif // SEGUIPARAMETERPROPERTIES_H
