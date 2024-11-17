#include "Vector3f.h"
#include <iostream>
#include <cmath>

Vector3f::Vector3f()
	:pos{ 0.f, 0.f, 0.f } {}

Vector3f::Vector3f(float x, float y, float z)
	:pos{ x,y,z } {}

void Vector3f::setPos(float x, float y, float z) {
	pos[0] = x; pos[0] = y; pos[0] = z;
}

const float* Vector3f::getPos() const { // 개인적으로 추가한 함수
	return pos;
}

float& Vector3f::operator[](const int i) {
	return pos[i];
}

float Vector3f::operator[](const int i) const {
	return pos[i];
}

void Vector3f::print() const {
	std::cout << '(' << pos[0] << ',' << pos[1] << ',' << pos[2] << ')' << std::endl;
}

Vector3f& Vector3f::operator=(const Vector3f& other) {
	pos[0] = other[0]; pos[1] = other[1]; pos[2] = other[2];
	return *this;
}

bool operator==(const Vector3f& pos, const Vector3f& other) {
	return (pos[0] == other[0]) && (pos[1] == other[1]) && (pos[2] == other[2]);
}

float Vector3f::norm() const {
	return sqrt(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
}

Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
	return Vector3f(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
	return Vector3f(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
}

Vector3f operator*(const float s, const Vector3f& vec3) {
	return Vector3f(vec3[0]*s, vec3[1]*s, vec3[2]*s);
}
Vector3f operator*(const Vector3f& vec3, const float s) {
	return Vector3f(vec3[0] * s, vec3[1] * s, vec3[2] * s);
}
Vector3f operator/(const Vector3f& vec3, const float s) {
	return Vector3f(vec3[0] / s, vec3[1] / s, vec3[2] / s);
}

float dotProduct(const Vector3f& v1, const Vector3f& v2) {
	return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}