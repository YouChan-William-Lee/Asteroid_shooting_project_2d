#include "vectormath.h"

vec2f::vec2f() : x(0), y(0) {}
vec2f::vec2f(float xValue, float yValue) : x(xValue), y(yValue) {}
vec2f::vec2f(const vec2f& other) : x(other.x), y(other.y) {}
vec2f::~vec2f() = default;

float vec2f::length() const {
	return static_cast<float>(sqrt((x * x) + (y * y)));
}

float vec2f::distance(const vec2f& other) const {
	vec2f temp(other.x - x, other.y - y);
	return temp.length();
}

float vec2f::dot(vec2f v1, vec2f v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float vec2f::cross(vec2f v1, vec2f v2) {
	return (v1.x * v2.y) + (v1.y * v2.x);
}

vec2f& vec2f::normalize() {
	float templength = length();
	if (templength == 0)
		return *this;
	return (*this) /= templength;
}

vec2f vec2f::operator=(const vec2f& other) {
	x = other.x;
	y = other.y;
	return vec2f(x, y);
}

vec2f vec2f::operator+(const vec2f& other) {
	vec2f temp;
	temp.x = x + other.x;
	temp.y = y + other.y;
	return temp;
}

vec2f vec2f::operator+(float num) {
	vec2f temp;
	temp.x = x + num;
	temp.y = y + num;
	return temp;
}

vec2f vec2f::operator-(const vec2f& other) {
	vec2f temp;
	temp.x = x - other.x;
	temp.y = y - other.y;
	return temp;
}

vec2f vec2f::operator/(float num) {
	vec2f temp;
	temp.x = x / num;
	temp.y = y / num;
	return temp;
}

vec2f& vec2f::operator/=(float num) {
	x = x / num;
	y = y / num;
	return *this;
}

vec2f vec2f::operator*(float num) {
	vec2f temp;
	temp.x = x * num;
	temp.y = y * num;
	return temp;
}

vec2f& vec2f::operator*=(float num) {
	x = x * num;
	y = y * num;
	return *this;
}