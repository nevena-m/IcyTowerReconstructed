/* Copy-paste sa primera sa 7.casa */

#include "../include/texture.h"
#include <stdio.h>
#include <string.h>

GLuint textures[NUM_OF_TEXTURES];

/* Ove promenljive kontrolisu koji se deo teksture mapira na pravougaonik
 * Inicijalno se koriste podrazumevane (0,0) dole levo i (1,1) gore desno
 */
float botLeftX = 0;
float botLeftY = 0;
float topRightX = 1;
float topRightY = 1;

// da li se koristi alpha chanel kada se tekstura ucitava/iscrtava
short int alphaChanel = DONT_USE; 

// koliko treba skalirati teksturu u glMatrixMode(GL_TEXTURE)
float scaleTexX = 1;
float scaleTexY = 1;
float scaleTexZ = 1;

static void errorMsg(char *s){
	printf("%s\n", s);
	exit(EXIT_FAILURE);
}

void loadTextures()
{
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	Image* image = image_init(0, 0);
	
	glGenTextures(NUM_OF_TEXTURES, textures);
	
	createTexture(image, textures, WALL_TEX_LOC, WALL_TEX_IND);
	createTexture(image, textures, STEP_TEX_LOC, STEP_TEX_IND);
	createTexture(image, textures, JEANS_TEX_LOC, JEANS_TEX_IND);
	createTexture(image, textures, SHIRT_TEX_LOC, SHIRT_TEX_IND);
	createTexture(image, textures, FACE_TEX_LOC, FACE_TEX_IND);
	setAlphaChanel(USE);
	createTexture(image, textures, WELCOME_TEX_LOC, WELCOME_TEX_IND);
	createTexture(image, textures, LOST_TEX_LOC, LOST_TEX_IND);
	createTexture(image, textures, HELP_TEX_LOC, HELP_TEX_IND);
	createTexture(image, textures, MENU_TEX_LOC, MENU_TEX_IND);
	createTexture(image, textures, TITLES_TEX_LOC, TITLES_TEX_IND);
	createTexture(image, textures, HURRY_TEX_LOC, HURRY_TEX_IND);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	image_done(image);
}

void createTexture(Image* image, GLuint* textures, const char* fileName, int textureIndex)
{
    image_read(image, fileName);

    glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if(alphaChanel == DONT_USE){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
					image->width, image->height, 0,
					GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	}
	else{		// dodato da cita i alpha chanel
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
                 image->width, image->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	}
}

void freeTextures()
{
	glDeleteTextures(NUM_OF_TEXTURES, textures);
}
void setAlphaChanel(short int value){
	if(value == USE || value == DONT_USE){
		alphaChanel = value;
		return;
	}
	errorMsg("setAlphaChanel(): value can only be USE or DONT_USE");
	
}
short int getAlphaValue(void){
	return alphaChanel;
}

void setTexCoords(float bottomLeftX, float bottomLeftY, float upperRightX, float upperRightY){
	botLeftX = bottomLeftX;
	botLeftY = bottomLeftY;
	topRightX = upperRightX;
	topRightY = upperRightY;
}
void resetTexCoords(void){
	botLeftX = 0;
	botLeftY = 0;
	topRightX = 1;
	topRightY = 1;
}
/* Nakon sto se zavrsi sa skaliranjem teksture treba da se resetuju
 * vrednosti tih promenljivih kako ostale ne bi koristile isto skaliranje
 */
void setTextureScaling(float x, float y, float z){
	scaleTexX = x;
	scaleTexY = y;
	scaleTexZ = z;
}
void resetTextureScaling(void){
	scaleTexX = 1;
	scaleTexY = 1;
	scaleTexZ = 1;
}

