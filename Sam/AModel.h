#ifndef __AModel_h__
#define __AModel_h__

#include "AStateManager.h"
#include "AObject.h"

namespace Atom {

class AModel : public AObject
{
public:
	AModel(AModel *parent = 0);
	~AModel();

	AModel* parent(void) { return m_parent; }
	void setParent(AModel *parent) { m_parent = parent; }

	AModel* root(void);

	virtual void setSelected(bool selected, void *from = 0, bool shouldUpdate = true);
	inline bool isSelected(void) const { return m_selected; }

	void updateState(int context, int item, void* updatedObject, void *fromObject, float value = 0.0f, AString str = "");
	void registerListener(AStateListener *listener, int context);
	void unregisterListener(AStateListener *listener, int context);

protected:
private:
	AModel *m_parent;
	AStateManager *m_stateManager;
	bool m_selected;
};

}

#endif