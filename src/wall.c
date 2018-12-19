#include "../include/global.h"
#include "../include/wall.h"

float *gameplayWall_1;
float *gameplayWall_2;
WallToSwitch switchWalls;

float *newWallPos(float x, float y, float z){
	float *new = malloc(sizeof(float)*3);
	if(new == NULL){
		printf("initWall(): Failed alocation\n");
		exit(EXIT_FAILURE);
	}
	new[0]=x;
	new[1]=y;
	new[2]=z;
	return new;
}
void freeWallPos(float *w){
	free(w);
}
