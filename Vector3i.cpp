#include "Vector3i.h"
#include <cmath>

Vector3i::Vector3i()
	:pos{ 0, 0, 0 } {}

Vector3i::Vector3i(int x, int y, int z)
	:pos{ x,y,z } {}

void Vector3i::setPos(int x, int y, int z) {
	pos[0] = x; pos[0] = y; pos[0] = z;
}

const int* Vector3i::getPos() const { // 개인적으로 추가한 함수
	return pos;
}

int& Vector3i::operator[](const int i) {
	return pos[i];
}

int Vector3i::operator[](const int i) const {
	return pos[i];
}

float Vector3i::norm() const {
	return sqrt((float)(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]));
}

Vector3i operator+(const Vector3i& v1, const Vector3i& v2) {
	return Vector3i(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vector3i operator-(const Vector3i& v1, const Vector3i& v2) {
	return Vector3i(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vector3i operator*(const int s, const Vector3i& vec3) {
	return Vector3i(vec3[0]*s, vec3[1]*s, vec3[2]*s);
}
Vector3i operator*(const Vector3i& vec3, const int s) {
	return Vector3i(vec3[0] * s, vec3[1] * s, vec3[2] * s);
}

bool operator==(const Vector3i& v1, const Vector3i& v2) {
	if ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2])) return true;
	return false;
}

int dotProduct(const Vector3i& v1, const Vector3i& v2) {
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}