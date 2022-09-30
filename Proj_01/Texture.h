#pragma once
#include "FreeImage.h"
#include <GL/glut.h>
using namespace std;
class Texture {
public:
	FIBITMAP* createBitMap(char const* filename);
	void generateTexture();
	void initTexture(char const* filename, GLuint ID);
	GLuint getTextureID()const;

private:
	GLuint textureID;
	int imageWidth;
	int imageHeight;
	GLubyte* textureData;
};
void drawSquareWithTexture(Texture texture);
void drawBackgroundWithTexture(Texture texture);
