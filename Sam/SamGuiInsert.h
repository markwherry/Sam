#ifndef SAMGUIINSERT_H
#define SAMGUIINSERT_H

#include <QWidget>
//#include "ui_SamGuiInsert.h"

#include "UiStateButton.h"

class SamGuiInsert : public QWidget
{
	Q_OBJECT

public:
	SamGuiInsert(QWidget *parent = 0);
	~SamGuiInsert();

private:
	//Ui::SamGuiInsertClass ui;

	UiStateButton *m_buttonInsert;
};

#endif // SAMGUIINSERT_H
