#include "Vector4f.h"

Vector4f::Vector4f()
	: pos{ 0.f,0.f,0.f,0.f } {}
Vector4f::Vector4f(float x, float y, float z, float w)
	: pos{x,y,z,w} {}
void Vector4f::setPos(float x, float y, float z, float w) {
	pos[0] = x; pos[0] = y; pos[0] = z; pos[0] = w;
}

float& Vector4f::operator[](const int i) {
	return pos[i];
}
float Vector4f::operator[](const int i) const {
	return pos[i];
}
const float* Vector4f::getPos() const {
	return pos;
}