#include "../include/global.h"
#include "../include/character.h"
#include "../include/step.h"
#include "../include/collision.h"
#include "../include/animation.h"
#include "../include/texture.h"
#include "../include/display.h"
#include "../include/shapes.h"

Character *Harold;
WoodenStep **steps;
long int gameScore = 0;

static float THETA = 5;
float rotationAngLegs[] = {0, 0}; // ugao za koji rotiramo 1. i 2. nogu
float rotationAngArms[] = {0, 0}; // krecu se simetricno u razlicite strane
float absolutValAngLegs = 0;	// apsolutna vrednost uglova rotacija
float absolutValAngArms = 0;
short int direction = 0; // 1 za kretanje u desno, -1 za kretanje u levo

void greska(char* msg){
	printf("\n%s\n", msg);
	exit(EXIT_FAILURE);
}
void rotateExtremities(Character *c){
	absolutValAngLegs = fabs(rotationAngLegs[0]);
	absolutValAngArms = fabs(rotationAngLegs[0]);
	direction = c->moveTo == LEFT ? -1 : (c->moveTo == RIGHT ? 1:0);
	if(c->moveTo != IDLE && c->state == GROUNDED){		// pomera noge kada se krece
		if(absolutValAngLegs>30){
			THETA *= -1.0;
		}
		rotationAngLegs[0] -= direction*THETA*c->velocity[0]/MAX_VELOCITY_X;
		rotationAngLegs[1] += direction*THETA*c->velocity[0]/MAX_VELOCITY_X;
		rotationAngArms[0] = rotationAngLegs[1]*1.5;
		rotationAngArms[1] = rotationAngLegs[0]*1.5;
	}
	else if(c->moveTo == IDLE && c->state == GROUNDED){
		rotationAngLegs[0] = 0;
		rotationAngLegs[1] = 0;
		rotationAngArms[0] = 0;
		rotationAngArms[1] = 0;
	}
	/* Skocio je ulevo, mora ruke uvis i noge unazad */
	if(c->moveTo == LEFT){
		if(absolutValAngArms<60 && c->state == JUMP){
			rotationAngArms[0] += THETA;
			rotationAngArms[1] = rotationAngArms[0];
		}
		if(absolutValAngLegs<30 && c->state == JUMP){
			rotationAngLegs[0] -= THETA;
			rotationAngLegs[1] = rotationAngLegs[0];
		}
	}
	if(c->moveTo == RIGHT){
		if(absolutValAngArms<60 && c->state == JUMP){
			rotationAngArms[0] += THETA;
			rotationAngArms[1] = rotationAngArms[0];
		}
		if(absolutValAngLegs<30 && c->state == JUMP){
			rotationAngLegs[0] += THETA;
			rotationAngLegs[1] = rotationAngLegs[0];
		}
	}
	if(c->moveTo == IDLE && c->state == JUMP){
		if(absolutValAngArms < 120){
			rotationAngArms[0] += THETA;
			rotationAngArms[1] = rotationAngArms[0];
		}
	}
}

/* velocity moze da bude i plus i minus, kako bi se na poziciju samo
 * dodavalo bez brige o znaku.
 */
void characterUpdate(Character *c){
	//azuriranje pozicije Harolda : position_x += velocity_x * dtime;
	correctParams(c);
	c->position[0] += c->velocity[0] * dtime;
	c->position[1] += c->velocity[1] * dtime;
	c->velocity[1] += GRAVITY * dtime;
	
	if(c->velocity[1] < 0 ){
		c->velocity[1] += GRAVITY * (FALL_MULTIPLIER - 1)*dtime;
	}
	rotateExtremities(c);
	
}
float getDiffOfPos(Character *c){
	//formula za izracunavanje prethodne pozicije kada pada, posto raste razlika
	// ova razlika treba da posluzi pri racunanju collision sa stepenicima
	float gravityInfluence = c->velocity[1] < 0 ? GRAVITY* FALL_MULTIPLIER * dtime * speedMultiplier: GRAVITY*dtime*speedMultiplier;
	float pom = c->position[1] - (c->velocity[1] - gravityInfluence)*dtime;
	
	float diff = c->position[1] - pom;
	diff = diff<0 ? -diff : diff;
	return diff;
}

void jump(Character *c){
	if(c->state == GROUNDED){
		c->state = JUMP;
		c->velocity[1] = INIT_ACCELERATION_Y;
		characterUpdate(c);
	}
}

void correctParams(Character *c){
	float collision = WallCollision(c->position[0]);
	// blago odbijanje o zid kada se lik zaleti po x osi
	if(collision){
		c->position[0] = collision;		// UBACENO IZ CHARACTER_UPDATE
		c->velocity[0] = -c->velocity[0]; 
	}
	if((c->velocity[0] < 0) && (c->velocity[0] < -MAX_VELOCITY_X))
		c->velocity[0] = -MAX_VELOCITY_X;
	if((c->velocity[0] > 0) && (c->velocity[0] > MAX_VELOCITY_X))
		c->velocity[0] = MAX_VELOCITY_X;
}

void slowDown(Character *c){
	correctParams(c);
	if(c->velocity[0] < -EPSILON){
		c->velocity[0] += c->acceleration[0]*dtime*10;
	}
	else if(c->velocity[0] > EPSILON)
		c->velocity[0] -= c->acceleration[0]*dtime*10;
	else
		c->velocity[0] =0;
}
/* Proverava da li je doslo do kolizije, i "Odbija ga o zid" smanjenom brzinom
 * Ako je premasena dozvonjena brzina stavlja je na maximalnu odredjenu
 */
void manageMovement(Character *c){
	if(c->moveTo == IDLE)	
		slowDown(c); 		// ako se ne krece 
	int stepInd = isOnStep(c);
	
	if(stepInd > -1){		// na nekoj stepenici je
		c->position[1] = steps[stepInd]->pos_y + STEP_HEIGHT + CHAR_HALF_HEIGHT;
		c->state = GROUNDED;
		c->velocity[1] = 0;
		c->acceleration[1] = 0;
		c->curr_step = stepInd; //azurira se trenutni stepenik, score
		calculateScore(c);		// i sada su mu trenutna i prosla iste
		c->last_step = c->curr_step;
	}
	else if(c->state == GROUNDED){
		c->state = FALLING;
	}
	
	if(c->velocity[1] < 0 ){
		c->state = FALLING;
		c->velocity[1] += (FALL_MULTIPLIER - 1) * dtime;
	}
}
/* ADAPTACIJA KRETANJA nedelja 10. */
void moveCharacter(Character *c, Move side){
	//azuriranje brzine Harolda : velocity += acceleration * dtime;
	if(side == LEFT){
		if(c->moveTo == RIGHT || c->velocity[0]>0)
			c->velocity[0] = -c->velocity[0]*0.4;
		c->velocity[0] -= c->acceleration[0] * dtime*10;
		c->moveTo=LEFT;
		correctParams(c);
		
	}
	else if(side == RIGHT){
		if(c->moveTo == LEFT || c->velocity[0]<0)
			c->velocity[0] = -c->velocity[0]*0.4;
		c->velocity[0] += c->acceleration[0] * dtime*10;
		c->moveTo = RIGHT;
		correctParams(c);
		
	}
	else {
		c->moveTo = IDLE;
	}
}
void calculateScore(Character *c){
	if(c->state == GROUNDED){
		if(c->last_step!=c->curr_step && c->last_step < c->curr_step)
			gameScore += 10;
		else if(c->last_step!=c->curr_step && c->last_step > c->curr_step){
			gameScore -= 10;
		}
	}
}
Character *init_character(float x, float y, float z){
	
	Character *c = malloc(sizeof(Character));
	if(c == NULL)
		greska("init_character: malloc failed");
	c->position[0] = x;
	c->position[1] = y;
	c->position[2] = z;
	c->acceleration[0] = INIT_ACCELERATION_X;
	c->acceleration[1] = 0;
	c->acceleration[2] = 0;
	c->velocity[0] = 0;
	c->velocity[1] = 0;
	c->velocity[2] = 0;
	c->moveTo = IDLE;
	
	c->jumpHeight = INIT_JUMP_HEIGHT;
	c->curr_step = 0;
	c->last_step = 0;
	c->state = GROUNDED;
	
	return c;
}
/* -----------------------e9 --------------------------------------------*/
/* -------------- DODATO ------------------------------------------ */
/* Red Book - chapter 3.
 * gluProject() mimics the actions of the transformation pipeline. 
 * Given three-dimensional world coordinates and all the transformations 
 * that affect them, gluProject() returns the transformed window coordinates. 
 */
double getScreenYOf(Character *c){
	/* meni treba samo y koordinata za upravljanje animacijom
	 *	 saznala za funkciju sa:
	 *	 https://www.opengl.org/discussion_boards/showthread.php/170252-Get-screen-coordinates-of-a-transformed-point
	 *	 iskoristila i kod
	 */
	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	
	GLint viewport[4] = {0, 0, window_w, window_h};
	
	GLdouble winX, winY, winZ;
	gluProject((GLdouble)c->position[0], 
			   (GLdouble)c->position[1], 
			   (GLdouble)c->position[2],
			   model_view, projection, viewport,
			&winX, &winY, &winZ);
	
	return (double)winY;
}
/* ----------------------------------------------------------------- */
void freeCharacter(Character *c){
	free(c);
}
void rotateCharacter(Character *c){
	if(c->moveTo == LEFT){
		glRotatef(-180, 0, 1, 0);
	}
	else if(c->moveTo == IDLE){
		glRotatef(-90, 0, 1, 0);
	}
}
void drawCharacter(Character *c){
	
	glPushMatrix();
	
	glTranslatef(c->position[0], c->position[1], c->position[2]);
	rotateCharacter(c);
	//glava
	glPushMatrix();
	/* Kopirala odgovarajuci deo koda sa:
	 * http://www.codeincodeblock.com/2012/05/simple-method-for-texture-mapping-on.html
	 */
	glDisable(GL_BLEND); // ---------------------- PROBA
	glBindTexture(GL_TEXTURE_2D, textures[FACE_TEX_IND]);
	gluQuadricTexture(quad, 1);
	glTranslatef(0, 0.6, 0);
	glScalef(HEAD_WIDTH, 1, HEAD_WIDTH);
	
	glRotatef(90, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	
	gluSphere(quad,0.5,20,20);
	glPopMatrix();
	
	//trup
	glPushMatrix();
	glTranslatef(0, -0.05, 0);
	drawTexQuad(SHIRT_TEX_IND, 0.5, 0.5, 0.35, 0.5, 0.5);
	glPopMatrix();
	/* 1. Translirati do centra objekta (centar je ono oko cega zelimo da rotiramo)
	 * 2. Rotiraj za zeljeni ugao
	 * 3. Vrati nazad gde je bio ("ponisti" prvu translaciju)
	 */
	// ruka 1
	glPushMatrix();
	glTranslatef(0, ARM_LENGTH/2, 0);
	glRotatef(rotationAngArms[0], 0, 0, 1);
	glTranslatef(0, -ARM_LENGTH/2-0.05, 0.25);
	drawTexQuad(SHIRT_TEX_IND, 0.5, 0.5, 0.25, ARM_LENGTH, 0.125);
	glPopMatrix();
	// ruka 2
	glPushMatrix();
	glTranslatef(0, ARM_LENGTH/2, 0);
	glRotatef(rotationAngArms[1], 0, 0, 1);
	glTranslatef(0, -ARM_LENGTH/2-0.05, -0.25);
	drawTexQuad(SHIRT_TEX_IND, 0.5, 0.5, 0.25, ARM_LENGTH, 0.125);
	glPopMatrix();
	
	// noga 1
	glPushMatrix();
	glTranslatef(0, -LEG_LENGTH/2, 0);
	glRotatef(rotationAngLegs[0], 0,0,1);
	glTranslatef(0, -0.3, 0.125);
	drawTexQuad(JEANS_TEX_IND, 0.5, 0.5, 0.35, LEG_LENGTH, 0.25);
	glPopMatrix();
	// noga 2
	glPushMatrix();
	glTranslatef(0, -LEG_LENGTH/2, 0);
	glRotatef(rotationAngLegs[1], 0,0,1);
	glTranslatef(0,-0.3, -0.125);
	drawTexQuad(JEANS_TEX_IND, 0.5, 0.5, 0.35, LEG_LENGTH, 0.25);
	glPopMatrix();
	
	glPopMatrix();
}
