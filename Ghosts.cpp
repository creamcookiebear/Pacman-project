#include "Ghosts.h"
#include <GL/freeglut.h>
#include <FreeImage.h>

Blinky::Blinky():
	Ghost(),
	destination{ MAP_WIDTH-2, MAP_HEIGHT-2 } {respawnTick = 0;}
Blinky::Blinky(int x, int y, int z):
	Ghost(x,y,z),
	destination{ MAP_WIDTH - 2, MAP_HEIGHT - 2 } {respawnTick = 0;}

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
	if (::isPow) {
		switchColorAtPow();
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}

Pinky::Pinky() :
	Ghost(),
	destination{ 1, MAP_HEIGHT - 2 } {
	bisRespawn = true;
	respawnTick = FPS_SET * 5;
}
Pinky::Pinky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, MAP_HEIGHT - 2 } {
	respawnTick = FPS_SET * 5;
	bisRespawn = true;
}

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
		if(!bisActive) destPos = Agent::map2float(destination[0], destination[1]); // going to fixed point
	}
	if(::isPow) {// running away from Pacman
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
	if (::isPow) {
		switchColorAtPow();
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}

Inky::Inky() :
	Ghost(),
	destination{ MAP_WIDTH - 2, 1 } {
	respawnTick = FPS_SET*10;
	bisRespawn = true;
}
Inky::Inky(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ MAP_WIDTH - 2, 1 } {
	respawnTick = FPS_SET * 10;
	bisRespawn = true;
}

void Inky::updateVel() {
	Vector3f destPos;
	PRINT("=========================================\nAt Inky" << std::endl);
	PRINT("PacPos: "); pacPos.print();

	
	PRINT("isPow :" << ::isPow << std::endl);
	PRINT("isActive :" << bisActive << std::endl);
	if (!::isPow) {
		if (bisActive) {
			Vector3f primalDest = (2 * pacPos) - blinkyPos; // reflected pos of Blinky by Pacman
			int xpi = primalDest[0]; int ypi = primalDest[1];
			int xi = Agent::float2map(pos)[0]; int yi = Agent::float2map(pos)[1];

			float minDist = 1000000;
			std::array<int, 3> minPos = { {Agent::float2map(pacPos)[0],Agent::float2map(pacPos)[1],0}};

			for (std::vector<Intersection>::const_iterator itr = map.getIntersections().begin(); \
				itr != map.getIntersections().end(); itr++) {
				const std::array<int, 3>& interPos = itr->getPosition();
				if (interPos[0] > 100000 || interPos[1] > 100000) {
					PRINT("Error case: Improper Intersection position" << std::endl);
					continue;
				}
				if (sqrt(SQ(interPos[0] - xpi) + SQ(interPos[1] - ypi)) < minDist) {
					minPos = interPos;
					minDist = sqrt((float)(SQ(interPos[0] - xpi) + SQ(interPos[1] - ypi)));
				}
			}
			destPos = map2float(minPos[0], minPos[1]);
		}
		if(!bisActive) destPos = Agent::map2float(destination[0], destination[1]); // going to fixed point
	}
	if(::isPow) {// running away from Pacman
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

void Inky::draw() {
	// draw
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission().getPos());
	if (::isPow) {
		switchColorAtPow();
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}

Clyde::Clyde() :
	Ghost(),
	destination{ 1, 1 } {
	respawnTick = FPS_SET * 15;
	bisRespawn = true;
}
Clyde::Clyde(int x, int y, int z) :
	Ghost(x, y, z),
	destination{ 1, 1 } {
	respawnTick = FPS_SET * 15;
	bisRespawn = true;
}

void Clyde::updateVel() {
	Vector3f destPos;
	PRINT("PacPos: "); pacPos.print();
	if (!::isPow) {
		if (bisActive){// chasing Pacman
			float dist = (pacPos - pos).norm();
			if (dist < 8.f * BLOCK_SIZE) {// if closer than 8 block, go to scatter
				destPos = Agent::map2float(destination[0], destination[1]);
			}
			else {
				destPos = pacPos;
			}
		}
		if (!bisActive) destPos = Agent::map2float(destination[0], destination[1]); // going to fixed point
	}
	if(::isPow) {// running away from Pacman
		int xi = idxPos[0]; int yi = idxPos[1];

		int ring[4] = { 1,0,0,0 };
		int maxDist = 0;
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
	PRINT("Destination: "); destPos.print();
	DIRECTION direction = navigator(destPos);
	vel = Agent::direction2vec(direction);
	bInxPosUpdated = false;
}

void Clyde::draw() {
	// draw
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission().getPos());
	if (::isPow) {
		switchColorAtPow();
	}
	else {
		glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	}
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2] + BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE / 3.f, 20, 20);
	glPopMatrix();
}