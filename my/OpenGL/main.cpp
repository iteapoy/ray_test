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
#include "color.h"
#include "CheckMaterial.h"
#include "PhongMaterial.h"
#include "Plane.h"
#include "Union.h"
using namespace std;

float dx = 1.0f / WIDTH;
float dy = 1.0f / HEIGHT;

// �ݹ����
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
		//��Ӧ�¼�
		glfwPollEvents();

		glClearColor(0.5f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pix);
		glfwSwapBuffers(window); //��������
	}
	delete[] pix;
	glfwTerminate();
}

void renderUnion(GLFWwindow* window, PerspectiveCamera &camera)
{

	long maxDepth = 20;
	Sphere* sphere1 = new Sphere(Vec3f(0, 6, -10), 6.0);
	Plane* plane1 = new Plane(Vec3f(0, 1, 0), 1.0);
	plane1->material = new CheckMaterial(0.1f);
	sphere1->material = new PhongMaterial(Color(1, 0, 0), Color(1, 1, 1), 20);
	Union scene;
	scene.push(plane1);
	scene.push(sphere1);
	float dD = 255.0f / maxDepth;

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
				Ray ray(camera.generateRay(sx, sy));
				IntersectResult result = scene.isIntersected(ray);
				if (result.isHit)
				{
					Color color = result.geometry->material->sample(ray, result.position, result.normal);
					color.saturate();
					glColor3ub(color.r * 255, color.g * 255, color.b * 255);
					glVertex2f(sx, sy);
				}
			}
		}
		glEnd();

		glfwSwapBuffers(window); // ��������
	}
	glfwTerminate();
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{
	// �����ʼ��
	PerspectiveCamera camera(Vec3f(0, 10, 10), Vec3f(0, -0.5, -1), Vec3f(0, 1, 0), 90);

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

	renderUnion(window,camera);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}