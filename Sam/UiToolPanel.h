#ifndef UITOOLPANEL_H
#define UITOOLPANEL_H

#include <QWidget>
//#include "ui_UiToolPanel.h"
#include <QMenu>

#define HEADER_SIZE 35

class UiToolPanel : public QWidget
{
	Q_OBJECT

public:
	UiToolPanel(QWidget *parent = 0);
	~UiToolPanel();

	class UiToolPanelSection
	{
	public:
		UiToolPanelSection(QString title, QWidget *widget)
			: m_title(title)
			, m_widget(widget)
			, m_open(false)
			, m_y(0)
			, m_height(0)
			, m_mouseOver(false)
			, m_menu(0)
			, m_mouseOverMenu(false)
		{
		}

		QString title(void) { return m_title; }
		void setTitle(QString title)
		{
			m_title = title;
		}

		QWidget *widget(void) { return m_widget; }
		
		bool isOpen(void) { return m_open; }
		
		void setOpen(bool open)
		{
			m_open = open;
			if(m_open) {
				m_widget->show();
			} else {
				m_widget->hide();
			}
		}

		void toggle(void)
		{
			if(m_open) {
				setOpen(false);
			} else {
				setOpen(true);
			}
		}

		int y(void) { return m_y; }
		void setY(int y) { m_y = y; }

		int height(void) { return m_height; }
		void setHeight(int height) { m_height = height; }

		bool isMouseOver(void) { return m_mouseOver; }
		void setMouseOver(bool mouseOver) { m_mouseOver = mouseOver; }

		bool hasMenu(void)
		{
			if(m_menu) {
				return true;
			}

			return false;
		}

		QMenu *menu(void) { return m_menu; }

		void addActionToMenu(QAction *action) 
		{
			if(!m_menu) {
				m_menu = new QMenu(0);
			}

			m_menu->addAction(action);
		}

		void addSeparatorToMenu(void)
		{
			if(m_menu) {
				m_menu->addSeparator();
			}
		}

		bool isMouseOverMenu(void) { return m_mouseOverMenu; }
		void setMouseOverMenu(bool mouseOver) { m_mouseOverMenu = mouseOver; }

	private:
		QString m_title;
		QWidget *m_widget;
		bool m_open;
		int m_y;
		int m_height;
		bool m_mouseOver;
		QMenu *m_menu;
		bool m_mouseOverMenu;
	};

	void addSection(QString title, QWidget *widget);
	void addActionToMenu(int sectionId, QAction *action);
	void addSeparatorToMenu(int sectionId);
	void setSectionTitle(int index, QString title);

protected:
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *ev);

private:
	//Ui::ToolPanelClass ui;
	void calculatePositions(void);
	QList<UiToolPanelSection*> sectionList;
	bool m_mouseOver;
	QPixmap *pixAddRemove[2];
};

#endif // TOOLPANEL_H
