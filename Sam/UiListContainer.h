#ifndef UILISTCONTAINER_H
#define UILISTCONTAINER_H

#include <QWidget>
//#include "ui_UiListContainer.h"

#include "UiList.h"
#include <QScrollArea>

class UiListContainer : public QWidget
{
	Q_OBJECT

public:
	UiListContainer(QWidget *parent = 0);
	~UiListContainer();

	UiList* list(void) { return m_list; }

	void createSimpleItem(QString label);

protected:
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::UiListContainerClass ui;
	UiList *m_list;
	QScrollArea *m_scrollArea;
};

#endif // UILISTCONTAINER_H
