#pragma once

#include <string>
#include <GL/freeglut.h>
#include <GL/GL.h>
#include <FreeImage.h>

class Texture {
private:
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
	std::string fileName;
	//FIBITMAP* bitMap;
public:
	Texture(std::string filename);
	static FIBITMAP* createBitMap(std::string filename);
	FIBITMAP* createBitMap();
	void generateTexture();
	void initializeTexture();
	GLuint getTextureID() const;

	void drawSquareWithTexture();
};
