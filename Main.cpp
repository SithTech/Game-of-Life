//==============================================================================//
//---------------------------Game of Life Simulation----------------------------//
//                                                                              //
// Coded by: Travis Stewart                                                     //
//                                                                              //
//------------------------------------------------------------------------------//
//==============================================================================//

//Personal utility functions
#include "Util.h"
#include "Vector.h"
#include "Buffer.h"



//OpenGL Includes
#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OpenGL/gl.h>
#else
#include <windows.h>
#include "GL/glut.h"
#include <gl/gl.h>
#endif


//Frame redraw timer variables
#define CONTINUOUS_DISPLAY true		//Defines whether the frame is continuously updated and displayed
#define MAX_DELAY_TIME 200
#define DELAY_TIME 0

//Define whether using a framebuffer or OpenGL objects
#define USE_FRAMEBUFFER true

//---------------------------------Window Variables---------------------------------
char* window_title_str = "Game of Life Simulation";
float Center_X, Center_Y, Center_Z;

//Window diminsions
int ImageX = 1260;
int ImageY = 720;
int ImageZ = 1000;
//int WINDOW_WIDTH = ImageX;
//int WINDOW_HEIGHT = ImageY;
//int WINDOW_DEPTH = ImageZ;

float Z_NEAR = 0.0001;
float Z_FAR = 1000;

float zoom_factor = 0.8f;

int field_of_view = 90;
int delay_time = DELAY_TIME;
int max_delay_time = MAX_DELAY_TIME;

//------------------------------------FPS Counter-----------------------------------
timer fps_timer;		//Frames Per Second
int fps_delay = 1;
int fps_min = 10000;
int fps_max = 0;
int fps_ct = 0;
int fps = 0;

//------------------------------------SPS Counter-----------------------------------
timer sps_timer;		//Simulation-Steps Per Second
int sps_delay = 1;
int sps_min = 10000;
int sps_max = 0;
int sps_ct = 0;
int sps = 0;

//-----------------------------------PSPS Counter-----------------------------------
timer psps_timer;		//Particle Simulation-Steps Per Second
int psps_delay = 1;
int psps_min = 10000;
int psps_max = 0;
int psps_ct = 0;
int psps = 0;

//----------------------------------Mouse Variables---------------------------------
int global_mouse_x, global_mouse_y, g_mouse_z;	//Global mouse values
bool mouse_left_pressed = false;
bool mouse_right_pressed = false;


//---------------------------------Utility Variables--------------------------------

timer sim_timer;			//Precise timer class. Found in Global.h
string info_str = "";		//Used to display information on the window's title bar
string save_name = "";		//File name used when saving the framebuffer to disk

bool toggle_shift = false;

bool toggle_simulation_info = false;
bool toggle_simulation = false;
bool toggle_integration = true;
bool toggle_collision = true;

bool toggle_look_at = false;
bool toggle_grid = true;
bool toggle_render = true;
bool toggle_render_ground = false;
bool toggle_inverted_normals = false;
bool toggle_wireframe = false;
bool toggle_mesh = false;
bool toggle_centroid = false;
bool toggle_points = false;
bool toggle_normals = false;
bool toggle_vectors = false;
bool toggle_smooth_shading = false;
bool toggle_collision_points = false;
bool toggle_camera_follow = false;
bool toggle_struts = false;
bool toggle_wind = false;

bool toggle_smd_particles = false;


//Setup Rotation matrix
float rotationMatrix[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

//--------------------------------------Camera--------------------------------------

Vector camera(1, 400, 0);
Vector cameraLookAt(0, 0, 0);
Vector cameraUp(0, 0, 1);

Vector grid(1000, -10, 1000);
float grid_spacing = 10;

//--------------------------------------Buffers-------------------------------------
int pixel_offset = 2;

//Framebuffer used to draw the image
Buffer<float> framebuffer(ImageX * pixel_offset, ImageY * pixel_offset, 3);

//Cell buffers
Buffer<bool> cellbuffer = Buffer<bool>(ImageX, ImageY, 2);


Vector offset(0.0f, 0.0f, -200.0f);
Vector center_of_mass;

//--------------------------------------Threads-------------------------------------

//Simulation threads
std::thread sim_thread;

std::vector<std::thread> sim_threads;
int num_threads = 1;

//--------------------------------------Objects-------------------------------------

float color[3];
bool swap_buffer_idx = true;
int survey_generation = 100;	//Survey the population every 100 generations
int alive_ct = 0;
int generation_ct = 0;
int population_ct = 0;
int dot_size = 1;

int num_colonies = 100;


float alive_color[3] = { 1.0, 1.0, 1.0 };
float dead_color[3] = { 0.0, 0.0, 0.0 };


//--------------------------------------Simulation-------------------------------------

float target_frame_time = 12.0f;		//Target frame time for rendering in milliseconds
int sim_delay = 0;//800;					//Simulation delay between simulation steps in microseconds
float maxTimeStep = 1000.0f;			//Maximum number of timesteps to simulate

//Integration variables
float t_sim = 0.0f;
float t_step = 1.0f;
float h = 0.1f;//0.00001f;						//Timestep amount



//=========================================================================================================================
//--------------------------------------------------Function Declarations--------------------------------------------------
//=========================================================================================================================

void simulate(int thrd, int thrd_delay);
void startSim();
void stopSim();

void spawn(int x, int y, int size);
void survey(void);

void initObj();
void clearObj();
void resetObj();
void initStaticObj();
void resetStaticObj();

void glRender(void);
void fbRender(void);
void fbUpdate(int idx);

void display(void);
void mouseMove(int x, int y);
void mouseClick(int btn, int state, int x, int y);
void passiveMouseMove(int x, int y);
void specialKeyboard(int key, int x, int y);
void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
void glViewMatrices(void);
void initGL(void);


//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void initObj() {
	for(int i = 0; i < num_colonies; i++) {
		spawn(randi(cellbuffer.width()), randi(cellbuffer.height()), randi(40));
	}
	survey();

	sim_timer.start();
}

void clearObj() {
	toggle_simulation = false;
	t_sim = 0;

	//Make sure all simulation threads have terminated
	std::this_thread::sleep_for (std::chrono::milliseconds(10));

	swap_buffer_idx = true;

	generation_ct = 0;
	population_ct = 0;

	framebuffer.clear(0);
	cellbuffer.clear(false);
}

void resetObj() {
	clearObj();

	initObj();
	
	//glRender();
	//glutSwapBuffers();
}

void initStaticObj() {

}

void resetStaticObj() {
	stopSim();

	initStaticObj();

	//glRender();
	//glutSwapBuffers();
}


void spawn(int x, int y, int size) {
	swap_buffer_idx = true;
	for (int i = -size; i <= size; i++) {
		for (int j = -size; j<= size; j++) {
			cellbuffer( cellbuffer.wrapX(x + i), cellbuffer.wrapY(y + j), swap_buffer_idx) = true;
		}
	}
}

void survey(void) {
	population_ct = 0;

	for (int j = 0; j < cellbuffer.height(); j++) {
		for (int i = 0; i < cellbuffer.width(); i++) {
			//Survey the currently active buffer
			population_ct += cellbuffer(i, j, swap_buffer_idx);
		}
	}
}


//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================
void startSim() {
	sim_threads.clear();
	for(int i = 0; i < num_threads; i++) {
		sim_threads.push_back(std::thread(simulate, i, 0));
		sim_threads[i].detach();
	}
	toggle_simulation = true;
}

void stopSim() {
	if(toggle_simulation) {
		sim_threads.clear();

		toggle_simulation = false;
		t_sim = 0;

		//Make sure all simulation threads have terminated
		std::this_thread::sleep_for (std::chrono::milliseconds(10));
	}
}

void simulate(int thrd, int thrd_delay) {
	//Thread Variables
	int thrd_sps = 0;
	int thrd_sps_ct = 0;
	timer thrd_sps_timer;
	float thrd_t_sim = 0.0f;
	int thread_check = 0;

	//Start timers
	sim_timer.start();
	thrd_sps_timer.start();

	while(toggle_simulation) {
		sps_ct++;
		thrd_sps_ct++;

		//---------------------------------------------------
		//---------------------------------------------------
		
		if(thrd == 0) {
			//cellbuffer.clearZ(!swap_buffer_idx);
			generation_ct++;
		}

		for (int j = thrd; j < cellbuffer.height(); j+=num_threads) {
			for (int i = 0; i < cellbuffer.width(); i++)
			{
				cellbuffer(i, j, !swap_buffer_idx) = false;

				alive_ct = 0;

				alive_ct += cellbuffer(cellbuffer.wrapX(i - 1), cellbuffer.wrapY(j - 1), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(  i  ), cellbuffer.wrapY(j - 1), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(i + 1), cellbuffer.wrapY(j - 1), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(i - 1), cellbuffer.wrapY(  j  ), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(i + 1), cellbuffer.wrapY(  j  ), swap_buffer_idx); 
				alive_ct += cellbuffer(cellbuffer.wrapX(i - 1), cellbuffer.wrapY(j + 1), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(  i  ), cellbuffer.wrapY(j + 1), swap_buffer_idx);
				alive_ct += cellbuffer(cellbuffer.wrapX(i + 1), cellbuffer.wrapY(j + 1), swap_buffer_idx);

				//Apply conditions to current cell
				if (cellbuffer(i, j, swap_buffer_idx))
				{
					if (alive_ct < 2)
					{
						cellbuffer(i, j, !swap_buffer_idx) = false;
						population_ct--;
					}
					else if (alive_ct == 2 || alive_ct == 3)
					{
						cellbuffer(i, j, !swap_buffer_idx) = true;
					}
					else if (alive_ct > 3)
					{
						cellbuffer(i, j, !swap_buffer_idx) = false;
						population_ct--;
					}
					else
					{
						cellbuffer(i, j, !swap_buffer_idx) = true;
					}
				}
				else if (alive_ct == 3)
				{
					cellbuffer(i, j, !swap_buffer_idx) = true;
					population_ct++;
				}
			}
		}

		
		//---------------------------------------------------
		//Integrate States
		//---------------------------------------------------

		//-------------------------	
		//-------------------------

		//---------------------------------------------------
		//---------------------------------------------------		
		//---------------------------------------------------
		//---------------------------------------------------
		//---------------------------------------------------
		
		//t_sim++;
		t_sim += t_step;
		thrd_t_sim += t_step;
	
		//---------------------------------------------------
		//---------------------------------------------------
		//---------------------------------------------------
		//---------------------------------------------------
		//---------------------------------------------------

		if(thrd == 0) {
			//Check if time to render
			if(sim_timer.current(1000) >= target_frame_time) {
				swap_buffer_idx = !swap_buffer_idx;
				
				
				if(toggle_simulation_info) {
					sim_timer.stop();
					printf("\rFrame Time: %f ms\n", sim_timer.time(1000));
				}

				//Restart the simulation timer
				sim_timer.start();
			}

			//------------------------
			
			//Update simulations per second
			if (sps_ct % sps_delay == 0) {
				sps_timer.stop();
				sps = (1000 / (sps_timer.timef(1000))) * sps_delay;
				sps_min = (sps < sps_min) ? sps : sps_min;
				sps_timer.start();
			}
		}

		//---------------------------------------------------

		//Update thread steps per second
		if (thrd_sps_ct % sps_delay == 0) {
			thrd_sps_timer.stop();
			thrd_sps = (1000 / (thrd_sps_timer.timef(1000))) * sps_delay;
			thrd_sps_timer.start();
		}

		//---------------------------------------------------

		//Limit Simulation rate to maintain a constant display rate
		if(sim_delay != 0) {
			thrd_delay += thrd_sps - sim_delay;
			thrd_delay = (thrd_delay < 0)? 0 : thrd_delay;
			//cout << thrd << " | " << thrd_delay << endl;
			//cout.flush();
		}
		else {
			thrd_delay = 0;
		}

		//---------------------------------------------------

		//Apply simulation thread delay
		std::this_thread::sleep_for (std::chrono::microseconds(thrd_delay));
		//std::this_thread::sleep_for (std::chrono::microseconds(sim_delay));

		//---------------------------------------------------
		//---------------------------------------------------		
		//---------------------------------------------------
		//---------------------------------------------------

	} //END_OF_LOOP
}

//=========================================================================================================================
//---------------------------------------------------Display Function------------------------------------------------------
//=========================================================================================================================

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	fps_ct++;

	//---------------------------------------------------

	//Display information on window's title bar
	info_str = "";
	//info_str += "\t\tTarget Sim Rate: " + to_string(sim_delay) + " us ";

	//info_str += "h: " + to_string(h);

	info_str += "FPS: " + to_string(fps);
	info_str += "\t SPS: " + to_string(sps);

	info_str += "\t\tGen: " + to_string(generation_ct);
	info_str += "\t\tPop: " + to_string(population_ct);
	info_str += "\t\tDraw Size: " + to_string(dot_size);
	//info_str += "\t\t\tPSPS: " + to_string(psps);

		
	//---------------------------------------------------

	glViewMatrices();

	//Camera
	glTranslatef(offset.x, offset.y, offset.z);
	glMultMatrixf(rotationMatrix);
	glRotatef(20, 1, 0, 0);
	
	//---------------------------------------------------
	//---------------------------------------------------

	glutSetWindowTitle(info_str.c_str());

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
	
	
	//Render Objects
	if(USE_FRAMEBUFFER) 
		fbRender();		//Use pixel framebuffer
	else
		glRender();		//Use OpenGL framebuffer
	
	glFinish();
	//Swap framebuffers
	glutSwapBuffers();
	//glFlush();

	//---------------------------------------------------
	//---------------------------------------------------

	//Update frames per second
	if (fps_ct % fps_delay == 0) {
		fps_timer.stop();
		fps = (1000 / (fps_timer.timef(1000))) * fps_delay;
		fps_min = (fps < fps_min) ? fps : fps_min;
		fps_timer.start();
	}

	//---------------------------------------------------
}




//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void glRender() {
	glLineWidth(2);
	glPointSize(5);
	

	//---------------------------------------------------
	//---------------------------------------------------

	//World Grid
	if(toggle_grid) {
		glColor3f(0.0, 0.2, 0.1);
		
		glBegin(GL_LINES);
			//Center Lines
			glVertex3f(-grid.x, grid.y, 0);
			glVertex3f( grid.x, grid.y, 0);
			glVertex3f( 0, grid.y, -grid.z);
			glVertex3f( 0, grid.y,  grid.z);

		for(int i=0; i < grid.x/grid_spacing; i++) {
			glVertex3f(-grid.x + (grid_spacing * i), grid.y, -grid.z);
			glVertex3f(-grid.x + (grid_spacing * i), grid.y,  grid.z);
			glVertex3f( grid.x - (grid_spacing * i), grid.y, -grid.z);
			glVertex3f( grid.x - (grid_spacing * i), grid.y,  grid.z);
		}
		for(int i=0; i < grid.z/grid_spacing; i++) {

			glVertex3f(-grid.x, grid.y, -grid.z + (grid_spacing * i));
			glVertex3f( grid.x, grid.y, -grid.z + (grid_spacing * i));
			glVertex3f(-grid.x, grid.y,  grid.z - (grid_spacing * i));
			glVertex3f( grid.x, grid.y,  grid.z - (grid_spacing * i));
		}
		glEnd();
	}

	//---------------------------------------------------

	//Ground Plane
	if(toggle_render_ground) {
		
	}

	//---------------------------------------------------

	//Object Centroids
	if (toggle_centroid) {
		glColor3f(0.8, 0.2, 0.2);
		glBegin(GL_POINTS);	glVertex3f(center_of_mass.x, center_of_mass.y, center_of_mass.z);	glEnd();
	}

	//---------------------------------------------------

	//Object Vertices
	if (toggle_points) {
		glColor3f(0.0, 0.6, 1.0);
		glBegin(GL_POINTS);

		glEnd();
	}

	//---------------------------------------------------

	//Object Normals
	if(toggle_normals) {
		glColor3f(1, 0.2, 0.2);
		
	}

	//---------------------------------------------------

	//Object Wireframes
	if (toggle_wireframe) {
		glColor3f(0.0, 0.6, 0.4);

	}

	//---------------------------------------------------

	//Draw Object's triangle mesh
	if (toggle_mesh) {
		glColor3f(0.0, 0.6, 0.2);
		
	}

	//---------------------------------------------------

	//Draw Objects
	if(toggle_render) {
		glColor3f(0, 0.6, 0.6);
		//glLineWidth(1);
		glPointSize(3);
		
	}

	//---------------------------------------------------
	//---------------------------------------------------
	//---------------------------------------------------
}

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

// Draws the scene
void fbRender(void) {
	
	fbUpdate(swap_buffer_idx);
	
	//Draws the pixel values from the framebuffer
	glDrawPixels(framebuffer.width(), framebuffer.height(), GL_RGB, GL_FLOAT, framebuffer.data());
	glFlush();
}

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void fbUpdate(int idx) {
	int x, y;
    for (int j = 0; j < cellbuffer.height(); j++){
		for (int i = 0; i < cellbuffer.width(); i++) {
			x = i * pixel_offset;
			y = j * pixel_offset;

			if(cellbuffer(i, j, idx)) {
				for(int r = 0; r < pixel_offset; r++)
				for(int c = 0; c < pixel_offset; c++) {
					framebuffer(x+r, y+c, 0) = alive_color[0];
					framebuffer(x+r, y+c, 1) = alive_color[1];
					framebuffer(x+r, y+c, 2) = alive_color[2];
				}
			}
			else {
				for(int r = 0; r < pixel_offset; r++)
				for(int c = 0; c < pixel_offset; c++) {
					framebuffer(x+r, y+c, 0) = dead_color[0];
					framebuffer(x+r, y+c, 1) = dead_color[1];
					framebuffer(x+r, y+c, 2) = dead_color[2];
				}
			}	
		}
	}
}

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

void fbResize(int width, int height) {
	framebuffer.resize(width * 2, height * 2, 3);
	ImageX = width;
	ImageY = height;
}



//=========================================================================================================================
//--------------------------------------------------Keyboard Function------------------------------------------------------
//=========================================================================================================================

void keyboard(unsigned char key, int x, int y) {
	switch (key)
	{
		//----------------------------------------------------------------------------------------------------------
		//-------------------------------------------------Numbers--------------------------------------------------
		//----------------------------------------------------------------------------------------------------------

		//--------------------------------------------

		case '`': {		//
			
			break;
		}

		//--------------------------------------------
		
		case '1': {		//
			
			break;
		}
		
		//--------------------------------------------

		case '2': {		//
			
			break;
		}

		//--------------------------------------------

		case '3': {		//
			
			break;
		}

		//--------------------------------------------

		case '4': {		//
			
			break;
		}

		//--------------------------------------------

		case '5': {		//
         	
			break;
		}

		//--------------------------------------------

		case '6': {		//
			
			break;
		}

		//--------------------------------------------

		case '7': {		//
			
			break;
		}

		//--------------------------------------------
		
		case '8': {		//
			
			break;
		}

		//--------------------------------------------
		
		case '9': {		//
			
			break;
		}

		//--------------------------------------------

		case '0': {		//
			
			break;
		}

		//----------------------------------------------------------------------------------------------------------
		//-------------------------------------------------Symbols--------------------------------------------------
		//----------------------------------------------------------------------------------------------------------

		//--------------------------------------------

		case '~': {		//

			break;
		}

		//--------------------------------------------

		case '!': {		//
			
			break;
		}
		
		//--------------------------------------------

		case '@': {		//
			
			break;
		}
		
		//--------------------------------------------

		case '#': {		//
			
			break;
		}
		
		//--------------------------------------------

		case '$': {		//

			break;
		}
				
		//--------------------------------------------

		case '%': {		//
			
			break;
		}
				
		//--------------------------------------------

		case '^': {		//
			
			break;
		}
				
		//--------------------------------------------

		case '&': {		//

			break;
		}
				
		//--------------------------------------------

		case '*': {		//

			break;
		}
				
		//--------------------------------------------

		case '(': {		//

			break;
		}
				
		//--------------------------------------------

		case ')': {		//

			break;
		}
		
		//--------------------------------------------

		case '-': {
			dot_size -= (dot_size <= 0)? 0 : 1;
			break;
		}

		case '_': {
			dot_size -= (dot_size < 10)? 0 : 10;
			break;
		}

		//--------------------------------------------
		
		case '=': {
			dot_size += 1;
			break;
		}

		case '+': {
			dot_size += 10;
			break;
		}

		//--------------------------------------------

		case ',': {		//
			h -= (h <= 0)? 0 : 0.00001f;
			break;
		}

		//--------------------------------------------

		case '<': {		//Decrease h
			h -= (h < 0.0001f)? 0 : 0.0001f;
			break;
		}

		//--------------------------------------------

		case '.': {		//
			h += 0.00001f;
			break;
		}

		//--------------------------------------------

		case '>': {		//Increase h
			h += 0.0001f;
			break;
		}

		//--------------------------------------------

		case '/': {		//

			break;
		}

		//--------------------------------------------

		case '?': {		//

			break;
		}

		//----------------------------------------------------------------------------------------------------------
		//-------------------------------------------------Letters--------------------------------------------------
		//----------------------------------------------------------------------------------------------------------

		//--------------------------------------------

		case 'a': {		//
			
			break;
		}

		//--------------------------------------------

		case 'b': {		//
			
			break;
		}

		case 'B': {		//
			
			break;
		}

		//--------------------------------------------

		case 'c': {		//
			clearObj();
			printf("Cleared Cells\n");
			cout.flush();
			break;
		}

		case 'C': {		//
			
			break;
		}

		//--------------------------------------------

		case 'd': {		//
			
			break;
		}

		//--------------------------------------------

		case 'e': {		//
			
			break;
		}

		//--------------------------------------------

		case 'f': {		//	
			
			break;
		}

		//--------------------------------------------
		
		case 'g': {		//
			toggle_grid = !toggle_grid;
			printf("Grid | %s\n", (toggle_grid)? "On" : "Off" );
			cout.flush();
			break;
		}

		case 'G': {		//
			toggle_render_ground = !toggle_render_ground;
			printf("Render Ground Plane | %s\n", (toggle_render_ground)? "On" : "Off" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 'h': {		//
			
			break;
		}

		//--------------------------------------------

		case 'i': {		//Toggle Simulation Info
			toggle_simulation_info = !toggle_simulation_info;
			printf("Simulation Info | %s\n", (toggle_simulation_info)? "On" : "Off" );
			break;
		}

		//--------------------------------------------

		case 'j': {		//
			
			break;
		}

		//--------------------------------------------

		case 'k': {		//
			
			break;
		}

		//--------------------------------------------

		case 'l': {		//		
			
			break;
		}

		//--------------------------------------------
		
		case 'm': {		//
			toggle_mesh = !toggle_mesh;
			printf("Render Mesh | %s\n", (toggle_mesh)? "On" : "Off" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 'n': {		//
			toggle_normals = !toggle_normals;
			printf("Render Normals | %s\n", (toggle_normals)? "On" : "Off" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 'o': {		//
			
			break;
		}

		//--------------------------------------------

		case 'p': {		//
			toggle_points = !toggle_points;
			printf("Render Points | %s\n", (toggle_points)? "On" : "Off" );
			cout.flush();
			break;
		}

		case 'P': {		//
			
			break;
		}

		//--------------------------------------------

		case 'q': {		//
			
			break;
		}

		//--------------------------------------------
		
		case 'r': {		//
			resetObj();
			break;
		}
		
		case 'R': {		//
			toggle_render = !toggle_render;
			printf("Render | %s\n", (toggle_render)? "On" : "Off" );
			cout.flush();

			break;
		}

		//--------------------------------------------

		case 's': {		//
			
			break;
		}

		case 'S': {		//
			
			break;
		}

		//--------------------------------------------

		case 't': {		//
			toggle_integration = !toggle_integration;
			printf("Integration Method | %s\n", (toggle_integration)? "RK4" : "Euler" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 'u': {		//
			
			break;
		}

		//--------------------------------------------

		case 'v': {		//
			
			break;
		}

		//--------------------------------------------

		case 'w': {		//		
			toggle_wireframe = !toggle_wireframe;
			printf("Render Wireframe | %s\n", (toggle_wireframe)? "On" : "Off" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 'x': {		//
			
			break;
		}

		//--------------------------------------------

		case 'y': {		//
			
			break;
		}

		//--------------------------------------------

		case 'z': {		//
			
			break;
		}

		//--------------------------------------------

		//----------------------------------------------------------------------------------------------------------
		//-------------------------------------------------Others---------------------------------------------------
		//----------------------------------------------------------------------------------------------------------

		//--------------------------------------------

		case 32: {		//Space bar
			toggle_simulation = !toggle_simulation;
			if(toggle_simulation) {
				survey();
				startSim();
			}
			else {
				stopSim();
			}
			
			printf("Simulation Status | %s\n", (toggle_simulation)? "Running" : "Stopped" );
			cout.flush();
			break;
		}

		//--------------------------------------------

		case 13: {		//Enter

			break;
		}

		//--------------------------------------------

		case 127: {		//Backspace

			break;
		}

		//--------------------------------------------

		case 9: {		//Tab

			break;
		}

		//--------------------------------------------

		case 27: {		//Esc
			for(int i = 0; i < num_threads; i++) {
				sim_threads[i].~thread();
			}
			exit(0);
			break;
		}

		//--------------------------------------------

		default: {
			printf("Key: %i\n", key);
			break;
		}

	}
	glutPostRedisplay();
}




//=========================================================================================================================
//---------------------------------------------------Special Keyboard------------------------------------------------------
//=========================================================================================================================

void specialKeyboard(int key, int x, int y) {
	float dx = 0;
	float dy = 0;
	float dz = 0;
	float d = grid_spacing;

	switch (key)
	{
		case GLUT_KEY_RIGHT: {		//Right Arrow Key
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
				
			}
			else
				dx += d;
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_LEFT: {		//Left Arrow Key
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
				
			}
			else
				dx -= d;
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_UP: {			//Up Arrow Key
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
				dz -= d;
			}
			else {
				dy += d;
			}
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_DOWN: {		//Down Arrow Key
			if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
				dz += d;
			}
			else {
				dy -= d;
			}
			
			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_F1: {

			break;
		}

		//-------------------------------------------------------------------------------------------------------------------------

		case GLUT_KEY_F4: {
			exit(0);
			break;
		}
	}
	
	glutPostRedisplay();
}




//=========================================================================================================================
//----------------------------------------------------Mouse Movement-------------------------------------------------------
//=========================================================================================================================

//Draws when the mouse is clicked and dragged
void mouseMove(int x, int y) {
	//Corrects the Screen to Window coordinate systems
	y = ImageY - 1 - y;

	//Compute mouse movement
	float dx = (x - global_mouse_x);
	float dy = (y - global_mouse_y);

	//-------------------------Left Mouse Button-------------------------------
	if (mouse_left_pressed) {
		spawn(x, y, dot_size);


		//Rotate matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef(dx, 0, 1, 0);
		glRotatef(dy, -1, 0, 0);

		glMultMatrixf(rotationMatrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, rotationMatrix);
	}

	//-------------------------Right Mouse Button-------------------------------
	if (mouse_right_pressed) {
		//Compute zoom
		if(toggle_look_at) {
			camera.z += zoom_factor * dy;
		}
		else {
			offset.z -= zoom_factor * dy;
		}
	}

	//Update previous mouse position
	global_mouse_x = x;
	global_mouse_y = y;

	if(!CONTINUOUS_DISPLAY) glutPostRedisplay();
}




//=========================================================================================================================
//-----------------------------------------------------Mouse Click---------------------------------------------------------
//=========================================================================================================================

//Draws when the mouse button is clicked
void mouseClick(int btn, int state, int x, int y) {
	//Corrects the Screen to Window coordinate systems
	y = ImageY - 1 - y;

	//Update mouse position
	global_mouse_x = x;
	global_mouse_y = y;

	//-------------------------Left Mouse Button-------------------------------
	if (btn == GLUT_LEFT_BUTTON) {
		if(state == GLUT_ACTIVE_SHIFT) {
			//Toggle mouse button pressed
			if (state == GLUT_DOWN) {
				mouse_left_pressed = true;
				
			}
			else {   //Reset button state
				mouse_left_pressed = false;

			}
		}
		else {
			//Toggle mouse button pressed
			if (state == GLUT_DOWN) {
				mouse_left_pressed = true;
				
				stopSim();
				fbRender();

				//swap_buffer_idx = true;
				spawn(x, y, dot_size);

			}
			else {   //Reset button state
				mouse_left_pressed = false;
			}
		}
	}

	//-------------------------Right Mouse Button-------------------------------
	if (btn == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_ACTIVE_SHIFT) {
			//Toggle mouse button pressed
			if (state == GLUT_DOWN) {
				mouse_left_pressed = true;
				
			}
			else {   //Reset button state
				mouse_left_pressed = false;

			}
		}
		else {
			//Toggle mouse button pressed
			if (state == GLUT_DOWN) {
				mouse_left_pressed = true;
				
			}
			else {   //Reset button state
				mouse_left_pressed = false;

			}
		}
	}

	//-------------------------------------------------------------------------------------------------------------------------

	if (btn == 3) {		//Scroll wheel up
		if(state == GLUT_ACTIVE_SHIFT) {

		}
		else {

		}
	}

	//-------------------------------------------------------------------------------------------------------------------------

	if (btn == 4) {		//Scroll wheel down
		if(state == GLUT_ACTIVE_SHIFT) {

		}
		else {
			
		}
	}

	if(!CONTINUOUS_DISPLAY) glutPostRedisplay();
}




//=========================================================================================================================
//------------------------------------------------Passive Mouse Function---------------------------------------------------
//=========================================================================================================================

void passiveMouseMove(int x, int y) {
	//Corrects the Screen to Window coordinate systems
	y = ImageY - 1 - y;

	//Update mouse position
	global_mouse_x = x;
	global_mouse_y = y;

	if(!CONTINUOUS_DISPLAY) glutPostRedisplay();
}




//=========================================================================================================================
//----------------------------------------------------Reshape Function-----------------------------------------------------
//=========================================================================================================================

void reshape(int width, int height) {
	//if(USE_FRAMEBUFFER) {
	//	fbResize(width, height);
	//	cellbuffer.resize(width, height, 2);
	//}
	glViewMatrices();
}

//=========================================================================================================================
//--------------------------------------------------Frame Redraw Timer-----------------------------------------------------
//=========================================================================================================================

void updateFrameTimer(int value) {
	if(CONTINUOUS_DISPLAY) glutPostRedisplay();
	glutTimerFunc(delay_time, updateFrameTimer, 0);
}

void idle(void) {
	glutPostRedisplay();
}

//=========================================================================================================================
//--------------------------------------------------OpenGL View Matrices---------------------------------------------------
//=========================================================================================================================

void glViewMatrices(void) {
	// Set up viewing matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(field_of_view, float(ImageX) / ImageY, Z_NEAR, Z_FAR);

	#ifdef __APPLE__
		//Check if you are using Mac OSX with a Retina display.
		if((float)glutGet(GLUT_SCREEN_WIDTH) / glutGet(GLUT_SCREEN_HEIGHT) == 1.6f) {
			//Mac OSX Retina displays use a point system instead of pixels, where 1 point equals 2 pixels.
			//Since OpenGL uses a pixel system, we need to convert from points to pixels.
			glViewport(0, 0, ImageX * 2, ImageY * 2);	
		}	
		else {
			//Normal displays are not effected by Apple's insanity, so we don't need to change anything.
			glViewport(0, 0, ImageX, ImageY);
		}
	#else
		glViewport(0, 0, ImageX, ImageY);
	#endif
	
	//Origin set to top left of window
	//glOrtho(0.0, ImageX - 1, ImageY - 1, 0, -ImageZ, ImageZ);

	//Origin set to bottom left of window
	//glOrtho(0.0, ImageX - 1, 0, ImageY - 1, -ImageZ, ImageZ);
	

	//Origin set to center of window
	//glViewport(0, 0, ImageX, ImageY);
	glOrtho((-1 * ImageX / 2.0f), (ImageX / 2.0f), (-1 * ImageY / 2.0f), (ImageY / 2.0f), ImageZ, -ImageZ);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}

//=========================================================================================================================
//----------------------------------------------------Init Functions-------------------------------------------------------
//=========================================================================================================================

void initGL(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	glViewMatrices();

	//---------------------------------lighting Setup---------------------------------

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(passiveMouseMove);
	glutReshapeFunc(reshape);
	//glutTimerFunc(delay_time, updateFrameTimer, 0);
	glutIdleFunc(idle);

}

//=========================================================================================================================
//---------------------------------------------------------Main------------------------------------------------------------
//=========================================================================================================================

int main(int argc, char** argv) {
	initObj();
	initStaticObj();

	//------------------------------------OpenGL-------------------------------------

	//----------------Create Window----------------
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	#ifdef __APPLE__
		if(USE_FRAMEBUFFER) glutInitWindowSize(ImageX, ImageY);
		else glutInitWindowSize(ImageX, ImageY);
	#else
		glutInitWindowSize(ImageX, ImageY);
	#endif

	
	glutInitWindowPosition(100, 100);
	glutCreateWindow(window_title_str);
	
	//Initialize the OpenL Window
	initGL();

	//Start the fps frame timer
	fps_timer.start();
	sps_timer.start();
	
	glutMainLoop();

	return 0;
}

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================

//=========================================================================================================================
//-------------------------------------------------------------------------------------------------------------------------
//=========================================================================================================================
