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
#include "Ray.h"

using namespace std;

// ������С
const GLuint WIDTH = 800, HEIGHT = 800;

long maxDepth = 20;
float dD = 255.0f / maxDepth;
float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;

// �ݹ����
#define MAX_RAY_DEPTH 3

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

float hit_sphere(const Vec3f& center, float radius, const Ray& ray)
{
	Vec3f v = ray.origin - center;
	float a0 = v.sqrLength() - radius*radius;
	float DdotV = ray.direction.dot(v);

	if (DdotV <= 0) {
		float discr = DdotV * DdotV - a0;
		if (discr >= 0) {
			float t = -DdotV - sqrt(discr);
			return t;
		}
	}
	return -1.0;
}

Vec3f color(const Ray & ray)
{
	Vec3f center = Vec3f(0, 0, -1);
	float radius = 0.5f;
	float t = hit_sphere(center, radius, ray);
	if (t > 0.0) {
		Vec3f N = (ray.getPoint(t) - center).normalize();
		return Vec3f(N.x + 1, N.y + 1, N.z + 1)*0.5;
	}
	// ��õ�λ����
	Vec3f unit_dir = ray.direction.unit();
	// [-1,1]->[0,1]
	t = 0.5*(unit_dir.y + 1.0);
	// t=0ʱ,��ɫΪ(255,255,255),t=1ʱ����ɫΪ(127.5,178.5,255)
	return Vec3f(1.0, 1.0, 1.0)*(1.0 - t) + Vec3f(0.5, 0.7, 1.0)*t;
}

Vec3f lower_left_corner(-1.0, -1.0, -1.0);
Vec3f horizontal(2.0, 0.0, 0.0);
Vec3f vertical(0.0, 2.0, 0.0);
Vec3f origin(0.0, 0.0, 0.0);

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
				Ray ray(origin, lower_left_corner + horizontal*sx + vertical*sy);
				Vec3f c = color(ray);
				glColor3ub(c.x*255,c.y*255,c.z*255);
				glVertex2f(sx, sy);
			}
		}
		glEnd();

		glfwSwapBuffers(window); // ��������
	}

	glfwTerminate();

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}