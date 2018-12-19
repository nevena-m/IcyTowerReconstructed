#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "../include/global.h"
#include "../include/genfuncs.h"
#include "../include/texture.h"
#include "../include/character.h"
#include "../include/wall.h"
#include "../include/animation.h"
#include "../include/step.h"
#include "../include/collision.h"
#include "../include/display.h"
#include "../include/shapes.h"

int window_h = 650;
int window_w = 900;
static const int window_pos_x = 0;
static const int window_pos_y = 0;

float camera_pos_x = INIT_CAMERA_X;
float camera_pos_y = INIT_CAMERA_Y;
float camera_pos_z = INIT_CAMERA_Z;

float look_at_x = INIT_LOOK_X;
float look_at_y = INIT_LOOK_Y;
float look_at_z = INIT_LOOK_Z;

double dtime; // za racunanje vremena proteklog izmedju dva frejma
double lastTime;

void init_global(void);
void on_display(void);
void on_reshape(int width, int height);
void error_callback(int error, const char* description);

short int keys[NUM_OF_KEYS];

int main(int argc, char **argv){
	/* Inicijalizacija glut-a */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(window_w, window_h);
	glutInitWindowPosition(window_pos_x, window_pos_y);
	glutCreateWindow("Icy Tower");
	
	if (!glfwInit()){
		printf("\nGLFW Initialization Failed\n");
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);
	glfwSetTime(0);
	lastTime = glfwGetTime();
	init_global();
	glutFullScreen(); 		// Na pocetku neka odmah ode u full screens
	glutMainLoop();			// Ulazimo u glavnu petlju
	
	return 0;
}
void error_callback(int error, const char* description){
	fprintf(stderr, "GLFW Error: %s\n", description);
	exit(EXIT_FAILURE);
}
void init_global(void){
	
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	
	glutKeyboardFunc(on_keyboard);	// obrada dugmica
	glutReshapeFunc(on_reshape);	// obrada pri menjanju dimenzija 
	glutDisplayFunc(on_display);	// obrada prikaza scene za trenutni prozor
	glutSpecialFunc(special_keys); // obrada specijalnih dugmica (strelice)
	glutSpecialUpFunc(specialKeyUp);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	
	glClearColor(0.8, 0.8, 0.8, 0);
	
	loadTextures();			 // Ucitavamo sve teksture na pocetku 
	glEnable(GL_TEXTURE_2D); // Omogucavamo koriscenje 2D struktura
	
	glEnable(GL_DEPTH_TEST); // Omogucavamo proveru i azuriranje bafera dubine
	
	glEnable(GL_LIGHTING);		// Omogucavamo osvetljenje svene
	glEnable(GL_LIGHT0);
	
	// Podesavamo parametre osvetljenja
	
	GLfloat light_ambient[] = {0.6,0.6,0.6, 1 };
	GLfloat light_diffuse[] = { 1, 1, 1, 1 };
	GLfloat light_specular[] = {1, 1, 1, 1 };
	GLfloat model_ambient[] = { 0.8, 0.8, 0.8, 1 };
	GLfloat light_position[] = { 1.5, 1, 1, 0 }; //direkciono svetlo
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
	
	/* Inicijalizacija sastavnih cinilaca igrice */
	animation = init_animation();
	Harold = init_character(INIT_CHARACTER_X, INIT_CHARACTER_Y, 0);
	gameplayWall_1 = newWallPos(INIT_WALL_X_1, INIT_WALL_Y_1, INIT_WALL_Z_1);
	gameplayWall_2 = newWallPos(INIT_WALL_X_2, INIT_WALL_Y_2, INIT_WALL_Z_2);
	
	steps = allocate_steps();
	
	stepsHalf = SND_HALF;
	switchWalls = SWITCH_FST;
	
	topStepOnScreen = INIT_TOP_INDEX;
	botStepOnScreen = INIT_BOT_INDEX;
	
	generateNewSteps(steps, INIT_FIRST_STEP_Y, 0, NUM_OF_STEPS/2);
	
	initKeys();
	initQuads();
}

void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(
		camera_pos_x, camera_pos_y, camera_pos_z,
		look_at_x, look_at_y,  look_at_z,
		0, 1,  0
	);
	
	if(activeSceneIndex == PAUSE_INDEX)
		pauseGame(animation);
	
	if(activeSceneIndex == GAMEPLAY_INDEX || activeSceneIndex == PAUSE_INDEX || activeSceneIndex == LOSE_INDEX){
		drawGameplayScene();
	}
	drawBackground(activeSceneIndex);
	printSceneText(activeSceneIndex);
	glutSwapBuffers();
}
void on_reshape(int width, int height){
	glViewport(0,0,width,height);
	window_h = height;
	window_w = width;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float) width/height, 1, 50);
}
