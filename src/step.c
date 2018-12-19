#include "../include/step.h"
#include "../include/global.h"
#include "../include/texture.h"
#include "../include/wall.h"
#include "../include/display.h"
#include "../include/shapes.h"

WoodenStep **steps;

StepsToGen stepsHalf;

int topStepOnScreen;
int botStepOnScreen;

void drawSteps(WoodenStep **steps, int bot, int top){
	int i;
	
	for(i=bot; i<top; i++){
		glPushMatrix();
		glTranslatef(steps[i]->pos_x, steps[i]->pos_y, -B_SEMIAXIS);
		if(i%PLATFORM == 0)
			drawTexQuad(STEP_TEX_IND, 5, 5, A_SEMIAXIS*2, STEP_HEIGHT*2, B_SEMIAXIS*2);
		else{
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glScalef(0.5,0.5,0.5);
			drawTexQuad(STEP_TEX_IND, 5, 5, STEP_WIDTH, STEP_HEIGHT*2, B_SEMIAXIS*2);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			
		}
		glPopMatrix();
	}
}

void generateNewSteps(WoodenStep **steps, float init_y, int a, int b){
	float sign = random();
	float randomPosition = random();
	int i,j;
	for(j=0,i=a; i<=b; j++,i++){
		sign = sign/RAND_MAX;
		randomPosition = (randomPosition/RAND_MAX) * (A_SEMIAXIS - STEP_WIDTH/2);
		if( i%PLATFORM != 0 ){
			steps[i]->pos_x = (sign <= 0.5 ? randomPosition : -randomPosition);
			steps[i]->pos_y = init_y + (j)*STEP_DIFF;
			
		}
		else{		//na svakih 10 stepenika je jedan koji je preko celog zida
			steps[i]->pos_x = 0;
			steps[i]->pos_y = init_y + (j)*STEP_DIFF;
		}
		sign = random();
		randomPosition = random();
	}
	
}
void freeSteps(WoodenStep **steps){
	int i;
	for(i=0; i<=NUM_OF_STEPS; i++){
		free(steps[i]);
	}
	free(steps);
}
WoodenStep **allocate_steps(void){
	WoodenStep **steps = malloc(NUM_OF_STEPS * sizeof(WoodenStep *));
	if(steps==NULL){
		printf("Failed to allocate steps array\n");
		exit(EXIT_FAILURE);
	}
	int i;
	for(i=0; i<= NUM_OF_STEPS; i++){
		steps[i] = malloc(sizeof(WoodenStep));
		if(steps[i]==NULL){
			printf("failed to allocate step No.%d\n", i);
			exit(EXIT_FAILURE);
		}
	}
	return steps;
}
