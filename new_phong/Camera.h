#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

Vec3f random_in_unit_disk() {
	/*在z=0平面上产生一个“起点在原点，长度小于1，方向随机”的向量*/
	Vec3f p;
	do {
		p = Vec3f((rand() % (100) / (float)(100)), (rand() % (100) / (float)(100)), 0) * 2.0 - Vec3f(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}

class Camera
{
public:
	Vec3f lower_left_corner;  // 屏幕左下角
	Vec3f horizontal;   // 水平位置
	Vec3f vertical;       // 垂直方向
	Vec3f origin;         // 原点的位置
	Vec3f x, y, z;         // x,y,z轴的单位坐标
	float lens_radius;

	// lookfrom：相机位置   lookat：看的位置 up：相机的倾斜方向 FOV:视野  aspect：屏幕长宽比 aperture:光圈  focus_dist：成像位置
	Camera(Vec3f lookfrom, Vec3f lookat, Vec3f up, float FOV, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;
		float theta = FOV * PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		z = (lookfrom - lookat).unit();
		x = up.cross(z).unit();
		y = z.cross(x);
		lower_left_corner = origin - x * half_width * focus_dist - y * half_height * focus_dist - z * focus_dist;
		horizontal = x * 2 * half_width*focus_dist;
		vertical = y * 2 * half_height*focus_dist;
	}


	Ray generateRay(float sx, float sy) {
		Vec3f rd = random_in_unit_disk()*lens_radius;
		Vec3f offset = x * rd.x + y * rd.y;
		return Ray(origin + offset, lower_left_corner + horizontal * sx + vertical * sy - origin - offset);
	}

};

#endif