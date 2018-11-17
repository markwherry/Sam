#include <QtGui/QApplication>
#include <QMessageBox>

#include "Sam.h"
#include "SamGuiSplash.h"

#include "SamEngine.h"
#include "SamProject.h"
#include "SamRealTimeCommands.h"
#include "SamCommands.h"
#include "SamPreferences.h"

#include "AUtility.h"

//#define NO_UI_TEST

#ifdef NO_UI_TEST
void noUiTest(void);
#endif

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// Make sure an instance of Sam isn't already running
	// --------------------------------------------------
	if(AUtility::numProcessesByName("Sam2.exe") > 1 || AUtility::numProcessesByName("Sam.exe") > 1) {
		QMessageBox::critical(0, "Sam", "<b>A new instance of Sam could not be opened because the last time Sam was closed, it was not able to exit correctly.</b><br><br>Make sure no instance of Sam.exe is running in Task Manager and try again.", "OK");
		return 0;
	}

	int ret = 0;

	// Register Types
	// --------------
	qRegisterMetaType<SamCell*>("SamCell*");
	qRegisterMetaType<SamZone*>("SamZone*");
	qRegisterMetaType<CsMidiMessage>("CsMidiMessage");

	// Adjust the process working set size
	// -----------------------------------
	MEMORYSTATUS mst;
	GlobalMemoryStatus(&mst);

	if(!SetProcessWorkingSetSize(GetCurrentProcess(), mst.dwAvailPhys, mst.dwAvailPhys)) {
		qDebug("SetProcessWorkingSetSize() Failed");
	}

	// Preferences initialisation
	// --------------------------
	SamPreferences *preferences = new SamPreferences();

	// Engine initialisation
	// ---------------------
	SamEngine *e = new SamEngine();
	aLog("NumNetworkMidiInputs = %d", preferences->engine()->preference("NumNetworkMidiInputs", "8").toInt());
	//if(e->init("", "")) {
	//if(e->init("7212D7D9-63E8-4ef4-BBAD-EB43DC5AD52A", "ASIO Hammerfall DSP")) {
	if(e->init(preferences->engine()->preference("AudioDevice"), preferences->engine()->preference("AudioDriver"), preferences->engine()->preference("NumNetworkMidiInputs", "8").toInt())) {

		if(preferences->engine()->preference("UseVstPlugins", "0").toInt())  {
			SamGuiSplash *splash = new SamGuiSplash();
			e->vstPluginManager()->setCallback(splash);
			splash->show();
			//e->vstPluginManager()->addPath("C:\\Program Files\\VSTPlugIns\\Test");
			bool addingPaths = true;
			int currentPath = 0;
			while(addingPaths) {
				AString entry = AString("VstPluginsPath") + AString::number(currentPath);
				if(preferences->engine()->hasPreference(entry)) {
					aLog("%s = %s", entry.toChar(), preferences->engine()->preference(entry).toChar());
					e->vstPluginManager()->addPath(preferences->engine()->preference(entry));	
				} else {
					addingPaths = false;
					if(currentPath == 0) { // Assume first-time run
						aLog("Creating default VST path");
						preferences->engine()->setPreference("VstPluginsPath0", "C:\\Program Files\\VSTPlugIns");
						e->vstPluginManager()->addPath("C:\\Program Files\\VSTPlugIns");
					}
				}

				currentPath++;

				// Failsafe
				if(currentPath > 20) {
					addingPaths = false;
				}
			}
			e->vstPluginManager()->scanPlugins();
			splash->hide();
			e->vstPluginManager()->setCallback(0);
			delete splash;
		}

		e->setMidiControllerFloodgate(preferences->engine()->preference("MidiControllerFloodgate", "0").toInt());
		e->setLoadSampleDataWithInstrument(preferences->engine()->preference("LoadSampleDataWithInstrument", "1").toInt());

		e->setChannelPitchBendRemoteActive((bool)preferences->engine()->preference("ChannelPitchBendRemoteActive", "0").toInt());
		e->setChannelPitchBendRemoteActiveOnMidi((bool)preferences->engine()->preference("ChannelPitchBendRemoteActivateOnMidi", "0").toInt());
		e->setChannelPitchBendRemoteController(preferences->engine()->preference("ChannelPitchBendRemoteController", "5").toInt());
		for(int i=0; i<128; i++) { 
			if(i < 12) {
				e->setChannelPitchBendRemoteUp(i, preferences->engine()->preference(AString("ChannelPitchBendRemoteUp")+AString::number(i), AString::number((i+1)*100)).toInt()); //
				e->setChannelPitchBendRemoteDown(i, preferences->engine()->preference(AString("ChannelPitchBendRemoteDown")+AString::number(i), AString::number((i+1)*100)).toInt());
			} else {
				e->setChannelPitchBendRemoteUp(i, preferences->engine()->preference(AString("ChannelPitchBendRemoteUp")+AString::number(i), "200").toInt()); //
				e->setChannelPitchBendRemoteDown(i, preferences->engine()->preference(AString("ChannelPitchBendRemoteDown")+AString::number(i), "200").toInt());
			}
		}
		
#ifdef NO_UI_TEST
		noUiTest();

		while(1) {
			Sleep(1);
		}
#else
		Sam *w = new Sam();
		if(samPreferences->app()->preference("maximised", "0").toInt()) {
			w->showMaximized();
		} else {
			w->show();
		}

		//e->play(0,0);

		ret = a.exec();
aLog("Before delete w");
		delete w;
		aLog("After delete w");
#endif
		
		// Best to do this here because if the app crashes because of a driver change, the last used driver will be used when the app reloads
		preferences->engine()->setPreference("AudioDevice", e->audioManager()->currentAudioDevice());
		preferences->engine()->setPreference("AudioDriver", e->audioManager()->currentAudioDeviceDriver());
		preferences->engine()->setPreference("MidiControllerFloodgate", AString::number((int)e->isMidiControllerFloodgate()));
		preferences->engine()->setPreference("LoadSampleDataWithInstrument", AString::number((int)e->isLoadSampleDataWithInstrument()));

		preferences->engine()->setPreference("ChannelPitchBendRemoteActive", AString::number((int)samEngine->isChannelPitchBendRemoteActive()));
		preferences->engine()->setPreference("ChannelPitchBendRemoteActivateOnMidi", AString::number((int)samEngine->isChannelPitchBendRemoteActiveOnMidi()));
		preferences->engine()->setPreference("ChannelPitchBendRemoteController", AString::number(samEngine->channelPitchBendRemoteController()));
		for(int i=0; i<128; i++) {
			preferences->engine()->setPreference(AString("ChannelPitchBendRemoteUp")+AString::number(i), AString::number(samEngine->channelPitchBendRemoteUp(i)));
			preferences->engine()->setPreference(AString("ChannelPitchBendRemoteDown")+AString::number(i), AString::number(samEngine->channelPitchBendRemoteDown(i)));
		}

		e->exit();
	}

	delete e;

	delete preferences;

	return ret;
}

#ifdef NO_UI_TEST
void noUiTest(void)
{
	int m_id = -1;
	AString m_name = "Quad Output";
	CsSpeakerConfig m_config = CsSpeakerConfig(CsSpeakerConfig::Surround40);
	int m_hardwareStartStream = -1;

	if(m_id < 0) {
		m_id = samEngine->project()->nextAudioBusId();
	}

	SamAudioHardwareBus *audioHardwareBus = new SamAudioHardwareBus(samEngine->project(), m_id, m_name, m_config.numSpeakers(), m_config);

	if(m_hardwareStartStream<0) {
		m_hardwareStartStream = samEngine->project()->nextAudioHardwareBusStartStream();
	}
	
	for(int i=0; i<m_config.numSpeakers(); i++){
		aLog("[%d] = [%d]",i, m_hardwareStartStream);
		audioHardwareBus->setStreamHardwareChannel(i, m_hardwareStartStream);
		m_hardwareStartStream++;
	}

	audioHardwareBus->setAudioProcessInfo(samEngine->audioProcessInfo());
	audioHardwareBus->setActive();
	samEngine->executeRealTimeCommand(new RtcAddAudioHardwareBus(audioHardwareBus));

	// ---

	AList<AString> *files = new AList<AString>;
	files->append("S://Sam Instruments//AIR3 Celli 14//AIR3 Celli 14 All//AIR3 Cel14 short 07.sam");
	LoadInstruments li(files, 1, true);
	SamInstrument *instrument = li.loadInstrument("S://Sam Instruments//AIR3 Celli 14//AIR3 Celli 14 All//AIR3 Cel14 short 07.sam"); 
	if(instrument) {
		int nextMidiChannel = 1;
		int midiPort = 0;
		

		SamChannel *channel = new SamChannel(samEngine->project());
		channel->setMidiInputChannel(1, 0, false);
		channel->setMidiInputPort(2000, 0, false);
		channel->assignInstrument(instrument, 0);
		channel->setAudioProcessInfo(samEngine->audioProcessInfo());

		samEngine->executeRealTimeCommand(new RtcAddChannel(channel));

		samEngine->executeRealTimeCommand(new RtcSetAudioBusOutputId(channel->audioOutputBus(0), 100, 0));
	}
}
#endif