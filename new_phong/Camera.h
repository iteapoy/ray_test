#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

Vec3f random_in_unit_disk();

class Camera
{
public:
	Camera(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is top to bottom in degrees
		lens_radius = aperture / 2;
		float theta = vfov * PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = (lookfrom - lookat).unit();
		u = vup.cross(w).unit();
		v = w.cross(u);
		lower_left_corner = origin - u*half_width * focus_dist - v*half_height * focus_dist - w*focus_dist;
		horizontal = u*2 * half_width*focus_dist;
		vertical = v*2 * half_height*focus_dist;
	}
	Ray generateRay(float s, float t) {
		Vec3f rd = random_in_unit_disk()*lens_radius;
		Vec3f offset = u * rd.x+ v * rd.y;
		return Ray(origin + offset, lower_left_corner + horizontal*s +vertical*t - origin - offset);
	}

	Vec3f origin;
	Vec3f lower_left_corner;
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f u, v, w;
	float lens_radius;
};

Vec3f random_in_unit_disk() {
	/*在z=0平面上产生一个“起点在原点，长度小于1，方向随机”的向量。为什么是z=0平面，这个和相机的倾斜方向有关。（相机的倾斜方向为view up （简称vup，一般设置为（0，1，0）））*/
	Vec3f p;
	do {
		p = Vec3f((rand() % (100) / (float)(100)), (rand() % (100) / (float)(100)), 0)*2.0 - Vec3f(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}

#endif