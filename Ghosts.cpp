#include "Ghosts.h"
#include <GL/freeglut.h>
#include <FreeImage.h>

Blinky::Blinky():
	Ghost(),
	destination{ 1, 1 } {}
Blinky::Blinky(int x, int y, int z):
	Ghost(x,y,z),
	destination{ 1, 1 } {}

void Blinky::updateVel() {
	Vector3f destPos;
	PRINT("PacPos: "); pacPos.print();
	if (!::isPow) {
		if (bisActive) destPos = pacPos; // chasing Pacman
		else destPos = Agent::map2float(destination[0], destination[1]); // going to fixed point
	}
	else {// running away from Pacman
		int xi = idxPos[0]; int yi = idxPos[1];
		
		int ring[4] = { 1,0,0,0 };
		int maxDist = 0;
		std::array<int, 2> maxDistPos;
		int xp = Agent::float2map(pacPos)[0]; int yp = Agent::float2map(pacPos)[1];

		for (int i = 0; i < 4; i++) {
			int xs = xi + ring[0] - ring[1]; int ys = yi + ring[2] - ring[3];
			if (Map::isInbound(xs, ys) && !map.W(xs, ys) && \
				((xp - xs) * (xp - xs) + (yp - ys) * (yp - ys)>maxDist)) {//inbound & not wall & maxDist
				maxDistPos = std::array<int, 2>{{xs, ys}};
			}
			int temp = ring[0];
			ring[0] = ring[1]; ring[1] = ring[2]; ring[2] = ring[3]; ring[3] = temp;
		}
		destPos = Agent::map2float(maxDistPos[0], maxDistPos[1]);
	}
	PRINT("Destination: "); destPos.print();
	DIRECTION direction = navigator(destPos);
	vel = Agent::direction2vec(direction);
	bInxPosUpdated = false;
}

void Blinky::draw() {
	// draw
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission().getPos());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}

Pinky::Pinky() :
	Ghost(),
	destination{ 26, 29 } {}
Pinky::Pinky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 26, 29 } {}

void Pinky::updateVel() {
	Vector3f destPos;
	PRINT("PacPos: "); pacPos.print();
	if (!::isPow) {
		if (bisActive) {
			std::array<int,2> pacIdxPos = Agent::float2map(pacPos);
			if (!Map::isInbound(pacIdxPos)||map.W(pacIdxPos[0],pacIdxPos[1])) {
				destPos = pacPos;
			}
			else if(pacVel[0] == 0 && pacVel[1] == 0){// if Pacman is stop
				destPos = pacPos;
			}
			else {
				destPos = pacPos;
				for (int i = 1; i < 5; i++) {
					int xi = pacIdxPos[0] + i * (int)pacVel[0]; int yi = pacIdxPos[1] + i * (int)pacVel[1];
					
					if (map.isInbound(xi, yi) && !map.W(xi, yi)) {
						destPos[0] = pacPos[0] + pacVel[0] * i;
						destPos[1] = pacPos[1] + pacVel[1] * i;
					}
					else {
						break;
					}
				}
			}
			if (destPos[0] == 0 && destPos[1] == 0) {
				destPos = pacPos;
			}else{}
		}
		else destPos = Agent::map2float(destination[0], destination[1]); // going to fixed point
	}
	else {// running away from Pacman
		int xi = idxPos[0]; int yi = idxPos[1];

		int ring[4] = { 1,0,0,0 };
		int maxDist = -1;
		std::array<int, 2> maxDistPos;
		int xp = Agent::float2map(pacPos)[0]; int yp = Agent::float2map(pacPos)[1];

		for (int i = 0; i < 4; i++) {
			int xs = xi + ring[0] - ring[1]; int ys = yi + ring[2] - ring[3];
			if (Map::isInbound(xs, ys) && !map.W(xs, ys) && \
				((xp - xs) * (xp - xs) + (yp - ys) * (yp - ys) > maxDist)) {//inbound & not wall & maxDist
				maxDistPos = std::array<int, 2>{{xs, ys}};
			}
			int temp = ring[0];
			ring[0] = ring[1]; ring[1] = ring[2]; ring[2] = ring[3]; ring[3] = temp;
		}
		destPos = Agent::map2float(maxDistPos[0], maxDistPos[1]);
	}
	if (destPos[0] == 0.f && destPos[1] == 0.f) {
		destPos = pacPos;
	}
	PRINT("Destination: "); destPos.print();
	DIRECTION direction = navigator(destPos);
	vel = Agent::direction2vec(direction);
	bInxPosUpdated = false;
}

void Pinky::draw() {
	// draw
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission().getPos());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}

Inky::Inky() :
	Ghost(),
	destination{ 1, 1 } {}
Inky::Inky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {}

void Inky::draw() {
	// draw
}
void Inky::updateVel() {}

Clyde::Clyde() :
	Ghost(),
	destination{ 1, 1 } {}
Clyde::Clyde(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {}

void Clyde::draw() {
	// draw
}
void Clyde::updateVel() {}