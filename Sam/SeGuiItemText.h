#ifndef SEGUIITEMTEXT_H
#define SEGUIITEMTEXT_H

#include "UiListItem.h"
//#include "ui_SeGuiItemText.h"

#include "UiLabel.h"

class SeGuiItemText : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemText(QWidget *parent, int propertyId, QString label, QString tag = "");
	~SeGuiItemText();

	QString tag(void) { return m_tag; }

	void setText(QString text) { m_labelData->setText(text); }

public slots:
	void internalSetText(QString text);

signals:
	void textChanged(int propertyId, QString text);
	void textChanged(int propertyId, QString text, QString tag);

protected:
	void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemTextClass ui;
	int m_propertyId;
	QString m_tag;
	QString m_label;
	UiLabel *m_labelData;
};

#endif // SEGUIITEMTEXT_H
