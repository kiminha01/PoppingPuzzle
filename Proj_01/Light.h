#pragma once
#include "Sphere.h"


class Light {
public:
	Light(float x, float y, float z, int ID);
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setAttenuation(float f);
	void setAsMaterial(Material m, float f);
	void draw() const;
	int getID() const;
private:
	int lightID;
	Vector3f pos;
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float attenuation = 0;
};
