#ifndef __SamPreferences_h__
#define __SamPreferences_h__

#include "APreferences.h"
using namespace Atom;

class SamPreferences
{
public:
	SamPreferences();
	~SamPreferences();

	static SamPreferences* instance(void);

	APreferences* engine(void) { return m_engine; }
	APreferences* recentFiles(void) { return m_recentFiles; }
	APreferences* app(void) { return m_app; }
	APreferences* editor(void) { return m_editor; }

protected:
private:
	static SamPreferences *m_instance;

	APreferences *m_engine;
	APreferences *m_recentFiles;
	APreferences *m_app;
	APreferences *m_editor;
};

#define samPreferences SamPreferences::instance()

#endif