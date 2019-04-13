#pragma once
#include "object.h"
#include "main.h"

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6


extern GLfloat big_wing[6][2], small_wing[6][2], body[5][2], back[5][2], sidewinder1[5][2], sidewinder2[5][2], center[1][2], airplane_color[7][3];

extern int airplane_clock;
extern  float airplane_s_factor;

void prepareObject(Object* obj);
void draw_airplane(Object* obj);