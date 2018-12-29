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

//����
class Sphere
{
public:
	Vec3f center;                           // ����
	float radius, sqrRadius;                 // �뾶���뾶��ƽ��
	Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
	float transparency, reflection;         // ͸���Ⱥͷ����

	Sphere* left = NULL;
	Sphere* right = NULL;

	// �����ʼ��
	Sphere() {}
	// c�����ģ� r���뾶 ��sc��������ɫ�� ec����������ɫ ��transp��͸���ȣ� refl������
	Sphere(const Vec3f &c, const float &r, const Vec3f &sc,
		const float &refl = 0, const float &transp = 0, const Vec3f &ec = 0) :
		center(c), radius(r), sqrRadius(r * r), surfaceColor(sc), emissionColor(ec),
		transparency(transp), reflection(refl) {}

	// �ж��Ƿ��ཻ
	// rayorig�����ߵ�ԭ��
	// raydir:  ���ߵķ���
	// t0: ��һ������
	// t1: �ڶ�������

	bool intersect(const Ray &ray, float *t0, float *t1) const
	{}
};

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

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{
	Ray ray(Vec3f(1, 1, 1), Vec3f(2, 2, 2));
	ray.show();


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

	render(window);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}