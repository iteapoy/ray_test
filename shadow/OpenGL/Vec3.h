#pragma once
#include <iostream>
#include <algorithm>
#include"constant.h"

using namespace std;

#ifndef VEC3_H
#define VEC3_H

//������3ά�����ļӡ������ˡ�������ˡ���ˡ��������һ��
class Vec3f
{
public:
	float x, y, z;
	Vec3f() : x(float(0)), y(float(0)), z(float(0)) {}
	Vec3f(float xx) : x(xx), y(xx), z(xx) {}
	Vec3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

	Vec3f operator * (const float &f) const { return Vec3f(x * f, y * f, z * f); }
	Vec3f operator * (const Vec3f &v) const { return Vec3f(x * v.x, y * v.y, z * v.z); }
	// ���
	Vec3f cross(const Vec3f &v) const {return Vec3f(-z * v.y + y * v.z, z * v.x - x * v.z,-y * v.x + x * v.y);}
	// ���
	float dot(const Vec3f &v) const { return x * v.x + y * v.y + z * v.z; }

	Vec3f operator - (const Vec3f &v) const { return Vec3f(x - v.x, y - v.y, z - v.z); }
	Vec3f operator + (const Vec3f &v) const { return Vec3f(x + v.x, y + v.y, z + v.z); }
	Vec3f& operator += (const Vec3f &v) { x += v.x, y += v.y, z += v.z; return *this; }
	Vec3f& operator *= (const Vec3f &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
	Vec3f operator - () const { return Vec3f(-x, -y, -z); }

	// ���ȵ�ƽ��
	float sqrLength() const { return x * x + y * y + z * z; }
	// ����
	float length() const { return sqrt(sqrLength()); }

	// ������һ��
	Vec3f normalize()
	{
		float length = this->length();
		if (length == 0) return Vec3f(0, 0, 0);
		x = x / length;
		y = y / length;
		z = z / length;
		return Vec3f(x, y, z);
	}
	friend ostream & operator << (ostream &os, const Vec3f &v)
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};

#endif