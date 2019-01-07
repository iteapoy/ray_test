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

// ������С
const GLuint WIDTH = 400, HEIGHT = 400;

long maxDepth = 20;
float dD = 255.0f / maxDepth;
float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;
// ����ݵĲ�����
int ns = 5;

// �ݹ����
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

	if (result.isHit&&maxReflect>0) {
		Vec3f target = result.position + result.normal.unit() + random_in_unit_sphere();
		return color(Ray(result.position,target-result.position),scene, maxReflect-1)*0.5;
	}
	// ��õ�λ����
	Vec3f unit_dir = ray.direction.unit();
	// [-1,1]->[0,1]
	float t = 0.5*(unit_dir.y + 1.0);
	// t=0ʱ,��ɫΪ(255,255,255),t=1ʱ����ɫΪ(127.5,178.5,255)
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
		glfwPollEvents(); //��Ӧ�¼�
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

		glfwSwapBuffers(window); // ��������
	}

	glfwTerminate();
}

int main(int argc, char **argv)
{
	// ���ڳ�ʼ��
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracing", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//�ڴ�������Ⱦ
	glfwSetKeyCallback(window, key_call_back);

	renderScene(window);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}