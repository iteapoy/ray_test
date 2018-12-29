#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H

// 定义了PI、无穷大、无穷小
#define PI 3.141592653589793
#define INF 1e8
#define EPSILON 1e-8

// 定义了没有相交
#define NOT_INTERSECT -1

// 定义了浮点数下的0
#define ALMOST_ZERO(x) ((x)>-EPSILON && (x)<EPSILON)

#endif