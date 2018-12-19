#include "../include/global.h"
#include "../include/animation.h"
#include "../include/display.h"

StepsToGen stepsHalf;
WallToSwitch switchWalls;
Animation *animation;

float gameTime = 0;
float animSpeed;
float speedMultiplier = 1;
float hurryPosY = -20;
float countdownShutdown = 0;

// float prev_cam_y;
// float prev_look_y;

void keyOperations(void){
	if(keys[K_LEFT]){				// kada je dugme pritisnuto -> 1
		if(activeSceneIndex==GAMEPLAY_INDEX){
			moveCharacter(Harold, LEFT);
		}
	}
	else if(keys[K_RIGHT]){
		if(activeSceneIndex==GAMEPLAY_INDEX){
			moveCharacter(Harold, RIGHT);
		}
	}
	else{
		Harold->moveTo = IDLE;
	}
}
void switchWallPos(float *first, float *second){
	/* Zid naveden kao prvi arg premesta iznad drugog zida
	 * Azurira se y koordinata samo
	 */
	first[1] = second[1] + 2*WALL_HEIGHT;
}

void checkWallSwichingCond(
	Animation *anim, float *wallPos1, float *wallPos2){
	
	// wall_1 treba prebaciti iznad
	if(switchWalls == SWITCH_FST){
		if(anim->param >= wallPos1[1] + WALL_HEIGHT*1.5){
			switchWallPos(wallPos1, wallPos2);
			switchWalls = SWITCH_SND;
		}
	}
	else{	//wall_2 treba prebaciti iznad
		if(anim->param >= wallPos2[1] + WALL_HEIGHT*1.5){
			switchWallPos(wallPos2, wallPos1);
			switchWalls = SWITCH_FST;
		}
	}
}

Animation *init_animation(void){
	Animation *animation = malloc(sizeof(Animation));
	if(animation == NULL){
		printf("init_animation: malloc failed\n");
		exit(EXIT_FAILURE);
	}
	animation->status = NOTSTARTED;
	animation->param = 0;
	animation->checkpointSteps = CHEKPOINT_STEPS;
	animation->checkpointStepOnScr = CHEKPOINT_SOS;
	return animation;
}
/* ADAPTACIJA  31.07. */
void changeAnimParams(Animation *animation, Character *c, float *wallPos1, float *wallPos2){
	double y = getScreenYOf(c);
	if(animation->status == PAUSE || animation->status == NOTSTARTED){
		if(y > 0.66*window_h){
			animation->status = ONGOING;
		}
	}
	if(animation->status == ONGOING){
		if(y > 0.80*window_h){
			//dobije razliku prethodne i trenutne pozicije lika
			animSpeed = getDiffOfPos(c);
		}
		else{
			animSpeed = dtime * speedMultiplier;
		}
		animation->param += animSpeed;
		camera_pos_y += animSpeed;
		look_at_y += animSpeed;
		
// 		prev_cam_y = camera_pos_y;		/* DODATO jer se ispomeraju ostale scene */
// 		prev_look_y = look_at_y;
		
		checkWallSwichingCond(animation, wallPos1, wallPos2);
		checkStepsGenCond(animation);
		updateBotTopSteps(animation);
	}
}

void checkStepsGenCond(Animation* animation){
	// Kada treba da generise nove stepenice na odgovarajucoj polovini niza 
	
	int p = animation->checkpointSteps;
	if( p < animation->param && animation->param < p + animSpeed){
		animation->checkpointSteps += DIFF_STEPS_HALFS;
		if(stepsHalf == SND_HALF){
			generateNewSteps(steps, steps[NUM_OF_STEPS/2]->pos_y, NUM_OF_STEPS/2, NUM_OF_STEPS);
			stepsHalf = FST_HALF;
		}
		else{
			generateNewSteps(steps, steps[NUM_OF_STEPS]->pos_y, 0, NUM_OF_STEPS/2);
			stepsHalf = SND_HALF;
		}
	}
}
void updateBotTopSteps(Animation *animation){
	int p = animation->checkpointStepOnScr;
	if( p < animation->param && animation->param < p + animSpeed){
		animation->checkpointStepOnScr += CHEKPOINT_SOS;
		botStepOnScreen++;
		topStepOnScreen++;
		if(topStepOnScreen > NUM_OF_STEPS)
			topStepOnScreen = 1;
		if(botStepOnScreen > NUM_OF_STEPS)
			botStepOnScreen = 1;
	}
}

int gameIsLost(Character *c){
	if(c->position[1] < steps[botStepOnScreen]->pos_y){
		return -1;
	}
	else return 1;
}
void exitGame(void){
	freeWallPos(gameplayWall_1);
	freeWallPos(gameplayWall_2);
	freeCharacter(Harold);
	freeSteps(steps);
	freeQuads();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
void restartGame(void){
	/* Postaviti da brise i pravi sve sa initom... */
/* Sve parametre koji su na pocetku postavljeni vraca na originalno stanje */
	camera_pos_x = INIT_CAMERA_X;
	camera_pos_y = INIT_CAMERA_Y;
	camera_pos_z = INIT_CAMERA_Z;
	
	look_at_x = INIT_LOOK_X;
	look_at_y = INIT_LOOK_Y;
	look_at_z = INIT_LOOK_Z;
	
	stepsHalf = SND_HALF;
	switchWalls = SWITCH_FST;
	
	generateNewSteps(steps, INIT_FIRST_STEP_Y, 0, NUM_OF_STEPS/2);
	
	gameplayWall_1[0] = INIT_WALL_X_1;
	gameplayWall_1[1] = INIT_WALL_Y_1;
	gameplayWall_1[2] = INIT_WALL_Z_1;
	gameplayWall_2[0] = INIT_WALL_X_2;
	gameplayWall_2[1] = INIT_WALL_Y_2;
	gameplayWall_2[2] = INIT_WALL_Z_2;
	
	switchWalls = SWITCH_FST;
	animation->status = NOTSTARTED;
	animation->param = 0;
	animation->checkpointSteps = CHEKPOINT_STEPS;
	
	speedMultiplier = 1;
	gameTime = 0;
	gameScore = 0;
	
	topStepOnScreen = INIT_TOP_INDEX;
	botStepOnScreen = INIT_BOT_INDEX;
	
	freeCharacter(Harold);
	Harold = init_character(INIT_CHARACTER_X, INIT_CHARACTER_Y, 0);
}
/* DODATO 31.06.  */
void pauseGame(Animation *a){
	if(a->status != NOTSTARTED)
		a->status = PAUSE;
}
void resumeGame(Animation *a){
	if(a->status != NOTSTARTED)
		a->status = ONGOING;
}
