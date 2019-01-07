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
		/*�������ʵ��metal::scatter()�����������飺��ȡ���淴��ķ�����ߣ���ȡ���ϵ�˥��ϵ���� */
		Vec3f reflected = reflect(ray.direction.unit(), result.normal);
		out = Ray(result.position, reflected);
		attenuation = albedo;
		return (out.direction.dot(result.normal) > 0);
	}

	Vec3f reflect(const Vec3f& v, const Vec3f& n) {
		/*��ȡ���淴��ķ�����ߵķ���������������㣬�������*/
		return v - n * 2 * v.dot(n);
	}

};

#endif