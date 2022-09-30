#pragma once
#include "Material.h"
#include "Vector3f.h"

class Shape3D {
public:
	Shape3D();
	void setCenter (const Vector3f& c);
	Vector3f getCenter() const;
	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;
	void setDest(const Vector3f& dst);
	Vector3f getDest() const;
	void setFollowV(const float v);
	float getFollowV() const;
	void move();
	void setMTL(const Material& m);
	Material getMTL() const;
	virtual void draw() const = 0;
protected:
	Vector3f center;
	Vector3f velocity;
	Material mtl;
	Vector3f dest;
	float followV;
};