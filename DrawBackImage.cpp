/************************************************************
************************************************************/
#include "DrawBackImage.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_BACK_IMG::DRAW_BACK_IMG()
{
}

/******************************
******************************/
DRAW_BACK_IMG::~DRAW_BACK_IMG()
{
}

/******************************
******************************/
void DRAW_BACK_IMG::setup()
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : DRAW_BACK_IMG\n");
	fflush(stdout);
	
	/********************
	********************/
	BackImg.load("img/img.png");
}

/******************************
******************************/
void DRAW_BACK_IMG::update()
{
}

/******************************
******************************/
void DRAW_BACK_IMG::draw(ofFbo& fbo)
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
		ofSetColor(Gui_Global->col_Img, Gui_Global->col_Img, Gui_Global->col_Img, 255);
		BackImg.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	fbo.end();
}
