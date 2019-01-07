#pragma once
#ifndef METAL_H
#define METAL_H

#include "Material.h"

class Metal : public Material
{
public:
	Vec3f albedo;
	Metal(const Vec3f& _a) : albedo(_a) {}
	Metal() {}
	virtual ~Metal() {}
	virtual bool sample(const Ray& ray, const  IntersectResult& result, Vec3f& attenuation, Ray& out) {
		/*这里具体实现metal::scatter()。做两件事情：获取镜面反射的反射光线；获取材料的衰减系数。 */
		Vec3f reflected = reflect(ray.direction.unit(), result.normal);
		out = Ray(result.position, reflected);
		attenuation = albedo;
		return (out.direction.dot(result.normal) > 0);
	}

	Vec3f reflect(const Vec3f& v, const Vec3f& n) {
		/*获取镜面反射的反射光线的方向向量。具体计算，后面解释*/
		return v - n * 2 * v.dot(n);
	}

};

#endif