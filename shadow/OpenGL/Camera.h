#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vec3.h"
#include "Ray.h"
#include <cmath>
using namespace std;


// 定义了透视摄像机 近大远小

class PerspectiveCamera {
private:
	// eye：视点，forward：前， refUp:上， fov：视野 field of view
	Vec3f eye;
	Vec3f forward;
	Vec3f refUp;
	float   fov;

	Vec3f right;
	Vec3f up;
	float   fovScale;

public:
	PerspectiveCamera(const Vec3f& _eye, const Vec3f& _front, const Vec3f& _refUp, float _fov)
		:eye(_eye), forward(_front), refUp(_refUp), fov(_fov)
	{
		right = forward.cross(refUp);
		up = right.cross(forward);
		fovScale = tan(fov* PI  * 0.5f / 180) * 2 ; // fovScale预先乘了2，因为sx映射到[-1,1]每次都要乘以2
	}

	Ray generateRay(float x, float y)const {
		// 右
		Vec3f r = right *((x - 0.5f) * fovScale);
		// 上
		Vec3f u = up*((y - 0.5f) * fovScale);
		return Ray(eye, (forward + r + u).normalize());
	}
};
#endif
