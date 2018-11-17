#ifndef SEGUIITEMSELECTOUTPUT_H
#define SEGUIITEMSELECTOUTPUT_H

#include "UiListItem.h"
//#include "ui_SeGuiItemSelectOutput.h"

#include "UiSelectMenu.h"
#include "SamVersion.h"

class SeGuiItemSelectOutput : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemSelectOutput(QWidget *parent, int propertyId, QString label);
	~SeGuiItemSelectOutput();

	void setVersion(SamVersion *version);
	void setOutput(int outputListId);

	void updateOutputList(void);

public slots:
	void internalSetOutput(int output);

signals:
	void outputChanged(int, float);

protected:
	void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemSelectOutputClass ui;
	UiSelectMenu *m_selectOutput;
	SamVersion *m_version;
	int m_propertyId;
	QString m_label;
};

#endif // SEGUIITEMSELECTOUTPUT_H
