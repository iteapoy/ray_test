#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H

// 定义了PI、无穷大、无穷小
#define PI 3.141592653589793
#define EPSILON 1e-8
const float INF=1e30;

// 定义了浮点数下的0
#define ALMOST_ZERO(x) ((x)>-EPSILON && (x)<EPSILON)

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#endif