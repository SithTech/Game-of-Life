/*
Util.h -- This is a colection of misc functions that have been found useful at some point
Coded by: Travis Stewart
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <random>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <unordered_map>
#include <queue>

//OpenGL Includes
#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OpenGL/gl.h>
#else
#include <windows.h>
#include "GL/glut.h"
#include <gl/gl.h>
#endif


#define MAX_FLOAT MAXINT64		//Used for the default Z-Buffer value

#define WINDOW_CENTER_X glutGet(GLUT_WINDOW_WIDTH)/2
#define WINDOW_CENTER_Y glutGet(GLUT_WINDOW_HEIGHT)/2
#define RAD_TO_DEG 57.2957795131	// 180˚/π
#define DEG_TO_RAD 0.01745329252	// π/180˚
#define PI 3.14159265359

using namespace std;

//Float world/screen/window conversions
inline float xtoWindow(const float& x) { return roundf(((glutGet(GLUT_WINDOW_WIDTH) - 1) / 2.0)*(1 + x)); }
inline float ytoWindow(const float& y) { return roundf(((glutGet(GLUT_WINDOW_HEIGHT) - 1) / 2.0)*(1 - y)); }	
inline float xtoScreen(const float& x) { return ( ( x * ( 2.0f / (glutGet(GLUT_WINDOW_WIDTH) - 1) )) - 1.0f); }
inline float ytoScreen(const float& y) { return (1.0f - ( y * ( 2.0f / (glutGet(GLUT_WINDOW_HEIGHT) - 1) )) ); }



//Double Uniform Real Distribution Random Number Generator -- http://www.cplusplus.com/reference/random/uniform_real_distribution/
inline double randd() { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_real_distribution<double> distribution(0.0,1.0); return distribution(generator); }
inline double randd(double max) { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_real_distribution<double> distribution(0.0, max); return distribution(generator); }
inline double randd(double low, double high) { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_real_distribution<double> distribution(low, high); return distribution(generator); }

//Float Uniform Real Distribution Random Number Generator -- http://www.cplusplus.com/reference/random/uniform_real_distribution/
inline float randf() { std::random_device rd; std::mt19937 generator(rd()); std::uniform_real_distribution<float> distribution(0.0,1.0); return distribution(generator); }
inline float randf(float max) { std::random_device rd; std::mt19937 generator(rd()); std::uniform_real_distribution<float> distribution(0.0, max); return distribution(generator); }
inline float randf(float low, float high) { std::random_device rd; std::mt19937 generator(rd()); std::uniform_real_distribution<float> distribution(low, high); return distribution(generator); }

//Integer Uniform Int Distribution Random Number Generator -- http://www.cplusplus.com/reference/random/uniform_int_distribution/
inline int randi() { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_int_distribution<int> distribution(0,1); return distribution(generator); }
inline int randi(int max) { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_int_distribution<int> distribution(0, max); return distribution(generator); }
inline int randi(int low, int high) { std::random_device rd; std::mt19937_64 generator(rd()); std::uniform_int_distribution<int> distribution(low, high); return distribution(generator); }

//Map conversion functions
template<class T> T mapValue(T x, T in_min, T in_max, T out_min, T out_max) { return out_min + floor( ((1.0 * (T)(out_max - out_min) / (T)(in_max - in_min)) * (x - in_min)) + 0.5); }
template<class T> T mapValue(T x, T in_min, T in_max, T out_min, T out_max, int opt) {
	// Options: 
	// 0 - none
	// 1 - roundf()
	// 2 - ceilf()
	// 3 - floorf()

	T slope = 1.0 * (T)(out_max - out_min) / (T)(in_max - in_min);
	switch(opt) 
	{
		case 0: return out_min + ( (slope * (x - in_min)) + 0.5);
		case 1: return out_min + round( (slope * (x - in_min)) + 0.5);
		case 2: return out_min + ceil( (slope * (x - in_min)) + 0.5);
		case 3: return out_min + floor( (slope * (x - in_min)) + 0.5);
		default: return out_min + ( (slope * (x - in_min)) + 0.5);
	}
}

//Console based progress bar
void updateBarProgress(const char* str, int prog, int total)
{
	int percentage = (100.0 / total) * prog;
	printf("\r%s%i of %i [", str, prog, total);
	for (int i = 0; i < 50; i++)
	{
		if (i < percentage / 50) printf(".");
		else printf(" ");
	}
	printf("] %i %%", percentage);
}

inline void updateProgress(const char* str, int prog, int total) { printf("\r%s... %i %%", str, (int)((100.0 / total) * prog)); cout.flush(); }

//Tokenizes the given string using the delimiter value delm
vector<string> split(const string& str, char delm)
{
	vector<string> tokens;
	string token;
	stringstream ss;
	ss.str(str);

	while (getline(ss, token, delm)) {
		if (token != "") {
			tokens.push_back(token);
		}	
	}
	return tokens;
}

vector<string> splitCSV(const string& str)
{
	vector<string> tokens;
	string token;
	stringstream ss;
	ss.str(str);

	string quote_buf;
	bool quote_found = false;

	while (getline(ss, token, ',')) {
		if (token != "") {

			if (quote_found) {
				quote_buf.append(token);

				if (token.back() == '"') {
					quote_found = false;
					token = quote_buf;
				}
				else quote_buf.append(",");
			}
				

			if (token.front() == '"' && token.back() != '"') {
				quote_found = true;
				quote_buf.clear();
				quote_buf.append(token + ',');
			}
			
			if (!quote_found) tokens.push_back(token);
		}
		
	}
	return tokens;
}

//Print Rasterized Text on OpenGL Window
void printStr(std::string str, float x, float y, float z, float r, float g, float b)
{
	int str_length = str.length();

	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < str_length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}

//==========================================================================================================================================================================================================================
//---------------------------------------------------------------------------------------------------Global Structs---------------------------------------------------------------------------------------------------------
//==========================================================================================================================================================================================================================

//Timer class
struct timer {
	chrono::high_resolution_clock::time_point t_begin, t_end, t_lap;

	//Start timer
	inline void start() { t_begin = t_lap = chrono::high_resolution_clock::now(); }

	//Stop timer
	inline void stop() { t_end = chrono::high_resolution_clock::now(); }
	
	//Reset Lap timer
	inline void reset_lap() { t_lap = chrono::high_resolution_clock::now(); }

	//Returns the time without an offset
	inline double time() {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(t_end - t_begin);
		return t.count();
	}

	//Returns the time with the given offset
	inline double time(double offset) {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(t_end - t_begin);
		return t.count() * offset;
	}

	//Returns the time without an offset
	inline float timef() {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(t_end - t_begin);
		return t.count();
	}

	//Returns the time with the given offset
	inline float timef(float offset) {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(t_end - t_begin);
		return t.count() * offset;
	}

	//Returns the lap time without an offset
	inline double lap() {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(chrono::high_resolution_clock::now() - t_lap);
		reset_lap();
		return t.count();
	}

	//Returns the lap time with the given offset
	inline double lap(double offset) {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(chrono::high_resolution_clock::now() - t_lap);
		reset_lap();
		return t.count() * offset;
	}

	//Returns the lap time without an offset
	inline float lapf() {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(chrono::high_resolution_clock::now() - t_lap);
		reset_lap();
		return t.count();
	}

	//Returns the lap time with the given offset
	inline float lapf(float offset) {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(chrono::high_resolution_clock::now() - t_lap);
		reset_lap();
		return t.count() * offset;
	}

	//Returns the current time without an offset
	inline double current() {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(chrono::high_resolution_clock::now() - t_begin);
		return t.count();
	}

	//Returns the current time with the given offset
	inline double current(double offset) {
		chrono::duration<double> t = chrono::duration_cast<chrono::duration<double> >(chrono::high_resolution_clock::now() - t_begin);
		return t.count() * offset;
	}

	//Returns the current time without an offset
	inline float currentf() {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(chrono::high_resolution_clock::now() - t_begin);
		return t.count();
	}

	//Returns the current time with the given offset
	inline float currentf(float offset) {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(chrono::high_resolution_clock::now() - t_begin);
		return t.count() * offset;
	}


};




#endif