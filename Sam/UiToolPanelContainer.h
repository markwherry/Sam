#ifndef UITOOLPANELCONTAINER_H
#define UITOOLPANELCONTAINER_H

#include <QWidget>
//#include "ui_UiToolPanelContainer.h"

#include <QScrollArea>

#include "UiToolPanel.h"
#include "UiScrollBar.h"

class UiToolPanelContainer : public QWidget
{
	Q_OBJECT

public:
	enum UiToolPanelPlacement {
		Left = 0,
		Right = 1
	};

	UiToolPanelContainer(QWidget *parent, UiToolPanelPlacement = Left);
	~UiToolPanelContainer();

	UiToolPanel *toolPanel(void) { return m_toolPanel; }

public slots:
	void updateScrollBarRange(int min, int max);
	void updateScrollBarValue(int value);

protected:
	void paintEvent(QPaintEvent *ev);
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::ToolPanelContainerClass ui;
	UiToolPanel *m_toolPanel;
	QScrollArea *scrollArea;
	UiScrollBar *scrollBar;
	UiToolPanelPlacement m_placement;
};

#endif // UITOOLPANELCONTAINER_H
