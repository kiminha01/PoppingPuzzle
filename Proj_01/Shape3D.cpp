#include "Shape3D.h"
#include "Vector3f.h"

Shape3D::Shape3D() {
	velocity = Vector3f(0, 0, 0);
	center = Vector3f(0, 0, 0);
	dest = Vector3f(0, 0, 0);
	followV = 1;
}
Vector3f Shape3D::getCenter() const {
	return center;
}
void Shape3D::setVelocity(const Vector3f& vel) {
	velocity[0] = vel[0]; velocity[1] = vel[1]; velocity[2] = vel[2];
}
void Shape3D::setCenter(const Vector3f& cen) {
	center = cen;
}

void Shape3D::setDest(const Vector3f& dst) {
	dest = dst;
}

Vector3f Shape3D::getVelocity() const {
	return velocity;
}
Vector3f Shape3D::getDest() const {
	return dest;
}

void Shape3D::setFollowV(const float v) {
	followV = v;
}
float Shape3D::getFollowV() const {
	return followV;
}
void Shape3D::move() {
	dest = dest + velocity;
	Vector3f dir = dest - center;
	if (dir.len() < followV) {
		center = dest;
		return;
	}
	dir = dir.norm();
	dir = followV * dir;
	center = center + dir;
}


void Shape3D::setMTL(const Material& m) {
	mtl = m;
}

Material Shape3D::getMTL() const {
	return mtl;
} 
/*
void Shape3D::draw() const {
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
*/