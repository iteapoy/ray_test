#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "color.h"
#include "intersectresult.h"
#include "Ray.h"

// 定义一个材质基类
class Material
{
protected:
	// 材质的反射系数
	float reflectiveness;
public:
	// 初始化
	Material() {};
	Material(float _reflectiveness) :reflectiveness(_reflectiveness){};
	// 获得反射系数
	float getRef() { return reflectiveness; }
	// 设置反射系数
	void setRef(float _reflectiveness) { reflectiveness = _reflectiveness; }
	// 析构函数
	virtual ~Material() {};
	// 虚函数，获得光照和材质相交后的颜色属性
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal) = 0 {}
};

#endif