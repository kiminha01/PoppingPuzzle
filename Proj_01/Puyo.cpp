#include "Puyo.h"
#include <GL/glut.h>

Puyo::Puyo() {
	radius = PUYOSIZE;
	slice = 20;
	stack = 20;
}
Puyo::Puyo(int c,int s,int Y) {
	radius = PUYOSIZE;
	slice = 20;
	stack = 20;
	puyoType = c;
	x = s;
	y = Y;
	isMoving = true;
	setCenter(Vector3f(-gameBoundaryX + x * 2 * PUYOSIZE + radius,
		-gameBoundaryY + y * 2 * PUYOSIZE + radius, 0.f));
	setDest(Vector3f(-gameBoundaryX + x * 2 * PUYOSIZE + radius,
		-gameBoundaryY + y * 2 * PUYOSIZE + radius, 0.f));
	setFollowV(defaultV);
	setVelocity(Vector3f(0.f,passiveV,0.f));
	switch (puyoType) {
	case red:
		mtl.setEmission(0.0f, 0.f, 0.f, 1.0f);
		mtl.setAmbient(0.1745f, 0.01175f, 0.01175f, 1.f);
		mtl.setDiffuse(0.65424f, 0.04136f, 0.04136f, 1.f);
		mtl.setSpecular(0.827811f,	0.326959f,	0.326959f,1.f);
		mtl.setShininess(.6f);
		break;
	case green:
		mtl.setEmission(0.0f, 0.f, 0.f, 1.0f);
		mtl.setAmbient(0.0215f,	0.1745f,	0.0215f, 1.f);
		mtl.setDiffuse(0.07568f,	0.71424f,	0.07568f, 1.f);
		mtl.setSpecular(0.633f,	0.527811f,	0.333f, 1.f);
		mtl.setShininess(.6f);
		break;
	case blue:		
		mtl.setEmission(0.0f, 0.f, 0.f, 1.0f);
		mtl.setAmbient(0.0215f, 0.0215f, 0.9f, 1.f);
		mtl.setDiffuse(0.07568f, 0.17568f, 0.71424f, 1.f);
		mtl.setSpecular(0.1f, 0.3f, 0.967811f, 1.f);
		mtl.setShininess(0.6f);
		break;
	case yellow:
		mtl.setEmission(0.0f, 0.f, 0.f, 1.0f);
		mtl.setAmbient (0.24725f, 0.1995f,	0.0745f,1.f);
		mtl.setDiffuse(0.65164f,	0.50648f,	0.12648f, 1.0f);
		mtl.setSpecular(0.528281f,	0.455802f,	0.166065f, 1.0f);
		mtl.setShininess(0.3f);
		break;
	case purple:
		mtl.setEmission(0.0f, 0.f, 0.f, 1.0f);
		mtl.setAmbient(0.135f, 0.2225f, 0.1575f, 0.95f);
		mtl.setDiffuse(0.6f, 0.05f, 0.6f, 1.0f);
		mtl.setSpecular(0.516228f, 0.116228f, 0.516228f, 0.95f);
		mtl.setShininess(0.8f);
		break;
	}
}

int Puyo::getX() const {
	return x;
}

int Puyo::getY() const {
	return y;
}
void Puyo::setRadius(float r) {
	radius = r;
}

float Puyo::getRadius() const {
	return radius;
}
void Puyo::setSlice(int sl) {
	slice = sl;
}
void Puyo::setStack(int st) {
	stack = st;
}
void Puyo::setX(int X) {
	x = X;
}
void Puyo::setY(int Y) {
	y = Y;
}
void Puyo::updateDest() {
	setDest(Vector3f(-gameBoundaryX + x * 2 * PUYOSIZE + radius,
		-gameBoundaryY + y * 2 * PUYOSIZE + radius, 0.f));
}
bool Puyo::isAtDest() {
	return getDest() == getCenter();
}
void Puyo::setColor(int c) {
	puyoType = c;
}
int Puyo::getColor() const {
	return puyoType;
}
void Puyo::draw() const {
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);

	glutSolidSphere(radius, slice, stack);

	glPopMatrix();
}
const bool Puyo::getisMoving() const {
	return isMoving;
}
void Puyo::setisMoving(bool t) {
	isMoving = t;
}