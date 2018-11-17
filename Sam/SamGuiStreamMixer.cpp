#include "SamGuiStreamMixer.h"

#include "SamEngine.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStreamMixer::SamGuiStreamMixer(QWidget *parent)
	: SamGuiToolEditor(parent)
{
	//ui.setupUi(this);

	int x = 0;
	for(int i=0; i<64; i++) {
		SamGuiStreamMixStrip *strip = new SamGuiStreamMixStrip(this, i);
		strip->move(x, 0);
		x += strip->width();
		m_strips.append(strip);
	}
	setFixedSize(300, m_strips[0]->height());

	setEditorChannel(0);

	samEngine->registerListener(this, SamEngine::ContextStream);
	samEngine->registerListener(this, SamEngine::ContextSet);

	connect(this, SIGNAL(streamGainUpdated(int)), this, SLOT(updateStreamGain(int)));
	connect(this, SIGNAL(streamPanXUpdated(int)), this, SLOT(updateStreamPanX(int)));
	connect(this, SIGNAL(streamPanYUpdated(int)), this, SLOT(updateStreamPanY(int)));
	connect(this, SIGNAL(streamMuteUpdated(int)), this, SLOT(updateStreamMute(int)));
	connect(this, SIGNAL(streamSoloUpdated(int)), this, SLOT(updateStreamSolo(int)));

	connect(this, SIGNAL(setActiveUpdated()), this, SLOT(updateSetActive()));
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamGuiStreamMixer::~SamGuiStreamMixer()
{
	/*for(int i=0; i<m_strips.size(); i++) {
		delete m_strips;
	}*/
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::onState(AStateEvent *ev)
{
	//qDebug("SamGuiStreamMixer::onState()");

	if(ev->context() == SamEngine::ContextStream) {
		switch(ev->item()) {
			case SamStream::ItemGain:
				for(int i=0; i<m_strips.size(); i++) {
					if(!m_strips[i]->stream()) {
						break;
					}
					if(m_strips[i]->isVisible() && m_strips[i]->stream() == ev->updatedObject()) {// && m_strips[i] != ev->fromObject()) {
						//aLog("emit streamGainUpdated(i);");
						emit streamGainUpdated(i);
					}
				}
				break;
			case SamStream::ItemPanX:
				for(int i=0; i<m_strips.size(); i++) {
					if(!m_strips[i]->stream()) {
						break;
					}
					if(m_strips[i]->isVisible() && m_strips[i]->stream() == ev->updatedObject()) {// && m_strips[i] != ev->fromObject()) {
						//aLog("emit streamGainUpdated(i);");
						emit streamPanXUpdated(i);
					}
				}
				break;
			case SamStream::ItemPanY:
				for(int i=0; i<m_strips.size(); i++) {
					if(!m_strips[i]->stream()) {
						break;
					}
					if(m_strips[i]->isVisible() && m_strips[i]->stream() == ev->updatedObject()) {// && m_strips[i] != ev->fromObject()) {
						//aLog("emit streamGainUpdated(i);");
						emit streamPanYUpdated(i);
					}
				}
				break;
			case SamStream::ItemMute:
				for(int i=0; i<m_strips.size(); i++) {
					if(!m_strips[i]->stream()) {
						break;
					}
					if(m_strips[i]->isVisible() && m_strips[i]->stream() == ev->updatedObject()) {// && m_strips[i] != ev->fromObject()) {
						//aLog("emit streamGainUpdated(i);");
						emit streamMuteUpdated(i);
					}
				}
				break;
			case SamStream::ItemSolo:
				for(int i=0; i<m_strips.size(); i++) {
					if(!m_strips[i]->stream()) {
						break;
					}
					if(m_strips[i]->isVisible() && m_strips[i]->stream() == ev->updatedObject()) {// && m_strips[i] != ev->fromObject()) {
						//aLog("emit streamGainUpdated(i);");
						emit streamSoloUpdated(i);
					}
				}
				break;
		}
	} else if(ev->context() == SamEngine::ContextSet) {
		switch(ev->item()) {
			case SamSet::ItemActive:
				emit setActiveUpdated();
				break;
		}
	}

	
	//qDebug("EXIT: SamGuiStreamMixer::onState()");
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::setEditorChannel(SamChannel *channel)
{
	
	SamGuiToolEditor::setEditorChannel(channel);

	for(int i=0; i<m_strips.size(); i++) {
		m_strips[i]->setStream(0, 0);
		m_strips[i]->hide();
	}

	if(!channel) {
		return;
	}

	for(int i=0; i<editorChannel()->streamMix()->numStreams(); i++) {
		m_strips[i]->setStream(editorChannel(), editorChannel()->streamMix()->stream(i));
		m_strips[i]->show();
	}

	int w = editorChannel()->streamMix()->numStreams() * m_strips[0]->width();
	if(w < 50) {
		w = 300;
	}
	setFixedWidth(w);
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
SamStreamMix* SamGuiStreamMixer::currentStreamMix(void)
{
	if(editorChannel()) {
		return editorChannel()->streamMix();
	}

	return 0;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateStreamGain(int streamIndex)
{
	m_strips[streamIndex]->updateFader();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateStreamPanX(int streamIndex)
{
	m_strips[streamIndex]->updatePan();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateStreamPanY(int streamIndex)
{
	m_strips[streamIndex]->updatePan();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateStreamMute(int streamIndex)
{
	m_strips[streamIndex]->updateMute();
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateStreamSolo(int streamIndex)
{
	m_strips[streamIndex]->updateSolo();
	for(int i=0; i<m_strips.size(); i++) {
		if(m_strips[i]->isVisible()) {
			m_strips[i]->update();
		}
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void SamGuiStreamMixer::updateSetActive(void)
{
	for(int i=0; i<m_strips.size(); i++) {
		m_strips[i]->updateActive();
	}
}