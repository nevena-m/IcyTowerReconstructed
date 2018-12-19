#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>

/* Ispis pomocnih parametara tokom razvoja programa */
#define DEBUG

/* Inicijalne pozicije kamere */
#define INIT_CAMERA_X 0
#define INIT_CAMERA_Y 0
#define INIT_CAMERA_Z 20

/* Inicijalne pozicije tacke u koju kamera gleda */
#define INIT_LOOK_X 0
#define INIT_LOOK_Y 0
#define INIT_LOOK_Z 0

#define GRAVITY (-9.81)
#define EPSILON 0.001

#define GAME_LOST -100

/* Da bih se resila baga koji se desava pri kretanju koristicu niz
 * u kome je 1 ako se dugme drzi pritisnuto, odnosno 0 inace.
 * Dozvoljeni tasteri su esc space gore dole levo desno i koristicu
 * svoje kodove za njih
 */
#define NUM_OF_KEYS 2
#define K_LEFT 0
#define K_RIGHT 1

extern short int keys[NUM_OF_KEYS];

extern short int activeSceneIndex;
extern short int cursorPosition;

/* Pozicije kamere i tacke pogleda */
extern int window_h;
extern int window_w;

extern float camera_pos_x;
extern float camera_pos_y;
extern float camera_pos_z;

extern float look_at_x;
extern float look_at_y;
extern float look_at_z;

extern double lastTime;
extern double dtime;
/* Promenljive koje ce cuvati prethodnu poziciju kamere
 * Potrebne jer se nakon pokrenute animacije ispomeraju i ostale scene
 * POPRAVITI OVO...
 */
extern float prev_cam_y;
extern float prev_look_y;
#endif
