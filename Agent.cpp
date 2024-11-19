#include "Agent.h"
#include <GL/freeglut.h>
#include <math.h>
//#include <FreeImage.h>

Vector3f Agent::map2float(int x, int y) {
	float xf = 0.5f * BLOCK_SIZE * (1.f + x*2.f - MAP_WIDTH);
	float yf = 0.5f * BLOCK_SIZE * (1.f + y*2.f - MAP_HEIGHT);
	return Vector3f(xf, yf, 0.f);
}

std::array<int, 2> Agent::float2map(Vector3f p) { //get nearist block idx
	float xf = p[0]; float yf = p[1];
	int x, y;
	//x = static_cast<int>(0.5 * (2.f * xf / BLOCK_SIZE + MAP_WIDTH - 1) + 0.5);
	//y = static_cast<int>(0.5 * (2.f * yf / BLOCK_SIZE + MAP_HEIGHT - 1) + 0.5);
	x = static_cast<int>(xf / BLOCK_SIZE + MAP_WIDTH * 0.5f);
	y = static_cast<int>(yf / BLOCK_SIZE + MAP_HEIGHT * 0.5f);
	return std::array<int, 2> {{x, y}};;
}

Agent::Agent() :
	bisPow(false),
	powerTick(0),
	bisRespawn(false),
	pos(0.f,0.f,0.f),
	vel(0.f, 0.f, 0.f),
	idxPos{ 4,4 }, bInxPosUpdated(true) {}

Agent::Agent(int x, int y, int z):
	bisPow(false),
	powerTick(0),
	bisRespawn(false),
	pos(x, y, z),
	vel(0.f, 0.f, 0.f),
	idxPos{ 4,4 }, bInxPosUpdated(true){}

bool Agent::directionCheck() {
	return directionCheck(pos, Vector3f());
}

bool Agent::directionCheck(const Vector3f& p, const Vector3f& v) const {
	if (v[0] && v[1]) {
		return true;
	}
	if (v[0] == 0.f) {
		if (v[1] > 0.f) return idxPos[1]!=MAP_HEIGHT &&!map.W(idxPos[0], idxPos[1]+1); // Up
		else return idxPos[1] != 0 && !map.W(idxPos[0], idxPos[1] - 1); //Down
	}
	else {
		if (v[0] > 0.f) return idxPos[0] != MAP_WIDTH && !map.W(idxPos[0] + 1, idxPos[1]);  // Right
		else return idxPos[0] != 0 && !map.W(idxPos[0]-1, idxPos[1]); //Left
	}
	return true;
}

void Agent::setPos(float x, float y, float z) {
	pos[0] = x; pos[1] = y; pos[2] = z;
	idxPos = float2map(pos);
}
void Agent::setVel(float x, float y, float z) {
	vel[0] = x; vel[1] = y; vel[2] = z;
}
const Vector3f& Agent::getPos() const {
	return pos;
}
const Vector3f& Agent::getVel() const {
	return vel;
}

void Agent::setIndexPosition(int x, int y) {
	idxPos[0] = x; idxPos[1] = y;
	pos = map2float(x, y);
}
bool Agent::isIndexPositionUpdated() const { return bInxPosUpdated; }

bool Agent::isPow() const {
	return bisPow;
}
bool Agent::isRespawn() const {
	return bisRespawn;
}
void Agent::setMTL(const Material& m) { mtl = m; }


Pacman::Pacman() :
	Agent(),
	lifes(3),
	animationState(0),
	point(0)
	{}

Pacman::Pacman(int x, int y, int z) :
	Agent(x, y, z),
	lifes(3),
	animationState(0),
	point(0),
	nextVel(0, 0, 0) {}

void Pacman::updateVel() {
	int xi = Agent::float2map(pos)[0];
	int yi = Agent::float2map(pos)[1];
	std::cout << "idxPos: " << xi << ", " << yi << std::endl;
	if (nextVel[0] == 0.f && nextVel[1] == 0.f) {
		std::cout << "no nextVel case" << std::endl;
	}
	else {
		std::cout << "nextVel case" << std::endl;
		// Check if NextVel is valid
		bool flag = false;
		
		/*
		if (nextVel[0] > 0) {// Right
			flag = (MAP_WIDTH - 1 != xi) && !map.W(xi + 1, yi);
		}
		else if (nextVel[0] < 0) {// Left
			flag = (0 != xi) && !map.W(xi - 1, yi);
		}
		else if (nextVel[1] > 0) {// Up
			flag = (MAP_HEIGHT != yi) && !map.W(xi, yi + 1);
		}
		else if (nextVel[1] < 0) {// Down
			std::cout <<"map check" << !map.W(xi, yi - 1) << std::endl;
			flag = (0 != yi) && !map.W(xi, yi - 1);
		}*/

		int x_next = xi + (int)nextVel[0]; int y_next = yi + (int)nextVel[1];
		float xf = Agent::map2float(x_next, y_next)[0]; float yf = Agent::map2float(x_next, y_next)[1];
		float dist = abs(xf - pos[0]) + abs(yf - pos[1]);
		if (!map.W(x_next, y_next) && (dist < (BLOCK_SIZE + VEL_SCALE * 0.5f)) && (dist > (BLOCK_SIZE - VEL_SCALE * 0.5f))) {
			flag = true;
		}

		std::cout << "flag: " << flag << std::endl;
		if (flag) {// if next Velocity is legal move, update
			std::cout << "Changed Vel" << std::endl;
			vel = nextVel;
			nextVel[0] = 0.f; nextVel[1] = 0.f;
		}
		// don't care of Vel before update.

		std::cout << "discard nextVel. Illegal move";
		std::cout << "\tat idxPos: " << idxPos[0] << ", " << idxPos[1] << std::endl;
	}

	// Check if curr Vel is Valid
	int x_next = xi + (int)vel[0]; int y_next = yi + (int)vel[1];
	float xf = Agent::map2float(x_next, y_next)[0]; float yf = Agent::map2float(x_next, y_next)[1];
	float dist = abs(xf - pos[0]) + abs(yf - pos[1]);
	if (map.W(x_next, y_next) && (dist<(BLOCK_SIZE+VEL_SCALE*0.5f))&& (dist > (BLOCK_SIZE - VEL_SCALE * 0.5f))) {// If Wall
		vel[0] = 0.f; vel[1] = 0.f; // Stop
		std::cout << "stopped by wall col" << std::endl;
	}

	// else, Keep going
}

int Pacman::getLifes() const {
	return lifes;
}
int Pacman::getAnimationState() const {
	return animationState;
}
void Pacman::setNextVel(DIRECTION dir) {
	switch (dir) {
	case UP:
		nextVel[0] = 0.f; nextVel[1] = 1.f;
		break;
	case DOWN:
		nextVel[0] = 0.f; nextVel[1] = -1.f;
		break;
	case RIGHT:
		nextVel[0] = 1.f; nextVel[1] = 0.f;
		break;
	case LEFT:
		nextVel[0] = -1.f; nextVel[1] = 0.f;
		break;
	default:
		break;
	}
	nextVel.print();
}

void Pacman::prevMoveHandler() {
	// check status before movement
	if (powerTick > 0) {
		if (--powerTick == 0) {
			bisPow = false;
			::isPow = false; // set Global isPow = fasle
			powerTick = 0; // initialize power tick
		}
	}
}

void Pacman::postMoveHandler() {
	int xi = Agent::float2map(pos)[0]; int yi = Agent::float2map(pos)[1];
	if (map.P(xi, yi)) {
		point += 1;
		map.sP(xi, yi, false);
	}
	if (map.Pow(xi, yi)) { // Check if at PowerPellet
		point += 1;
		map.sPow(xi, yi, false);
		bisPow = true;
		::isPow = true; // set Global isPow = true
		
		powerTick = Agent::maxPowerTick;
	}
	pacPos = pos; // update global Pacman pos
	pacVel[0] = static_cast<int>(vel[0]); pacVel[1] = static_cast<int>(vel[1]);
}

void Pacman::move() {
	// "Move()" does not check collision with wall! If collides with wall, Move() should not be called!
	// 
	// What is does:
	// 1. update pos based on vel
	// 2. update idxPos
	// 3. if updated, set bInxPosUpdated = true
	//	ㄴ this is for handling direction(vel) changes, and also collision check with wall, outside of Move().
	// 4. calls post/prev Move Handler(), which manages time-related parameters, map interactive objects.
	//
	// if vel == zero, it will set bInxPosUpdated = true.

	prevMoveHandler();

	if (vel[0] == 0.f && vel[1] == 0.f) {
		
		postMoveHandler();
		return; // stop if no vel
	}
	
	else {
		pos[0] = pos[0] + vel[0] * VEL_SCALE; pos[1] = pos[1] + vel[1] * VEL_SCALE;
	}
	if (abs(Agent::map2float(idxPos)[0] - pos[0]) >= BLOCK_SIZE || \
		abs(Agent::map2float(idxPos)[1] - pos[1]) >= BLOCK_SIZE) {
		bInxPosUpdated = true;
		idxPos[0] = Agent::float2map(pos)[0];
		idxPos[1] = Agent::float2map(pos)[1];
	}
	

	postMoveHandler();
}

void Pacman::draw() {
	//draw something
	if (bisPow) {
		int counter = powerTick;
		// Generate rainbow colors based on the counter
		float frequency = 0.1f; // Frequency controls the speed of color change
		float red = sin(frequency * counter + 0) * 0.5f + 0.5f;
		float green = sin(frequency * counter + 2.0f) * 0.5f + 0.5f;
		float blue = sin(frequency * counter + 4.0f) * 0.5f + 0.5f;

		// Update material properties with new colors
		mtl.setAmbient(red * 0.6f, green * 0.6f, blue * 0.6f, 1.0f);
		mtl.setDiffuse(red * 0.8f, green * 0.8f, blue * 0.8f, 1.0f);
	}
	else {
		mtl.setAmbient(0.6f, 0.6f, 0.0f, 1.0f);
		mtl.setDiffuse(0.8f, 0.8f, 0.0f, 1.0f);
	}
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission().getPos());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient().getPos());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse().getPos());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular().getPos());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());
	
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]+ BLOCK_SIZE / 3.f);
	glutSolidSphere(BLOCK_SIZE/3.f, 20, 20);
	glPopMatrix();
}

const int Ghost::maxActiveCnt = ::FPS * Ghost::ghostActiveSec;

Ghost::Ghost() :
	Agent(),
	bisActive(true),
	activeCnt(Ghost::maxActiveCnt) {}

Ghost::Ghost(int x, int y, int z) :
	Agent(x, y, z),
	bisActive(true),
	activeCnt(Ghost::maxActiveCnt) {}

Vector3f Agent::direction2vec(DIRECTION dir) {
	switch (dir) {
	case UP:
		return Vector3f(0.f, 1.f, 0.f);
		break;
	case DOWN:
		return Vector3f(0.f, -1.f, 0.f);
		break;
	case LEFT:
		return Vector3f(-1.f, 0.f, 0.f);
		break;
	case RIGHT:
		return Vector3f(1.f, 0.f, 0.f);
		break;
	}
	return Vector3f();
}

void Ghost::prevMoveHandler() {
	if (::isPow) {// set Global isPow = fasle
		// bla bla
	}
	if (activeCnt == Ghost::maxActiveCnt) {
		bisActive = !bisActive;
		activeCnt = 0;
	}
	activeCnt++;
}
void Ghost::postMoveHandler() {
	if (abs(Agent::map2float(idxPos)[0] - pos[0]) >= BLOCK_SIZE || \
		abs(Agent::map2float(idxPos)[1] - pos[1]) >= BLOCK_SIZE) {
		bInxPosUpdated = true;
		pastVel = vel;
		idxPos = Agent::float2map(pos);
	}
	else bInxPosUpdated = false;
}

void Ghost::move() {
	// move
	prevMoveHandler();
	if (vel[0] == 0.f && vel[1] == 0.f) {
		//bInxPosUpdated = true;
		return;
	}

	pos[0] = pos[0] + vel[0] * VEL_SCALE; pos[1] = pos[1] + vel[1] * VEL_SCALE;


	postMoveHandler();
}

bool Ghost::isActive() const { return bisActive; }