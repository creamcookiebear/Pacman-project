#pragma once
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Vector3i.h"
#include "Vector3f.h"
#include "Intersection.h"
#include <GL/freeglut.h>
#include "Material.h"
#include "Constants.h"

class Map {
public:
	static const int width = MAP_WIDTH; // original=28
	static const int hight = MAP_HEIGHT; // original=31
	static const int blockSize = BLOCK_SIZE;

	static Vector3f map2float(int, int);
	static std::array<int, 2> float2map(Vector3f p);
	static std::array<int, 2> toMap(int x,int y) {
		return std::array<int, 2>{{MAP_HEIGHT -1 - y, x}};
	}
	static std::array<int, 2> toAge(std::array<int, 2> arr) {
		return std::array<int, 2>{{arr[1], MAP_HEIGHT - 1 - arr[0]}};
	}
	static bool isInbound(int, int);
	static bool isInbound(std::array<int, 2>);

	void drawWall(Vector3f);
	void drawPellet(Vector3f);
	void drawPower(Vector3f);

	Map();
	//Map(int w, int h);
	void sW(int i, int j, bool val); //set Wall, single element
	void sW(Vector3i pos, bool val);
	//const std::vector<std::vector<bool>>& W() const; //get Wall
	bool W(int x, int y) const;
	bool W(Vector3i pos) const; //get Pellet by element, by Vec3i
	bool W(Vector3f pos) const;

	Intersection getClosestIntersection(const Vector3f& pos) const;
	Intersection getIntersection(const std::array<int, 3>&);
	//bool getIntersectionWall(Vector3i pos); // get isWall bool by intersection coordinate

	void sP(int i, int j, bool val); //set Pellet, single element
	void sP(Vector3i pos, bool val);
	//const std::vector<std::vector<bool>>& P() const; //get Pellet
	bool P(int x, int y) const;
	bool P(Vector3i pos) const; //get Pellet by element, by Vec3i

	void sPow(int i, int j, bool val); //set Power, single element
	void sPow(Vector3i pos, bool val);
	
	bool Pow(int x, int y) const;
	bool Pow(Vector3i pos) const; //get Power by element, by Vec3i

	void draw();

	void setMtl_wall(Material);
	void setMtl_pellet(Material);
	void setMtl_power(Material);

	void MapInitializer(const std::string& filename);
	void loadMapFromFile(const std::string& filename, int width, int height);

private:
	std::vector<std::vector<std::array<bool, 4>>> arrMap;
	// arrMapd은 각 맵 타일마다 4개의 bool arr을 갖고 있다.
	// 각 array는 순서대로 (isBlock, isPel, isPow, isEmt)로
	// 상호 베타적인 상태 4가지를 one-hot encoding하였다.

	std::vector<Intersection> intersections;

	Material mtl_wall;
	Material mtl_pellet;
	Material mtl_power;

	void generateIntersections(); // run only once!
};
