/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "fft.h"
#include <ofxVboSet.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
class DRAW_LOGO{
private:
	/****************************************
	****************************************/
	ofImage LogoImg;
	
	float t_Last = 0;
	
	float _rot = 0;
	float _scale = 1.0;
	ofVec2f shake_pos = ofVec2f(0, 0);
	
public:
	/****************************************
	****************************************/
	DRAW_LOGO();
	~DRAW_LOGO();
	
	void setup();
	void update(const FFT& fft);
	void draw(ofFbo& fbo);
};




