#pragma once
#include "Agent.h"

extern Vector3f blinkyPos;

class Blinky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Blinky();
	Blinky(int x, int y, int z);
	void draw() override;
	void updateVel() override;
};

class Pinky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Pinky();
	Pinky(int x, int y, int z);
	void draw() override;
	void updateVel() override;
};

class Inky : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Inky();
	Inky(int x, int y, int z);
	void draw() override;
	void updateVel() override;
};

class Clyde : public Ghost {
private:
	std::array<int, 2> destination; // un-active objective
public:
	Clyde();
	Clyde(int x, int y, int z);
	void draw() override;
	void updateVel() override;
};