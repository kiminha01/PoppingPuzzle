#pragma once
#include "Shape3D.h"
#include "Vector3f.h"
#include "Material.h"

class Sphere:public Shape3D {
public:
	Sphere();
	Sphere(float r, int sl, int st);
	void setRadius(float r);
	float getRadius() const;
	void setSlice(int sl);
	void setStack(int st);
	
	virtual void draw() const;
private:
	float radius;
	int slice;
	int stack;

	
};


class SpherePair {
public:
	SpherePair();
	void setSpherePair(const Sphere& sph1, const Sphere& sph2);
	const Sphere& getFirstSphere() const;
	const Sphere& getSecondSphere() const;
	void move();
	void draw() const;
private:
	Sphere sphere1, sphere2;
};