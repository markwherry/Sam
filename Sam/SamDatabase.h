#ifndef __SamDatabase_h__
#define __SamDatabase_h__

#include "AList.h"
#include "AString.h"
using namespace Atom;

class SamDatabaseEntry
{
public:
	SamDatabaseEntry(AString uuid, AString name, AString filename);

	AString uuid(void) { return m_uuid; }
	AString name(void) { return m_name; }
	AString filename(void) { return m_filename; }

protected:
private:
	AString m_uuid;
	AString m_name;
	AString m_filename;
};

class SamDatabase
{
public:
	void addEntry(SamDatabaseEntry *entry);
	int numEntries(void) { return m_entries.size(); }
	SamDatabaseEntry* entry(int index) { return m_entries[index]; }

protected:
private:
	AList<SamDatabaseEntry*> m_entries;
};

#endif