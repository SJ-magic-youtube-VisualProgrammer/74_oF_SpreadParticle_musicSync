/************************************************************
************************************************************/
#include "DrawFFT.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_FFT::DRAW_FFT()
{
	font.load("font/RictyDiminished-Regular.ttf", 10, true, true, true);
}

/******************************
******************************/
DRAW_FFT::~DRAW_FFT()
{
}

/******************************
******************************/
void DRAW_FFT::setup(const FFT& fft)
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : DRAW_FFT\n");
	fflush(stdout);
	
	/********************
	********************/
	VboSet__FFT.setup(AUDIO_BUF_SIZE/2 * 4);
	Refresh_vboVerts__FFT(fft);
	Refresh_vboColor__FFT(fft);
	VboSet__FFT.update_vertex_color();
}

/******************************
1--2
|  |
0--3
******************************/
void DRAW_FFT::Refresh_vboVerts__FFT(const FFT& fft)
{
	const ofVec2f ofs(0, 700);
	const int width_of_bar = 3;
	const int space_per_bar = 5;
	const int BarHeight = 100;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		VboSet__FFT.set_vboVerts( i * 4 + 0, ofs.x + i * space_per_bar,					ofs.y );
		VboSet__FFT.set_vboVerts( i * 4 + 1, ofs.x + i * space_per_bar,					ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 2, ofs.x + i * space_per_bar + width_of_bar,	ofs.y - fft.get_Gain(i) * BarHeight );
		VboSet__FFT.set_vboVerts( i * 4 + 3, ofs.x + i * space_per_bar + width_of_bar,	ofs.y );
	}
}

/******************************
******************************/
void DRAW_FFT::Refresh_vboColor__FFT(const FFT& fft)
{
	ofColor col;
	
	for (int i = 0; i < fft.get_sizeof_GainArray(); i++) {
		if ( i == (int)Gui_Global->bandRad || i == (int)Gui_Global->bandVel )	col = ofColor( 0, 255, 0 , 150);
		else																	col = ofColor( 0, 100, 255, 150 );
		
		VboSet__FFT.setColor_perShape(4, i, col); // setColor_perShape(int NumPerShape, int id, const ofColor& color);
	}
}

/******************************
******************************/
void DRAW_FFT::update(const FFT& fft)
{
	/********************
	********************/
	Refresh_vboVerts__FFT(fft);
	VboSet__FFT.update_vertex();
	
	Refresh_vboColor__FFT(fft);
	VboSet__FFT.update_color();
}

/******************************
******************************/
void DRAW_FFT::draw(ofFbo& fbo, const FFT& fft)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	ofDisableDepthTest();
	
	/********************
	********************/
	fbo.begin();
		ofDisableDepthTest();
		
		//Draw background rect for spectrum
		const int line_per = 5;
		const int BarHeight = 100;
		const int width_of_bar = 3;
		const int space_per_bar = 5;
		
		// 座布団 : spectrum
		ofSetColor(255, 255, 255, 20);
		ofFill();
		ofDrawRectangle( 0, 700, fbo.getWidth(), -BarHeight * 2 );
		
		// 縦 line
		char buf[256];
		ofSetColor(255, 255, 255, 100);
		for(int i = 0; i * space_per_bar * line_per < fbo.getWidth(); i++){
			int _x = i * space_per_bar * line_per; // i 本目のlineのx座標
			ofDrawLine(_x, 700, _x, 700 - BarHeight );
			
			sprintf( buf, "%d", i * line_per);
			font.drawString(buf, _x, 700 - BarHeight);
		}
		
		ofSetColor(255);
		// glPointSize(Gui_Global->particleSize);
		VboSet__FFT.draw(GL_QUADS);
	fbo.end();
}
