#include "SamMidiBus.h"

SamMidiBus::SamMidiBus(AModel *parent, CsMidiManager *midiManager, int id, AString name, int deviceIndex, int portIndex)
: SamBus(parent, id, name)
, m_midiManager(midiManager)
, m_deviceIndex(deviceIndex)
, m_portIndex(portIndex)
{
}