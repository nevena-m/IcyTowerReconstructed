#include "../include/global.h"
#include "../include/shapes.h"
#include "../include/texture.h"
GLUquadric *quad;
GLUquadric *cilinderQuad;

GLfloat const white[] = {0, 0, 0};
GLfloat const black[] = {1, 1, 1};
/* Koristimo promenljive koje odredjuju deo teksurure koristimo, definisane u texture.h
float botLeftX;
float botLeftY;
float topRightX;
float topRightY;
*/
void drawTexQuad(short int texture_index, 
	float scaleX, float scaleY,
	float w, float h, float l
){
	/* scaleX puta po x osi, analogno za y.
	 * http://users.polytech.unice.fr/~buffa/cours/synthese_image/DOCS/Tutoriaux/Nehe/opengl.htm 
	 */
	restartColors();
	glDisable(GL_BLEND); 
	glBindTexture(GL_TEXTURE_2D, textures[texture_index]);
	
	float x = w/2; float y = h/2; float z = l/2;
	
	glBegin(GL_QUADS); // idu prvo koordinate za teksturu onda tacka za koju se vezuju
		glNormal3f( 0.0f, 0.0f, 1.0f); // Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);	// Bottom Left
		glTexCoord2f(scaleX, 0.0f); glVertex3f( x, -y,  z);	// Bottom Right
		glTexCoord2f(scaleX, scaleY/w); glVertex3f( x,  y,  z);	// Top Right
		glTexCoord2f(0.0f, scaleY/w); glVertex3f(-x,  y,  z);	// Top Left
		glNormal3f(0.0f, 0.0f, -1.0f); // Back Face
		glTexCoord2f(scaleX, 0.0f); glVertex3f(-x, -y, -z);	// Bottom Right
		glTexCoord2f(scaleX, scaleY); glVertex3f(-x,  y, -z);	// Top Right
		glTexCoord2f(0.0f, scaleY); glVertex3f( x,  y, -z);	// Top Left
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);	// Bottom Left
		glNormal3f( 0.0f, 1.0f, 0.0f);  // Top Face
		glTexCoord2f(0.0f, scaleY); glVertex3f(-x,  y, -z);	// Top Left
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,  y,  z);	// Bottom Left
		glTexCoord2f(scaleX, 0.0f); glVertex3f( x,  y,  z);	// Bottom Right
		glTexCoord2f(scaleX, scaleY); glVertex3f( x,  y, -z);	// Top Right
		glNormal3f( 0.0f, -1.0f, 0.0f); // Bottom Face
		glTexCoord2f(scaleX, scaleY); glVertex3f(-x, -y, -z);	// Top Right
		glTexCoord2f(0.0f, scaleY); glVertex3f( x, -y, -z);	// Top Left
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);	// Bottom Left
		glTexCoord2f(scaleX, 0.0f); glVertex3f(-x, -y,  z);	// Bottom Right
		glNormal3f( 1.0f, 0.0f, 0.0f); // Right face
		glTexCoord2f(scaleX, 0.0f); glVertex3f( x, -y, -z);	// Bottom Right
		glTexCoord2f(scaleX, scaleY); glVertex3f( x,  y, -z);	// Top Right
		glTexCoord2f(0.0f, scaleY); glVertex3f( x,  y,  z);	// Top Left
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);	// Bottom Left
		glNormal3f(-1.0f, 0.0f, 0.0f); // Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);	// Bottom Left
		glTexCoord2f(scaleX, 0.0f); glVertex3f(-x, -y,  z);	// Bottom Right
		glTexCoord2f(scaleX, scaleY); glVertex3f(-x,  y,  z);	// Top Right
		glTexCoord2f(0.0f, scaleY); glVertex3f(-x,  y, -z);	// Top Left
	glEnd();
	
}

void drawCilinder( GLUquadric *q, int textureIndex, 
	float scaleTexX, float scaleTexY, float scaleTexZ,
	float height, float radius,
	int slices, int stacks
	){
	/* void gluCylinder(GLUquadric* quad,  GLdouble base,  GLdouble top,  GLdouble height,  GLint slices,  GLint stacks);
	  ovim moze da se crta i kupa :o
	  https://www.opengl.org/discussion_boards/showthread.php/171218-map-a-repeating-texture-to-glucylinder%28-%29
	 */
	glTranslatef(0, height/2, 0);
	glRotatef(90, 1, 0, 0);
	
	glMatrixMode(GL_TEXTURE); // hocu da sacuvam kakva je bila
	glPushMatrix();
	
	gluQuadricTexture(q, 1);
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
	glLoadIdentity();
	glScalef(scaleTexX,scaleTexY,scaleTexZ);
	gluCylinder(q, radius, radius, height, slices, stacks); 
	
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawRectAlphaCh(float height, float width, short int textureIndex){
	restartColors(); 
	glEnable(GL_BLEND);
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawRectangle(height, width, 1, 1, USE_TEXTURE, textureIndex);
	glPopMatrix();
	glDisable(GL_BLEND);
}
void drawRectangle(
	float height, float width,
	float scaleX, float scaleY,
	int textureIndicator, int textureIndex
){
	glPushMatrix();
	float a = width/2; float b = height/2;
	if(textureIndicator == USE_TEXTURE){
		restartColors(); 
		/* Biramo koju teksturu koristimo */
		glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
		glBegin(GL_QUADS);
		/* tacka dole levo */
		glNormal3f(0, 0, 1); /* ista je za sve tacke */
		glTexCoord2f(botLeftX,botLeftY);
		glVertex3f(-a, -b , 0);
		
		/* tacka dole desno */
		glTexCoord2f(topRightX*scaleX,botLeftY);
		glVertex3f(a, -b, 0);
		
		/* tacka gore desno */
		glTexCoord2f(topRightX*scaleX,topRightY*scaleY);
		glVertex3f(a, b, 0);
		
		/* tacka gore levo */
		glTexCoord2f(botLeftX,topRightY*scaleY);
		glVertex3f(-a, b, 0);
		glEnd();
	}
	else if(textureIndicator == USE_MATERIAL){
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
// 		glColor4f(1, 1, 1, 0.7);
		glBegin(GL_QUADS);
		glVertex3f(-a, -b , 0);
		glVertex3f(a, -b, 0);
		glVertex3f(a, b, 0);
		glVertex3f(-a, b, 0);
		glEnd();
		glDisable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
	}
		
	glPopMatrix();
}
void drawArrow(void){
	glPushMatrix();
	restartColors();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0,0,0);
	glBegin(GL_POLYGON); // crtamo rep strelice
	
	glVertex2f(0,0);
	glVertex2f(0.125,1.2);
	glVertex2f(-0.125,1.2);
	
	glEnd();
	glBegin(GL_POLYGON); // crtamo vrh strelice
	
	glVertex2f(0.25,1.2);
	glVertex2f(0,1.7);
	glVertex2f(-0.25,1.2);
	
	glEnd();
	
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void drawCircle(float radius, int slices){
	glPushMatrix();
	restartColors();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.8,1,0.8);
	int i;
	glBegin(GL_POLYGON);
	for(i=0; i<360; i++){
		glVertex2f(cos(i*M_PI/slices)*radius, sin(i*M_PI/slices)*radius);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void drawHalfCilinder(
	float height, float a_semiaxis, float b_semiaxis,
	int slices, int repeat, int textureIndex
){ 
	/* Biramo koju teksturu koristimo */
	restartColors(); // ---------------------- PROBA
	glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);
	
	glPushMatrix();
	int i;
	
	glBegin(GL_QUADS); /* povezace svake 4 zajedno, mora se vise puta navoditi jedna ista */
	
	for(i=0; i < slices; i++){
		/* dole i gore desno moraju da se poklope u svakoj iteraciji */
		/* tacka dole levo */
		glNormal3f(a_semiaxis*cos(i*M_PI/slices), 0, b_semiaxis*sin(i*M_PI/slices));
		glTexCoord2f(0,0);
		glVertex3f(-a_semiaxis*cos(i*M_PI/slices), -height, -b_semiaxis*sin(i*M_PI/slices));
		
		/* tacka dole desno */
		glNormal3f(a_semiaxis*cos((i+1)*M_PI/slices), 0, b_semiaxis*sin((i+1)*M_PI/slices));
		glTexCoord2f(1,0);
		glVertex3f(-a_semiaxis*cos((i+1)*M_PI/slices), -height, -b_semiaxis*sin((i+1)*M_PI/slices));
		
		/* tacka gore desno */
		glNormal3f(a_semiaxis*cos((i+1)*M_PI/slices), 0, b_semiaxis*sin((i+1)*M_PI/slices));
		glTexCoord2f(1,repeat);
		glVertex3f(-a_semiaxis*cos((i+1)*M_PI/slices), height, -b_semiaxis*sin((i+1)*M_PI/slices));
		
		/* tacka gore levo */
		glNormal3f(a_semiaxis*cos((i)*M_PI/slices), 0, b_semiaxis*sin(i*M_PI/slices));
		glTexCoord2f(0,repeat);
		glVertex3f(-a_semiaxis*cos((i)*M_PI/slices), height, -b_semiaxis*sin((i)*M_PI/slices));
	}
	glEnd();
	glPopMatrix();
}
void initQuads(void){
	cilinderQuad = gluNewQuadric(); // za ivice u gameplay
	quad = gluNewQuadric();
}
void freeQuads(void){
	gluDeleteQuadric(quad);
	gluDeleteQuadric(cilinderQuad);
}
void restartColors(void){
	glColor3fv(white);
	glColor4f(0, 0, 0, 1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
}
