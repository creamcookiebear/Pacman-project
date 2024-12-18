#include <GL/freeglut.h>
#include "Vector3f.h"
#include "Vector4f.h"
#include "Material.h"

Material::Material() {}
void Material::setEmission(float r, float g, float b, float a) {
	emission[0] = r;emission[1] = g;
	emission[2] = b;emission[3] = a;
}
void Material::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;ambient[1] = g;
	ambient[2] = b;ambient[3] = a;
}
void Material::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r;diffuse[1] = g;
	diffuse[2] = b;diffuse[3] = a;
}
void Material::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;specular[1] = g;
	specular[2] = b;specular[3] = a;
}
void Material::setShininess(float sh) {
	shininess = sh;
}

Vector4f Material::getEmission() const { return emission; }
Vector4f Material::getAmbient() const { return ambient; }
Vector4f Material::getDiffuse() const { return diffuse; }
Vector4f Material::getSpecular() const { return specular; }
const float* Material::getShininess() const { return &shininess; }