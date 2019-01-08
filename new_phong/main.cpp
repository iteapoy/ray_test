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
#include "Lambertian.h"
#include "Metal.h"

using namespace std;

// 画布大小
const GLuint WIDTH = 800, HEIGHT = 400;

long maxDepth = 20;
float dD = 255.0f / maxDepth;
float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;
// 消锯齿的采样率
int ns = 20;
float aperture = 0.1; //光圈
Vec3f lookfrom(0, 0, 4);
Vec3f lookat(0, 0, -1);
float dist_to_focus = (lookfrom - lookat).length();

// 递归深度
#define MAX_RAY_DEPTH 3

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

Vec3f random_in_unit_sphere() {
	Vec3f p;
	do {
		p = Vec3f((rand() % (100) / (float)(100)),
			(rand() % (100) / (float)(100)),
			(rand() % (100) / (float)(100)))*2.0 - Vec3f(1, 1, 1);
	} while (p.sqrLength() >= 1.0);
	return p;
}

Vec3f color(const Ray & ray, Object* scene, long maxReflect)
{
	IntersectResult result = scene->isIntersected(ray);

	if (result.isHit&&maxReflect > 0) {
		Ray out;
		Vec3f attenuation;
		if (result.geometry->material&&maxReflect > 0 && result.geometry->material->sample(ray, result, attenuation, out)) {
			return attenuation * color(out, scene, maxReflect - 1);
		}
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
	Camera cam(lookfrom, lookat, Vec3f(0, 1, 0), 20, WIDTH / HEIGHT, aperture, dist_to_focus);
	Sphere* sphere1 = new Sphere(Vec3f(0, 0, -1), 0.5);
	Sphere* sphere2 = new Sphere(Vec3f(0, -100.5, -1), 100);
	Sphere* sphere3 = new Sphere(Vec3f(1, 0, -1), 0.5);
	Sphere* sphere4 = new Sphere(Vec3f(-1, 0, -1), 0.5);
	sphere1->material = new Lambertian(Vec3f(0.8, 0.3, 0.3));
	sphere2->material = new Lambertian(Vec3f(0.8, 0.8, 0.0));
	sphere3->material = new Metal(Vec3f(0.8, 0.6, 0.2));
	sphere4->material = new Metal(Vec3f(0.8, 0.8, 0.8));
	Union scene;
	scene.push(sphere1);
	scene.push(sphere2);
	scene.push(sphere3);
	scene.push(sphere4);

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
				Vec3f c(0, 0, 0);
				for (int s = 0; s < ns; s++) {
					float random = rand() % (100) / (float)(100);
					float u = sx + random / WIDTH;
					float v = sy + random / HEIGHT;
					Ray ray = cam.generateRay(u, v);
					c += color(ray, &scene, MAX_RAY_DEPTH);
				}
				c /= float(ns);
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