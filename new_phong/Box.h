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
		float array1[6];

		/*光线方向向量和坐标轴平行。若光线起点不在两个平行平面之间，则没有交点
		并排除除零的可能*/

		if (direction.x == 0) {
			if ((origin.x < vertex_l.x) || (origin.x > vertex_h.x)) {
				return IntersectResult::noHit();
			}
			array1[0] = -INF;
			array1[1] = INF;
		}
		if (direction.y == 0) {
			if ((origin.y < vertex_l.y) || (origin.y > vertex_h.y)) {
				return IntersectResult::noHit();
			}
			array1[2] = -INF;
			array1[3] = INF;
		}
		if (direction.z == 0) {
			if ((origin.z < vertex_l.z) || (origin.z > vertex_h.z)) {
				return IntersectResult::noHit();
			}
			array1[4] = -INF;
			array1[5] = INF;
		}

		/*计算六个（三组）交点坐标，一次保存在数组中*/
		if ((direction.x != 0) && (direction.y != 0) && (direction.z != 0)) {
			array1[0] = (vertex_l.x - origin.x) / direction.x;
			array1[1] = (vertex_h.x - origin.x) / direction.x;
			array1[2] = (vertex_l.y - origin.y) / direction.y;
			array1[3] = (vertex_h.y - origin.y) / direction.y;
			array1[4] = (vertex_l.z - origin.z) / direction.z;
			array1[5] = (vertex_h.z - origin.z) / direction.z;
		}

		/*分X、Y、Z三组测试。*/
		for (int i = 0; i < 6; i = i + 2) {
			if (array1[i] > array1[i + 1]) {//将每组中较小的放在i位置，较大的放在i+1位置
				float t = array1[i];
				array1[i] = array1[i + 1];
				array1[i + 1] = t;
			}
			if (array1[i] >= t_near) { t_near = array1[i]; near_flag = i; }
			/*较小的值比t_near大时，把较小的值赋给t_near，同时记住当前t_near的位置near_flag。这里的near_flag可能的值0（t_x_小）、2（t_y_小）、4（t_z_小）（交点位置数组（array1[6]）中的每组中较小值的位置），后面我们会根据该值去法向量数组（Vec3f normals[6]）中获取撞击点的法向量，这里可以看出array1[6]和normals[6]是对应的：0、1对应X对应左右，2、3对应Y对应上下，4、5对应Z对应前后*/
			if (array1[i + 1] <= t_far) { t_far = array1[i + 1]; far_flag = i + 1; }
			/*较大的值比t_far小时，把较大的值赋给t_far，同时记住当前t_far的位置far_flag*/
			if (t_near > t_far) {//t_near>t_far，测试失败，光线不会撞击到长方体
				return IntersectResult::noHit();;
			}
			if (t_far < 0) {
				return IntersectResult::noHit();; //t_far<0，测试失败，光线不会撞击到长方体
			}
		}

		//测试通过（光线撞击到长方体），获得t_far、t_near和其对应的位置（对着X或者Y或者Z值）far_flag、near_flag
		IntersectResult result;
		result.geometry = this;
		result.isHit = true;
		result.distance = t_near;
		result.position = ray.getPoint(result.distance);

		/*求撞击点处的法向量*/
		Vec3f normals_choose[6];
		for (int j = 0; j < 6; j++)
			normals_choose[j] = Vec3f(0, 0, 0);

		for (int i = 0; i < 6; i++) {
			if (normals[i].dot(ray.direction) < 0) {
				/*选出面向光线起点的表面的法向量（最多三个），保存在新的数组normals_choose[]中*/
				normals_choose[i] = normals[i];
			}
		}
		for (int k = near_flag; k < 6; k++) {
			/*前面求得near_flag可能的值0（t_x_小）、2（t_y_小）、4（t_z_小）（也就是t_near可能出现的位置）。0、1对应X对应左右，2、3对应Y对应上下，4、5对应Z对应前后。在normals_choose[6]中只在对应位置保存了面向光线起点的长方体表面的法向量。所以，根据t_near值可以在该数组中取的唯一的不等于零的向量，这个向量即为撞击点处的法向量*/
			if (!isAlmostSame(normals_choose[k], Vec3f(0, 0, 0))) {
				result.normal = normals_choose[k];
				break;
			}
		}

		return result;

	}
};

#endif