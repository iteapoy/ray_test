#pragma once
#ifndef CONSTANT_H
#define CONSTANT_H

// ������PI�����������С
#define PI 3.141592653589793
#define INF 1e8
#define EPSILON 1e-8

// �����˸������µ�0
#define ALMOST_ZERO(x) ((x)>-EPSILON && (x)<EPSILON)

// �����˻�����С
const GLuint WIDTH = 800, HEIGHT = 800;

#endif