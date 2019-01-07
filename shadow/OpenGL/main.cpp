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

#include "constant.h"
#include "Camera.h"
#include "Sphere.h"
#include "CheckMaterial.h"
#include "PhongMaterial.h"
#include "Plane.h"
#include "Union.h"
#include "DirectLight.h"
using namespace std;

// 画布大小
const GLuint WIDTH = 800, HEIGHT = 800;

long maxDepth = 20;
float dD = 255.0f / maxDepth;
float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;

// 递归深度
#define MAX_RAY_DEPTH 3

// 光线追踪算法
Color rayTrace(Object* scene, Ray &ray, long maxReflect)
{
	//  scene->isIntersected(ray) 求出场景scene中与ray最近的交点
	IntersectResult result = scene->isIntersected(ray);

	// 如果有交点就递归
	if (result.geometry)
	{
		// 获得交点对象的材质反射系数
		float reflectiveness = result.geometry->material->getRef();
		// 获得交点对象的材质的颜色
		Color color = result.geometry->material->sample(ray, result.position, result.normal.normalize());
		if ((reflectiveness > 0) && (maxReflect > 0))
		{
			// 获得交点对象的实际的颜色
			color = color * (1 - reflectiveness);
			// R=2(L·N)·N-L
			Vec3f r = result.normal *(-2 * result.normal.dot(ray.direction)) + ray.direction;
			Ray ray = Ray(result.position, r);
			Color reflectedColor = rayTrace(scene, ray, maxReflect - 1);
			color = color + reflectedColor * reflectiveness;
		}
		return color;
	}
	// 如果没有交点就返回背景色（默认：La=black)
	else return La;
}

void renderScene(GLFWwindow* window, PerspectiveCamera &camera,Union &scene)
{
	DirectLight light1(Color::white(), lightDir*(-1),true);

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
				// 从 (sx,sy） 发出光线
				Ray ray(camera.generateRay(sx, sy));
				IntersectResult result = scene.isIntersected(ray);
				if (result.isHit)
				{
					// 利用光线追踪求颜色
					//Color color = rayTrace(&scene, ray, MAX_RAY_DEPTH);
					// 阴影效果
					//Color color = light1.intersect(scene, result);
					Color color = light1.intersect(scene, result) * rayTrace(&scene, ray, MAX_RAY_DEPTH);
					color.saturate();
					glColor3ub(color.r * 255, color.g * 255, color.b * 255);
					//glColor3ub((result.normal.x+1) * 128, (result.normal.y + 1) * 128, (result.normal.z + 1) * 128);
					glVertex2f(sx, sy);
				}
			}
		}
		glEnd();

		glfwSwapBuffers(window); // 交换缓存
	}
	glfwTerminate();
}


void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{
	// 相机初始化
	// 第一个参数：视点，第二个参数：前，第三个参数：上，第四个参数：FOV 视野
	PerspectiveCamera camera(Vec3f(0,10, 10), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 90);

	// 创建场景，两个球体，一个带棋盘格子的平面
	//Sphere* sphere1 = new Sphere(Vec3f(-3, 5, -10), 5.0);
	Sphere* sphere1 = new Sphere(Vec3f(-5, 5, -15), 5.0);
	Sphere* sphere2 = new Sphere(Vec3f(3, 3, -10), 3.0);

	Plane* plane1 = new Plane(Vec3f(0, 1, 0), 0.0);
	Plane* plane2 = new Plane(Vec3f(0, 0, 1), -50);
	Plane* plane3 = new Plane(Vec3f(1, 0, 0), -20);

	//plane1->material = new CheckMaterial(0.1f);
	plane1->material = new  PhongMaterial(Color::gray(), Color::white(), 5, 0.15);
	plane2->material = new PhongMaterial(Color::gray(), Color::white(), 10, 0);
	plane3->material = new PhongMaterial(Color::gray(), Color::white(), 10, 0);
	sphere1->material = new PhongMaterial(Color::yellow(), Color::white(), 20, 0.05);
	sphere2->material = new PhongMaterial(Color::cyan(), Color::white(), 16, 0.04);
	Union scene;
	scene.push(plane1);
	//scene.push(plane2);
	//scene.push(plane3);
	scene.push(sphere1);
	scene.push(sphere2);

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

	//在窗口中渲染3
	glfwSetKeyCallback(window, key_call_back);

	renderScene(window,camera,scene);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}