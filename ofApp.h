/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"

#include "sj_common.h"
#include "fft.h"

#include "DrawFFT.h"
#include "DrawBackImage.h"
#include "DrawLogo.h"
#include "DrawSpreadParticle.h"

/************************************************************
************************************************************/

class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		MAIN_WINDOW__WIDTH	= 1280,	// 切れの良い解像度でないと、ofSaveScreen()での画面保存が上手く行かなかった(真っ暗な画面が保存されるだけ).
		MAIN_WINDOW__HEIGHT	= 720,
	};
	
	/****************************************
	****************************************/
	FFT fft_thread;
	// VISUALIZE Visualize;
	DRAW_FFT DrawFFT;
	DRAW_BACK_IMG DrawBackImg;
	DRAW_LOGO DrawLogo;
	DRAW_SPREAD_PARTICLE DrawSpreadParticle;
	
	ofSoundStream *soundStream;
	bool b_EnableAudioOut;
	int soundStream_Input_DeviceId;
	int soundStream_Output_DeviceId;
	
	ofFbo fbo_WorkSpace;
	
	/****************************************
	****************************************/
	void setup_Gui();
	void setup_sound();
	void ClearFbo(ofFbo& fbo);
	
public:
	/****************************************
	****************************************/
	ofApp(int _soundStream_Input_DeviceId, int _soundStream_Output_DeviceId);
	~ofApp();
	
	void setup();
	void update();
	void draw();
	
	void exit();
	
	void audioIn(ofSoundBuffer & buffer);
	void audioOut(ofSoundBuffer & buffer);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
