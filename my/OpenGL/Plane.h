#pragma once
#ifndef PLANE_H
#define PLANE_H

#include"Object.h"
// 定义一个平面类

class Plane:public Object {
	// normal：法向量， d：到原点的距离, position 取光源离平面最近的点（垂足）
public:
	Vec3f normal;
	float  d;
	Vec3f position;

	// 初始化
	Plane() {};
	Plane(const Vec3f& _normal, float _d) :normal(_normal), d(_d) {
		position = normal * d;
	};
	// 析构函数
	virtual ~Plane() {};

	// 平面的定义： 
	// (p-p1)·n=0, p =r(t)=o+td
	// t = (p1-o)·n/(d·n)
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