/************************************************************
************************************************************/
#include "sj_common.h"

/************************************************************
************************************************************/
/********************
********************/
int GPIO_0 = 0;
int GPIO_1 = 0;

/********************
********************/
GUI_GLOBAL* Gui_Global = NULL;

FILE* fp_Log = nullptr;


/************************************************************
func
************************************************************/
/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt)
{
	double Alpha;
	if((Alpha_dt <= 0) || (Alpha_dt < dt))	Alpha = 1;
	else									Alpha = 1/Alpha_dt * dt;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double LPF(double LastVal, double CurrentVal, double Alpha)
{
	if(Alpha < 0)		Alpha = 0;
	else if(1 < Alpha)	Alpha = 1;
	
	return CurrentVal * Alpha + LastVal * (1 - Alpha);
}

/******************************
******************************/
double sj_max(double a, double b)
{
	if(a < b)	return b;
	else		return a;
}

/******************************
******************************/
bool checkIf_ContentsExist(char* ret, char* buf)
{
	if( (ret == NULL) || (buf == NULL)) return false;
	
	string str_Line = buf;
	Align_StringOfData(str_Line);
	vector<string> str_vals = ofSplitString(str_Line, ",");
	if( (str_vals.size() == 0) || (str_vals[0] == "") ){ // no_data or exist text but it's",,,,,,,".
		return false;
	}else{
		return true;
	}
}

/******************************
******************************/
void Align_StringOfData(string& s)
{
	size_t pos;
	while((pos = s.find_first_of(" 　\t\n\r")) != string::npos){ // 半角・全角space, \t 改行 削除
		s.erase(pos, 1);
	}
}

/******************************
******************************/
void print_separatoin()
{
	printf("---------------------------------\n");
}

/******************************
return
	[0, 1)
******************************/
double get_Random()
{
	return ( (double)rand() / ((double)RAND_MAX + 1) );
}

/******************************
return
	[-1, 1)
******************************/
double get_SignedRandom()
{
	return get_Random() * 2.0 - 1.0;
}

/************************************************************
class
************************************************************/

/******************************
******************************/
void GUI_GLOBAL::setup(string GuiName, string FileName, float x, float y)
{
	/********************
	********************/
	gui.setup(GuiName.c_str(), FileName.c_str(), x, y);
	
	/********************
	********************/
	Group_Img.setup("Img");
		Group_Img.add(col_Img.setup("col_Img", 50.0, 0.0, 255.0));
		Group_Img.add(img_ShakeRad.setup("ShakeRad", 100, 0, 300));
	gui.add(&Group_Img);
	
	Group_Logo.setup("Logo");
		Group_Logo.add(Size_Logo.setup("Size", 265, 10.0, 500.0));
		Group_Logo.add(b_Logo_RotSync.setup("RotSync", true));
		Group_Logo.add(Logo_RotSpeed_min.setup("RotSpeed_min", 75, 0, 150));
		Group_Logo.add(Logo_RotSpeed_max.setup("RotSpeed_max", 1150, 0, 4000));
		Group_Logo.add(Logo_Scale_max.setup("Scale_max", 1.8, 1.0, 3.0));
		Group_Logo.add(Logo_ScaleSync_K.setup("ScaleSync_K", 157.0, 0.0, 500.0));
		Group_Logo.add(Logo_ScaleSync_Friction.setup("ScaleSync_Friction", 0.8, 0.0, 1.0));
		Group_Logo.add(Logo_ShakeRad.setup("ShakeRad", 60, 0, 300));
		Group_Logo.add(Logo_Shake_K.setup("Shake_K", 200.0, 0.0, 500.0));
		Group_Logo.add(Logo_Shake_Friction.setup("Shake_Friction", 0.78, 0.0, 1.0));
		{
			ofColor initColor = ofColor(255, 255, 255, 255);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_Logo.add(col_Logo.setup("color", initColor, minColor, maxColor));
		}
		
	gui.add(&Group_Logo);
	
	Group_FFT.setup("FFT");
		Group_FFT.add(Gain.setup("Gain", 1.0, 1.0, 5.0));
		Group_FFT.add(k_smooth.setup("k_smooth", 0.965, 0.8, 1.0));
		Group_FFT.add(dt_smooth_2.setup("dt_smooth_2", 167, 10, 300));
		Group_FFT.add(b_Window.setup("b_window", true));
	gui.add(&Group_FFT);
	
	Group_FFT_Sync.setup("FFT sync");
		Group_FFT_Sync.add(bandRad.setup("bandRad", 1.0, 1.0, 10.0));
		Group_FFT_Sync.add(bandVel.setup("bandVel", 100.0, 1.0, 255.0));
	gui.add(&Group_FFT_Sync);
	
	Group_SpreadParticle.setup("SpreadParticle");
		Group_SpreadParticle.add(SpreadParticle_initpos_max.setup("initpos_max", 30.0, 0.0, 300.0));
		Group_SpreadParticle.add(SpreadParticle_init_Spread_max.setup("init_Spread_max", 500, 0.0, 1000.0));
		Group_SpreadParticle.add(SpreadParticle_Size_min.setup("Size_min", 8.0, 2.0, 50));
		Group_SpreadParticle.add(SpreadParticle_Size_max.setup("Size_max", 30.0, 2.0, 50));
		{
			ofColor initColor = ofColor(255, 255, 255, 255);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_SpreadParticle.add(col_SpreadParticle.setup("color", initColor, minColor, maxColor));
		}
		Group_SpreadParticle.add(SpreadParticle_Alpha_min.setup("a_min", 0.3, 0.0, 1.0));
		Group_SpreadParticle.add(SpreadParticle_Alpha_max.setup("a_max", 0.8, 0.0, 1.0));
		Group_SpreadParticle.add(SpreadParticle_TextureId.setup("texture id", 0, 0, NUM_TEXTURES_SPREADPARTICLE - 1));
	gui.add(&Group_SpreadParticle);
	
	Group_ArtSin.setup("ArtSin");
		Group_ArtSin.add(ArtSin_Band_min.setup("ArtSin_Band_min", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_Band_max.setup("ArtSin_Band_max", 1.0, 1.0, 255.0));
		Group_ArtSin.add(ArtSin_PhaseMap_k.setup("ArtSin_PhaseMap_k", 1.0, 0.0, 2.0));
		Group_ArtSin.add(b_ArtSin_abs.setup("b_ArtSin_abs", false));
		Group_ArtSin.add(b_Window_artSin.setup("b_Window_artSin", false));
		{
			ofColor initColor = ofColor(0, 100, 255, 150);
			ofColor minColor = ofColor(0, 0, 0, 0);
			ofColor maxColor = ofColor(255, 255, 255, 255);
			Group_ArtSin.add(col_ArtSin.setup("col_ArtSin", initColor, minColor, maxColor));
		}
	gui.add(&Group_ArtSin);
	
	/********************
	********************/
	gui.minimizeAll();
}

