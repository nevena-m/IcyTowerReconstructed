#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "global.h"
#include "wall.h"
#include "character.h"
#include "step.h"

float WallCollision(float char_x);

int isOnStep(Character *c);
int findClosestStep(Character *c, WoodenStep **steps);
#endif