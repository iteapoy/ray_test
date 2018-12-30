#pragma once
#ifndef CHECKMATERIAL_H
#define CHECKMATERIAL_H

#include"Material.h"
#include <cmath>

// 在Material基类的基础上定义一个格子材质
class CheckMaterial :public Material {
private:
	// scale的意义为1坐标单位有多少个格子,scale=0.1即一个格子的大小为10x10
	float scale;

public:
	// 初始化
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
