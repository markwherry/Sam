#ifndef UIEDITOR_H
#define UIEDITOR_H

#include <QWidget>
//#include "ui_UiEditor.h"

#include <QScrollArea>

//#include "Ui/UiScrollBar.h"

#include "UiTabBar.h"

class UiEditor : public QWidget
{
	Q_OBJECT

public:
	UiEditor(QWidget *parent, bool scrollableV = false, bool scrollableH = false);
	~UiEditor();

	enum UiEditorSideBar {
		Left  = 0,
		Right = 1
	};

	void setTitle(QString title);
	void setWidget(QWidget *widget, int offset = 0);
	void setHeaderColor(QColor color);

	void setSideBarWidth(UiEditorSideBar side, int width);
	void addWidgetToSideBar(UiEditorSideBar side, QWidget *widget);

	void setSideBarWidgetsVisible(bool visible = true);

	void addTab(QString name);
	void clearTabs(void);
	int selectedTab(void);
	void setSelectedTab(int index);
	UiTabBar::UiTab* tab(int index) { return m_tabBar->tab(index); }

	void sizeWidgetHeightToFit(void);

	void setBackgroundText(QString text) { m_backgroundText = text; }
	void setBackgroundTextVisible(bool visible) { m_backgroundTextVisible = visible; update(); }


public slots:
	void selectTab(int id);
	void updateVScrollBarRange(int min, int max);
	void updateHScrollBarRange(int min, int max);
	void updateVScrollBarValue(int value);
	void updateHScrollBarValue(int value);
	
signals:
	void tabSelected(int id);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::UiEditorClass ui;
	bool m_scrollableV;
	bool m_scrollableH;
	int m_headerHeight;
	QScrollArea *m_scrollArea;
	//UiScrollBar *m_hScrollBar;
	//UiScrollBar *m_vScrollBar;
	QScrollBar *m_hScrollBar;
	QScrollBar *m_vScrollBar;
	QWidget *m_widget;
	QString m_title;
	QColor m_headerColor;
	int m_sideBarLeftWidth;
	int m_sideBarRightWidth;

	QList<QWidget*> m_sideBarLeftWidgets;
	QList<QWidget*> m_sideBarRightWidgets;

	UiTabBar *m_tabBar;

	QPixmap *m_pixBackground;
	QString m_backgroundText;
	bool m_backgroundTextVisible;
};

#endif // UIEDITOR_H
