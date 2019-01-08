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
		float array1[6];

		/*���߷���������������ƽ�С���������㲻������ƽ��ƽ��֮�䣬��û�н���
		���ų�����Ŀ���*/

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

		/*�������������飩�������꣬һ�α�����������*/
		if ((direction.x != 0) && (direction.y != 0) && (direction.z != 0)) {
			array1[0] = (vertex_l.x - origin.x) / direction.x;
			array1[1] = (vertex_h.x - origin.x) / direction.x;
			array1[2] = (vertex_l.y - origin.y) / direction.y;
			array1[3] = (vertex_h.y - origin.y) / direction.y;
			array1[4] = (vertex_l.z - origin.z) / direction.z;
			array1[5] = (vertex_h.z - origin.z) / direction.z;
		}

		/*��X��Y��Z������ԡ�*/
		for (int i = 0; i < 6; i = i + 2) {
			if (array1[i] > array1[i + 1]) {//��ÿ���н�С�ķ���iλ�ã��ϴ�ķ���i+1λ��
				float t = array1[i];
				array1[i] = array1[i + 1];
				array1[i + 1] = t;
			}
			if (array1[i] >= t_near) { t_near = array1[i]; near_flag = i; }
			/*��С��ֵ��t_near��ʱ���ѽ�С��ֵ����t_near��ͬʱ��ס��ǰt_near��λ��near_flag�������near_flag���ܵ�ֵ0��t_x_С����2��t_y_С����4��t_z_С��������λ�����飨array1[6]���е�ÿ���н�Сֵ��λ�ã����������ǻ���ݸ�ֵȥ���������飨Vec3f normals[6]���л�ȡײ����ķ�������������Կ���array1[6]��normals[6]�Ƕ�Ӧ�ģ�0��1��ӦX��Ӧ���ң�2��3��ӦY��Ӧ���£�4��5��ӦZ��Ӧǰ��*/
			if (array1[i + 1] <= t_far) { t_far = array1[i + 1]; far_flag = i + 1; }
			/*�ϴ��ֵ��t_farСʱ���ѽϴ��ֵ����t_far��ͬʱ��ס��ǰt_far��λ��far_flag*/
			if (t_near > t_far) {//t_near>t_far������ʧ�ܣ����߲���ײ����������
				return IntersectResult::noHit();;
			}
			if (t_far < 0) {
				return IntersectResult::noHit();; //t_far<0������ʧ�ܣ����߲���ײ����������
			}
		}

		//����ͨ��������ײ���������壩�����t_far��t_near�����Ӧ��λ�ã�����X����Y����Zֵ��far_flag��near_flag
		IntersectResult result;
		result.geometry = this;
		result.isHit = true;
		result.distance = t_near;
		result.position = ray.getPoint(result.distance);

		/*��ײ���㴦�ķ�����*/
		Vec3f normals_choose[6];
		for (int j = 0; j < 6; j++)
			normals_choose[j] = Vec3f(0, 0, 0);

		for (int i = 0; i < 6; i++) {
			if (normals[i].dot(ray.direction) < 0) {
				/*ѡ������������ı���ķ�������������������������µ�����normals_choose[]��*/
				normals_choose[i] = normals[i];
			}
		}
		for (int k = near_flag; k < 6; k++) {
			/*ǰ�����near_flag���ܵ�ֵ0��t_x_С����2��t_y_С����4��t_z_С����Ҳ����t_near���ܳ��ֵ�λ�ã���0��1��ӦX��Ӧ���ң�2��3��ӦY��Ӧ���£�4��5��ӦZ��Ӧǰ����normals_choose[6]��ֻ�ڶ�Ӧλ�ñ���������������ĳ��������ķ����������ԣ�����t_nearֵ�����ڸ�������ȡ��Ψһ�Ĳ�����������������������Ϊײ���㴦�ķ�����*/
			if (!isAlmostSame(normals_choose[k], Vec3f(0, 0, 0))) {
				result.normal = normals_choose[k];
				break;
			}
		}

		return result;

	}
};

#endif