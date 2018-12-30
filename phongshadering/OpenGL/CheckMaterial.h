#pragma once
#ifndef CHECKMATERIAL_H
#define CHECKMATERIAL_H

#include"Material.h"
#include <cmath>

// ��Material����Ļ����϶���һ�����Ӳ���
class CheckMaterial :public Material {
private:
	// scale������Ϊ1���굥λ�ж��ٸ�����,scale=0.1��һ�����ӵĴ�СΪ10x10
	float scale;

public:
	// ��ʼ��
	CheckMaterial() {};
	CheckMaterial(float _scale, float _reflectiveness = 0) :
		scale(_scale) {reflectiveness = _reflectiveness;}
	virtual ~CheckMaterial() {};
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal)
	{
		float d = abs((floor(position.x * this->scale) + floor(position.z * this->scale)));
		d = fmod(d, 2);
		return  d < 1 ? Color::black() : Color::white();
	}
};

#endif
