#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "intersectresult.h"
#include "Ray.h"

// ����һ������Ļ���

class Object {
public:
	// ��ʼ��
	Object() {}
	// ��������
	virtual ~Object() {}
	// �麯�����󽻵�
	virtual IntersectResult isIntersected(const Ray& ray) = 0 {cout << "�������"<<endl;}
};

#endif 
