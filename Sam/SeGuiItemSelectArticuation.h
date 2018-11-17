#ifndef SEGUIITEMSELECTARTICUATION_H
#define SEGUIITEMSELECTARTICUATION_H

#include "UiListItem.h"
//#include "ui_SeGuiItemSelectArticuation.h"

#include "UiSelectMenu.h"

#include "SamChannel.h"

class SeGuiItemSelectArticuation : public UiListItem
{
	Q_OBJECT

public:
	SeGuiItemSelectArticuation(QWidget *parent);
	~SeGuiItemSelectArticuation();

	void setChannel(SamChannel *channel);

public slots:
	void internalSetArticulation(int index);

protected:
	//void changeEvent(QEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	//Ui::SeGuiItemSelectArticuationClass ui;
	//int m_propertyId;
	SamChannel *m_channel;
	UiSelectMenu *m_selectArticulation;
};

#endif // SEGUIITEMSELECTARTICUATION_H
