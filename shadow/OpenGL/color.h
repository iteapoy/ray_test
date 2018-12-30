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
		r = min(r, float(1.0));
		g = min(g, float(1.0));
		b = min(b, float(1.0));
	}
	void show() { cout << "r:" << r << "g:" << g << "b:" << b << endl; }
	virtual ~Color() {};

	static inline Color black() { return Color(0, 0, 0); }
	static inline Color white() { return Color(1, 1, 1); }
	static inline Color yellow() { return Color(0.988, 0.8, 0.208); }
	static inline Color cyan() { return Color(0.4196, 0.749, 0.7176); }
	static inline Color gray() { return  Color(0.5, 0.5, 0.5); }
	static inline Color red() { return  Color(1, 0.5, 0.5); }
};

#endif 
