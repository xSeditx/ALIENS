#pragma once

#include<iostream>
#include"cell.h"
#include<Windows.h>
#include"Window.h"
#include<string.h>

extern bool SHOW_VISION;
extern bool SHOW_SEX;
extern bool SHOW_COLLISION;


long BIRTHS = 0,
DEATHS = 0;

extern int  NUM_CREATURES;

extern int LEFT_BOUNDS;
extern int TOP_BOUNDS;
extern int RIGHT_BOUNDS;
extern int BOTTOM_BOUNDS;


void INITIALIZE_WORLD();
void DRAW_WORLD();
