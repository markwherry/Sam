#include "SamDatabase.h"

SamDatabaseEntry::SamDatabaseEntry(AString uuid, AString name, AString filename)
: m_uuid(uuid)
, m_name(name)
, m_filename(filename)
{

}

void SamDatabase::addEntry(SamDatabaseEntry *entry)
{
	m_entries.append(entry);
}