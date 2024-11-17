#include "Light.h"
#include <GL/freeglut.h>

Light::Light(float x, float y, float z, int L_ID)
	:pos( x,y,z ),
	lightID(L_ID) {}

void Light::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r; ambient[1] = g;
	ambient[2] = b; ambient[3] = a;
}

void Light::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r; diffuse[1] = g;
	diffuse[2] = b; diffuse[3] = a;
}

void Light::setSpecular(float r, float g, float b, float a) {
	specular[0] = r; specular[1] = g;
	specular[2] = b; specular[3] = a;
}

Vector4f Light::getAmbient() const {
	return ambient;
}
Vector4f Light::getDiffuse() const {
	return diffuse;
}
Vector4f Light::getSpecular() const {
	return specular;
}

int Light::getID() const { return lightID; }

void Light::draw() const {
	glEnable(lightID);

	glLightfv(lightID, GL_AMBIENT, ambient.getPos());
	glLightfv(lightID, GL_DIFFUSE, diffuse.getPos());
	glLightfv(lightID, GL_SPECULAR, specular.getPos());
	glLightfv(lightID, GL_POSITION, pos.getPos());
}

void Light::drawOff() const { // 불 끄는 것도 구현하고 싶어서 추가해봤음.
	glDisable(lightID);
}