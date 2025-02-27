#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include"Object.h"
using namespace std;

// 定义球体

class Sphere:public Object {
public:
	// center：球心、radius：半径
	Vec3f center;
	float   radius;

	Sphere(const Sphere& s) :center(s.center), radius(s.radius) {  }
	Sphere(const Vec3f& _center, float _radius) : center(_center), radius(_radius) {  }

	// 返回半径的平方
	float sqrRadius()const { return radius * radius; }

	// 判断球和光线ray是否有交点，如果有交点就返回交点，如果没有交点就返回一个空值
	virtual IntersectResult isIntersected(const Ray& ray) {
		// r(t) = o+td
		// ||x-c||=r --> ||o+td-c||=r --> v=o-c --> ||v+td||=r
		Vec3f v = ray.origin - center;
		float a0 = v.sqrLength() - sqrRadius();
		float DdotV = ray.direction.dot(v); 
		
		if (DdotV <= 0) {
			float discr = DdotV * DdotV - a0;
			if (discr >= 0) {
				IntersectResult result;
				result.geometry = this;
				result.isHit = true;
				result.distance = -DdotV - sqrt(discr);
				result.position = ray.getPoint(result.distance);
				result.normal = (result.position - this->center).normalize();
				return result;
			}
		}
		return IntersectResult::noHit();

	}
};


#endif