#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include"Object.h"

class Polygon :public Object
{
public:
	Vec3f *vertexes;/*多边形顶点数组*/
	int number;/*多边形顶点的个数*/

	Polygon() {}
	Polygon(Vec3f *v, int n) : vertexes(v), number(n){}

	/*根据多边形在UV平面的投影与+U轴交点的个数来判断交点是否在多边形内*/
	bool in_polygon(Vec3f*vertexes_uv, int number) {
		int sh, nsh;
		int nc = 0;
		sh = (vertexes_uv[0].y < 0) ? -1 : 1;

		for (int j = 0; j < number; j++) {
			nsh = (vertexes_uv[j + 1].y < 0) ? -1 : 1;

			if (sh != nsh) {
				if ((vertexes_uv[j].x > 0) && (vertexes_uv[j + 1].x > 0)) {
					if (vertexes_uv[j].y > vertexes_uv[j + 1].y)  nc ++; 
					else  nc --; 
				}
				else
					if ((vertexes_uv[j].x > 0) || (vertexes_uv[j + 1].x > 0)) {
						if (vertexes_uv[j].x - (vertexes_uv[j].y)*(vertexes_uv[j + 1].x - vertexes_uv[j].x) / (vertexes_uv[j + 1].y - vertexes_uv[j].y) > 0) {
							if (vertexes_uv[j].y > vertexes_uv[j + 1].y)  nc ++; 
							else  nc --; 
						}
					}
			}
			sh = nsh;
		}
		if (nc != 0) { return true; }
		else { return false; }
	}

	virtual IntersectResult isIntersected(const Ray& ray) {
		Vec3f poly_n;
		for (int i = 0; i < number - 2; i++) {
			/*以连续三个顶点构成的两个平行的向量的叉乘得到平面的法向量，并对法向量进行标准化*/
			poly_n = ((vertexes[i] - vertexes[i + 1]).cross(vertexes[i + 2]- vertexes[i + 1] )).unit();
			//确定平面的法向量
			if (!isAlmostSame(poly_n, Vec3f(0, 0, 0))) break;  //法向量不为0，即所选的两个向量不平行
		}
		/*随便找一个顶点（这里找的是第一个顶点），代入方程求得参数D（即为原点到平面的距离）*/
		float vd = poly_n.dot(ray.direction);
		float v0 = poly_n.dot(vertexes[0] - ray.origin);

		if (vd == 0) //光线平行于多边形平面
			return IntersectResult::noHit();

		float t = v0 / vd;//确定t和交点pi
		Vec3f pi = ray.getPoint(t);//获得pi的坐标

		// 找到主导坐标轴：x(1)  y(2)  z(3)
		float temp = abs(poly_n.x);
		int i = 1;
		if (temp <= abs(poly_n.y)) {
			temp = abs(poly_n.y);
			i++;
		}
		if (temp <= abs(poly_n.z)) {
			i++;
		}

		/*获得主导坐标轴的uv平面的二维向量*/
		Vec3f *vertexes_uv=new Vec3f[number + 1];

		switch (i) {
		case 1://丢弃x轴
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].y, vertexes[i].z, 0);
			}
			vertexes_uv[number] = Vec3f(pi.y, pi.z, 0);
			break;	
		case 2: //丢弃y轴
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].x, vertexes[i].z, 0);
			}
			vertexes_uv[number] = Vec3f(pi.x, pi.z, 0);
			break;
		case 3: //丢弃z轴
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].x, vertexes[i].y, 0);
			}
			vertexes_uv[number] = Vec3f(pi.x, pi.y, 0);
			break;
		}

		/*把交点移到原点
		把所有的点减去交点坐标*/
		for (int i = 0; i < number; i++) 
			vertexes_uv[i] = vertexes_uv[i] - vertexes_uv[number];
		vertexes_uv[number] = vertexes_uv[0];

		if (in_polygon(vertexes_uv, number)) {
				IntersectResult result;
				result.distance = t;
				result.geometry = this;
				result.isHit = true;
				result.position = ray.getPoint(result.distance);
				result.normal = poly_n;
				return result;
		}
		return IntersectResult::noHit();
	}
};

#endif