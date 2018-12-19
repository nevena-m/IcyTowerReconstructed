#include "../include/global.h"
#include "../include/display.h"
#include "../include/wall.h"
#include "../include/step.h"
#include "../include/character.h"
#include "../include/texture.h"
#include "../include/shapes.h"
#include "../include/animation.h"

#define TITLE_SPACING (-1)
#define OPT_SPACING (-1)

short int activeSceneIndex = WELCOME_INDEX; // inicijalno
short int previousScene = -1;
short int cursorPosition = 0;

short int displayHurryText = 0;
char scoreString[20];

GLfloat const text_color[] = {0.498f, 1.000f, 0.831f};
GLfloat const cursor_color[] = {0.5f, 0.1f, 0.1f};
GLfloat const stopwatch_color[] ={1, 0.78, 0.1};

void drawScoreWall(void){
	glPushMatrix();
	glTranslatef(-A_SEMIAXIS-2, camera_pos_y, 0);
	drawRectangle(WALL_HEIGHT, 6, 1, NUM_OF_STACKS/2, USE_TEXTURE, WALL_TEX_IND);
	
	glTranslatef(0, 4, 0.1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, stopwatch_color);	// okvir sata
	glutSolidTorus(0.1, 2, 10, 20);
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cursor_color);		// 
	glutSolidSphere(0.2, 5,5);
	
	drawCircle(2, 20);						// pozadina sata
	
	glTranslatef(0, 0, 0.1);
	/* hocemo da je rotiramo kada je animacija u toku */
	if(animation->status == ONGOING){
		gameTime += dtime*10;
	}
	glPushMatrix();
	glRotatef(-gameTime, 0,0,1);
	drawArrow();					// strelica
	glPopMatrix();
	
	glTranslatef(-2, -5, 0.1);
	
	sprintf(scoreString, "Score: %li", gameScore);
	printRaster(scoreString, 0,0,0);
	
	glPopMatrix();
	if(gameTime > 360){					// nakon punog kruga 
		gameTime = 0;					// resetuj na nula
		speedMultiplier *= 2;			// i ubrzaj animaciju propadanja
		displayHurryText = 1;			// prikazi tekst da je ubrzano
		hurryPosY = camera_pos_y-10;
	}
	drawHurryText();
}

void drawHurryText(void){
	glPushMatrix();
	
	if(displayHurryText){
		glPushMatrix();
		glTranslatef(0, hurryPosY, 0.1);
		resetTexCoords();
		drawRectAlphaCh(2, 10, HURRY_TEX_IND);
		glPopMatrix();
		if(animation->status != PAUSE)
			hurryPosY += dtime*10;
	}
	if(hurryPosY > camera_pos_y + 10){
		hurryPosY = 0;
		displayHurryText = 0;
	}
	glPopMatrix();
}

void drawGameplayScene(void){
	glPushMatrix(); // da bi se zapamtio prethodni koordinatni sistem
	// Cela scena treba da se pomeri u stranu kako bi bilo mesta za ispis teksta
	glTranslatef(3, 0, 0);
	
	glPushMatrix(); // Iscrtavanje kule - Prvi zid (donji)
	glTranslatef(gameplayWall_1[0], gameplayWall_1[1], gameplayWall_1[2]);
	drawHalfCilinder( WALL_HEIGHT, A_SEMIAXIS, B_SEMIAXIS, NUM_OF_WALL_SLICES,
						NUM_OF_STACKS, WALL_TEX_IND );
	glPopMatrix();
	
	glPushMatrix(); // Iscrtavanje kule - Drugi zid (gornji)
	glTranslatef(gameplayWall_2[0], gameplayWall_2[1], gameplayWall_2[2]);
	drawHalfCilinder( WALL_HEIGHT, A_SEMIAXIS, B_SEMIAXIS, NUM_OF_WALL_SLICES, 
						NUM_OF_STACKS, WALL_TEX_IND );
	glPopMatrix();
	
	glPushMatrix(); // Iscrtavanje cilindara koji su ivica kule, uskladjeni sa Prvim
	glTranslatef(gameplayWall_1[0]-A_SEMIAXIS-CILINDER_RADIUS, 
					gameplayWall_1[1], gameplayWall_1[2]);
	drawCilinder(cilinderQuad, WALL_TEX_IND, 
					2, NUM_OF_STACKS, 1, WALL_HEIGHT*2, 
			CILINDER_RADIUS, CILINDER_SLICES, CILINDER_STACKS);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(gameplayWall_1[0]+A_SEMIAXIS + CILINDER_RADIUS,
					gameplayWall_1[1], gameplayWall_1[2]);
	drawCilinder(cilinderQuad, WALL_TEX_IND, 
					2, NUM_OF_STACKS, 1, 
			  WALL_HEIGHT*2, CILINDER_RADIUS, 
			CILINDER_SLICES, CILINDER_STACKS);
	glPopMatrix();
	
	glPushMatrix(); // Iscrtavanje cilindara koji su ivica kule, uskladjeni sa Drugim
	glTranslatef(gameplayWall_2[0]-A_SEMIAXIS-CILINDER_RADIUS, 
					gameplayWall_2[1], gameplayWall_2[2]);
	drawCilinder(cilinderQuad, WALL_TEX_IND, 
					2, NUM_OF_STACKS, 1, WALL_HEIGHT*2, 
			CILINDER_RADIUS, CILINDER_SLICES, CILINDER_STACKS);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(gameplayWall_2[0]+A_SEMIAXIS + CILINDER_RADIUS,
					gameplayWall_2[1], gameplayWall_2[2]);
	drawCilinder(cilinderQuad, WALL_TEX_IND, 
					2, NUM_OF_STACKS, 1, WALL_HEIGHT*2, 
			CILINDER_RADIUS, CILINDER_SLICES, CILINDER_STACKS);
	glPopMatrix();
	
	
	drawCharacter(Harold);
	drawSteps(steps, 0, NUM_OF_STEPS);
	glPopMatrix();
	drawScoreWall(); // objekti koji imaju transparentne slike na sebi
	// moraju na kraju da se iscrtaju, inace alfa chanel nema efekta
}

void drawBackground(short int activeSceneIndex){
	glPushMatrix();
	switch(activeSceneIndex){
		case WELCOME_INDEX:
			glTranslatef(0,camera_pos_y,WELCOME_OPT_Z-2);
			drawRectangle(30, 30, 6, 6, USE_TEXTURE, WALL_TEX_IND);
			drawCursor(WELCOME_OPT_X + OPT_SPACING,  
				WELCOME_OPT_Y + cursorPosition*OPT_SPACING, WELCOME_OPT_Z );
			break;
		case PAUSE_INDEX:
			glTranslatef(0,camera_pos_y,PAUSE_OPT_Z-2);
			glColor4f(1, 1, 1, 0.7);
			drawRectangle(30, 30, 1, 1, USE_MATERIAL, 0);
			drawCursor(PAUSE_OPT_X + OPT_SPACING,  
				PAUSE_OPT_Y + cursorPosition*OPT_SPACING, PAUSE_OPT_Z );
			break;
		case EXIT_INDEX:
			glTranslatef(0,camera_pos_y,EXIT_TITLE_Z-2);
			drawRectangle(30, 30, 6,6, USE_TEXTURE, WALL_TEX_IND);
			if(countdownShutdown > 10){
				exitGame();
			}
			countdownShutdown += SHUTDOWN_DELTA;
			break;
		case INSTR_INDEX:
			glTranslatef(0,camera_pos_y,INST_TITLE_Z-2);
			drawRectangle( 30, 30, 6, 6, USE_TEXTURE, WALL_TEX_IND);
			break;
		case LOSE_INDEX:
			glTranslatef(0,camera_pos_y,LOSE_OPT_Z-2);
			glColor4f(1, 1, 1, 0.7);
			drawRectangle(30, 30, 1, 1, USE_MATERIAL, 0);
			drawCursor(LOSE_OPT_X + OPT_SPACING,  
				LOSE_OPT_Y + cursorPosition*OPT_SPACING, LOSE_OPT_Z );
			break;
		default:
			break;
	}
	glPopMatrix();
}

void drawCursor(float x, float y, float z){
	glPushMatrix();
	glTranslatef(x, y, z);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cursor_color);
	glutSolidSphere(0.3, 5, 5);
	glPopMatrix();
}

void printSceneText(short int activeSceneIndex){
	
	switch(activeSceneIndex){
		case WELCOME_INDEX:
			glPushMatrix();
			glTranslatef(0, camera_pos_y+3, 2);		// podesi gde naslov treba da stoji
			drawRectAlphaCh(15, 15, WELCOME_TEX_IND); // WELCOME to ICY TOWER reconst
			
			glTranslatef(-5, -5, 1);				// podesi gde opcija treba da stoje
			setTexCoords(0, 0.25, 1, 0.5);			// koje koordinate tex treba mapirati
			drawRectAlphaCh(1, 5, MENU_TEX_IND); 	// START
			glTranslatef(1.5, -1, 0);
			setTexCoords(0,0.75,1,1);
			drawRectAlphaCh(1, 6, MENU_TEX_IND);	// Instructions
			glTranslatef(-1.7, -0.7, 0);
			setTexCoords(0,0,1,0.25);
			drawRectAlphaCh(1,6, MENU_TEX_IND);		// EXIT
			
			glTranslatef(13,-1, 0);			// instrukcije ukratko
			setTexCoords(0,0.2,1,0.4);
			drawRectAlphaCh(2.5, 8, HELP_TEX_IND);
			resetTexCoords();						// vratimo nazad na 0,0 1,1
// 			printRaster("Play", xo, camera_pos_y + WELCOME_OPT_Y, zo);
// 			printRaster("Instructions", xo, camera_pos_y + WELCOME_OPT_Y + OPT_SPACING, zo);
// 			printRaster("Exit", xo, camera_pos_y + WELCOME_OPT_Y + 2*OPT_SPACING, zo);
			glPopMatrix();
			break;
		case GAMEPLAY_INDEX:
			break;
		case PAUSE_INDEX:
			glPushMatrix();
			glTranslatef(0, camera_pos_y+5, 3);
			setTexCoords(0,0.7,1,0.9);			// title za PAUZU
			drawRectAlphaCh(5, 18, TITLES_TEX_IND);
			
			glTranslatef(-3, -5, 1);
			setTexCoords(0,0.5,1,0.75);			// continue 
			drawRectAlphaCh(1, 6, MENU_TEX_IND);
			
			glTranslatef(1.3, -1, 1);
			setTexCoords(0,0.75, 1, 1);			// Instructions
			drawRectAlphaCh(1, 6, MENU_TEX_IND);
			
			glTranslatef(2, -1.7, 1);
			setTexCoords(0,0,1,0.3);			// exit to menu
			drawRectAlphaCh(2.5, 9, LOST_TEX_IND);
			
			resetTexCoords();
			glPopMatrix();
			
// 			printRaster("Continue", xo, camera_pos_y+PAUSE_OPT_Y, zo);
// 			printRaster("Instructions", xo, camera_pos_y+PAUSE_OPT_Y+ OPT_SPACING, zo);
// 			printRaster("Exit to main menu", xo, camera_pos_y+PAUSE_OPT_Y+ 2*OPT_SPACING, zo);
			break;
		case INSTR_INDEX:
			glPushMatrix();
			glTranslatef(0, camera_pos_y+5, 2);
			setTexCoords(0,0.55,1,0.7);
			drawRectAlphaCh(2.5, 10, TITLES_TEX_IND);	// naslov za instrukcije
			glTranslatef(0,-5,1);
			setTexCoords(0,0.35,1,0.9);
			drawRectAlphaCh(10,15, HELP_TEX_IND);	// tekst objasnjenja igre
			resetTexCoords();
			glPopMatrix();
			
			break;
			
		case EXIT_INDEX:
			glPushMatrix();
			glTranslatef(0, camera_pos_y, 3);
			setTexCoords(0,0.1,1,0.55);
			drawRectAlphaCh(10,18, TITLES_TEX_IND);
			resetTexCoords();
			glPopMatrix();
			break;
		case LOSE_INDEX:
			glPushMatrix();
			glTranslatef(0, camera_pos_y, 2);
			setTexCoords(0,0.3, 1,0.8);
			drawRectAlphaCh(6,10, LOST_TEX_IND);
			resetTexCoords();
			glPopMatrix();
			break;
		
		default: break;
	}
}

void printRaster(char *text, float x, float y, float z){
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPushMatrix();
	
	glColor3fv(text_color);
	
	glRasterPos3i( x,  y,  z);
	int i;
	for(i=0; text[i]!='\0'; i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	}
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
