#pragma once
#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H

#include "color.h"
#include "Vec3.h"
#include"Union.h"

// 定义一个方向光源，产生阴影
class DirectLight {
private:
	// color:光源颜色， direction:光源方向， isShadow：是否产生阴影
	Color color;
	Vec3f direction;
	bool isShadow;

public:
	// 初始化
	DirectLight() {};
	DirectLight(Color _color, Vec3f _direction, bool _isShadow):
		color(_color), direction(_direction), isShadow(_isShadow)
	{};
	// 析构函数
	virtual ~DirectLight() {};

	//通过光线与场景的相交结果计算光照结果
	Color intersect(Union &scence, IntersectResult &result) {
		//生成与光照相反方向的shadowRay
		Vec3f shadowDir = direction.normalize()*(-1);
		Ray shadowRay = Ray(result.position, shadowDir);

		//计算shadowRay是否与场景相交
		IntersectResult lightResult = scence.isIntersected(shadowRay);
		Color resultColor = Color::black();
		if (isShadow)
			if (lightResult.geometry)
				return resultColor;

		//计算光强
		//float NdotL = result.normal.dot(shadowDir);
		//if (NdotL >= 0)
		//	resultColor = resultColor + this->color*NdotL;
		// //因为直接和原有的phong材质做叠加，所以返回white=(1，1，1)，乘上后就变得正常了
		//return resultColor;
		return Color::white();
	};
};


#endif 
