#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include"Material.h"
#include "intersectresult.h"
#include "Ray.h"

// 定义一个物体的基类

class Object {
public:
	// 材质属性
	Material *material;

	// 初始化
	Object() {}
	// 析构函数
	virtual ~Object() {}
	// 虚函数，求交点
	virtual IntersectResult isIntersected(const Ray& ray) = 0 { cout << "物体基类" << endl; }
};

#endif 
