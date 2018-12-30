#pragma once
#ifndef PLANE_H
#define PLANE_H

#include"Object.h"
// ����һ��ƽ����

class Plane:public Object {
	// normal���������� d����ԭ��ľ���, position ȡ��Դ��ƽ������ĵ㣨���㣩
public:
	Vec3f normal;
	float  d;
	Vec3f position;

	// ��ʼ��
	Plane() {};
	Plane(const Vec3f& _normal, float _d) :normal(_normal), d(_d) {
		position = normal * d;
	};
	// ��������
	virtual ~Plane() {};

	// ƽ��Ķ��壺 
	// (p-p1)��n=0, p =r(t)=o+td
	// t = (p1-o)��n/(d��n)
	virtual IntersectResult isIntersected(const Ray& ray) {
		IntersectResult result = IntersectResult::noHit();
		float a = ray.direction.dot(this->normal);
		if (a < 0) {
			result.isHit = true;
			float b = this->normal.dot(this->position - ray.origin);
			result.distance = b / a;
			result.position = ray.getPoint(result.distance);
			result.normal = this->normal;
		}
		return result;
	}
};

#endif