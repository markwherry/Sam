#ifndef SEGUIITEMBOOL_H
#define SEGUIITEMBOOL_H

#include "UiListItem.h"
//#include "ui_SeGuiItemBool.h"

#include "UiCheckBox.h"

class SeGuiItemBool : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemBool(QWidget *parent, int propertyId, QString label);
	~SeGuiItemBool();

	void setChecked(bool checked) { m_checkBox->setChecked(checked); }
	void setEditable(bool editable) { m_checkBox->setEditable(editable); }

public slots:
	void internalSetChecked(bool checked);

signals:
	void changed(int propertyId, bool checked);

protected:
	void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemBoolClass ui;
	int m_propertyId;
	QString m_label;
	UiCheckBox *m_checkBox;
};

#endif // SEGUIITEMBOOL_H
