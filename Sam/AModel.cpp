#include "AModel.h"

namespace Atom {

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AModel::AModel(AModel *parent)
: m_parent(parent)
, m_stateManager(0)
, m_selected(false)
{
	if(!m_parent) {
		m_stateManager = new AStateManager();
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AModel::~AModel()
{
	if(m_stateManager) {
		delete m_stateManager;
		m_stateManager = 0;
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
AModel* AModel::root(void)
{
	if(m_parent) {
		return m_parent->root();
	}

	return this;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AModel::setSelected (bool selected, void *from, bool shouldUpdate)
{
	m_selected = selected;
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AModel::updateState(int context, int item, void* updatedObject, void *fromObject, float value, AString str)
{
	if(m_stateManager) {
		m_stateManager->updateState(context, item, updatedObject, fromObject, value, str);
	} else {
		root()->updateState(context, item, updatedObject, fromObject, value, str);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AModel::registerListener(AStateListener *listener, int context)
{
	if(m_stateManager) {
		m_stateManager->registerListener(listener, context);
	} else {
		root()->registerListener(listener, context);
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------------------------
void AModel::unregisterListener(AStateListener *listener, int context)
{
	if(m_stateManager) {
		m_stateManager->unregisterListener(listener, context);
	} else {
		root()->unregisterListener(listener, context);
	}
}

}