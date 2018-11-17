#ifndef SAMGUIVIEW_H
#define SAMGUIVIEW_H

#include <QWidget>
//#include "ui_SamGuiView.h"

#include "AStateManager.h"
using namespace Atom;

#include "SamGuiMainWidget.h"
#include "SamGuiEditor.h"

#include "UiTabBar.h"

class Sam;

class SamGuiView : public QWidget, public AStateListener
{
	Q_OBJECT

friend class Sam;

public:
	SamGuiView(QWidget *parent = 0);
	~SamGuiView();

	void onState(AStateEvent *ev);

	inline SamGuiMainWidget* mainWidget(void) const { return m_mainWidget; }

public slots:
	void updateChannels(void);
	void selectTab(int id);

signals:
	void channelsUpdated(void);

protected:
	void resizeEvent(QResizeEvent *ev);

private:
	//Ui::SamGuiViewClass ui;
	UiTabBar *m_tabBar;
	QStackedWidget *m_stackedWidget;
	SamGuiMainWidget *m_mainWidget;
	SamGuiEditor *m_editor;
};

#endif // SAMGUIVIEW_H
