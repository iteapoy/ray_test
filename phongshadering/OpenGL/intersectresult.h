#pragma once
#ifndef INTERSECTRESULT_H
#define INTERSECTRESULT_H

#include "Vec3.h"
// ��ʾ�ཻ�����һ����

class Object;
// �����Ƿ��ཻ(isHit)���������(geometry)������(distance)��λ��(position)�ͷ�����(normal)
class IntersectResult {
public:
	float distance;
	Object* geometry;
	bool isHit;
	Vec3f position;
	Vec3f normal;

	IntersectResult() {
		geometry = NULL;
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