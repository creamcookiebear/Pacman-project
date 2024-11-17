#include "Shape3D.h"
//#include "Constants.h"
using namespace std;

Shape3D::Shape3D(): center(), velocity() {}
Shape3D::Shape3D(float x, float y, float z): center(x,y,z), velocity() {}

void Shape3D::setCenter(float x, float y, float z) {
	center[0] = x; center[1] = y; center[2] = z;
}
void Shape3D::setCenter(const Vector3f& c) {
	center = c;
}
Vector3f Shape3D::getCenter() const { return center; }

void Shape3D::setVelocity(float x, float y, float z) {
	velocity[0] = x; velocity[1] = y; velocity[2] = z;
}
void Shape3D::setVelocity(const Vector3f& v) {
	velocity = v;
}
Vector3f Shape3D::getVelocity() const { return velocity; }

void Shape3D::setMTL(const Material& m) { mtl = m; }
const Material& Shape3D::getMTL() const { return mtl; }