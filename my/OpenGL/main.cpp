#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <sstream>

#define GLEW_STATIC
#define GLFW_INCLUDE_GLU
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\vec3.hpp>
#include <glm\glm.hpp>

#include "Vec3.h"
#include "constant.h"
#include "Ray.h"
using namespace std;

const GLuint WIDTH = 800, HEIGHT = 600;

//球体
class Sphere
{
public:
	Vec3f center;                           // 球心
	float radius, sqrRadius;                 // 半径，半径的平方
	Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
	float transparency, reflection;         // 透明度和反射度

	Sphere* left = NULL;
	Sphere* right = NULL;

	// 球体初始化
	Sphere() {}
	// c：球心， r：半径 ，sc：表面颜色， ec：发出的颜色 ，transp：透明度， refl：反射
	Sphere(const Vec3f &c, const float &r, const Vec3f &sc,
		const float &refl = 0, const float &transp = 0, const Vec3f &ec = 0) :
		center(c), radius(r), sqrRadius(r * r), surfaceColor(sc), emissionColor(ec),
		transparency(transp), reflection(refl) {}

	// 判断是否相交
	// rayorig：光线的原点
	// raydir:  光线的方向
	// t0: 第一个交点
	// t1: 第二个交点

	bool intersect(const Ray &ray, float *t0, float *t1) const
	{}
};

// 递归深度
#define MAX_RAY_DEPTH 5

void render(GLFWwindow* window)
{
	//unsigned WIDTH = 640, HEIGHT = 480;

	// Render PIXELS
	unsigned int* pix = new unsigned int[WIDTH*HEIGHT * 3];

	//// Setting camera (you can using (0,0,0) as orignal point)
	Vec3f *image = new Vec3f[WIDTH * HEIGHT], *pixel = image;
	float invWidth = 1 / float(WIDTH), invHeight = 1 / float(HEIGHT);
	float fov = 45, aspectratio = WIDTH / float(HEIGHT);
	float angle = tan(PI * 0.5 * fov / 180.);

	double beforetime = glfwGetTime();

	// ray tracing

	double aftertime = glfwGetTime();
	double totaltime = aftertime - beforetime;

	for (unsigned i = 0; i < HEIGHT; i++)
		for (unsigned j = 0; j < WIDTH; j++) {
			pix[3 * ((HEIGHT - 1 - i)*WIDTH + j)] = min(image[i*WIDTH + j].x, float(1)) * 255;
			pix[3 * ((HEIGHT - 1 - i)*WIDTH + j) + 1] = min(image[i*WIDTH + j].y, float(1)) * 255;
			pix[3 * ((HEIGHT - 1 - i)*WIDTH + j) + 2] = min(image[i*WIDTH + j].z, float(1)) * 255;
		}

	while (!glfwWindowShouldClose(window)) {
		//响应事件
		glfwPollEvents();

		glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pix);
		glfwSwapBuffers(window); //交换缓存
	}
	delete[] pix;
	glfwTerminate();
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{
	Ray ray(Vec3f(1, 1, 1), Vec3f(2, 2, 2));
	ray.show();


	// 窗口初始化
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//在窗口中渲染
	glfwSetKeyCallback(window, key_call_back);

	render(window);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}