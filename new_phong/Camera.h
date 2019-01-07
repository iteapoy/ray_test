#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
	Vec3f lower_left_corner;
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f origin;

	Camera() {
		lower_left_corner = Vec3f(-1.0, -1.0, -1.0);
		horizontal = Vec3f(2.0, 0.0, 0.0);
		vertical = Vec3f(0.0, 2.0, 0.0);
		origin = Vec3f(0.0, 0.0, 0.0);
	}
	Ray generateRay(float x, float y) { return Ray(origin, lower_left_corner + horizontal * x + vertical * y); }

};

#endif