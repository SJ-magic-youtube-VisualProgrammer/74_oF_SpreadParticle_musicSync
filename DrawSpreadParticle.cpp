/************************************************************
************************************************************/
#include "DrawSpreadParticle.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
PARTICLE::PARTICLE()
{
	init();
	Proceed_RandomAmount();
}

/******************************
******************************/
PARTICLE::~PARTICLE()
{
}

/******************************
******************************/
void PARTICLE::init()
{
	pos.x = get_SignedRandom() * Gui_Global->SpreadParticle_initpos_max;
	pos.y = get_SignedRandom() * Gui_Global->SpreadParticle_initpos_max;
	
	dir.x = get_SignedRandom();
	dir.y = get_SignedRandom();
	if( (dir.x == 0) && (dir.y == 0) ) { dir.y = 1; }
	dir.normalize();
	
	Size.x = ofMap( get_Random(), 0, 1, Gui_Global->SpreadParticle_Size_min, Gui_Global->SpreadParticle_Size_max );
	Size.y = Size.x;
	
	col_Alpha = ofMap( get_Random(), 0, 1, Gui_Global->SpreadParticle_Alpha_min, Gui_Global->SpreadParticle_Alpha_max );
	
	/*
	printf("(%f, %f)\n", pos.x, pos.y);
	printf("(%f, %f)\n", dir.x, dir.y);
	fflush(stdout);
	*/
}

/******************************
******************************/
void PARTICLE::Proceed_RandomAmount()
{
	float move = ofMap(get_Random(), 0, 1, 0, Gui_Global->SpreadParticle_init_Spread_max);
	
	pos += dir * move;
}

/******************************
******************************/
void PARTICLE::Proceed(float vel, float dt)
{
	if(IsScreenOut()) init();
	
	pos += dir * (vel * dt);
}

/******************************
******************************/
bool PARTICLE::IsScreenOut()
{
	float margin = 5;
	if( (pos.x < -ofGetWidth()/2 - margin) || (ofGetWidth()/2 + margin < pos.x) || (pos.y < -ofGetHeight()/2 - margin) || (ofGetHeight()/2 + margin < pos.y) )	return true;
	else																																						return false;
}

/******************************
******************************/
DRAW_SPREAD_PARTICLE::DRAW_SPREAD_PARTICLE()
{
}

/******************************
******************************/
DRAW_SPREAD_PARTICLE::~DRAW_SPREAD_PARTICLE()
{
}

/******************************
******************************/
void DRAW_SPREAD_PARTICLE::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : DRAW_SPREAD_PARTICLE\n");
	fflush(stdout);
	
	/********************
	********************/
	for(int i = 0; i < NUM_TEXTURES_SPREADPARTICLE; i++){
		char buf[512];
		sprintf(buf, "texture/particleTexture_%d.png", i);
		TextureImg[i].load(buf);
	}
	
	/********************
	********************/
	clouds.resize(NUM_CLOUDS);
	
	VboSet__SpreadParticle.setup(NUM_VBO_VERTS, true);
	Refresh_vboVerts__SpreadParticle();
	Refresh_vboColor__SpreadParticle();
	VboSet__SpreadParticle.update_vertex_color();
	
	Refresh_vboTexCoords__SpreadParticle();
	VboSet__SpreadParticle.update_TexCoords();
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_SPREAD_PARTICLE::Refresh_vboVerts__SpreadParticle()
{
	for(int i = 0; i < NUM_CLOUDS; i++){
		VboSet__SpreadParticle.set_vboVerts(i * 4 + 0, clouds[i].getPos_LeftLow().x, clouds[i].getPos_LeftLow().y );
		VboSet__SpreadParticle.set_vboVerts(i * 4 + 1, clouds[i].getPos_LeftUp().x, clouds[i].getPos_LeftUp().y );
		VboSet__SpreadParticle.set_vboVerts(i * 4 + 2, clouds[i].getPos_RightUp().x, clouds[i].getPos_RightUp().y );
		VboSet__SpreadParticle.set_vboVerts(i * 4 + 3, clouds[i].getPos_RightLow().x, clouds[i].getPos_RightLow().y );
	}
}

/******************************
******************************/
void DRAW_SPREAD_PARTICLE::Refresh_vboColor__SpreadParticle()
{
	for(int i = 0; i < NUM_CLOUDS; i++){
		ofColor col = Gui_Global->col_SpreadParticle;
		col.a *= clouds[i].get_ColAlpha();
		
		VboSet__SpreadParticle.set_vboColor(i * 4 + 0, col);
		VboSet__SpreadParticle.set_vboColor(i * 4 + 1, col);
		VboSet__SpreadParticle.set_vboColor(i * 4 + 2, col);
		VboSet__SpreadParticle.set_vboColor(i * 4 + 3, col);
	}
}

/******************************
******************************/
void DRAW_SPREAD_PARTICLE::Refresh_vboTexCoords__SpreadParticle()
{
	for(int i = 0; i < NUM_CLOUDS; i++){
		VboSet__SpreadParticle.set_TexCoords(i * 4 + 0, 0, TextureImg[0].getHeight());
		VboSet__SpreadParticle.set_TexCoords(i * 4 + 1, 0, 0);
		VboSet__SpreadParticle.set_TexCoords(i * 4 + 2, TextureImg[0].getWidth(), 0);
		VboSet__SpreadParticle.set_TexCoords(i * 4 + 3, TextureImg[0].getWidth(), TextureImg[0].getHeight());
	}
}

/******************************
******************************/
void DRAW_SPREAD_PARTICLE::update(const FFT& fft)
{
	/********************
	********************/
	float now = (float)ofGetElapsedTimeMillis() / 1000.0f;
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );	
	t_Last = now;
	
	vel_SpreadParticle = ofMap( fft.get_Gain((int)Gui_Global->bandVel), 0, 0.1, 30, 300 );
	for(int i = 0; i < NUM_CLOUDS; i++){
		clouds[i].Proceed(vel_SpreadParticle, dt);
	}
	
	Refresh_vboVerts__SpreadParticle();
	Refresh_vboColor__SpreadParticle();
	VboSet__SpreadParticle.update_vertex_color();
}

/******************************
******************************/
void DRAW_SPREAD_PARTICLE::draw(ofFbo& fbo, const FFT& fft)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	ofDisableDepthTest();
	
	fbo.begin();
		ofPushMatrix();
			/********************
			********************/
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
			
			/********************
			********************/
			ofSetColor(255);
			
			TextureImg[int(std::floor(Gui_Global->SpreadParticle_TextureId))].bind(0); // GPU側にimgを転送.
			VboSet__SpreadParticle.draw(GL_QUADS);
			TextureImg[int(std::floor(Gui_Global->SpreadParticle_TextureId))].unbind();
		ofPopMatrix();
	fbo.end();
}

