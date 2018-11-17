#include "SamCrossfadeStringsSetup.h"

#include "SamParameter.h"

#include "ALog.h"

SamCrossfadeStringsSetup::SamCrossfadeStringsSetup(SamToolMaster *toolMaster)
	: SamMusicTool(toolMaster, "Crossfade Strings Setup", SAM_CROSSFADESTRINGSSETUP_UUID)
{
	
}

SamCrossfadeStringsSetup::~SamCrossfadeStringsSetup()
{
}

void SamCrossfadeStringsSetup::processEvent(SamEvent *ev)
{
	//aLog("SamCrossfadeStringsSetup::processEvent()");

	switch(ev->type) {
		case SamEvent::Program:
			{
				//aLog("SamEvent::Program: %d", ev->midiValue1);

				//bool swallow = false;
				int p1 = -1;
				int p2 = -1;

				switch(ev->midiValue1) {
					case 14:
						p1 = 0;
						p2 = 5;
						break;
					case 24:
						p1 = 5;
						p2 = 6;
						break;
					case 34:
						p1 = 0;
						p2 = 6;
						break;
					case 44:
						p1 = 0;
						p2 = 55;
						break;
					case 54:
						p1 = 0;
						p2 = 25;
						break;
					case 64:
						p1 = 5;
						p2 = 20;
						break;
					case 74:
						p1 = 20;
						p2 = 35;
						break;
					case 84:
						p1 = 20;
						p2 = 30;
						break;
				}
				
				if(p1 > -1 && p2 > -1) {
					ev->type = SamEvent::Unassigned;
					//aLog("Swallow!");

					// Set Crossfade On
					SamEvent vev(SamEvent::Parameter);
					vev.timestamp = ev->timestamp;
					vev.subType   = SamParameter::ArticulationMode;
					vev.iValue    = 1;
					toolMaster()->queueEvent(vev);

					// Set Default Articulation
					vev.subType   = SamParameter::ArticulationDefault;
					vev.iValue    = p1;
					vev.articulationIndex = 1;
					toolMaster()->queueEvent(vev);

					// Set Articulation
					vev.subType   = SamParameter::Articulation;
					vev.iValue    = p2;
					toolMaster()->queueEvent(vev);

				} else {
					// Set Crossfade Off
					SamEvent vev(SamEvent::Parameter);
					vev.timestamp = ev->timestamp;
					vev.subType   = SamParameter::ArticulationMode;
					vev.iValue    = 0;
					toolMaster()->queueEvent(vev);
				}
			}
			break;

		//case SamEvent::Parameter:
		//	setParameterValueNormalised(ev->iValue, ev->fValue);
		//	break;
	}
}

float SamCrossfadeStringsSetup::parameterValueNormalised(int id)
{
	//switch(id) {
	//	
	//}

	return 0.0f;
}

void SamCrossfadeStringsSetup::setParameterValueNormalised(int id, float value)
{
	//switch(id) {
	//	
	//}
}