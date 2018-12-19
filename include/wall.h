#ifndef _WALL_H_
#define _WALL_H_

	/* Da li se koristi tekstura ili crni providni zid, koristi se u funkciji drawRectangles */
#define USE_TEXTURE 0
#define USE_MATERIAL 1

/* Velika i mala poluosa za konstrukciju elipse na kojoj su ivice zida */

#define A_SEMIAXIS 10
#define B_SEMIAXIS 13

/* Pocetne koordinate jednog i drugog zida */
#define INIT_WALL_X_1 0 
#define INIT_WALL_Y_1 15
#define INIT_WALL_Z_2 0
#define INIT_WALL_X_2 0
#define INIT_WALL_Y_2 60
#define INIT_WALL_Z_1 0

/* Broj tacaka na elipsi za konstrukciju zida i polovina visine zida */
#define NUM_OF_WALL_SLICES 15
#define WALL_HEIGHT 30
#define NUM_OF_STACKS 12 // koliko puta ponoviti teksturu u vis..

#define CILINDER_RADIUS 1
#define CILINDER_STACKS 1
#define CILINDER_SLICES 10

/* Enum za odredjivanje smenjivanja dva zida
 * Ako je SWITCH_FST : Donji (prvi) zid se prebacuje iznad gornjeg (drugog)
 * Analogno za SWITCH_SND
 */
typedef enum {SWITCH_FST, SWITCH_SND} WallToSwitch;

extern float *gameplayWall_1;
extern float *gameplayWall_2;
extern WallToSwitch switchWalls;

float *newWallPos(float x, float y, float z);
void freeWallPos(float *w);

#endif
