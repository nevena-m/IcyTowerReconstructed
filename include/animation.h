#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "wall.h"
#include "character.h"
#include "step.h"
#include "collision.h"
#include "shapes.h"

/* Korak kojim napreduje propadanje
 * CHEKPOINT_STEPS - Chekpoint do koga animacija treba da dodje da bi se izgenerisala/azurirala odgovarajuca polovina niza stepenica
 * DIFF_STEPS_HALFS - Konstanta koju treba dodati da bi se doslo do narednog chekpoint-a 
 * 		(Razlika steps[NUM_OF_STEPS/2]->pos_y - steps[0]->pos_y)
 */

#define CHEKPOINT_STEPS 7
#define DIFF_STEPS_HALFS 30

/* Chekpoint koji mora da dostigne animacija da bi se azuriralo koje se 
 * stepenice vide na ekranu ( topStepOnScreen i botStepOnScreen )
 * sos - steps on screen :)
 */
#define CHEKPOINT_SOS 3

#define SHUTDOWN_DELTA 0.1		// Magicni broj koji se dodaje countdownShutdown-u

/* Status animacije, da li je u toku ili je pauzirana ili jos nije ni pocela
 */
typedef enum {PAUSE, ONGOING, NOTSTARTED} AnimationStatus;

/* Parametar animacije sluzi za provere generisanja stepenika
 * Ista je kao pozicija kamere 
 */
typedef struct {
	AnimationStatus status;
	float param;
	float checkpointSteps;
	float checkpointStepOnScr;
} Animation;

extern float gameTime;			// Merac vremena za stopericu, kada navrsi 360 HurryUp!
extern Animation *animation;
extern float animSpeed;			// Parametar za koji propada scena (pomera kameru)
extern float speedMultiplier;	// Kada stoperica zavrsi krug duplo ubrza animaciju 
extern float hurryPosY;			// pozicija za Hurry up tekst koji preleti preko ekrana
extern float countdownShutdown;	// Prikazuje Thanks scenu dok ne navrsi 10, onda exit()

void keyOperations(void);	// akcije kada se dugme drzi pritisnuto

void restartGame(void); /* Dovodi program u pocetno stanje */
void exitGame(void); /* Izlazi iz programa i oslobadja svu memoriju */

/* Ako je animacija u toku, azurira poziciju kamere i tacku pogleda
 * U njoj se proverava da li treba zameniti prvi i drugi zid, kao i kada 
 * treba generisati nove stepenice. Ako se lik krece brze onda pomera i kameru
 */
void changeAnimParams(Animation *animation, Character *c, float *wallPos1, float *wallPos2);

Animation *init_animation(void);

/* Stavlja prvi iznad drugog zida, menja samo y koordinatu */
void switchWallPos(float *first, float *second);

/* Proverava da li mora da zameni mesta zidovima */
void checkWallSwichingCond(Animation *anim, float *wallPos1, float *wallPos2);

/* Proverava da li treba da se izgenerisu nove random stepenice na 
 * odgovarajucoj polovini vec napravljenog niza.
 */
void checkStepsGenCond(Animation* animation);

/* Vodi racuna o indeksima prve i poslednje stepenice koje se vide na sceni
 */
void updateBotTopSteps(Animation *animation);

/* Ako lik nije uspeo da padne na skroz donji vidljiv stepenik
 * onda se racuna kao da je pao i izgubio
 */
int gameIsLost(Character *c);	// ako je pao sa poslednje vidljive stepenice vraca -1
void pauseGame(Animation *a);	// zaustavlja animaciju ako anim nije pokrenuta
void resumeGame(Animation *a);	// nastavlja animaciju tamo gde je pauzirana
#endif
