#pragma once

#ifndef VECTORMATH
#define VECTORMATH

#include <math.h>

class vec2f {
public:
	float x;
	float y;

	vec2f();
	vec2f(float xValue, float yValue);
	vec2f(const vec2f& other);
	~vec2f();

	float length() const;
	float distance(const vec2f& other) const;
	static float dot(vec2f v1, vec2f v2);
	static float cross(vec2f v1, vec2f v2);

	vec2f& normalize();

	vec2f operator=(const vec2f& other);
	vec2f operator+(const vec2f& other);
	vec2f operator+(float num);
	vec2f operator-(const vec2f& other);
	vec2f operator/(float num);
	vec2f& operator/=(float num);
	vec2f operator*(float num);
	vec2f& operator*=(float num);
};

#endif