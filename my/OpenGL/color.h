#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "constant.h"
#include<iostream>
using namespace std;

// 定义一个颜色类，被后面的phong所用

class Color {
public:
	float r, g, b;

	// 初始化
	Color() {};
	Color(float _r, float _g, float _b) :r(_r), g(_g), b(_b) {}

	// 颜色相加
	Color operator + (const Color& c) const {return Color(r + c.r, g + c.g, b + c.b);}
	// 颜色乘以一个浮点数
	Color operator * (const float &f) const { return Color(r * f, g * f, b * f); }
	// 颜色相乘
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
};

#endif 
