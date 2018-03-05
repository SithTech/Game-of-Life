/*
CSCE 441 Assignment 6
Completed by: Travis Stewart
*/

#ifndef GLOABL_H
#define GLOABL_H

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
#include <chrono>
#include <map>
#include <unordered_map>

//OpenGL Includes
#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OpenGL/gl.h>
#else
#include <windows.h>
#include "GL/glut.h"
#include <gl/gl.h>
#endif

//#define ImageW 1000
//#define ImageH 1000
//#define ImageZ 1000

#define MAX_FLOAT MAXINT64		//Used for the default Z-Buffer value

#define WINDOW_CENTER_X glutGet(GLUT_WINDOW_WIDTH)/2
#define WINDOW_CENTER_Y glutGet(GLUT_WINDOW_HEIGHT)/2
#define RAD_TO_DEG 57.2958
#define DEG_TO_RAD 0.0114533
#define PI 3.14159265359

using namespace std;

//X to Window --> return as float
float xtoWindow(const float& x);	

//Y to Window --> return as float
float ytoWindow(const float& y);	

float xtoScreen(const float& x);
float ytoScreen(const float& y);

void updateBarProgress(const char* str, int prog, int total);
void updateProgress(const char* str, int prog, int total);

//Tokenizes the given string using the delimiter value delm
vector<string> split(const string& str, char delm);

void printStr(std::string str, float x, float y, float z, float r, float g, float b);

//==========================================================================================================================================================================================================================
//---------------------------------------------------------------------------------------------------Global Structs---------------------------------------------------------------------------------------------------------
//==========================================================================================================================================================================================================================

struct timer {
	chrono::high_resolution_clock::time_point t_start, t_end;

	void start() { t_start = chrono::high_resolution_clock::now(); }
	void end() { t_end = chrono::high_resolution_clock::now(); }

	float get_time() {
		chrono::duration<float> t = chrono::duration_cast<chrono::duration<float> >(t_end - t_start);
		return t.count();
	}
};




#endif