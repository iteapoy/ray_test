#pragma once
#ifndef UNION_H
#define UNION_H

// ����һ������Ĳ���

#include"Object.h"
#include"constant.h"
#include <vector>

class Union :public Object {
private:
	vector<Object*> objects;

public:
	Union() {};
	virtual ~Union() {};
	void push(Object* object) { objects.push_back(object); }

	// ���������������ray����Ľ���minResult�����
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

