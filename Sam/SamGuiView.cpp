#include "SamGuiView.h"

#include "SamEngine.h"
#include "SamProject.h"

#include "Sam.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiView::SamGuiView(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	m_tabBar = new UiTabBar(this);
	m_tabBar->addTab("PROJECT");
	m_tabBar->addTab("EDITOR  ");
	m_tabBar->tab(1)->setEnabled(false);
	m_tabBar->setFont(QFont("Calibri", 12));
	m_tabBar->setCeiling(10);
	connect(m_tabBar, SIGNAL(tabSelected(int)), this, SLOT(selectTab(int)));

	m_stackedWidget = new QStackedWidget(this);
	//setCentralWidget(m_stackedWidget);

	m_mainWidget = new SamGuiMainWidget(0);
	m_stackedWidget->addWidget(m_mainWidget);

	
	m_editor = new SamGuiEditor(0);
	m_stackedWidget->addWidget(m_editor);

	samEngine->registerListener(this, SamEngine::ContextProject);
	connect(this, SIGNAL(channelsUpdated()), this, SLOT(updateChannels()));

	connect(m_tabBar, SIGNAL(tabSelected(int)), this, SLOT(selectTab(int)));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiView::~SamGuiView()
{

}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiView::onState(AStateEvent *ev)
{
	//qDebug("SamGuiChannelList::onState() ev=%d", ev);

	if(ev->context() == SamEngine::ContextProject) {

		switch(ev->item()) {
			case SamProject::ItemChannelAdded:
				emit channelsUpdated();
				break;
			case SamProject::ItemChannelRemoved:
				emit channelsUpdated();
				break;
		}

	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiView::selectTab(int id)
{
	qDebug("SamGuiView::selectTab(%d)", id);

	if(id) {
		SamChannel *channel = samEngine->project()->firstSelectedChannel();
		if(channel) {
			samGui->openEditor(channel);
		}
	} else {
		samGui->closeEditor();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiView::updateChannels(void)
{
	//if(samEngine->project()->numChannels() > 0) {
	m_tabBar->tab(1)->setEnabled(samEngine->project()->numChannels() > 0);
	m_tabBar->update();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiView::resizeEvent(QResizeEvent *ev)
{
	m_tabBar->move(0,0);
	m_tabBar->setFixedWidth(160); //130
	m_stackedWidget->move(0,30);
	m_stackedWidget->setFixedSize(width(), height()-30+1);
}