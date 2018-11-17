#ifndef SEGUIDESCRIPTIONPROPERTIES_H
#define SEGUIDESCRIPTIONPROPERTIES_H

#include <QWidget>
//#include "ui_SeGuiDescriptionProperties.h"

#include "UiList.h"
#include "SeGuiItemText.h"
#include "SeGuiItemValue.h"

#include "SamInstrument.h"

class SeGuiDescriptionProperties : public QWidget
{
	Q_OBJECT

public:
	SeGuiDescriptionProperties(QWidget *parent = 0);
	~SeGuiDescriptionProperties();

	void setInstrument(SamInstrument *instrument);

public slots:
	void setText(int propertyId, QString text, QString tag);

protected:
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::SeGuiDescriptionPropertiesClass ui;
	SamInstrument *m_instrument;
	QList<SeGuiItemText*> m_metaDataItemList;
	UiList *m_list;
};

#endif // SEGUIDESCRIPTIONPROPERTIES_H
