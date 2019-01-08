#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

Vec3f random_in_unit_disk() {
	/*��z=0ƽ���ϲ���һ���������ԭ�㣬����С��1�����������������*/
	Vec3f p;
	do {
		p = Vec3f((rand() % (100) / (float)(100)), (rand() % (100) / (float)(100)), 0) * 2.0 - Vec3f(1, 1, 0);
	} while (p.dot(p) >= 1.0);
	return p;
}

class Camera
{
public:
	Vec3f lower_left_corner;  // ��Ļ���½�
	Vec3f horizontal;   // ˮƽλ��
	Vec3f vertical;       // ��ֱ����
	Vec3f origin;         // ԭ���λ��
	Vec3f x, y, z;         // x,y,z��ĵ�λ����
	float lens_radius;

	// lookfrom�����λ��   lookat������λ�� up���������б���� FOV:��Ұ  aspect����Ļ����� aperture:��Ȧ  focus_dist������λ��
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