/* Copy-paste sa primera sa 7.casa */
#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <GL/glut.h>
#include "image.h"

#define USE 1
#define DONT_USE 0

#define NUM_OF_TEXTURES 11
#define WALL_TEX_LOC "assets/wall_pattern.bmp" 
#define STEP_TEX_LOC "assets/step_pattern.bmp"
#define JEANS_TEX_LOC "assets/denim.bmp"
#define SHIRT_TEX_LOC "assets/shirt.bmp" 
#define FACE_TEX_LOC "assets/face.bmp"
#define WELCOME_TEX_LOC "assets/welcome_text.bmp"
#define LOST_TEX_LOC "assets/gameLost_text.bmp"
#define HELP_TEX_LOC "assets/help_text.bmp"
#define MENU_TEX_LOC "assets/menu_text.bmp"
#define TITLES_TEX_LOC "assets/titles_text.bmp"
#define HURRY_TEX_LOC "assets/hurry_text.bmp"

#define WALL_TEX_IND 0
#define STEP_TEX_IND 1 
#define JEANS_TEX_IND 2
#define SHIRT_TEX_IND 3
#define FACE_TEX_IND 4
#define WELCOME_TEX_IND 5
#define LOST_TEX_IND 6
#define HELP_TEX_IND 7
#define MENU_TEX_IND 8
#define TITLES_TEX_IND 9
#define HURRY_TEX_IND 10

GLuint textures[NUM_OF_TEXTURES];

extern float botLeftX;
extern float botLeftY;
extern float topRightX;
extern float topRightY;

void createTexture(Image* image, GLuint* textures, const char* fileName, int textureIndex);

void loadTextures();
void freeTextures();

/* Postavlja promenljive koje se koriste za mapiranje tekture na objekte.
 * Od datih koordinata se izvodi oblik pravougaonika. */
void setTexCoords(float bottomLeftX, float bottomLeftY, float upperRightX, float upperRightY);
/* Postavlja promenljive na njihovu inicijalnu vrednost
 */
void resetTexCoords(void); // vraca na (0,0) (1,1)
void setAlphaChanel(short int value); // postavlja vrednost alphaChanel na USE ili DONT_USE
short int getAlphaValue(void); // vraca trenutnu vrednost alphaChanel-a
void setTextureScaling(float x, float y, float z);
void resetTextureScaling(void);

#endif
