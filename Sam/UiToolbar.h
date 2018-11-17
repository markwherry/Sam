#ifndef UITOOLBAR_H
#define UITOOLBAR_H

#include <QWidget>
//#include "ui_UiToolbar.h"

#include "UiToolbarObject.h"

class UiToolbar : public QWidget
{
	Q_OBJECT

public:
	UiToolbar(QWidget *parent, int height);
	~UiToolbar();

	void addObject(UiToolbarObject *button);
	void addSeparator(void);

private:
	//Ui::UiToolbarClass ui;
	QList<UiToolbarObject*> m_objects;
};

#endif // UITOOLBAR_H
