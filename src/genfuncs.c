#include "../include/global.h"
#include "../include/animation.h"
#include "../include/genfuncs.h"
#include "../include/display.h"
#include "../include/shapes.h"

void specialKeyUp(int key, int x, int y) {  
	switch(key){
		case GLUT_KEY_LEFT:
			keys[K_LEFT] = 0;
			break;
		case GLUT_KEY_RIGHT:
			keys[K_RIGHT] = 0;
			break;
		default:
			break;
	}
	
}
void keyUp(unsigned char key, int x, int y){

}
void on_keyboard(unsigned char key, int x, int y){
	
	switch(key){
		case 'q':
			exit(EXIT_SUCCESS);
			break;
		case SPACE:
			switch(activeSceneIndex){
				case GAMEPLAY_INDEX:
					jump(Harold);
					break;
				case WELCOME_INDEX: // HARDCODE 
					if(cursorPosition==0)
						activeSceneIndex = GAMEPLAY_INDEX;
					else if(cursorPosition==1){
						activeSceneIndex = INSTR_INDEX;
						previousScene = WELCOME_INDEX;
					}
					else activeSceneIndex = EXIT_INDEX;
					cursorPosition = 0;
					break;
				case PAUSE_INDEX:
					if(cursorPosition == 0){
						activeSceneIndex = GAMEPLAY_INDEX;
						resumeGame(animation);
					}
					else if(cursorPosition == 1){
						previousScene = PAUSE_INDEX;
						activeSceneIndex = INSTR_INDEX;
					}
					else {
						activeSceneIndex = WELCOME_INDEX;
						restartGame();
					}
					cursorPosition = 0;
					break;
				case LOSE_INDEX:
					if(cursorPosition==0){
						restartGame();
						activeSceneIndex = GAMEPLAY_INDEX;
					}
					else {
						activeSceneIndex = WELCOME_INDEX;
						restartGame();
					}
					cursorPosition = 0;
					break;
			}
			glutPostRedisplay();
			break;
		case ESC:
			switch(activeSceneIndex){
				case WELCOME_INDEX:
					if(cursorPosition == WELCOME_OPT_COUNT - 1){
						activeSceneIndex = EXIT_INDEX;
						cursorPosition = 0;
					}
					else
						cursorPosition = WELCOME_OPT_COUNT - 1;
					break;
				case GAMEPLAY_INDEX:
					activeSceneIndex = PAUSE_INDEX;
					cursorPosition = 0;
					pauseGame(animation);
					break;
				case EXIT_INDEX:
					exitGame();
					break;
				case PAUSE_INDEX:
					if(cursorPosition == PAUSE_OPT_COUNT - 1){
						activeSceneIndex = WELCOME_INDEX;
						cursorPosition = 0;
						restartGame();
					}
					else
						cursorPosition = PAUSE_OPT_COUNT - 1;
					break;
				case INSTR_INDEX:
					if(previousScene != -1)
						activeSceneIndex = previousScene;
					break;
				default:
					break;
			}
			glutPostRedisplay();
			break;
		default: break;
	}
}
void special_keys(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			keys[K_LEFT] = 1;
			break;
		case GLUT_KEY_RIGHT:
			keys[K_RIGHT] = 1;
			break;
		default:
			break;
	}
	if(activeSceneIndex != GAMEPLAY_INDEX 
		|| activeSceneIndex != INSTR_INDEX ){
		switch(key){
			case GLUT_KEY_UP:
				switch(activeSceneIndex){
					case WELCOME_INDEX:
						cursorPosition = (cursorPosition-1)%WELCOME_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = WELCOME_OPT_COUNT-1;
						break;
					case PAUSE_INDEX:
						cursorPosition = (cursorPosition-1)%PAUSE_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = PAUSE_OPT_COUNT-1;
						break;
					case LOSE_INDEX:
						cursorPosition = (cursorPosition-1)%LOSE_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = LOSE_OPT_COUNT-1;
						break;
					default: break;
				}
				break;
			case GLUT_KEY_DOWN:
				switch(activeSceneIndex){
					case WELCOME_INDEX:
						cursorPosition = (cursorPosition+1)%WELCOME_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = WELCOME_OPT_COUNT-1;
						break;
					case PAUSE_INDEX:
						cursorPosition = (cursorPosition+1)%PAUSE_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = PAUSE_OPT_COUNT-1;
						break;
					case LOSE_INDEX:
						cursorPosition = (cursorPosition+1)%LOSE_OPT_COUNT;
						if(cursorPosition < 0)
							cursorPosition = LOSE_OPT_COUNT-1;
						break;
					default: break;
				}
				break;
				default: break;
		}
	}
	glutPostRedisplay();
}

void calcTime(void){
	// Azuriraj proteklo vreme od poslednje promene, delta time!!!!
	double thisTime = glfwGetTime();
	dtime = thisTime - lastTime;
	lastTime = thisTime;
}
void idle(void){
	calcTime();
	animSpeed = dtime;
	keyOperations();
	if(gameIsLost(Harold)<0){
		activeSceneIndex = LOSE_INDEX;
	}
	if(activeSceneIndex == GAMEPLAY_INDEX){
		changeAnimParams(animation, Harold, gameplayWall_1, gameplayWall_2);
		characterUpdate(Harold);
		manageMovement(Harold);
	}
	glutPostRedisplay();
}
void initKeys(void){
	int i;
	//inicijalizacija dugmica - ni jedan nije pritisnut
	for(i=0; i<NUM_OF_KEYS; i++){
		keys[i] = 0;
	}
}
