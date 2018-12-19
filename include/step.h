#ifndef _STEP_H_
#define _STEP_H_

#include "character.h"

#define INIT_TOP_INDEX 6
#define INIT_BOT_INDEX 0

/* Polovina visine stepenika
 * Fiksirana sirina stepenika
 * FIksiran razmak izmedju susednih stepenika
 */
#define STEP_HEIGHT 0.25
#define STEP_WIDTH 4.0
#define STEP_DIFF 3

/* Inicijalna pozicija prvog stepenika koja zavisi od pozicije Harolda */
#define INIT_FIRST_STEP_Y (INIT_CHARACTER_Y - CHAR_HALF_HEIGHT - STEP_HEIGHT)

/* Broj stepenika koji se generise */
#define NUM_OF_STEPS 20

/* Svaki 10. stepenik je platforma, i ima fiksiran izgled kada se iscrtava */
#define PLATFORM 10

/* Enum koji govori koja se polovina niza svih stepenika treba generisati/azurirati */
typedef enum {FST_HALF, SND_HALF} StepsToGen;

typedef struct {
	float pos_x;
	float pos_y;
} WoodenStep;
/* Indeksi prve i poslednje stepenice koja se vidi na ekranu */
extern int topStepOnScreen;
extern int botStepOnScreen;

extern WoodenStep **steps;
extern StepsToGen stepsHalf;

WoodenStep **allocate_steps(void); 
void freeSteps(WoodenStep **steps);

/* Generise random rasporedjene stepenice fiksirane sirine */
void generateNewSteps(WoodenStep **steps, float init_y, int a, int b);
/* Crta stepenike koji se vide na sceni */
void drawSteps(WoodenStep **steps, int bot, int top);

#endif
