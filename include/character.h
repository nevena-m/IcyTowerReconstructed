#ifndef _CHARACTER_H_
#define _CHARACTER_H_

/* Inicijalne koordinate Harolda */
#define INIT_CHARACTER_X 0
#define INIT_CHARACTER_Y -6

/* Ako zatreba polovina visine lika, maksimalna visina skoka i
 * duzina nogu za skaliranje pri iscrtavanju
 */
#define CHAR_HALF_HEIGHT 1.1
#define LEG_LENGTH 0.8
#define HEAD_WIDTH 0.7
#define ARM_LENGTH 0.5
#define BOX_H_WIDTH (HEAD_WIDTH/2)

#define INIT_ACCELERATION_X 3		// pocetno ubrzanje za x, y
#define INIT_ACCELERATION_Y 8.5		
#define MAX_VELOCITY_X 13			// maksimalna brzina po x kojom sme
#define INIT_JUMP_HEIGHT 3

/* konstanta kojom mnozimo velocity_y kako bi brze padao lik i padanje
 * izgledalo realisticnije
 */
#define FALL_MULTIPLIER 4

/* Stanje lika, da li pada, da li je na zemlji ili skace.
 * Pravac u kome se lik krece ili miruje
 */
typedef enum {FALLING, GROUNDED, JUMP} CharacterState;
typedef enum {LEFT, RIGHT, IDLE} Move;

typedef struct {
	float position[3]; // odgovarajuci indeksi su 0-x osa 1-y osa 2-z osa
	float velocity[3]; // brzina za svaku osu
	float acceleration[3]; // ubrzanje za svaku osu
	
	float jumpHeight;	// ako zatreba
	int curr_step;		// indeks u nizu stepenika na kome je trenutno
	int last_step;		// indeks na kome je bio pre nego sto je skocio
	
	Move moveTo;
	CharacterState state;
} Character;

extern Character *Harold;
extern long int gameScore;			// Poeni za penjanje

Character *init_character(float x, float y, float z);
void drawCharacter(Character *c);
void freeCharacter(Character *c);

void characterUpdate(Character *c);
void slowDown(Character *c);

void correctParams(Character *c);
void jump(Character *c);
float getDiffOfPos(Character *c);
void moveCharacter(Character *c, Move side);
void manageMovement(Character *c);

double getScreenYOf(Character *c);

void rotateCharacter(Character *c);
void calculateScore(Character *c);
#endif
