#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "intersectresult.h"
#include "Ray.h"

// 定义一个材质基类
class Material
{
public:
	// 初始化
	Material() {};
	virtual ~Material() {};
	// 虚函数，获得光照和材质相交后的颜色属性
	virtual bool sample(const Ray& ray, const  IntersectResult& result, Vec3f& attenuation, Ray& out) = 0 {}
};

#endif