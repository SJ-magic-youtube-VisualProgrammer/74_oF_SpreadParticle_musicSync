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
class PARTICLE{
private:
	ofVec2f pos;
	ofVec2f dir;
	ofVec2f Size;
	float col_Alpha;
	
	bool IsScreenOut();

public:
	PARTICLE();
	~PARTICLE();
	
	void init();
	void Proceed(float vel, float dt);
	void Proceed_RandomAmount();
	
	float get_ColAlpha()	{ return col_Alpha; }
	
	ofVec2f getPos_LeftLow(){
		ofVec2f point;
		point.x = pos.x - Size.x/2;
		point.y = pos.y + Size.y/2;
		return point;
	}
	
	ofVec2f getPos_LeftUp(){
		ofVec2f point;
		point.x = pos.x - Size.x/2;
		point.y = pos.y - Size.y/2;
		return point;
	}
	
	ofVec2f getPos_RightUp(){
		ofVec2f point;
		point.x = pos.x + Size.x/2;
		point.y = pos.y - Size.y/2;
		return point;
	}
	
	ofVec2f getPos_RightLow(){
		ofVec2f point;
		point.x = pos.x + Size.x/2;
		point.y = pos.y + Size.y/2;
		return point;
	}
	
};

/**************************************************
**************************************************/
class DRAW_SPREAD_PARTICLE{
private:
	/****************************************
	****************************************/
	enum{
		// NUM_CLOUDS = 500,
		NUM_CLOUDS = 1000,
		NUM_VBO_VERTS = NUM_CLOUDS * 4, // GL_QUADS
	};
	
	/****************************************
	****************************************/
	vector <PARTICLE> clouds;
	
	ofx__VBO_SET VboSet__SpreadParticle;
	ofImage TextureImg[NUM_TEXTURES_SPREADPARTICLE];
	
	float vel_SpreadParticle;
	
	float t_Last = 0;
	
	/****************************************
	****************************************/
	void Refresh_vboVerts__SpreadParticle();
	void Refresh_vboColor__SpreadParticle();
	void Refresh_vboTexCoords__SpreadParticle();
	
public:
	/****************************************
	****************************************/
	DRAW_SPREAD_PARTICLE();
	~DRAW_SPREAD_PARTICLE();
	
	void setup(const FFT& fft);
	void update(const FFT& fft);
	void draw(ofFbo& fbo, const FFT& fft);
};




