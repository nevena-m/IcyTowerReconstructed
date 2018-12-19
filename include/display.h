#ifndef _DISPLAY_H_
#define _DISPLAY_H

/* Spisak konstanti:*/

#define WELCOME_INDEX 0 		/* Indikator aktivne scene */
#define GAMEPLAY_INDEX 1
#define PAUSE_INDEX 2
#define INSTR_INDEX 3
#define EXIT_INDEX 4
#define LOSE_INDEX 5
/* Koliko ima opcija po sceni */
#define WELCOME_OPT_COUNT 3
#define PAUSE_OPT_COUNT 3
#define LOSE_OPT_COUNT 2
	/* Pocetna pozicija za opcije u welcome sceni */
#define WELCOME_OPT_X -6
#define WELCOME_OPT_Y -2
#define WELCOME_OPT_Z 2
	/* Pocetna pozicija za naslov u welcome sceni */
#define WELCOME_TITLE_X -3
#define WELCOME_TITLE_Y 5
#define WELCOME_TITLE_Z 3
	/* Pocetna pozicija za opcije u pause sceni */
#define PAUSE_OPT_X -4
#define PAUSE_OPT_Y 0
#define PAUSE_OPT_Z 3
	/* Pocetna pozicija za naslov u pause sceni */
#define PAUSE_TITLE_X -3
#define PAUSE_TITLE_Y 2
#define PAUSE_TITLE_Z 3
	/* Pocetna pozicija za naslov u Instructions sceni */
#define INST_TITLE_X -2
#define INST_TITLE_Y 5
#define INST_TITLE_Z 3
	/* Pocetna pozicija za naslov u Exit sceni */
#define EXIT_TITLE_X -4
#define EXIT_TITLE_Y 0
#define EXIT_TITLE_Z 3
	/* Pocenta pozicija za naslov i opcije u sceni kada je game lost */
#define LOSE_OPT_X -4
#define LOSE_OPT_Y -1.3
#define LOSE_OPT_Z 3

/* Spisak promenljivih:*/

extern short int activeSceneIndex;
extern short int previousScene;

extern short int welcomeOptCount; // ovim moze po modulu da se racuna pozicija
extern short int pauseOptCount;   // kursora, kako bi isao u krug

extern short int cursorPosition;


/* Spisak funkcija:*/
void printRaster(char *text, float x, float y, float z);
void printSceneText(short int activeSceneIndex);

void drawBackground(short int activeSceneIndex);
void drawCursor(float x, float y, float z);
void drawGameplayScene(void);
void drawHurryText(void);
#endif
