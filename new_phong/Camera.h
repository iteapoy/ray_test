#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
	Vec3f lower_left_corner;  // ��Ļ���½�
	Vec3f horizontal;   // ˮƽλ��
	Vec3f vertical;       // ��ֱ����
	Vec3f origin;         // ԭ���λ��

	// lookfrom�����λ��   lookat������λ�� up���������б���� FOV:��Ұ  aspect����Ļ�����
	Camera(Vec3f lookfrom, Vec3f lookat, Vec3f up, float FOV, float aspect) { 
		Vec3f x, y, z;
		float theta = FOV * PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		z = (lookfrom - lookat).unit();
		x = up.cross(z).unit();
		y = z.cross(x);
		lower_left_corner = origin -  x * half_width - y * half_height - z;
		horizontal = x * 2 * half_width;
		vertical = y * 2 * half_height;
	}


	Ray generateRay(float x, float y) { return Ray(origin, lower_left_corner + horizontal * x + vertical * y - origin); }

};

#endif