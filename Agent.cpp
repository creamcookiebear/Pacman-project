#include "Agent.h"
#include <GL/freeglut.h>
#include <math.h>
//#include <FreeImage.h>

Vector3f Agent::map2float(int x, int y) {
	float xf = -MAP_WIDTH * BLOCK_SIZE / 2.f + BLOCK_SIZE / 2.f + x * BLOCK_SIZE;
	float yf = -MAP_HEIGHT * BLOCK_SIZE / 2.f + BLOCK_SIZE / 2.f + y * BLOCK_SIZE;
	return Vector3f(xf, yf, 0.f);
}

std::array<int, 2> Agent::float2map(Vector3f p) { //get nearist block idx
	float xf = p[0]; float yf = p[1];
	int x, y;
	x = static_cast<int>(0.5 * (2.f * xf / BLOCK_SIZE + MAP_WIDTH - 1) + 0.5);
	y = static_cast<int>(0.5 * (2.f * yf / BLOCK_SIZE + MAP_HEIGHT - 1) + 0.5);
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
		
		//bInxPosUpdated = true;
		//vel[0] = 0.f; vel[1] = 0.f;
		//return;
	}
	else {
		std::cout << "nextVel case" << std::endl;
		// Check if NextVel is valid
		bool flag = false;
		
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
		}
		std::cout << "flag: " << flag << std::endl;
		if (flag) {// if next Velocity is legal move, update
			std::cout << "Changed Vel" << std::endl;
			vel = nextVel;
			nextVel[0] = 0.f; nextVel[1] = 0.f;
		}
		return; // don't care of Vel before update.

		std::cout << "discard nextVel. Illegal move";
		std::cout << "\tat idxPos: " << idxPos[0] << ", " << idxPos[1] << std::endl;
	}

	// if Vel is not changed, Check if curr Vel is Valid
	if (map.W(xi + (int)vel[0], yi + (int)vel[1])) {// If Wall
		bInxPosUpdated = true;
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
	if (map.P(idxPos[0], idxPos[1])) {
		point += 1;
		map.sP(idxPos[0], idxPos[1], false);
	}
	if (map.Pow(idxPos[0], idxPos[1])) { // Check if at PowerPellet
		point += 1;
		map.sPow(idxPos[0], idxPos[1], false);
		bisPow = true;
		::isPow = true; // set Global isPow = true
		 
		powerTick = Agent::maxPowerTick;
	}
	pacPos = pos; // update global Pacman pos
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
		bInxPosUpdated = true;
		postMoveHandler();
		return; // stop if no vel
	}
	
	pos[0] = pos[0] + vel[0]; pos[1] = pos[1] + vel[1];
	/*
	if (static_cast<int>((pos[0]) / BLOCK_SIZE) + MAP_WIDTH / 2 != idxPos[0] || \
		static_cast<int>((pos[1]) / BLOCK_SIZE) + MAP_HEIGHT / 2 != idxPos[1]) {
		bInxPosUpdated = true;
		idxPos[0] = static_cast<int>((pos[0]) / BLOCK_SIZE) + MAP_WIDTH / 2;
		idxPos[1] = static_cast<int>((pos[1]) / BLOCK_SIZE) + MAP_HEIGHT / 2;
	}*/
	if (abs(Agent::map2float(idxPos)[0] - pos[0]) >= BLOCK_SIZE || \
		abs(Agent::map2float(idxPos)[1] - pos[1]) >= BLOCK_SIZE) {
		bInxPosUpdated = true;
		idxPos[0] = static_cast<int>((pos[0]) / BLOCK_SIZE) + MAP_WIDTH / 2;
		idxPos[1] = static_cast<int>((pos[1]) / BLOCK_SIZE) + MAP_HEIGHT / 2;
	}
	else bInxPosUpdated = false;

	postMoveHandler();
}

void Pacman::draw() {
	//draw something
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
	if (static_cast<int>((pos[0]) / BLOCK_SIZE) + MAP_WIDTH / 2 != idxPos[0] || \
		static_cast<int>((pos[1]) / BLOCK_SIZE) + MAP_HEIGHT / 2 != idxPos[1]) {
		bInxPosUpdated = true;
		pastVel = vel;
		idxPos[0] = static_cast<int>((pos[0]) / BLOCK_SIZE) + MAP_WIDTH / 2;
		idxPos[1] = static_cast<int>((pos[1]) / BLOCK_SIZE) + MAP_HEIGHT / 2;
	}
	else bInxPosUpdated = false;
}

bool Ghost::isActive() const { return bisActive; }