#pragma once
#ifndef UNION_H
#define UNION_H

// 定义一个物体的并集

#include"constant.h"
#include <vector>
#include "Object.h"

class Union :public Object {
private:
	vector<Object*> objects;

public:
	Union() {};
	virtual ~Union() {};
	void push(Object* object) { objects.push_back(object); }

	virtual IntersectResult isIntersected(const Ray& ray) {
		float minDistance = INF;
		IntersectResult minResult = IntersectResult::noHit();
		long size = this->objects.size();
		for (long i = 0; i < size; i++) {
			IntersectResult result = this->objects[i]->isIntersected(ray);
			if (result.geometry && (result.distance < minDistance)) {
				minDistance = result.distance;
				minResult = result;
			}
		}
		return minResult;
	}
};


#endif 

