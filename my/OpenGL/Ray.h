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
	float N; // �����

	// ��ʼ��
	Ray(const Vec3f & _o, const Vec3f & _d) :
		N(1.0), origin(_o), direction(_d * (1.0 / _d.length())) {}
	Ray() : N(1.0) {}
	Ray(const Ray & r) : N(1.0), origin(r.origin), direction(r.direction) {}

	Ray & operator = (const Ray & r) {
		origin = r.origin;
		direction = r.direction;
		N = r.N;
		return *this;
	}
	void show() const{
		cout << "origin: (" << origin.x << ", " << origin.y << ", " << origin.z << "), dir: ("
		<< direction.x << ", " << direction.y << ", " << direction.z << ")\n";
	}
};

#endif