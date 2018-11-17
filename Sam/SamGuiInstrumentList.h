#ifndef SAMGUIINSTRUMENTLIST_H
#define SAMGUIINSTRUMENTLIST_H

#include "UiList.h"
//#include "ui_SamGuiInstrumentList.h"

class SamGuiInstrumentList : public UiList
{
	Q_OBJECT

public:
	SamGuiInstrumentList(QWidget *parent = 0);
	~SamGuiInstrumentList();

private:
	//Ui::SamGuiInstrumentListClass ui;
};

#endif // SAMGUIINSTRUMENTLIST_H
