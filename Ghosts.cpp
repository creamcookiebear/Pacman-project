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
	DIRECTION direction = navigator(destPos);
	vel = Agent::direction2vec(direction);
}

void Blinky::move() {
	// move
	prevMoveHandler();
	if (vel[0] == 0 && vel[1] == 0) {
		bInxPosUpdated = true;
		return;
	}
	if (!::isPow) {
		pos[0] = pos[0] + vel[0]; pos[1] = pos[1] + vel[1];
	}

	postMoveHandler();
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
	destination{ 1, 1 } {}
Pinky::Pinky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {}
void Pinky::move() {
	// move
}
void Pinky::draw() {
	// draw
}

Inky::Inky() :
	Ghost(),
	destination{ 1, 1 } {}
Inky::Inky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {}
void Inky::move() {
	// move
}
void Inky::draw() {
	// draw
}

Clyde::Clyde() :
	Ghost(),
	destination{ 1, 1 } {}
Clyde::Clyde(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {}
void Clyde::move() {
	// move
}
void Clyde::draw() {
	// draw
}