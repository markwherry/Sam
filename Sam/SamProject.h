#ifndef __SamProject_h__
#define __SamProject_h__

#include "AModel.h"
#include "ACommandManager.h"
#include "AList.h"
using namespace Atom;

#include "SamChannel.h"
#include "SamAudioHardwareBus.h"

class SamProject : public AModel
{
public:
	SamProject(AModel *parent);
	~SamProject();

	ACommandManager* commandManager(void) { return m_commandManager; }

	void clear(void);

	// Channels
	int numChannels(void) { return m_channels.size(); }
	SamChannel* channel(int index) { return m_channels[index]; }
	AList<SamChannel*>* channels(void) { return &m_channels; }
	void addChannel(SamChannel *channel);
	void removeChannel(SamChannel *channel);
	SamChannel* selectedChannel(void);
	SamChannel* firstSelectedChannel(void);
	int numSelectedChannels(void);
	void deselectChannels(void);

	// Audio Buses
	int numAudioBuses(void) { return m_audioBuses.size(); }
	void addAudioBus(SamAudioBus *audioBus);
	void removeAudioBus(SamAudioBus *audioBus);
	int nextAudioBusId(void);
	SamAudioBus* audioBus(int index) { return m_audioBuses[index]; }
	AList<SamAudioBus*>* audioBuses(void) { return &m_audioBuses; }

	// AudioHardwareBuses
	int numAudioHardwareBuses(void) { return m_audioHardwareBuses.size(); }
	SamAudioHardwareBus* audioHardwareBus(int index) { return m_audioHardwareBuses[index]; }
	SamAudioHardwareBus* audioHardwareBusFromId(int id);
	AList<SamAudioHardwareBus*>* audioHardwareBuses(void) { return &m_audioHardwareBuses; }
	int audioHardwareBusIndexFromId(int id);
	void addAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus);
	void removeAudioHardwareBus(SamAudioHardwareBus *audioHardwareBus);
	int nextAudioHardwareBusStartStream(void);
	int nextAudioHardwareBusId(void);

	void calculateAudioBusDependencies(void);

	// 
	int numChannelMidiAssignemnts(int inputBusIndex, int midiChannel);// { return m_numChannelMidiAssignments[inputBusIndex][midiChannel]; }
	//int firstChannelMidiAssignemnt(int inputBusIndex, int midiChannel) { return m_firstChannelMidiAssignment[inputBusIndex][midiChannel]; }
	//void updateChannelMidiAssignments(void);

	float tempo(void) { return m_tempo; }
	void setTempo(float tempo, void *from = 0, bool shouldUpdate = true);

	enum ProjectItems {
		ItemChannelAdded            =  0,
		ItemChannelRemoved          =  1,
		ItemAudioHardwareBusAdded   = 10,
		ItemAudioHardwareBusRemoved = 11,
		ItemAudioBusAdded           = 20,
		ItemAudioBusRemoved         = 21
	};

	bool isEmpty(void);

	//void updateSolo(void);
	int numChannelsSolo(void);

	int readElement(AXmlDocument *document, AXmlElement *element, ACommand *command = 0);
	int writeElement(AXmlDocument *document, AXmlElement *element);

protected:
private:
	ACommandManager *m_commandManager;
	
	AList<SamChannel*>          m_channels;
	AList<SamAudioBus*>         m_audioBuses;
	AList<SamAudioHardwareBus*> m_audioHardwareBuses;
	
	int m_lastCreatedAudioBusId;
	int m_lastCreatedAudioHardwareBusId;

	float m_tempo;

	//int m_numChannelMidiAssignments[128][16];
	//int m_firstChannelMidiAssignment[128][16];
};

#endif // __SamProject_h__