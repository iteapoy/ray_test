#pragma once
#ifndef BOX_H
#define BOX_H

#include"Object.h"

class Box :public Object
{
public:
	Vec3f vertex_l;//ǰ���¶�������
	Vec3f vertex_h;//�����϶�������
	Vec3f normals[6];

	Box() {}
	Box(Vec3f _vl, Vec3f _vh) : vertex_l(_vl), vertex_h(_vh) {
		normals[0] = Vec3f(-1, 0, 0);   //��
		normals[1] = Vec3f(1, 0, 0);    //��
		normals[2] = Vec3f(0, 1, 0);    //��
		normals[3] = Vec3f(0, -1, 0);   //��
		normals[4] = Vec3f(0, 0, 1);    //ǰ
		normals[5] = Vec3f(0, 0, -1);   //��
		/*�������˳����������ķ����������������С�����˳��ͺ����ȡ���㴦�������Ƕ�Ӧ�ġ�*/
	}

	virtual IntersectResult isIntersected(const Ray& ray) {
		float t_near = -INF;
		float t_far = INF;
		int near_flag, far_flag;
		Vec3f direction = ray.direction;
		Vec3f origin = ray.origin;
		float arr[6];

		/*���߷���������������ƽ�С���������㲻������ƽ��ƽ��֮�䣬��û�н���
		���ų�����Ŀ���*/

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

		/*�������������飩�������꣬һ�α�����������*/
		if ((direction.x != 0) && (direction.y != 0) && (direction.z != 0)) {
			arr[0] = (vertex_l.x - origin.x) / direction.x;
			arr[1] = (vertex_h.x - origin.x) / direction.x;
			arr[2] = (vertex_l.y - origin.y) / direction.y;
			arr[3] = (vertex_h.y - origin.y) / direction.y;
			arr[4] = (vertex_l.z - origin.z) / direction.z;
			arr[5] = (vertex_h.z - origin.z) / direction.z;
		}

		/*��X��Y��Z������ԡ�*/
		/*0��1��X���ң�2��3��Y���£�4��5��Zǰ��*/
		for (int i = 0; i < 6; i = i + 2) {
			if (arr[i] > arr[i + 1]) {//��ÿ���н�С�ķ���iλ�ã��ϴ�ķ���i+1λ��
				float t = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = t;
			}
			if (arr[i] >= t_near) { t_near = arr[i]; near_flag = i; }
			if (arr[i + 1] <= t_far) { t_far = arr[i + 1]; far_flag = i + 1; }

			if (t_near > t_far || t_far < 0)	return IntersectResult::noHit();
		}

		//����ͨ��������ײ���������壩�����t_far��t_near�����Ӧ��λ�ã�����X����Y����Zֵ��far_flag��near_flag
		IntersectResult result;
		result.geometry = this;
		result.isHit = true;
		result.distance = t_near;
		result.position = ray.getPoint(result.distance);

		/*��ײ���㴦�ķ�����*/
		Vec3f normals_choose[6];

		for (int i = 0; i < 6; i++) {
			normals_choose[i] = Vec3f(0, 0, 0);
			if (normals[i].dot(ray.direction) < 0) {
				/*ѡ������������ı���ķ�������������������������µ�����normals_choose[]��*/
				normals_choose[i] = normals[i];
			}
		}

		for (int k = near_flag; k < 6; k++) {
			/*ǰ�����near_flag���ܵ�ֵ0��t_x_С����2��t_y_С����4��t_z_С����
			��normals_choose[6]��ֻ�ڶ�Ӧλ�ñ���������������ĳ��������ķ�������
			���ԣ�����t_nearֵ�����ڸ�������ȡ��Ψһ�Ĳ�����������������������Ϊײ���㴦�ķ�����*/
			if (!isAlmostSame(normals_choose[k], Vec3f(0, 0, 0))) {
				result.normal = normals_choose[k];
				break;
			}
		}
		return result;
	}
};

#endif