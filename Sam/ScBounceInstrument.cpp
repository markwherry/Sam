#include "ScBounceInstrument.h"

#include "AMemoryManager.h"
#include "ACommandManager.h"

#include "CsAudioFile.h"

#include "SamEngine.h"
#include "SamCommands.h"

#include <QFile>
#include <QDir>
//#include <QMessageBox>

// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------------
ScBounceInstrument::ScBounceInstrument(SamInstrument *instrument, AString filename, SamStreamMix *streamMix, AList<SamBounceGroup*> *bounceGroups)
: m_srcInstrument(instrument)
, m_filename(filename)
, m_streamMix(streamMix)
, m_dstInstrument(0)
, m_bounceGroups(bounceGroups)
{
	m_decompressionBuffer = (float**)aMalloc(sizeof(float*)*8);
	for(int i=0; i<8; i++) {
		m_decompressionBuffer[i] = (float*)aMalloc((sizeof(float)*SamElementBlock::size));
	}
}

void ScBounceInstrument::execute(ACommandManager *commandManager, int id)
{
	ACommand::execute(commandManager, id);
	setProgressLabel(0, "Bouncing Instrument...");

	// Create the new instrument
	// -------------------------

	

	// Create a directory for the sample data
	QFileInfo fi(m_filename.toChar());
	QString filename = fi.fileName();
	qDebug("filename = %s", qPrintable(filename));
	if(filename.contains(".sam")) {
		filename.remove(".sam");
	}
	qDebug("filename = %s", qPrintable(filename));
	QDir dir(fi.path());

	QDir dir2(fi.path()+"/"+filename);
	if(dir2.exists()) {
		qDebug("Directory exists.");
		setMessage("<b>The folder for the sample data could not be created.</b><br>This is because a folder with the name you entered for<br>the instrument already exists.<br><br>Try again with a different name for the Sam Instrument file.");
		//QMessageBox::warning(this,"Error!", "<b>The folder in which the sample data needs to be stored could not be created. This is because a folder with the name you entered for the instrument already exists.</b><br><br>Try again with a different name for the Sam Instrument file.", "OK");
		return;
	}
	dir.mkdir(filename);

	QString path = fi.path()+"/";
	QByteArray baPath = path.toLatin1();

	// Find the output configuration of the channel...
	// -----------------------------------------------
	/*int audioBusId = -1;
	if(channel->numOutputs()>0) {
		qDebug("channel->numOutputs() = %d", channel->numOutputs());
		audioBusId = channel->outputAudioBusId(0);
	} else {
		qDebug("Instrument/Channel has no outputs.");
		return;
	}
	if(audioBusId<0) {
		qDebug("audioBusId<0 (%d)", audioBusId);
		return;
	}

	int audioBusListId = -1;
	for(int i=0; i<samEngine->project()->numAudioOutputBuses(); i++) {
		if(samEngine->project()->audioOutputBus(i)->id()==audioBusId) {
			audioBusListId = i;
		}
	}

	if(audioBusListId<0) {
		qDebug("audioBusListId<0 (%d)", audioBusId);
		return;
	}
	
	SamAudioOutputBus *audioOutputBus = samEngine->project()->audioOutputBus(audioBusListId);

	int streams = audioOutputBus->numStreams();*/
	//int streams = 2;

	// ...and create the empty instrument

	
	
	m_dstInstrument = new SamInstrument(samEngine);
	m_dstInstrument->createFromBounceGroups(baPath.data(), m_bounceGroups);
	//m_dstInstrument->create(streams, true, false, fi.path()+"/");
	

	
	if(!m_dstInstrument) {
		qDebug("Invalid Instrument");
		return;
	}

	qDebug("Destination Instrument created.");

	int totalCells = m_srcInstrument->numCells();
	int cellsProcessed = 0;

	// Copy the data
	for(int i=0; i<m_srcInstrument->numVersions(); i++) {
		qDebug("version %d/%d", i, m_srcInstrument->numVersions());
		SamVersion *srcVersion = m_srcInstrument->version(i);
		SamVersion *version = new SamVersion(m_dstInstrument, *srcVersion, true);
		m_dstInstrument->addVersion(version); qDebug("addVersion");

		for(int j=0; j<m_srcInstrument->version(i)->numArticulations(); j++) {
			qDebug("articulation %d/%d", j, m_srcInstrument->version(i)->numArticulations());
			SamArticulation *srcArticulation = srcVersion->articulation(j);
			SamArticulation *articulation = new SamArticulation(version, *srcArticulation, 0, true);
			version->addArticulation(articulation); qDebug("addArticulation");

			for(int k=0; k<m_srcInstrument->version(i)->articulation(j)->numLayers(); k++) {
				qDebug("layer %d/%d", k, m_srcInstrument->version(i)->articulation(j)->numLayers());
				SamLayer *srcLayer = srcArticulation->layer(k);
				SamLayer *layer = new SamLayer(articulation, *srcLayer, 0, true);
				articulation->addLayer(layer); qDebug("addLayer");

				for(int l=0; l<srcLayer->numZones(); l++) {
					qDebug("zone %d/%d", l, srcLayer->numZones());
					SamZone *srcZone = srcLayer->zone(l); qDebug("Z1");
					SamZone *zone = new SamZone(layer, *srcZone, 0, true); qDebug("Z2");
					layer->addZone(zone); qDebug("addZone");
					//if(zone->outputDebug(

					for(int m=0; m<srcZone->numCells(); m++) {
						qDebug("cell %d/%d", m, srcZone->numCells());
						SamCell *srcCell = srcZone->cell(m); qDebug("C1");
						SamCell *cell = new SamCell(zone, *srcCell, 0, true); qDebug("C2");
						zone->addCell(cell); qDebug("addCell");

						


						CsAudioBuffer *audioBuffers[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
						CsAudioBuffer *relAudioBuffers[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
						for(int a=0; a<m_bounceGroups->size(); a++) {
							SamBounceGroup *bg = m_bounceGroups->at(a);

							int lengthFrames = srcCell->lengthFrames();
							int numBlocks = (lengthFrames/SamElementBlock::size)+1;
							audioBuffers[a] = new CsAudioBuffer(bg->config().numSpeakers(), numBlocks * SamElementBlock::size);

							int relLengthFrames = srcCell->releaseLengthFrames();
							if(relLengthFrames > 0) {
								int relNumBlocks = (relLengthFrames/SamElementBlock::size)+1;
								relAudioBuffers[a] = new CsAudioBuffer(bg->config().numSpeakers(), relNumBlocks * SamElementBlock::size);
							}

						}

						for(int a=0; a<srcCell->numElements(); a++) {
							SamElement *element = srcCell->element(a);
							bounceElement(element, audioBuffers);

							if(srcCell->hasRelease() && !srcCell->isSourceRelease()) {
								SamElement *relElement = srcCell->elementRelease(a);
								relElement->setSet(element->set());
								bounceElement(relElement, relAudioBuffers);
							}

						} // for a*/

						for(int a=0; a<m_bounceGroups->size(); a++) {
							SamBounceGroup *bg = m_bounceGroups->at(a);
							QString bgPrefix = bg->prefix().toChar();

							int srcId = -1;
							int relId = -1;

							// Write the source file
							// ---------------------
							QString afn = QString(fi.path()+"/"+filename+"/"+QString::number(m_dstInstrument->elementPool()->numElements())+bgPrefix+".wav");
							QByteArray ba = afn.toLatin1();

							CsAudioFile file(ba.data());

							if(!file.create()) {
								qDebug("Error creating audio file.");	
								return;
							}

							file.setFormat(CsAudioFile::Wave);
							file.setChannels(audioBuffers[a]->numStreams());
							//file.setBitDepth(16);
							file.setBitDepth(24);
							file.setSampleRate(44100.0);
							file.setLengthFrames(audioBuffers[a]->numFrames());
							file.writeHeader();
							file.writeAudioBuffer(audioBuffers[a]);
							file.close();


							// Now add the Elements to the Cell
							// --------------------------------
							srcId = m_dstInstrument->elementPool()->numElements();

							aLog("Before add element.");
							QString sefn = QString(filename+"/"+QString::number(srcId)+bgPrefix+".wav"); //m_dstInstrument->elementPool()->numElements()
							QByteArray bas = sefn.toLatin1();
							SamElement *elementSrc = m_dstInstrument->elementPool()->createElement(bas.data(), false);
							if(elementSrc) {
								aLog("elementSrc ok, adding set.");
								elementSrc->setSet(a);
							} else {
								aLog("elementSrc == NULL");
								return;
							}
							aLog("Next.");


							// Write the release file (if there is one)
							// ----------------------------------------
							if(srcCell->hasRelease() && !srcCell->isSourceRelease()) {
								QString rfn = QString(fi.path()+"/"+filename+"/"+QString::number(m_dstInstrument->elementPool()->numElements())+bgPrefix+"rel.wav");
								QByteArray rba = rfn.toLatin1();

								CsAudioFile relFile(rba.data());

								if(!relFile.create()) {
									aLog("Error creating release audio file.");	
									return;
								}

								relFile.setFormat(CsAudioFile::Wave);
								relFile.setChannels(relAudioBuffers[a]->numStreams());
								//relFile.setBitDepth(16);
								relFile.setBitDepth(24);
								relFile.setSampleRate(44100.0);
								relFile.setLengthFrames(relAudioBuffers[a]->numFrames());
								relFile.writeHeader();
								relFile.writeAudioBuffer(relAudioBuffers[a]);
								relFile.close();
							}


							

							

							if(srcCell->hasRelease() && !srcCell->isSourceRelease()) {

								relId = m_dstInstrument->elementPool()->numElements();

								aLog("Before add rel element.");
								QString refn = QString(filename+"/"+QString::number(relId)+bgPrefix+"rel.wav"); //m_dstInstrument->elementPool()->numElements()
								QByteArray bar = refn.toLatin1();
								SamElement *elementRel = m_dstInstrument->elementPool()->createElement(bar.data(), false);
								if(elementRel) {
									aLog("elementRel ok, adding set.");
									elementRel->setSet(a);
								} else {
									aLog("elementRel == NULL");
									return;
								}
								aLog("rel Next.");

							} else if(srcCell->hasRelease() && srcCell->isSourceRelease()) {
								aLog("hasRelease && releaseIsSource");
								relId = srcId;
							}



							aLog("srcId = %d, relId = %d", srcId, relId);
							if(!cell->addElement(srcId, relId)) {
								return;
							}
							aLog("After addElement");

						} // for a


						for(int a=0; a<8; a++) {
							if(audioBuffers[a]) {
								delete audioBuffers[a];
								audioBuffers[a] = 0;
							}
							if(relAudioBuffers[a]) {
								delete relAudioBuffers[a];
								relAudioBuffers[a] = 0;
							}
						}

						cellsProcessed++;
						setProgress((float)cellsProcessed / (float)totalCells);
						setProgressLabel(1, AString("Cell ") + AString::number(cellsProcessed) + AString(" / ") + AString::number(totalCells));

					} // m
				} // l
			} // k
		} // j
	} // i

	aLog("Done and saving...");
	
	// Save the new instrument
	// -----------------------

	// Save the audio data
	// TODO

	// Save the instrument file
	commandManager->execute(new SaveInstrument(m_dstInstrument, m_filename, true));
	//m_dstInstrument->save(m_filename, true);

	//delete destInstrument;

						

						/*CsAudioBuffer *audioBuffer = new CsAudioBuffer(streams, srcCell->lengthFrames());

						for(int n=0; n<srcCell->lengthFrames(); n++) {
							//qApp->processEvents();
							
							float output[8];
							for(int x=0; x<8; x++) output[x] = 0.0f;

							for(int o=0; o<srcCell->numElements(); o++) {
								float outputSample = 0.0f;
								for(int p=0; p<srcCell->element(o)->numStreams(); p++) {
									SamStream *stream = m_streamMix->getStream(srcCell->element(o)->set(),p);
									if(stream) { 
										if(stream->gain()>0.0f && !stream->mute()) {
											if(srcCell->element(i)->bitDepth()==16) { //qDebug("1");
												outputSample = (float)(srcCell->element(o)->audioData[p][n] * (1.0f/32768.0f)); //qDebug("2");
											} else if(srcCell->element(i)->bitDepth()==24) { //qDebug("3");
												outputSample = srcCell->element(o)->fAudioData[p][n]; //qDebug("4");
											}
											outputSample *= stream->gain();
											output[0] += (outputSample*((1.0f-stream->panX())*(1.0f-stream->panY())));
											output[1] += (outputSample*((stream->panX())*(1.0f-stream->panY())));
											output[2] += (outputSample*((1.0f-stream->panX())*(stream->panY())));
											output[3] += (outputSample*((stream->panX())*(stream->panY())));
										}
									}
								}
							}

							for(int x=0; x<streams; x++) {
								float *outputStream = audioBuffer->stream(x);
								outputStream[n] = output[x];
							}
						}

						


						for(int n=0; n<streams; n++) {

							// -------------------------------------
							CsAudioFile file;

							if(!file.create(QString(fi.path()+"/"+filename+"/"+QString::number(destInstrument->elementPool()->numElements())+".wav").toStdString())) {
								qDebug("Error creating audio file.");	
								return;
							}

							file.setFormat(CsAudioFile::Wave);
							file.setChannels(1);
							file.setBitDepth(16);
							file.setSampleRate(44100.0);
							file.setSizeData(audioBuffer->numFrames()*(16/8)*1);
							file.writeHeader();

							
							float *audioStream = audioBuffer->stream(n);
							if(16==16) {
								for(int x=0; x<audioBuffer->numFrames(); x++) {
									//for(int y=0; y<1; y++) {
										
										//qDebug("Writing frame %d/%d and stream %d/%d", i+1, m_lengthFrames, j+1, m_streams);
										double sc = (double)((double)0x7fffL) + .49999;
										file.write16LSB((CsInt16)(audioStream[x]*sc));
									//}
								}
							} else {
								//qDebug("Unsupported bit depth (%d)", m_bitDepth);
							}

							file.close(false);
							// -------------------------------------


							qDebug("Creating element %d/%d", n, streams);
							//SamElement *elementSrc = new SamElement(destInstrument->elementPool()); qDebug("1");
							SamElement *elementSrc = destInstrument->elementPool()->createElement(filename+"/"+QString::number(destInstrument->elementPool()->numElements())+".wav",true);
							//elementSrc->importAudioFile(QString(fi.path()+"/"), QString(filename+"/"+QString::number(destInstrument->elementPool()->numElements())+".wav"));
							//SamElement *elementRel = 0; qDebug("2");
							//elementSrc->create(1, 16, srcCell->lengthFrames()); qDebug("3");
							elementSrc->setSet(n); qDebug("4");

							

							

							//elementSrc->exportAudioFile(QString(fi.path()+"/"), QString(filename+"/"+QString::number(destInstrument->elementPool()->numElements())+".wav"));


							

							int srcId = destInstrument->elementPool()->numElements()-1; //destInstrument->elementPool()->addElement(elementSrc); qDebug("5");
							int relId = -1; qDebug("6");

							//sqDebug("hasRelease=%d isSourceRelease=%d", (int)srcCell->hasRelease(), (int)srcCell->isSourceRelease());

							if(srcCell->hasRelease() && !srcCell->isSourceRelease()) {
								qDebug("hasRelease && !releaseIsSource: Handling Release");
								

								CsAudioBuffer *releaseAudioBuffer = new CsAudioBuffer(streams, srcCell->releaseLengthFrames()); qDebug("1");

								for(int n=0; n<srcCell->releaseLengthFrames(); n++) {
									//qApp->processEvents();
									
									float output[8];
									for(int x=0; x<8; x++) output[x] = 0.0f;

									for(int o=0; o<srcCell->numElements(); o++) {
										float outputSample = 0.0f; //qDebug("2");
										for(int p=0; p<srcCell->elementRelease(o)->numStreams(); p++) {
											//qDebug("3");
											SamStream *stream = m_streamMix->getStream(srcCell->elementRelease(o)->set(),p);
											//qDebug("4");
											if(stream) { 
												if(stream->gain()>0.0f && !stream->mute()) {
													if(srcCell->element(i)->bitDepth()==16) { //qDebug("5");
														outputSample = (float)(srcCell->elementRelease(o)->audioData[p][n] * (1.0f/32768.0f)); //qDebug("6");
													} else if(srcCell->element(i)->bitDepth()==24) { //qDebug("7");
														outputSample = srcCell->elementRelease(o)->fAudioData[p][n]; //qDebug("8");
													}
													outputSample *= stream->gain();
													output[0] += (outputSample*((1.0f-stream->panX())*(1.0f-stream->panY())));
													output[1] += (outputSample*((stream->panX())*(1.0f-stream->panY())));
													output[2] += (outputSample*((1.0f-stream->panX())*(stream->panY())));
													output[3] += (outputSample*((stream->panX())*(stream->panY())));
												}
											}
										}
									}

									for(int x=0; x<streams; x++) {
										float *outputStream = releaseAudioBuffer->stream(x);
										outputStream[n] = output[x];
									}
								}

								qDebug("Before file creation");

								// -------------------------------------
								CsAudioFile file;

								if(!file.create(QString(fi.path()+"/"+filename+"/"+QString::number(destInstrument->elementPool()->numElements())+"rel.wav").toStdString())) {
									qDebug("Error creating audio file.");	
									return;
								}
								qDebug("File created.");

								file.setFormat(CsAudioFile::Wave);
								file.setChannels(1);
								file.setBitDepth(16);
								file.setSampleRate(44100.0);
								file.setSizeData(releaseAudioBuffer->numFrames()*(16/8)*1);
								file.writeHeader();

								
								float *audioStream = releaseAudioBuffer->stream(n);
								if(16==16) {
									for(int x=0; x<releaseAudioBuffer->numFrames(); x++) {
										//for(int y=0; y<1; y++) {
											
											//qDebug("Writing frame %d/%d and stream %d/%d", i+1, m_lengthFrames, j+1, m_streams);
											double sc = (double)((double)0x7fffL) + .49999;
											file.write16LSB((CsInt16)(audioStream[x]*sc));
										//}
									}
								} else {
									//qDebug("Unsupported bit depth (%d)", m_bitDepth);
								}
								qDebug("Closing file.");

								file.close(false);
								qDebug("File closed.");
								// -------------------------------------
								SamElement *elementRel = destInstrument->elementPool()->createElement(filename+"/"+QString::number(destInstrument->elementPool()->numElements())+"rel.wav",true);
								if(!elementRel) {
									qDebug("elementRel = NULL");
								}
								elementSrc->setSet(n); qDebug("4");
								relId = destInstrument->elementPool()->numElements()-1;

							}  if(srcCell->hasRelease() && srcCell->isSourceRelease()) {
								qDebug("hasRelease && releaseIsSource");
								relId = srcId;
							}

							qDebug("srcId = %d, relId = %d", srcId, relId);
							cell->addElement(srcId, relId);
							qDebug("After add");
							
						}


						delete audioBuffer;

						
					}
					
					
				}

				
			}

			
		}

		
	}

	qDebug("Done and saving...");
	
	// Save the new instrument
	// -----------------------

	// Save the audio data
	// TODO

	// Save the instrument file
	m_dstInstrument->save(m_filename, true);

	//delete destInstrument;
	*/
}

void ScBounceInstrument::bounceElement(SamElement *element, CsAudioBuffer **audioBuffers)
{
	int sp = 0; // sample pointer

	for(int b=0; b<element->numBlocks(); b++) {
								
		for(int c=0; c<element->numStreams(); c++) {
			//element->decompressBlock(&element->blocks()[c][b], m_decompressionBuffer[c]);
			element->decompressBlock(element->block(c,b), m_decompressionBuffer[c]);

			SamStream *stream = m_streamMix->getStream(element->set(), c);
			if(stream->gain() > 0.000001f && !stream->mute() && m_streamMix->indexFromStream(stream) > -1) {

				for(int d=0; d<m_bounceGroups->size(); d++) {
					SamBounceGroup *bg = m_bounceGroups->at(d);
					

					if(bg->isStreamEnabled(m_streamMix->indexFromStream(stream))) {
						float output[8];
						for(int e=0; e<8; e++) output[e] = 0.0f;
						float outputSample = 0.0f;
						
						

						for(int e=0; e<SamElementBlock::size; e++) {
							outputSample = m_decompressionBuffer[c][e];
							outputSample *= stream->gain();
							switch(bg->config().config()) {
								case CsSpeakerConfig::Mono:
									output[0] = outputSample;
									break;
								case CsSpeakerConfig::Stereo:
									output[0] = outputSample * (((1.0f-stream->panX())*(1.0f-stream->panY())));
									output[1] = outputSample * (((stream->panX())*(1.0f-stream->panY())));
									break;
								case CsSpeakerConfig::Surround40:
									output[0] = outputSample * ((1.0f-stream->panX())*(1.0f-stream->panY()));
									output[1] = outputSample * ((stream->panX())*(1.0f-stream->panY()));
									output[2] = outputSample * ((1.0f-stream->panX())*(stream->panY()));
									output[3] = outputSample * ((stream->panX())*(stream->panY()));
									break;
							}
		
							for(int f=0; f<bg->config().numSpeakers(); f++) {
								float *outputStream = audioBuffers[d]->stream(f);
								outputStream[sp+e] += output[f];
							} // f
						} // e
					}

				} // for d
			} // if
		} // for c	

		sp += SamElementBlock::size;

		if(sp >= audioBuffers[0]->numFrames()) {
			break;
		}

	} // for b
}