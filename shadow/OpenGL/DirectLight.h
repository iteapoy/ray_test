#pragma once
#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H

#include "color.h"
#include "Vec3.h"
#include"Union.h"

// ����һ�������Դ��������Ӱ
class DirectLight {
private:
	// color:��Դ��ɫ�� direction:��Դ���� isShadow���Ƿ������Ӱ
	Color color;
	Vec3f direction;
	bool isShadow;

public:
	// ��ʼ��
	DirectLight() {};
	DirectLight(Color _color, Vec3f _direction, bool _isShadow):
		color(_color), direction(_direction), isShadow(_isShadow)
	{};
	// ��������
	virtual ~DirectLight() {};

	//ͨ�������볡�����ཻ���������ս��
	Color intersect(Union &scence, IntersectResult &result) {
		//����������෴�����shadowRay
		Vec3f shadowDir = direction.normalize()*(-1);
		Ray shadowRay = Ray(result.position, shadowDir);

		//����shadowRay�Ƿ��볡���ཻ
		IntersectResult lightResult = scence.isIntersected(shadowRay);
		Color resultColor = Color::black();
		if (isShadow)
			if (lightResult.geometry)
				return resultColor;

		//�����ǿ
		//float NdotL = result.normal.dot(shadowDir);
		//if (NdotL >= 0)
		//	resultColor = resultColor + this->color*NdotL;
		// //��Ϊֱ�Ӻ�ԭ�е�phong���������ӣ����Է���white=(1��1��1)�����Ϻ�ͱ��������
		//return resultColor;
		return Color::white();
	};
};


#endif 
