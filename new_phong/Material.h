#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "intersectresult.h"
#include "Ray.h"

// ����һ�����ʻ���
class Material
{
public:
	// ��ʼ��
	Material() {};
	virtual ~Material() {};
	// �麯������ù��պͲ����ཻ�����ɫ����
	virtual bool sample(const Ray& ray, const  IntersectResult& result, Vec3f& attenuation, Ray& out) = 0 {}
};

#endif