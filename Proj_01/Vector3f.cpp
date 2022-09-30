#include "Vector3f.h"
#include <math.h>

Vector3f::Vector3f(float x, float y, float z) {
	pos[0] = x; pos[1] = y; pos[2] = z;
}
void Vector3f::setPos(float x, float y, float z) {
	pos[0] = x; pos[1] = y; pos[2] = z;
}
float& Vector3f::operator[] (const int i) {
	return pos[i];
}
float  Vector3f::operator[] (const int i) const {
	return pos[i];
};
Vector3f operator+(const Vector3f& a, const Vector3f& b) {
	return Vector3f(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}
Vector3f operator-(const Vector3f& a, const Vector3f& b) {
	return Vector3f(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}
Vector3f operator*(const float a, const Vector3f& b) {
	return Vector3f(a * b[0], a * b[1], a * b[2]);
}
Vector3f operator/(const Vector3f& a, const float b) {
	return Vector3f(a[0] / b, a[1] / b, a[2] / b);
}
bool operator==(const Vector3f& a, const Vector3f& b) {
	return (a - b).len() < 1E-6;
}
float Vector3f::len() const {
	return sqrtf(pos[0] * pos[0] + pos[1] * pos[1] + pos[2] * pos[2]);
}
Vector3f Vector3f::norm() const {
	float l = len();
	return Vector3f(pos[0] / l, pos[1] / l, pos[2] / l);
}
float dotProduct(const Vector3f& v1, const Vector3f& v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

