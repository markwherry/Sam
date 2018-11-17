#ifndef SEGUIITEMVALUE_H
#define SEGUIITEMVALUE_H

#include "UiListItem.h"
//#include "ui_SeGuiItemValue.h"

#include "UiValue.h"

class SeGuiItemValue : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemValue(QWidget *parent, int propertyId, QString label, QString unit, int min, int max, float def, float step, int pixels);
	~SeGuiItemValue();

	void setValueNormalised(float normalisedValue);
	void setValue(float value) { guiValue->setValue(value); }
	void setMinValue(float min) { guiValue->setMinValue(min); }
	void setMaxValue(float max) { guiValue->setMaxValue(max); }
	void setLiveUpdate(bool liveUpdate = true) { guiValue->setLiveUpdate(liveUpdate); }

	void setExtraInfo(int extraInfo) { guiValue->setExtraInfo(extraInfo); }

	void editorFaderUnregistered(void);

	void setEditable(bool editable) { guiValue->setEditable(editable); }

public slots:
	void internalSetValue(float value);

signals:
	void valueChanged(int propertyId, float value);

protected:
	void changeEvent(QEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemValueClass ui;
	int m_propertyId;
	UiValue *guiValue;
	QString m_label;
	int m_extraInfo;
	//SamEditorFaderController *m_editorFaderController;
	bool editorFaderRegistered;
	QPixmap *pixEditorFader;
};

#endif // SEGUIITEMVALUE_H
