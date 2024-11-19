#pragma once

enum DIRECTION { UP, DOWN, RIGHT, LEFT, STAY, NONE };

#define FPS_SET 30
#define VEL_SCALE 0.5f

#define WINDOW_X 300
#define WINDOW_Y 300

#define WINDOW_W 640		// window's width
#define WINDOW_H 640		// window's height

#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

#define tick 5
#define pad tick / 2
#define MAP_WIDTH 28 // original=28
#define MAP_HEIGHT 31 // original=31
#define BLOCK_SIZE 10

// the bellow are for debug perpose
#define PRINT(C) std::cout<<C
#define PRINT_POS() std::cout<<"("<<idxPos[0]<<", "<<idxPos[1]<<")"<<std::endl