#pragma once
#ifndef BOX_H
#define BOX_H

#include"Object.h"

class Box :public Object
{
public:
	Vec3f vertex_l;//前左下顶点坐标
	Vec3f vertex_h;//后右上顶点坐标
	Vec3f normals[6];

	Box() {}
	Box(Vec3f _vl, Vec3f _vh) : vertex_l(_vl), vertex_h(_vh) {
		normals[0] = Vec3f(-1, 0, 0);   //左
		normals[1] = Vec3f(1, 0, 0);    //右
		normals[2] = Vec3f(0, 1, 0);    //上
		normals[3] = Vec3f(0, -1, 0);   //下
		normals[4] = Vec3f(0, 0, 1);    //前
		normals[5] = Vec3f(0, 0, -1);   //后
		/*按照这个顺序将六个表面的法向量保存在数组中。保存顺序和后面获取交点处法向量是对应的。*/
	}

	virtual IntersectResult isIntersected(const Ray& ray) {
		float t_near = -INF;
		float t_far = INF;
		int near_flag, far_flag;
		Vec3f direction = ray.direction;
		Vec3f origin = ray.origin;
		float arr[6];

		/*光线方向向量和坐标轴平行。若光线起点不在两个平行平面之间，则没有交点
		并排除除零的可能*/

		if (direction.x == 0) {
			if ((origin.x < vertex_l.x) || (origin.x > vertex_h.x)) {
				return IntersectResult::noHit();
			}
			arr[0] = -INF;
			arr[1] = INF;
		}
		if (direction.y == 0) {
			if ((origin.y < vertex_l.y) || (origin.y > vertex_h.y)) {
				return IntersectResult::noHit();
			}
			arr[2] = -INF;
			arr[3] = INF;
		}
		if (direction.z == 0) {
			if ((origin.z < vertex_l.z) || (origin.z > vertex_h.z)) {
				return IntersectResult::noHit();
			}
			arr[4] = -INF;
			arr[5] = INF;
		}

		/*计算六个（三组）交点坐标，一次保存在数组中*/
		if ((direction.x != 0) && (direction.y != 0) && (direction.z != 0)) {
			arr[0] = (vertex_l.x - origin.x) / direction.x;
			arr[1] = (vertex_h.x - origin.x) / direction.x;
			arr[2] = (vertex_l.y - origin.y) / direction.y;
			arr[3] = (vertex_h.y - origin.y) / direction.y;
			arr[4] = (vertex_l.z - origin.z) / direction.z;
			arr[5] = (vertex_h.z - origin.z) / direction.z;
		}

		/*分X、Y、Z三组测试。*/
		/*0、1：X左右，2、3：Y上下，4、5：Z前后*/
		for (int i = 0; i < 6; i = i + 2) {
			if (arr[i] > arr[i + 1]) {//将每组中较小的放在i位置，较大的放在i+1位置
				float t = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = t;
			}
			if (arr[i] >= t_near) { t_near = arr[i]; near_flag = i; }
			if (arr[i + 1] <= t_far) { t_far = arr[i + 1]; far_flag = i + 1; }

			if (t_near > t_far || t_far < 0)	return IntersectResult::noHit();
		}

		//测试通过（光线撞击到长方体），获得t_far、t_near和其对应的位置（对着X或者Y或者Z值）far_flag、near_flag
		IntersectResult result;
		result.geometry = this;
		result.isHit = true;
		result.distance = t_near;
		result.position = ray.getPoint(result.distance);

		/*求撞击点处的法向量*/
		Vec3f normals_choose[6];

		for (int i = 0; i < 6; i++) {
			normals_choose[i] = Vec3f(0, 0, 0);
			if (normals[i].dot(ray.direction) < 0) {
				/*选出面向光线起点的表面的法向量（最多三个），保存在新的数组normals_choose[]中*/
				normals_choose[i] = normals[i];
			}
		}

		for (int k = near_flag; k < 6; k++) {
			/*前面求得near_flag可能的值0（t_x_小）、2（t_y_小）、4（t_z_小）。
			在normals_choose[6]中只在对应位置保存了面向光线起点的长方体表面的法向量。
			所以，根据t_near值可以在该数组中取的唯一的不等于零的向量，这个向量即为撞击点处的法向量*/
			if (!isAlmostSame(normals_choose[k], Vec3f(0, 0, 0))) {
				result.normal = normals_choose[k];
				break;
			}
		}
		return result;
	}
};

#endif