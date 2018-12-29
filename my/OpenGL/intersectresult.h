#pragma once

#include "Vec3.h"

// 表示相交结果的一个类

#ifndef INTERSECTRESULT_H
#define INTERSECTRESULT_H

// 交点是否相交(isHit)、几何物件(geometry)、距离(distance)、位置(position)和法向量(normal)
class IntersectResult {
public:
	float distance;
	bool isHit;
	Vec3f position;
	Vec3f normal;

	IntersectResult() {
		distance = 0;
		isHit = false;
		position = Vec3f(0, 0, 0);
		normal = Vec3f(0, 0, 0);
	}

	static IntersectResult noHit() { 
		return IntersectResult();
	}
};

#endif