#pragma once
#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"

// 定义漫反射类，继承自Material类
class Lambertian : public Material
{
public:
	Vec3f albedo;/*保存衰减系数*/
	Lambertian() {};
	Lambertian(const Vec3f& _a) : albedo(_a) {}
	virtual ~Lambertian() {}

	virtual bool sample(const Ray& ray, const  IntersectResult& result, Vec3f& attenuation, Ray& out) {
		Vec3f target = result.position + result.normal.unit() + random_in_unit_sphere();
		out = Ray(result.position, target - result.position);
		attenuation = albedo;
		return true;
	}

	Vec3f random_in_unit_sphere() {
		Vec3f p;
		do {
			p = Vec3f((rand() % (100) / (float)(100)),
				(rand() % (100) / (float)(100)),
				(rand() % (100) / (float)(100)))*2.0 - Vec3f(1, 1, 1);
		} while (p.sqrLength() >= 1.0);
		return p;
	}
};

#endif 