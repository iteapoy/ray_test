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

#include "Union.h"
#include "Sphere.h"
#include "Camera.h"

using namespace std;

// 画布大小
const GLuint WIDTH = 800, HEIGHT = 800;

long maxDepth = 20;
float dD = 255.0f / maxDepth;
float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;

// 递归深度
#define MAX_RAY_DEPTH 3

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

Vec3f color(const Ray & ray, Object* scene)
{
	IntersectResult result = scene->isIntersected(ray);

	if (result.isHit) {
		Vec3f N =result.normal.unit();
		return Vec3f(N.x + 1, N.y + 1, N.z + 1)*0.5;
	}
	// 获得单位向量
	Vec3f unit_dir = ray.direction.unit();
	// [-1,1]->[0,1]
	float t = 0.5*(unit_dir.y + 1.0);
	// t=0时,颜色为(255,255,255),t=1时，颜色为(127.5,178.5,255)
	return Vec3f(1.0, 1.0, 1.0)*(1.0 - t) + Vec3f(0.5, 0.7, 1.0)*t;
}

void renderScene(GLFWwindow* window)
{
	Camera cam;
	Sphere* sphere1 = new Sphere(Vec3f(0, 0, -1), 0.5);
	Sphere* sphere2 = new Sphere(Vec3f(0, -100.5, -1), 100);
	Union scene;
	scene.push(sphere1);
	scene.push(sphere2);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); //响应事件
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glScaled(2.0f, 2.0f, 1.0f);
		glTranslatef(-0.5f, -0.5f, 1.0f);

		glBegin(GL_POINTS);
		for (long y = 0; y < HEIGHT; ++y)
		{
			float sy = 1 - dy * y;
			for (long x = 0; x < WIDTH; ++x)
			{
				float sx = dx * x;
				Ray ray = cam.generateRay(sx, sy);
				Vec3f c = color(ray, &scene);
				glColor3ub(c.x * 255, c.y * 255, c.z * 255);
				glVertex2f(sx, sy);
			}
		}
		glEnd();

		glfwSwapBuffers(window); // 交换缓存
	}

	glfwTerminate();
}

int main(int argc, char **argv)
{
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

	renderScene(window);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}