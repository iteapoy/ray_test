#pragma once

#include "Vec3.h"

using namespace std;
#ifndef RAY_H
#define RAY_H

// ��������

class Ray {
public:
	// r(t)=o+td
	// origin ����ԭ�㣬direction ���߷���
	Vec3f origin, direction;

	// ��ʼ�� Ray 
	Ray(const Vec3f & _o, const Vec3f & _d) : origin(_o), direction(_d) {}
	Ray()  {}
	Ray(const Ray & r) : origin(r.origin), direction(r.direction) {}
	Ray & operator = (const Ray & r) {
		origin = r.origin;
		direction = r.direction;
		return *this;
	}

	// ��ù��߽�������
	Vec3f getPoint(float t) const {return origin + direction * t;}

	// ��ʾorigin(x,y,z), dir(x,y,z)
	void show() const{
		cout << "origin: (" << origin.x << ", " << origin.y << ", " << origin.z << "), dir: ("
		<< direction.x << ", " << direction.y << ", " << direction.z << ")\n";
	}

};

#endif