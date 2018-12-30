#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "color.h"
#include "intersectresult.h"
#include "Ray.h"

// ����һ�����ʻ���
class Material
{
protected:
	// ���ʵķ���ϵ��
	float reflectiveness;
public:
	// ��ʼ��
	Material() {};
	Material(float _reflectiveness) :reflectiveness(_reflectiveness){};
	// ��÷���ϵ��
	float getRef() { return reflectiveness; }
	// ���÷���ϵ��
	void setRef(float _reflectiveness) { reflectiveness = _reflectiveness; }
	// ��������
	virtual ~Material() {};
	// �麯������ù��պͲ����ཻ�����ɫ����
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal) = 0 {}
};

#endif