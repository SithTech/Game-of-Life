/*
	Code by: Travis Stewart

	This is my attempt at implementing Conway's Game of Life using OpenGL.
	
*/

#include "Global.h"
#include "Vector.h"

//OpenGL Includes
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <windows.h>
#include "GL/glut.h"
#include <gl/gl.h>
#endif

//#include <omp.h>


#define ImageW 1000
#define ImageH 1000
#define ImageZ 1000

#define MAX_UNIT_TIME 200
#define UNIT_TIME 0//16.67		//~60Hz

using namespace std;

//=========================================================================================================================
//----------------------------------------------------Define Variables-----------------------------------------------------
//=========================================================================================================================

//-------------------------------Window Variables-------------------------------
int WINDOW_WIDTH = ImageW;
int WINDOW_HEIGHT = ImageH;
float WINDOW_DEPTH = ImageZ;
float Z_NEAR = 0.0001;
float Z_FAR = 1000;
int field_of_view = 90;
int unit_time = UNIT_TIME;
int max_unit_time = MAX_UNIT_TIME;


//-------------------------------Mouse Variables-------------------------------
int mouse_X, mouse_Y, mouse_Z;
bool mouse_left_pressed = false;
bool mouse_right_pressed = false;


//-------------------------------Draw Variables-------------------------------
bool toggle_simulation = true;
bool swap_buffers = true;


//-------------------------------Utility Variables-------------------------------
timer t;
string info_str = "";

//-------------------------------Object Data-------------------------------

float color[3];
int alive_ct = 0;
int generation_ct = 0;
int population_ct = 0;
int dot_size = 1;
int dh = 0; 
int dw = 0;

float framebuffer[ImageH][ImageW][3];
float genbuffer_1[ImageH][ImageW][3];
float genbuffer_2[ImageH][ImageW][3];



//=========================================================================================================================
//-----------------------------------------------------Clear Frame Buffer--------------------------------------------------
//=========================================================================================================================

// Clears framebuffer to black
void clearFramebuffer()
{
	int i, j;
	for (i = 0; i<ImageH; i++) {
		for (j = 0; j<ImageW; j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}


void clearbuffer(float (&buf)[ImageH][ImageW][3])
{
	int i, j;
	for (i = 0; i<ImageH; i++) {
		for (j = 0; j<ImageW; j++) {
			buf[i][j][0] = 0.0;
			buf[i][j][1] = 0.0;
			buf[i][j][2] = 0.0;
		}
	}
}

//=========================================================================================================================
//-----------------------------------------------------Set Frame Buffer----------------------------------------------------
//=========================================================================================================================

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;
	if (R <= 1.0)
		if (R >= 0.0)
			framebuffer[y][x][0] = R;
		else
			framebuffer[y][x][0] = 0.0;
	else
		framebuffer[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			framebuffer[y][x][1] = G;
		else
			framebuffer[y][x][1] = 0.0;
	else
		framebuffer[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			framebuffer[y][x][2] = B;
		else
			framebuffer[y][x][2] = 0.0;
	else
		framebuffer[y][x][2] = 1.0;
}


void setbuffer(float (&buf)[ImageH][ImageW][3], int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	//y = ImageH - 1 - y;
	if (R <= 1.0)
		if (R >= 0.0)
			buf[y][x][0] = R;
		else
			buf[y][x][0] = 0.0;
	else
		buf[y][x][0] = 1.0;
	if (G <= 1.0)
		if (G >= 0.0)
			buf[y][x][1] = G;
		else
			buf[y][x][1] = 0.0;
	else
		buf[y][x][1] = 1.0;
	if (B <= 1.0)
		if (B >= 0.0)
			buf[y][x][2] = B;
		else
			buf[y][x][2] = 0.0;
	else
		buf[y][x][2] = 1.0;
}


//=========================================================================================================================
//-----------------------------------------------------Draw Function-------------------------------------------------------
//=========================================================================================================================

// Draws the scene
void drawit(void)
{
	glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, framebuffer);
	glFlush();
}

void drawbuffer(float(&buf)[ImageH][ImageW][3])
{
	glDrawPixels(ImageW, ImageH, GL_RGB, GL_FLOAT, buf);
	glFlush();
}

void simulate(float(&buf_1)[ImageH][ImageW][3], float(&buf_2)[ImageH][ImageW][3])
{
	clearbuffer(buf_2);
	generation_ct++;

	for (int h = 0; h < ImageH; h++) {
		for (int w = 0; w < ImageW; w++) {
			alive_ct = 0;

			alive_ct += buf_1[h - 1][w - 1][2];
			alive_ct += buf_1[h - 1][  w  ][2];
			alive_ct += buf_1[h - 1][w + 1][2];
			alive_ct += buf_1[  h  ][w - 1][2];
			alive_ct += buf_1[  h  ][w + 1][2];
			alive_ct += buf_1[h + 1][w - 1][2];
			alive_ct += buf_1[h + 1][  w  ][2];
			alive_ct += buf_1[h + 1][w + 1][2];

			//Apply conditions to current cell
			if (buf_1[h][w][2] == 1)
			{
				if (alive_ct < 2)
				{
					setbuffer(buf_2, w, h, 0, 0, 0);
					population_ct--;
				}
				else if (alive_ct == 2 || alive_ct == 3)
				{
					setbuffer(buf_2, w, h, 1, 1, 1);
				}
				else if (alive_ct > 3)
				{
					setbuffer(buf_2, w, h, 0, 0, 0);
					population_ct--;
				}
				else
				{
					setbuffer(buf_2, w, h, 1, 1, 1);
				}
			}
			else if (alive_ct == 3)
			{
				setbuffer(buf_2, w, h, 1, 1, 1);
				population_ct++;
			}

		}
	}

}

//=========================================================================================================================
//---------------------------------------------------Display Function------------------------------------------------------
//=========================================================================================================================

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	info_str = "Generations: " + to_string(generation_ct) + "    Population: " + to_string(population_ct) + "    Refresh Delay: " + to_string(unit_time) + " ms";
	glutSetWindowTitle(info_str.c_str());

	if (toggle_simulation)
	{
		if (swap_buffers)
		{
			simulate(genbuffer_1, genbuffer_2);
			drawbuffer(genbuffer_2);
		}
		else
		{
			simulate(genbuffer_2, genbuffer_1);
			drawbuffer(genbuffer_1);
		}
		swap_buffers = !swap_buffers;
	}
	else
	{
		//drawbuffer(genbuffer_2);
		drawbuffer(genbuffer_1);	//Buffer that the user draws on
	}
}

//=========================================================================================================================
//----------------------------------------------------Mouse Movement-------------------------------------------------------
//=========================================================================================================================

//Draws when the mouse is clicked and dragged
void mouseMove(int x, int y)
{
	y = ImageH - 1 - y;
	//Compute mouse movement
	float dx = (x - mouse_X);
	float dy = (y - mouse_Y);

	//-------------------------Left Mouse Button-------------------------------
	if (mouse_left_pressed) {
		for (int i = -dot_size; i <= dot_size; i++) {
			for (int j = -dot_size; j<= dot_size; j++) {
				if (swap_buffers)
				{
					setbuffer(genbuffer_1, x + i, y + j, 1, 1, 1);
				}	
				else
				{
					setbuffer(genbuffer_2, x + i, y + j, 1, 1, 1);
				}
					
			}
		}
		
	}

	//-------------------------Right Mouse Button-------------------------------
	if (mouse_right_pressed) {

	}

	//Update previous mouse position
	mouse_X = x;
	mouse_Y = y;
	
	glutPostRedisplay();
}

//=========================================================================================================================
//-----------------------------------------------------Mouse Click---------------------------------------------------------
//=========================================================================================================================

//Draws when the mouse button is clicked
void mouseClick(int btn, int state, int x, int y)
{
	y = ImageH - 1 - y;
	//Update mouse position
	mouse_X = x;
	mouse_Y = y;

	//-------------------------Left Mouse Button-------------------------------
	if ( btn == GLUT_LEFT_BUTTON ) {

		//Toggle mouse button pressed
		if (state == GLUT_DOWN) {
			mouse_left_pressed = true;

			for (int i = -dot_size; i <= dot_size; i++) {
				for (int j = -dot_size; j <= dot_size; j++) {
					//if (swap_buffers)
					{
						setbuffer(genbuffer_1, x + i, y + j, 1, 1, 1);
					}
					//else
					{
						setbuffer(genbuffer_2, x + i, y + j, 1, 1, 1);
					}
				}
			}

		}
		else {	//Reset button state
			mouse_left_pressed = false;
		}
	}

	//-------------------------Right Mouse Button-------------------------------
	if ( btn == GLUT_RIGHT_BUTTON ) {	
		
		//Toggle mouse button pressed
		if (state == GLUT_DOWN) {
			mouse_right_pressed = true;
		}
		else {	//Reset button state
			mouse_right_pressed = false;
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------

	if (btn == 3) {		//Scroll wheel up
		dot_size += 1;
	}

	//-------------------------------------------------------------------------------------------------------------------------

	if (btn == 4) {		//Scroll wheel down
		dot_size -= 1;
		if (dot_size < 1) dot_size = 1;
	}

	glutPostRedisplay();
}

//=========================================================================================================================
//------------------------------------------------Passive Mouse Function---------------------------------------------------
//=========================================================================================================================

void passiveMouseMove(int x, int y)
{
	y = ImageH - 1 - y;
	//Update mouse position
	mouse_X = x;
	mouse_Y = y;

	//glutPostRedisplay();
}


//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void specialKeyboard(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_RIGHT: {
			
			break;
		}
		
		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_LEFT: {
			
			break;
		}
		
		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_UP: {
			dot_size += 1;
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_DOWN: {
			dot_size -= 1;
			if(dot_size < 1) dot_size = 1;
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------
		
		case GLUT_KEY_F1: {		//Toggle GameMode
			/*
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE)) {
				glutLeaveGameMode();
				window_id = glutCreateWindow("DnD Map");
				init();
			}
			else {
				glutGameModeString("");
				if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
					glutDestroyWindow(window_id);
					glutEnterGameMode();
					init();
				}
			}
			*/
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------
		
		case GLUT_KEY_F4: {
			exit(0);
			break;
		}
	}

	//glutPostRedisplay();
}

//=========================================================================================================================
//--------------------------------------------------Keyboard Function------------------------------------------------------
//=========================================================================================================================

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case '1': {		//Change material | Green
			drawbuffer(genbuffer_1);
			break;
		}

		case '2': {		//Change material | Blue
			drawbuffer(genbuffer_2);
			break;
		}

		case '3': {		//Change material | Red
			drawbuffer(framebuffer);
			break;
		}

		

		//---------------------------------Draw Style Options---------------------------------

		case 't': {		
			toggle_simulation = !toggle_simulation;
			break;
		}

		case 'p': {		
			
			break;
		}

		case 'c': {		
			toggle_simulation = false;
			swap_buffers = true;

			generation_ct = 0;
			population_ct = 0;

			clearFramebuffer();
			clearbuffer(genbuffer_1);
			clearbuffer(genbuffer_2);

			break;
		}

		case 'r': {		//Assign each pixel to a random color
			swap_buffers = true;
			population_ct = 0;
			clearbuffer(genbuffer_1);
			clearbuffer(genbuffer_2);
			for (int h = 0; h < ImageH; h++) {
				for (int w = 0; w < ImageW; w++) {
					if (rand() % 10 > rand() % 10) {
						setbuffer(genbuffer_1, w, h, 1.0, 1.0, 1.0);
						population_ct++;
					}
					else {
						setbuffer(genbuffer_1, w, h, 0.0, 0.0, 0.0);
					}
					
				}
			}
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------
		
		case '_':
		case '-': {
			unit_time += 1;
			//if (unit_time > 100) unit_time = 100;

			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------
		
		case '=':
		case '+': {
			unit_time -= 1;
			if (unit_time < 0) unit_time = 0;

			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------
	
		default: {
			printf("%i\n", key);
			break;
		}

	}
	glutPostRedisplay();
}

//=========================================================================================================================
//----------------------------------------------------Reshape Function-----------------------------------------------------
//=========================================================================================================================

void reshape(int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gluPerspective(field_of_view, float(WINDOW_WIDTH) / WINDOW_HEIGHT, Z_NEAR, Z_FAR);
	glOrtho(-1 * WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f, -1 * WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f, -1 * WINDOW_DEPTH / 2.0f, WINDOW_DEPTH / 2.0f);
	glMatrixMode(GL_MODELVIEW);
}

void updateFrameTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(unit_time, updateFrameTimer, 0);
}

//=========================================================================================================================
//-----------------------------------------------------Init Function-------------------------------------------------------
//=========================================================================================================================

void init(void)
{
	
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	gluPerspective(field_of_view, float(WINDOW_WIDTH) / WINDOW_HEIGHT, Z_NEAR, Z_FAR);
	glOrtho(-1 * WINDOW_WIDTH / 2.0f, WINDOW_WIDTH / 2.0f, -1 * WINDOW_HEIGHT / 2.0f, WINDOW_HEIGHT / 2.0f, -1 * WINDOW_DEPTH / 2.0f, WINDOW_DEPTH / 2.0f);
	glMatrixMode(GL_MODELVIEW);
	

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(passiveMouseMove);
	glutReshapeFunc(reshape);
	glutTimerFunc(unit_time, updateFrameTimer, 0);

	clearbuffer(genbuffer_1);
	for (int h = 0; h < ImageH; h++) {
		for (int w = 0; w < ImageW; w++) {
			if (rand() % 10 > 5) {
				setbuffer(genbuffer_1, w, h, 1.0, 1.0, 1.0);
				population_ct++;
			}
			else {
				setbuffer(genbuffer_1, w, h, 0.0, 0.0, 0.0);
			}
					
		}
	}

}

//=========================================================================================================================
//-----------------------------------------------------Main Function-------------------------------------------------------
//=========================================================================================================================

int main(int argc, char** argv)
{
	
	//---------------------------------Create Window---------------------------------

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Game of Life");
	init();

	glutMainLoop();

	return 0;
}
