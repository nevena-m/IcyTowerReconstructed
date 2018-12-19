#ifndef _SHAPES_H_
#define _SHAPES_H_

#include "texture.h"

#define USE_TEXTURE 0
#define USE_MATERIAL 1

extern GLUquadric *quad;
extern GLUquadric *cilinderQuad;

/* height(h) utice na y koordinatu, width(w) na x, a length(l) na z 
 */
void drawTexQuad(short int texture_index, 
	float scaleX, float scaleY,
	float w, float h, float l
);

/* Crta zelenkasti krug */
void drawCircle(float radius, int slices);
/* Crta crnu strelicu, zasiljenu */
void drawArrow(void);

void drawCilinder( GLUquadric *q, int textureIndex, 
	float scaleTexX, float scaleTexY, float scaleTexZ,
	float height, float radius,
	int slices, int stacks
);
/* Potrebno je naglasiti sa textureIndicator da li se koristi obican
 * materijal ili tekstura. Ako je njegova vrednost USE_MATERIAL, onda
 * se ignorise textureIndex, ali se ocekuje da je boja materijala 
 * postavljena sa glColor4f() pre poziva ove funkcije. Ako je vrednost
 * USE_TEXTURE onda se koristi tekstura iz definisanog niza koja se 
 * nalazi na prosledjenom indeksu. Tekstura se ponavlja scaleX i scaleY 
 * puta po osama.
 * Koristi (botLeftX, botLeftY) za donji levi ugao, i (topRightX, topRightY)
 * za gornji desni deo teksture koju iseca u obliku pravougaonika. 
 * Da bi se ispravno mapiralo, odnos stranica teksture mora biti isti kao 
 * odnos odgovarajucih stranica pravougaonika. Dozvoljeno je da se taj odnos
 * narusi, od pozivaoca se ocekuje da sam ispostuje odnos ako to zeli.
 */
void drawRectangle(
	float height, float width,
	float scaleX, float scaleY,
	int textureIndicator, int textureIndex
);
/* Za teksture koje koriste i alpha chanel, potrebno je naglasiti samo
 * koordinate teksture koja se koristi sa setTexCoords()
 */
void drawRectAlphaCh(float width, float height, short int textureIndex);
void drawHalfCilinder(
	float height, float a_semiaxis, float b_semiaxis,
	int slices, int repeat, int textureIndex
);
void initQuads(void);
void freeQuads(void);
void restartColors(void);
#endif
