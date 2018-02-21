/*

	Code by: Travis Stewart
		
	This was originally created as a simple helper library that I could use in my computer graphics course. 
	It contains various funtions I used in many assignments and didn't want to remake for each one.

*/

#include "Global.h"


//=========================================================================================================================
//----------------------------------------------Convert to Window Coordinates----------------------------------------------
//=========================================================================================================================

float xtoWindow(const float& x)	//X to Window --> return as float
{
	return roundf(((glutGet(GLUT_WINDOW_WIDTH) - 1) / 2.0)*(1 + x));
}

float ytoWindow(const float& y)	//Y to Window --> return as float
{
	return roundf(((glutGet(GLUT_WINDOW_HEIGHT) - 1) / 2.0)*(1 - y));
}

float xtoScreen(const float& x)	//X to Window --> return as float
{
	return ( ( x * ( 2.0f / (glutGet(GLUT_WINDOW_WIDTH) - 1) )) - 1.0f);
}

float ytoScreen(const float& y)	//Y to Window --> return as float
{
	return (1.0f - ( y * ( 2.0f / (glutGet(GLUT_WINDOW_HEIGHT) - 1) )) );
}


//=========================================================================================================================
//--------------------------------------------------Progress Functions-----------------------------------------------------
//=========================================================================================================================

void updateBarProgress(const char* str, int prog, int total)
{
	int percentage = 100.0 / total*prog;
	printf("\r%s%i of %i [", str, prog, total);
	for (int i = 0; i < 50; i++)
	{
		if (i < percentage / 50) printf(".");
		else printf(" ");
	}
	printf("] %i %%", percentage);
}

void updateProgress(const char* str, int prog, int total)
{
	//printf("\r%s%i of %i ... %i %%", str, prog, total, (int)(100.0 / total*prog));
	printf("\r%s... %i %%", str, prog, total, (int)(100.0 / total*prog));
}

//=========================================================================================================================
//------------------------------------------------------Tokenizer----------------------------------------------------------
//=========================================================================================================================

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

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void printStr(std::string str, float x, float y, float z, float r, float g, float b) 
{
	int str_length = str.length();

	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < str_length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
	}
}


//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================
