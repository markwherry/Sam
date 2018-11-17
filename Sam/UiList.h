#ifndef UILIST_H
#define UILIST_H

#include <QWidget>
//#include "ui_UiList.h"

#include "UiListItem.h"
#include "UiRect.h"

class UiList : public QWidget
{
	Q_OBJECT

public:
	UiList(QWidget *parent = 0);
	~UiList();

	void addItem(UiListItem *item);
	void reorganise(void);
	void debugPositions(void);
	int numItems(void) { return m_items.size(); }
	void removeItem(UiListItem *item);
	void removeAllItems(void);
	void renumberPositions(void);

	UiListItem* itemFromUuid(QString uuid);
	UiListItem* itemFromPosition(int position);
	UiListItem* itemFromIndex(int index) { return m_items[index]; }
	int firstSelectedItemPosition(void);
	int lastSelectedItemPosition(void);

	int numItemsSelected(void);

	void setSelectable(bool selectable) { m_selectable = selectable; }
	void setMovable(bool movable) { m_movable = movable; }
	void setEmptyTitle(QString text);
	void setEmptyText(QString text);

public slots:
	void selectItem(QString uuid, bool exclusive = true);
	void selectNextItem(bool exclusive = true);
	void selectPrevItem(bool exclusive = true);
	void updateHeights(void);

signals:
	void itemSelectedListPosition(int position);

protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *event);

private:
	//Ui::UiListClass ui;
	QList<UiListItem*> m_items;
	bool m_selectable;
	bool m_movable;
	bool m_didMove;
	bool m_clickedOnSelectedItem;
	//int clickedItemIndex;
	UiListItem* m_clickedItem;
	int m_movePosition;
	int movingItemIndex;
	UiRect* m_rect;
	QString m_emptyTitle;
	QString m_emptyText;
};

#endif // UILIST_H
