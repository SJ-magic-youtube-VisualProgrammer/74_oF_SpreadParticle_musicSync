/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
enum{
	BUF_SIZE_S = 500,
	BUF_SIZE_M = 1000,
	BUF_SIZE_L = 6000,
	BUF_SIZE_LL = 100000,
	UDP_BUF_SIZE = 100000,
};

enum{
	AUDIO_BUF_SIZE = 512,
	
	AUDIO_OUT_CHS = 2,
	AUDIO_IN_CHS = 2,
	AUDIO_BUFFERS = 2,			// 使われてないっぽい
	AUDIO_SAMPLERATE = 44100,
};

enum{
	NUM_HISTORY = 60,
};

enum{
	NUM_TEXTURES_SPREADPARTICLE = 6,
};


/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	ofxGuiGroup Group_FFT;
		ofxFloatSlider Gain;
		ofxFloatSlider k_smooth;
		ofxFloatSlider dt_smooth_2;
		ofxToggle b_Window;
		
	ofxGuiGroup Group_FFT_Sync;
		ofxFloatSlider bandRad;
		ofxFloatSlider bandVel;
	
	ofxGuiGroup Group_Img;
		ofxFloatSlider col_Img;
		ofxFloatSlider img_ShakeRad;
		
	ofxGuiGroup Group_Logo;
		ofxFloatSlider Size_Logo;
		ofxToggle b_Logo_RotSync;
		ofxFloatSlider Logo_RotSpeed_min;
		ofxFloatSlider Logo_RotSpeed_max;
		ofxFloatSlider Logo_Scale_max;
		ofxFloatSlider Logo_ScaleSync_K;
		ofxFloatSlider Logo_ScaleSync_Friction;
		ofxFloatSlider Logo_ShakeRad;
		ofxFloatSlider Logo_Shake_K;
		ofxFloatSlider Logo_Shake_Friction;
		ofxColorSlider col_Logo;
		
	ofxGuiGroup Group_SpreadParticle;
		ofxFloatSlider SpreadParticle_initpos_max;
		ofxFloatSlider SpreadParticle_init_Spread_max;
		ofxFloatSlider SpreadParticle_Size_min;
		ofxFloatSlider SpreadParticle_Size_max;
		ofxColorSlider col_SpreadParticle;
		ofxFloatSlider SpreadParticle_Alpha_min;
		ofxFloatSlider SpreadParticle_Alpha_max;
		ofxFloatSlider SpreadParticle_TextureId;
		
	ofxGuiGroup Group_ArtSin;
		ofxFloatSlider ArtSin_Band_min;
		ofxFloatSlider ArtSin_Band_max;
		ofxFloatSlider ArtSin_PhaseMap_k;
		ofxToggle b_ArtSin_abs;
		ofxToggle b_Window_artSin;
		ofxColorSlider col_ArtSin;		

	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);
void print_separatoin();
double get_Random();
double get_SignedRandom();


/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;


/************************************************************
************************************************************/

