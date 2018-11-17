#ifndef UISELECTMENU_H
#define UISELECTMENU_H

#include "UiObject.h"
//#include "ui_UiSelectMenu.h"

#include <QMenu>

class UiSelectMenu : public UiObject
{
	Q_OBJECT

public:
	UiSelectMenu(QWidget *parent, int width, int height = 20);
	~UiSelectMenu();

	QAction* addItem(QString item, bool enabled = true);
	void addSeparator(void);
	void clear(void);
	int numItems(void) { return m_actions.size(); }
	int selectedItem(void) { return m_selectedItem; }

	void setDoubleClick(bool doubleClick) { m_doubleClick = doubleClick; }

public slots:
	virtual void selectItem(QAction *action);
	void setSelectedItem(int item);
	void setSelectedAction(QAction *action);
	void setSelectedItem(QString text);

signals:
	void itemSelected(int id);
	void itemSelected(QAction *action);
	void doubleClicked(void);

protected:
	bool event(QEvent *ev);
	void leaveEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void timerEvent(QTimerEvent *event);

	void paint(QPainter &p);

private:
	//Ui::NeSelectMenuClass ui;
	QMenu *menu;
	QList<QAction*> m_actions;
	int m_selectedItem;
	int m_timerId;
	bool m_doubleClicked;
	bool m_needToOpenMenu;
	bool m_doubleClick;
};

#endif // UISELECTMENU_H
