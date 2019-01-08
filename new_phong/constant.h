#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H

// ������PI�����������С
#define PI 3.141592653589793
#define EPSILON 1e-8
const float INF=1e30;

// �����˸������µ�0
#define ALMOST_ZERO(x) ((x)>-EPSILON && (x)<EPSILON)

#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))

#endif