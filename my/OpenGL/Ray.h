#pragma once

#include "Vec3.h"

using namespace std;
#ifndef RAY_H
#define RAY_H

// 定义射线

class Ray {
public:
	// r(t)=o+td
	// origin 射线原点，direction 射线方向
	Vec3f origin, direction;

	// 初始化 Ray 
	Ray(const Vec3f & _o, const Vec3f & _d) : origin(_o), direction(_d) {}
	Ray()  {}
	Ray(const Ray & r) : origin(r.origin), direction(r.direction) {}
	Ray & operator = (const Ray & r) {
		origin = r.origin;
		direction = r.direction;
		return *this;
	}

	// 获得光线交点坐标
	Vec3f getPoint(float t) const {return origin + direction * t;}

	// 显示origin(x,y,z), dir(x,y,z)
	void show() const{
		cout << "origin: (" << origin.x << ", " << origin.y << ", " << origin.z << "), dir: ("
		<< direction.x << ", " << direction.y << ", " << direction.z << ")\n";
	}

};

#endif