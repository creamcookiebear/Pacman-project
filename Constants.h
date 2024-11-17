#pragma once

enum DIRECTION { UP, DOWN, RIGHT, LEFT, STAY };

#define WINDOW_X 300
#define WINDOW_Y 300

#define WINDOW_W 640		// window's width
#define WINDOW_H 640		// window's height

#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

#define tick 5
#define pad tick / 2
#define MAP_WIDTH 9 // original=28
#define MAP_HEIGHT 9 // original=31
#define BLOCK_SIZE 40