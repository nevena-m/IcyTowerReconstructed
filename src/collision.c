#include "../include/collision.h"

/* Oznacava koja polovina niza svih stepenica treba sledeca da se izgenerise
 * Znaci da onda onu drugu mogu da proveravam za kolizije
 */
StepsToGen stepsHalf;

float WallCollision(float char_x){
	if( char_x > A_SEMIAXIS - BOX_H_WIDTH ){
		return A_SEMIAXIS - BOX_H_WIDTH - EPSILON;
	}
	if( char_x < -A_SEMIAXIS + BOX_H_WIDTH ){
		return -A_SEMIAXIS + BOX_H_WIDTH + EPSILON;
	}
	return 0; // nema kolizije
}
/*  ------------------ NOVO s9 ---------------------------- */
int isOnStep(Character *c){
	int closest = findClosestStep(c, steps);
	
	if(closest == -1){
		return GAME_LOST;
	}
	float feet = c->position[1] - CHAR_HALF_HEIGHT;
	
	float area_Y = steps[closest]->pos_y + STEP_HEIGHT;
	float area_X_Left = steps[closest]->pos_x - STEP_WIDTH/2;
	float area_X_Right = steps[closest]->pos_x + STEP_WIDTH/2;
	float diffPos = getDiffOfPos(c);
	float epsilon;
	
	/* Ako fall_multiplier previse utice na promenu pozicije
	 * Onda moramo koristiti razliku u pozicijama za proveru 
	 * da li je na stepenici ili nije
	 */
	
	if(diffPos > EPSILON*50){
		epsilon = diffPos;
	}
	else
		epsilon = EPSILON*50; 
	
	if(closest % PLATFORM == 0){
		if(feet > area_Y - epsilon && feet < area_Y + epsilon){
			return closest;
		}
	}
	else if(area_X_Left < c->position[0] && c->position[0] < area_X_Right){
		if(feet > area_Y - epsilon && feet < area_Y + epsilon){
			return closest;
		}
	}
	
	return -1; //nije na stepenici
}
int findClosestStep(Character *c, WoodenStep **steps){
	
	int i;
	float feet = c->position[1] - CHAR_HALF_HEIGHT; // pozicija nogu
	// PAZITI DA BOT MOZE BITI NEKAD VECI OD TOP
	if(botStepOnScreen < topStepOnScreen){
		for(i=botStepOnScreen; i<topStepOnScreen; i++){
			float t = steps[i+1]->pos_y; //top
			float d = steps[i]->pos_y;	 //down
			if(t  > feet && feet > d )
				return i;
		}
	}
	else{ //obrnuo se krug
		for(i=botStepOnScreen; i<NUM_OF_STEPS; i++){//prvo se proveravaju "donje" na ekranu
			float t = steps[i+1]->pos_y;
			float d = steps[i]->pos_y;
			if(t  > feet && feet > d )
				return i;
		}
		for(i=0; i<topStepOnScreen; i++){				// "gornje" na ekranu
			float t = steps[i+1]->pos_y;
			float d = steps[i]->pos_y;
			if(t  > feet && feet > d )
				return i;
		}
	}
	return -1; // doslo je do greske?
}
/*  ------------------------e9----------------------------- */
