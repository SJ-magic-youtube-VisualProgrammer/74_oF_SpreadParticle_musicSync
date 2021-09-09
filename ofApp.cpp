/************************************************************
************************************************************/
#include "ofApp.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId)
: b_EnableAudioOut(false)
, soundStream_Input_DeviceId(_soundStream_Input_DeviceId)
, soundStream_Output_DeviceId(_soundStream_Output_DeviceId)
{
	srand((unsigned int)time(NULL));
}

/******************************
******************************/
ofApp::~ofApp()
{
	delete Gui_Global;
	
	if(fp_Log)	fclose(fp_Log);
}

/******************************
******************************/
void ofApp::exit(){
	/********************
	********************/
	soundStream->close();
	delete soundStream;
	
	/********************
		fft_thread.startThread()
	していないので、すぐ抜ける。
	********************/
	try{
		/********************
		********************/
		if(fft_thread.isThreadRunning()){
			fft_thread.waitForThread(true); // means call stopThread() inside the function, then wait for thread to Stop.
		}
	}catch(...){
		printf("Thread exiting Error\n");
	}
	
	/********************
	********************/
	printf("> Good-bye\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::setup(){
	/********************
	注)
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofEnableSmoothing();
		
		は同時に使えない(OF_BLENDMODE_ADDでなくなる).
	********************/
	ofSetBackgroundAuto(true);
	
	ofSetWindowTitle("SpreadParticle");
	// ofSetVerticalSync(false);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofSetWindowShape(MAIN_WINDOW__WIDTH, MAIN_WINDOW__HEIGHT);
	ofSetEscapeQuitsApp(false);
	
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	fbo_WorkSpace.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	ClearFbo(fbo_WorkSpace);
	
	/********************
	********************/
	fp_Log = fopen("../../../data/Log.csv", "w");
	
	setup_Gui();
	
	/* */
	fft_thread.setup();
	
	DrawFFT.setup(fft_thread);
	DrawBackImg.setup();
	DrawLogo.setup();
	DrawSpreadParticle.setup(fft_thread);
	
	/* */
	print_separatoin();
	printf("> sound Device\n");
	fflush(stdout);
	
	/*
	settings.setInListener(this);
	settings.setOutListener(this);
	settings.sampleRate = 44100;
	settings.numInputChannels = 2;
	settings.numOutputChannels = 2;
	settings.bufferSize = bufferSize;
	
	soundStream.setup(settings);
	*/
	soundStream = new ofSoundStream();
	soundStream->printDeviceList();
	
	if( soundStream_Input_DeviceId == -1 ){
		ofExit();
		return;
		
	}else{
		/********************
			soundStream->setup()
		の直後、audioIn()/audioOut()がstartする.
		これらのmethodは、fft_threadにaccessするので、この前に、fft_threadが初期化されていないと、不正accessが発生してしまう.
		********************/
		setup_sound();
	}
	
	/********************
	********************/
	printf("> start process\n");
	fflush(stdout);
}

/******************************
******************************/
void ofApp::ClearFbo(ofFbo& fbo)
{
	fbo.begin();
		ofClear(0, 0, 0, 0);
	fbo.end();
}

/******************************
description
	memoryを確保は、app start後にしないと、
	segmentation faultになってしまった。
******************************/
void ofApp::setup_Gui()
{
	/********************
	********************/
	Gui_Global = new GUI_GLOBAL;
	Gui_Global->setup("param", "gui.xml", 10, 10);
}

/******************************
******************************/
void ofApp::setup_sound(){
	/********************
	********************/
	vector<ofSoundDevice> devices = soundStream->getDeviceList();
	
	ofSoundStreamSettings settings;
	
	settings.setInDevice(devices[soundStream_Input_DeviceId]);
	if(devices[soundStream_Input_DeviceId].name.find("Native Instruments") != std::string::npos){
		/********************
		"name" でDeviceを選択する場合は、
		このblockを参考にしてください。
		********************/
		printf("> name match : Native Instruments selected\n");
		fflush(stdout);
	}
	
	if( soundStream_Output_DeviceId != -1 ){
		settings.setOutDevice(devices[soundStream_Output_DeviceId]);
		b_EnableAudioOut = true;
	}

	settings.setInListener(this);
	settings.numInputChannels = AUDIO_IN_CHS;
	
	if(b_EnableAudioOut){
		settings.setOutListener(this); /* Don't forget this */
		settings.numOutputChannels = AUDIO_OUT_CHS;
	}else{
		settings.numOutputChannels = 0;
	}
	
	settings.sampleRate = AUDIO_SAMPLERATE;
	settings.bufferSize = AUDIO_BUF_SIZE;
	settings.numBuffers = AUDIO_BUFFERS; // 使ってないっぽい
	
	/********************
		soundStream->setup()
	の直後、audioIn()/audioOut()がstartする.
	これらのmethodは、fft_threadにaccessするので、この前に、fft_threadが初期化されていないと、不正accessが発生してしまう.
	********************/
	soundStream->setup(settings);
}

/******************************
******************************/
void ofApp::update(){
	/********************
	********************/
	fft_thread.update();
	
	/********************
	********************/
	DrawFFT.update(fft_thread);
	DrawBackImg.update();
	DrawLogo.update(fft_thread);
	DrawSpreadParticle.update(fft_thread);

}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0, 0, 0);
	ofSetColor(255);
	
	/********************
	********************/
	ClearFbo(fbo_WorkSpace);
	
	DrawFFT.draw(fbo_WorkSpace, fft_thread);
	DrawBackImg.draw(fbo_WorkSpace);
	DrawSpreadParticle.draw(fbo_WorkSpace, fft_thread);
	DrawLogo.draw(fbo_WorkSpace); // 順序注意
	
	/********************
	********************/
	fbo_WorkSpace.draw(0, 0, ofGetWidth(), ofGetHeight());
	
	/********************
	********************/
	ofDisableDepthTest(); // need this to draw gui properly.
	if(Gui_Global->b_Disp){
		Gui_Global->gui.draw();
		
		ofSetColor(255);
		string info;
		info += "FPS = " + ofToString(ofGetFrameRate(), 2) + "\n";
		ofDrawBitmapString(info, 30, 30);
	}
}

/******************************
******************************/
void ofApp::audioIn(ofSoundBuffer & buffer){
	fft_thread.SetVol(buffer);
}

/******************************
******************************/
void ofApp::audioOut(ofSoundBuffer & buffer){
	fft_thread.GetVol(buffer, b_EnableAudioOut);
}

/******************************
******************************/
void ofApp::keyPressed(int key){
	switch(key){
		case 's':
			soundStream->start();
			printf("> soundStream : start\n");
			fflush(stdout);
			break;
			
		case 'e':
			soundStream->stop();
			printf("> soundStream : stop\n");
			fflush(stdout);
			break;
			
		case 'r':
			soundStream->close();
			delete soundStream;
			
			soundStream = new ofSoundStream();
			setup_sound();
			
			printf("> soundStream : close->restart\n");
			fflush(stdout);
			break;
			
		case 'd':
			Gui_Global->b_Disp = !Gui_Global->b_Disp;
			break;
			
		case 'm':
			Gui_Global->gui.minimizeAll();
			break;
	}
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
