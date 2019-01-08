#pragma once
#ifndef POLYGON_H
#define POLYGON_H

#include"Object.h"

class Polygon :public Object
{
public:
	Vec3f *vertexes;/*����ζ�������*/
	int number;/*����ζ���ĸ���*/

	Polygon() {}
	Polygon(Vec3f *v, int n) : vertexes(v), number(n){}

	/*���ݶ������UVƽ���ͶӰ��+U�ύ��ĸ������жϽ����Ƿ��ڶ������*/
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
			/*�������������㹹�ɵ�����ƽ�е������Ĳ�˵õ�ƽ��ķ����������Է��������б�׼��*/
			poly_n = ((vertexes[i] - vertexes[i + 1]).cross(vertexes[i + 2]- vertexes[i + 1] )).unit();
			//ȷ��ƽ��ķ�����
			if (!isAlmostSame(poly_n, Vec3f(0, 0, 0))) break;  //��������Ϊ0������ѡ������������ƽ��
		}
		/*�����һ�����㣨�����ҵ��ǵ�һ�����㣩�����뷽����ò���D����Ϊԭ�㵽ƽ��ľ��룩*/
		float vd = poly_n.dot(ray.direction);
		float v0 = poly_n.dot(vertexes[0] - ray.origin);

		if (vd == 0) //����ƽ���ڶ����ƽ��
			return IntersectResult::noHit();

		float t = v0 / vd;//ȷ��t�ͽ���pi
		Vec3f pi = ray.getPoint(t);//���pi������

		// �ҵ����������᣺x(1)  y(2)  z(3)
		float temp = abs(poly_n.x);
		int i = 1;
		if (temp <= abs(poly_n.y)) {
			temp = abs(poly_n.y);
			i++;
		}
		if (temp <= abs(poly_n.z)) {
			i++;
		}

		/*��������������uvƽ��Ķ�ά����*/
		Vec3f *vertexes_uv=new Vec3f[number + 1];

		switch (i) {
		case 1://����x��
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].y, vertexes[i].z, 0);
			}
			vertexes_uv[number] = Vec3f(pi.y, pi.z, 0);
			break;	
		case 2: //����y��
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].x, vertexes[i].z, 0);
			}
			vertexes_uv[number] = Vec3f(pi.x, pi.z, 0);
			break;
		case 3: //����z��
			for (int i = 0; i < number; i++) {
				vertexes_uv[i] = Vec3f(vertexes[i].x, vertexes[i].y, 0);
			}
			vertexes_uv[number] = Vec3f(pi.x, pi.y, 0);
			break;
		}

		/*�ѽ����Ƶ�ԭ��
		�����еĵ��ȥ��������*/
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