/************************************************************
************************************************************/
#include "DrawLogo.h"
#include <time.h>

/************************************************************
************************************************************/

/******************************
******************************/
DRAW_LOGO::DRAW_LOGO()
{
}

/******************************
******************************/
DRAW_LOGO::~DRAW_LOGO()
{
}

/******************************
******************************/
void DRAW_LOGO::setup()
{
	/********************
	********************/
	print_separatoin();
	printf("> setup : DRAW_LOGO\n");
	fflush(stdout);
	
	/********************
	********************/
	LogoImg.load("texture/Logo.png");
}

/******************************
******************************/
void DRAW_LOGO::update(const FFT& fft)
{
	/********************
	********************/
	float now = (float)ofGetElapsedTimeMillis() / 1000.0f;
	float dt = now - t_Last;
	dt = ofClamp( dt, 0.0, 0.1 );
	t_Last = now;
	
	/********************
	********************/
	float _RotVel = ofMap(fft.get_Gain((int)Gui_Global->bandRad), 1, 2, Gui_Global->Logo_RotSpeed_min,  Gui_Global->Logo_RotSpeed_max, true);
	_rot += _RotVel * dt;
	while(360 <= _rot) { _rot -= 360; }
	
	/********************
	********************/
	enum STATE{
		STATE_FALL,
		STATE_RISE,
	};
	static STATE State = STATE_FALL;
	static float Last_Gain = 0;
	static ofVec2f shake_dir = ofVec2f(0.0, -1.0);

	const float hys = 0.1;
	switch(State){
		case STATE_FALL:
			if( Last_Gain + hys < fft.get_Gain((int)Gui_Global->bandRad) ){
				State = STATE_RISE;
				
				shake_dir.x = 0.0;		// shake_dir.x = get_SignedRandom();
				shake_dir.y = -1.0;		// shake_dir.y = get_SignedRandom();
				shake_dir.normalize();
			}
			break;
			
		case STATE_RISE:
			if( fft.get_Gain((int)Gui_Global->bandRad) < Last_Gain )	State = STATE_FALL;
			break;
	}
	
	Last_Gain = fft.get_Gain((int)Gui_Global->bandRad);
	
	/********************
	********************/
	{
		static float _scale_vel = 0;
		float RawScale = ofMap(fft.get_Gain((int)Gui_Global->bandRad), 1, 2, 1, Gui_Global->Logo_Scale_max, false);
		
		float a = -Gui_Global->Logo_ScaleSync_K * (_scale - 1.0);
		_scale_vel = (_scale_vel + a * dt) * Gui_Global->Logo_ScaleSync_Friction;
		_scale = _scale + _scale_vel * dt;
		
		if( (1.0 < fft.get_Gain((int)Gui_Global->bandRad)) && (State == STATE_RISE) && ( abs(_scale - 1.0) < abs(RawScale - 1.0) ) ){
			_scale = RawScale;
			_scale_vel = 0;
		}
	}
	
	/********************
	********************/
	{
		static float shake_r = 0;
		static float shake_vel = 0;
		
		float raw_r = ofMap(fft.get_Gain((int)Gui_Global->bandRad), 1.0, 2.0, 0, Gui_Global->Logo_ShakeRad, true);
		
		float a = -Gui_Global->Logo_Shake_K * shake_r;
		shake_vel = (shake_vel + a * dt) * Gui_Global->Logo_Shake_Friction;
		shake_r = shake_r + shake_vel * dt;
		
		if( (1.0 < fft.get_Gain((int)Gui_Global->bandRad)) && (State == STATE_RISE) && ( abs(shake_r) < abs(raw_r) ) ){
			shake_r = raw_r;
			shake_vel = 0;
		}
		
		shake_pos = shake_dir * shake_r;
	}
}

/******************************
******************************/
void DRAW_LOGO::draw(ofFbo& fbo)
{
	/********************
	********************/
	ofEnableAlphaBlending();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	// ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing(); // OF_BLENDMODE_ADDとofEnableSmoothingは同時に有効化できない
	
	ofDisableDepthTest();
	
	/********************
	********************/
	fbo.begin();
		ofPushMatrix();
			/********************
			********************/
			ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
			
			ofTranslate(shake_pos.x, shake_pos.y);
			
			if(Gui_Global->b_Logo_RotSync)		ofRotateDeg(_rot);
			
			ofScale(_scale, _scale, 1);
			
			/********************
			********************/
			ofDisableDepthTest();
			
			ofSetColor(Gui_Global->col_Logo);
			
			LogoImg.draw(-Gui_Global->Size_Logo/2, -Gui_Global->Size_Logo/2, Gui_Global->Size_Logo, Gui_Global->Size_Logo);
		ofPopMatrix();
	fbo.end();
}
