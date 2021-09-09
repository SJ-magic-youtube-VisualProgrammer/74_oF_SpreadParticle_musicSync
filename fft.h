/************************************************************
query = ZGameEditor Visualizer youlean

■ZGameEditor Visualizer - Agua Water | free template playing Melissa by Eveningland
	https://www.youtube.com/watch?v=wYX5zd2AvCc
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <ofMain.h>
#include "sj_common.h"

/************************************************************
************************************************************/

/**************************************************
**************************************************/
struct FRAME_DATA{
public:
	double val[AUDIO_BUF_SIZE];
	
	FRAME_DATA(){
		for(int i = 0; i < AUDIO_BUF_SIZE; i++){
			val[i] = 0;
		}
	}
	
	void set(const vector <double>& _val){
		if(_val.size() < AUDIO_BUF_SIZE) return;
		
		for(int i = 0; i < AUDIO_BUF_SIZE; i++){
			val[i] = _val[i];
		}
	}
};


/**************************************************
**************************************************/
class FFT : public ofThread{
private:
	/****************************************
	****************************************/
	enum{
		THREAD_SLEEP_MS = 1,
	};
	enum ANALYZE_CH{
		ANALYZE_CH__STEREO,
		ANALYZE_CH__L,
		ANALYZE_CH__R,
	};
	
	/****************************************
	****************************************/
	
	float t_LastUpdate = 0;
	
	/********************
	********************/
	vector <float> vol_l;
	vector <float> vol_r;
	
	/********************
	********************/
	const double _PI = 3.14159265398979;
	int N;
	
	vector <double> sintbl;
	vector <int> bitrev;
	vector <double> fft_window;
	vector <double> fft_x;
	vector <double> fft_y;
	vector <double> Gain;
	vector <double> Gain_smoothed;
	vector <double> Gain_smoothed_2;
	vector <double> phase_rad;
	vector <double> phase_deg;
	vector <double> phase_rad_madeFromGain;
	vector <double> artSin_x1;
	vector <double> artSin_y1;
	vector <double> artSin_x2;
	vector <double> artSin_y2;
	vector <double> artSin_x3;
	vector <double> artSin_y3;
	
	FRAME_DATA history[NUM_HISTORY];
	
	float acf; // Amplitude Correction Factor
	
	ANALYZE_CH AnalyzeCh = ANALYZE_CH__STEREO;
	
	/****************************************
	****************************************/
	bool Is_Factorial_of_2(double val);
	// int fft(double x[], double y[], int IsReverse = 0);
	int fft(vector <double>& x, vector <double>& y, int IsReverse = 0);
	void make_bitrev(void);
	void make_sintbl(void);
	
	void copy_vol_to_analyzeArray();
	void multiply_window(vector <double>& _x);
	void cal_Gain();
	void cal_Gain_smoothed(vector <double>& _Gain_smoothed, double k);
	void cal_Gain_LPF(vector <double>& _Gain_smoothed, double a);
	void cal_Phase();
	void cal_phase_MadeFromGain();
	void cal_artSin_xy(vector <double>& artSin_x, vector <double>& artSin_y, int sgn);
	void cal_SumOf_artSin();
	void update_history();
	
public:
	/****************************************
	****************************************/
	FFT();
	~FFT();
	
	void setup();
	void update();
	void draw();
	
	void threadedFunction();
	
	void SetVol(ofSoundBuffer & buffer);
	void GetVol(ofSoundBuffer & buffer, bool b_EnableAudioOut);
	
	double get_Gain(int id) const;
	double get_artSin(int id) const;
	int get_sizeof_GainArray() const;
	int get_sizeof_artSinArray() const;
	double get_history(int TimeId, int FrameId) const;
};



