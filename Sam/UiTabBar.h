#ifndef UITABBAR_H
#define UITABBAR_H

#include <QWidget>
//#include "ui_UiTabBar.h"

class UiTabBar : public QWidget
{
	Q_OBJECT

public:
	UiTabBar(QWidget *parent = 0);
	~UiTabBar();

	class UiTab
	{
	public:
		UiTab(QString name)
			: m_name(name)
			, m_mouseOver(false)
			, m_underline(false)
			, m_underlineColor(QColor(255,255,255))
			, m_enabled(true)
		{
		}

		QString name(void) { return m_name; }

		bool isMouseOver(void) { return m_mouseOver; }
		void setMouseOver(bool mouseOver) { m_mouseOver = mouseOver; }

		bool isUnderline(void) { return m_underline; }
		void setUnderline(bool underline) { m_underline = underline; }

		QColor underlineColor(void) { return m_underlineColor; }
		void setUnderlineColor(QColor color) { m_underlineColor = color; }

		bool isEnabled(void) { return m_enabled; }
		void setEnabled(bool enabled) { m_enabled = enabled; }

	private:
		QString m_name;
		bool m_mouseOver;
		bool m_underline;
		QColor m_underlineColor;
		bool m_enabled;
	};

	void addTab(QString name);
	int selectedTab(void) { return m_selectedTab; }
	void selectTab(int id, bool sendSignal = true);
	UiTab* tab(int index) { return tabList[index]; }

	void clear(void);

	void setBackgroundHighlights(bool backgroundHighlights) { m_backgroundHighlights = backgroundHighlights; }
	void setSeparatorColor(QColor color) { m_separatorColor = color; }

	void setCeiling(int ceiling) { m_ceiling = ceiling; }
	void setFont(QFont font) { m_font = font; }

signals:
	void tabSelected(int id);

protected:
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::UiTabBarClass ui;
	QList<UiTab*> tabList;
	int m_selectedTab;
	bool m_backgroundHighlights;
	QColor m_separatorColor;
	int m_ceiling;
	QFont m_font;
};

#endif // UITABBAR_H
