#pragma once
#include <iostream>
#include <algorithm>
#include"constant.h"

using namespace std;

#ifndef VEC3_H
#define VEC3_H

//������3ά�����ļӡ������ˡ�������ˡ���ˡ��������һ��
template<typename T>
class Vec3
{
public:
	T x, y, z;
	Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
	Vec3(T xx) : x(xx), y(xx), z(xx) {}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}

	// ������һ��
	Vec3<T> normalize()
	{
		T length = length();
		x = x / length;
		y = y / length;
		z = z / length;
		return Vec3<T>(x, y, z);
	}

	Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
	Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
	// ���
	T cross(const Vec3<T> &v) const {return Vec3<T>(-z * v.y + y * v.z, z * v.x - x * v.z,-y * v.x + x * v.y);}
	// ���
	T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }

	Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
	Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
	Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }

	// ���ȵ�ƽ��
	T sqrLength() const { return x * x + y * y + z * z; }
	// ����
	T length() const { return sqrt(sqrLength()); }
	friend ostream & operator << (ostream &os, const Vec3<T> &v)
	{
		os << "[" << v.x << " " << v.y << " " << v.z << "]";
		return os;
	}
};

typedef Vec3<float> Vec3f;

bool isAlmostSame(const Vec3f & a, const Vec3f & b) {
	if (ALMOST_ZERO(a.x - b.x) && ALMOST_ZERO(a.y - b.y) && ALMOST_ZERO(a.z - b.z))
		return true;
	return false;
}
#endif