#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "constant.h"
#include<iostream>
using namespace std;

// ����һ����ɫ�࣬�������phong����

class Color {
public:
	float r, g, b;

	// ��ʼ��
	Color() {};
	Color(float _r, float _g, float _b) :r(_r), g(_g), b(_b) {}

	// ��ɫ���
	Color operator + (const Color& c) const {return Color(r + c.r, g + c.g, b + c.b);}
	// ��ɫ����һ��������
	Color operator * (const float &f) const { return Color(r * f, g * f, b * f); }
	// ��ɫ���
	Color operator * (const Color &c) const { return Color(r * c.r, g * c.g, b * c.b); }
	void saturate() {
		r = MIN(r, 1.0);
		g = MIN(g, 1.0);
		b = MIN(b, 1.0);
	}
	void show() { cout << "r:" << r << "g:" << g << "b:" << b << endl; }
	virtual ~Color() {};
};

#endif 
