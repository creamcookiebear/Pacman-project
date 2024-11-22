#pragma once

#include "Map.h"
#include "Vector3f.h"
#include "Vector3i.h"
#include <array>
#include <vector>
#include "Material.h"
#include "Constants.h"

extern Map map;
extern bool isPow;
extern const int FPS;
extern Vector3f pacPos;
extern Vector3i pacVel;

class Agent {
protected:
	Vector3f pos;
	Vector3f vel;
	std::array<int,2> idxPos;
	bool bInxPosUpdated;
	
	bool bisPow;
	int powerTick; // Powerup for 100 tick
	bool bisRespawn;

	bool directionCheck();
	bool directionCheck(const Vector3f&, const Vector3f&) const;
	Material mtl;

	virtual void prevMoveHandler()=0;
	virtual void postMoveHandler()=0;
public:
	constexpr static const int maxPowerSec = 15;
	static const int maxPowerTick = Agent::maxPowerSec * FPS_SET;
	static Vector3f map2float(int, int);
	static Vector3f map2float(std::array<int, 2> arr) {
		return map2float(arr[0], arr[1]);
	}
	static std::array<int,2> float2map(Vector3f p);
	static Vector3f direction2vec(DIRECTION);

	//static Vector3f float2Map(float x, float y);
	Agent();
	Agent(int x, int y, int z);
	void setPos(float x, float y, float z);
	void setVel(float x, float y, float z);
	const Vector3f& getPos() const;
	const Vector3f& getVel() const;

	void setIndexPosition(int x, int y);
	bool isIndexPositionUpdated() const;

	virtual void updateVel()=0;

	bool isPow() const;
	bool isRespawn() const;
	void setMTL(const Material& m);
	virtual void move() = 0;
	virtual void draw() = 0;
};

class Pacman : public Agent {
private:
	int lifes;
	int animationState;
	int point;
	Vector3f nextVel;
	void prevMoveHandler() override;
	void postMoveHandler() override;
public:
	Pacman();
	Pacman(int x, int y, int z);
	void updateVel() override;
	int getLifes() const;
	int getAnimationState() const;
	void setNextVel(DIRECTION);
	void move() override;
	void draw() override;
};

class Ghost : public Agent {
protected:
	bool bisActive;
	int activeCnt;
	int respawnTick;
	int scatterCnt;
	DIRECTION navigator(Vector3f destination) const;
	DIRECTION getOppositeDirection(DIRECTION dir) const;
	void prevMoveHandler() override;
	void postMoveHandler() override;
	Vector3f pastVel;
public:
	static const int maxActiveCnt;
	static const int maxRespawnTick = FPS_SET * 5;
	static const int ghostActiveSec = 20;
	void switchColorAtPow() const;

	Ghost();
	Ghost(int x, int y, int z);
	
	void setIsRespawn(bool b);
	bool isActive() const;
	void move() override;
	virtual void draw() override = 0;
};