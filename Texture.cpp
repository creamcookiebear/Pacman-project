#include "Texture.h"
#include <iostream>

#define WINDOW_W 640
#define WINDOW_H 640
#define BOUNDARY_X (WINDOW_W)/2
#define BOUNDARY_Y (WINDOW_H)/2

Texture::Texture(std::string filename): fileName(filename){}

FIBITMAP* Texture::createBitMap(std::string filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);
	if (format == -1) { // couldn't find image
		exit(-1);
	}
	else if (format == FIF_UNKNOWN) {
	// couldn't determine file format // attemp to get from file extension
		format = FreeImage_GetFIFFromFilename(filename.c_str());
		if (!FreeImage_FIFSupportsReading(format)) {
			std::cout << "Detected image format cannot be read!" << std::endl;
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename.c_str());
	int bitsPerPixel = FreeImage_GetBPP(bitmap);
	std::cout << "Source image has " << bitsPerPixel
		<< " bits per pixel." << std::endl;
	FIBITMAP* bitmap32 = bitmap;
	if (bitsPerPixel != 32) {
		std::cout << "Converting to 32-bits." << std::endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	return bitmap32;
}

FIBITMAP* Texture::createBitMap() {
	return createBitMap(fileName);
}

void Texture::generateTexture() {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
}
void Texture::initializeTexture() {
	FIBITMAP* bitmap32 = createBitMap(fileName);
	imageWidth = FreeImage_GetWidth(bitmap32);
	imageHeight = FreeImage_GetHeight(bitmap32);
	textureData = FreeImage_GetBits(bitmap32);
	generateTexture();
}

GLuint Texture::getTextureID() const { return textureID; }

void Texture::drawSquareWithTexture() {
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f * BOUNDARY_X, -0.5f * BOUNDARY_Y, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f * BOUNDARY_X, 0.5f * BOUNDARY_Y, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f * BOUNDARY_X, 0.5f * BOUNDARY_Y, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f * BOUNDARY_X, -0.5f * BOUNDARY_Y, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}