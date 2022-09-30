#pragma once
class Vector3f {
public:
	Vector3f() {}
	Vector3f(float x, float y, float z);
	void setPos(float x, float y, float z);
	float& operator[] (const int i);
	float operator[] (const int i) const;
	float len() const;
	Vector3f norm() const;
	float pos[3];
};
Vector3f operator+(const Vector3f& a, const Vector3f& b);
Vector3f operator-(const Vector3f& a, const Vector3f& b);
Vector3f operator*(const float a, const Vector3f& b);
Vector3f operator/(const Vector3f& a, const float b);
bool operator==(const Vector3f& a, const Vector3f& b);
float dotProduct(const Vector3f& v1, const Vector3f& v2);