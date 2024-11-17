#pragma once
#include "Agent.h"

class Blinky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Blinky();
	Blinky(int x, int y, int z);
	void move() override;
	void draw() override;
	void updateVel() override;
};

class Pinky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Pinky();
	Pinky(int x, int y, int z);
	void move() override;
	void draw() override;
};

class Inky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Inky();
	Inky(int x, int y, int z);
	void move() override;
	void draw() override;
};

class Clyde : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Clyde();
	Clyde(int x, int y, int z);
	void move() override;
	void draw() override;
};