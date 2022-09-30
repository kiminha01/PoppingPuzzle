#include <iostream>
#include "Texture.h"
#include "Puyo.h"
FIBITMAP* Texture::createBitMap(char const* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	if (format == -1) {
		cout << "Could not find image: filename " << filename << "- Aborting." << endl;
		exit(-1);
	}
	if (format == FIF_UNKNOWN) {
		cout << "Couldn't determine file format attempting to get from file extension..." << endl;
		format = FreeImage_GetFIFFromFilename(filename);
		if (!FreeImage_FIFSupportsReading(format)) {
			cout << "Detected image format cannot be read!" << endl;
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	int 	bitsPerPixel = FreeImage_GetBPP(bitmap);
	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32) {
		cout << "Source image has" << bitsPerPixel << " bits per pixel.Skipping conversion" << endl;
		bitmap32 = bitmap;
	}
	else {
		cout << "Source image has bits " << bitsPerPixel << ". Converting to 32 bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}
	return bitmap32;
}
void Texture::generateTexture() {
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight,
		0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);
}
void Texture::initTexture(char const* filename, GLuint ID) {
	FIBITMAP* bitmap32 = createBitMap((filename));
	imageWidth = FreeImage_GetWidth(bitmap32);
	imageHeight = FreeImage_GetHeight(bitmap32);
	textureData = FreeImage_GetBits(bitmap32);
	glGenTextures(1, &textureID);

	generateTexture();
}

GLuint Texture::getTextureID()const {
	return textureID;
}

void drawSquareWithTexture(Texture texture) {
	/* Implement: Bind texture */
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-boundaryX, -gameBoundaryY );
	glTexCoord2f(0, 1);
	glVertex2f(-boundaryX, gameBoundaryY );
	glTexCoord2f(1, 1);
	glVertex2f(boundaryX, gameBoundaryY );
	glTexCoord2f(1, 0);
	glVertex2f(boundaryX , -gameBoundaryY );
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawBackgroundWithTexture(Texture texture) {
	/* Implement: Bind texture */
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texture.getTextureID());
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(-boundaryX , -boundaryY);
	glTexCoord2f(0, 1);
	glVertex2f(-boundaryX , boundaryY);
	glTexCoord2f(1, 1);
	glVertex2f(boundaryX , boundaryY);
	glTexCoord2f(1, 0);
	glVertex2f(boundaryX , -boundaryY);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}