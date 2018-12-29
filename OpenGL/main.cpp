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
using namespace std;

#define M_PI 3.141592653589793
#define INF 1e8

const GLuint WIDTH = 800, HEIGHT = 600;

typedef Vec3<float> Vec3f;

int aa = 0;
int bb = 0;
int cc = 0;
int dir = 0;

//����
class Sphere
{
public:
	Vec3f center;                           // ����
	float radius, radius2;                 // �뾶���뾶��ƽ��
	Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
	float transparency, reflection;         /// surface transparency and reflectivity

	Sphere* left = NULL;
	Sphere* right = NULL;

	// �����ʼ��
	Sphere() {}
	// c�����ģ� r���뾶 ��sc��������ɫ�� ec����������ɫ 
	// transp��͸���ȣ� refl������
	Sphere(const Vec3f &c, const float &r, const Vec3f &sc,
		const float &refl = 0, const float &transp = 0, const Vec3f &ec = 0) :
		center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec),
		transparency(transp), reflection(refl) {}

	// �ж��Ƿ��ཻ
	// rayorig�����ߵ�ԭ��
	// raydir:  ���ߵķ���
	// t0: ��һ������
	// t1: �ڶ�������

	bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1) const
	{
		// ----------------------- ��ʽ ------------------------
		// r(t) = o + td
		// v������ԭ������ĵľ���
		// v = o-d
		// DdotV = v��d
		// t^2 + (2v��d)t + (v^2-r^2) = 0
		// t0 = -v��d - sqrt((v��d)^2-(v^2-r^2)) = -DdotV -sqrt(DdotV^2-(v^2-r^2))
		// ----------------------- ��ʽ ------------------------

		Vec3f v = center - rayorig; 
		float DdotV = v.dot(raydir);   

		if (DdotV < 0) return false;
		float d2 = v.dot(v) - (DdotV * DdotV);
		if (d2 > radius2) return false;
		float thc = sqrt(radius2 - d2);
		if (t0 != NULL && t1 != NULL) {
			// ��ǰһ������ľ���
			*t0 = DdotV - thc;
			// ����һ������ľ���
			*t1 = DdotV + thc;
		}
		return true;
	}

	void my_intersect(const Vec3f &rayorig, const Vec3f &raydir, float &tnear, Sphere &sph) {
		if (this != NULL)
		{
			if (left == NULL && right == NULL) {
				float t0 = INF, t1 = INF;
				Vec3f v = center - rayorig;
				float DdotV = v.dot(raydir);

				if (DdotV < 0) return;
				float d2 = v.dot(v) - (DdotV * DdotV);
				if (d2 > radius2) return;
				float thc = sqrt(radius2 - d2);
				dir++;
				t0 = DdotV - thc;
				t1 = DdotV + thc;
				if (t0 < 0) t0 = t1;
				if (t0 < tnear) {
					tnear = t0;
					sph = *this;
				}
			}
			left->my_intersect(rayorig, raydir, tnear, sph);
			right->my_intersect(rayorig, raydir, tnear, sph);
		}
	}
};

//[comment]
// This variable controls the maximum recursion depth
//[/comment]
#define MAX_RAY_DEPTH 5

float mix(const float &a, const float &b, const float &mix)
{
	return b * mix + a * (1 - mix);
}

//[comment]
// This is the main trace function. It takes a ray as argument (defined by its origin
// and direction). We test if this ray intersects any of the geometry in the scene.
// If the ray intersects an object, we compute the intersection point, the normal
// at the intersection point, and shade this point using this information.
// Shading depends on the surface property (is it transparent, reflective, diffuse).
// The function returns a color for the ray. If the ray intersects an object that
// is the color of the object at the intersection point, otherwise it returns
// the background color.
//[/comment]
Vec3f trace(const Vec3f &rayorig,const Vec3f &raydir,const vector<Sphere *> &spheres,const int &depth,Sphere* my_sphere)
{
	//if (raydir.length() != 1) cerr << "Error " << raydir << endl;
	float tnear = INF;
	const Sphere* sphere = NULL;
	// find intersection of this ray with the sphere in the scene
	
	for (unsigned i = 0; i < spheres.size(); ++i) {
		float t0 = INF, t1 = INF;
		if (spheres[i]->intersect(rayorig, raydir, &t0, &t1)) {
			dir++;
			// ���������������棬�Ͳ���ǰ��Ľ���
			if (t0 < 0) t0 = t1;
			if (t0 < tnear) {
				//�ж� tnear �Ƿ�������Ľ���
				tnear = t0;
				sphere = spheres[i];
			}
		}
	}
	/*Sphere sph;
	float t0 = INF, t1 = INF;
	my_sphere->my_intersect(rayorig, raydir, tnear, sph);
	sphere = &sph;*/
	// *******************************************************************************
	if (sphere != NULL) {
		aa++;
		Vec3f surfaceColor = 0.0; // color of the ray/surfaceof the object intersected by the ray
		Vec3f phit = rayorig + raydir * tnear; // point of intersection
		Vec3f nhit = phit - sphere->center; // ���㷨����
		nhit.normalize(); // �淶��
						  // If the normal and the view direction are not opposite to each other
						  // reverse the normal direction. That also means we are inside the sphere so set
						  // the inside bool to true. Finally reverse the sign of IdotN which we want
						  // positive.
		float bias = 1e-4; // add some bias to the point from which we will be tracing
		bool inside = false;
		if (raydir.dot(nhit) > 0) {
			nhit = -nhit;
			inside = true;
		}
		if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {

			// ����
			float IdotN = raydir.dot(nhit); // ���߷���淶��
			float facingratio = max(float(0), -IdotN);
			// ��ͬ�Ⲩ����������ͷ��� ���ߴ�ֱ�ڱ���ʱ������������������߷Ǵ�ֱ����ʱ���н�ԽС������Խ���� (������ЧӦ)
			float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
			Vec3f refldir;
			//refldir = glm::reflect(raydir, nhit);          �������ʵ�ʽ��еļ��������������
			refldir = raydir - nhit * 2 * raydir.dot(nhit);
			refldir.normalize();
			// �ݹ����
			Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1, my_sphere);

			bb++;																	//BB���㷴���������
			Vec3f refraction = 0;
			// ���͸���Ȳ�Ϊ�㣬�����������
			if (sphere->transparency > 0) {
				// ����
				float ior = 1.1;
				float eta = 1 / ior;												//�����������1/1.2
				float k = 1 - eta * eta * (1 - IdotN * IdotN);
				Vec3f refrdir = raydir * eta - nhit * (eta *  IdotN + sqrt(k));
				refrdir.normalize();

				cc++;																//CC���������������
				refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1, my_sphere);		// �ݹ����,ʹ���Լ��Ĳ�
			}
			// the result is a mix of reflection and refraction (if the sphere is transparent)
			surfaceColor = (
				reflection * fresneleffect +
				refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
		}
		else {
			// it's a diffuse object, no need to raytrace any further
			double shadow = 1.0;
			for (unsigned i = 0; i < spheres.size(); ++i) {
				if (spheres[i]->emissionColor.x > 0) {
					// 
					// shadering

					// *******************************************************************************
					// YOUR CODES
					Vec3f transmission = 1.0;
					Vec3f lightDirection = spheres[i]->center - phit;
					lightDirection.normalize();
					float light_angle = (acos(raydir.dot(lightDirection))
						/ (sqrt(raydir.dot(raydir))
							* sqrt(lightDirection.dot(lightDirection))));
					float light_incidence = sin(light_angle);

					for (unsigned j = 0; j < spheres.size(); ++j) {
						if (i != j) {
							float t0, t1;
							// �жϸõ�Ĺ��Ƿ��Դ�����ཻ������ཻ��������Ӱ

							if (spheres[j]->intersect(phit + (nhit * bias), lightDirection, &t0, &t1)) {
								shadow = max(0.0, shadow - (1.0 - spheres[j]->transparency));
								transmission = transmission * spheres[j]->surfaceColor * shadow;
								//break;
							}
						}
					}
					// *******************************************************************************

					// using phong model to calculate
					// ÿһ�����ߣ����������������Ӱ��
					surfaceColor += sphere->surfaceColor * transmission *
						max(float(0), nhit.dot(lightDirection)) * spheres[i]->emissionColor;

				}
			}
		}

		return surfaceColor + sphere->emissionColor;

	}
	else {
		return Vec3f(0.57, 0.68, 0.450);
	}
}

//[comment]
// Main rendering function. We compute a camera ray for each pixel of the image
// trace it and return a color. If the ray hits a sphere, we return the color of the
// sphere at the intersection point, else we return the background color.
//[/comment]

void render(const vector<Sphere *> spheres, GLFWwindow* window, Sphere* my)
{

	//unsigned WIDTH = 640, HEIGHT = 480;

	// Render PIXELS
	unsigned char* pix = new unsigned char[WIDTH*HEIGHT * 3];

	//// Setting camera (you can using (0,0,0) as orignal point)
	Vec3f *image = new Vec3f[WIDTH * HEIGHT], *pixel = image;
	float invWidth = 1 / float(WIDTH), invHeight = 1 / float(HEIGHT);
	float fov = 45, aspectratio = WIDTH / float(HEIGHT);
	float angle = tan(M_PI * 0.5 * fov / 180.);

	double beforetime = glfwGetTime();
	//// Trace rays

	for (unsigned y = 0; y < HEIGHT; ++y) {
		for (unsigned x = 0; x < WIDTH; ++x, ++pixel) {
			// create rays below
			// *******************************************************************************
			// YOUR CODES
			float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
			float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
			Vec3f raydir(xx, yy, -1);
			raydir.normalize();

			// *******************************************************************************
			// remember to normalize

			*pixel = trace(Vec3f(0), raydir, spheres, 0, my);
			//*pixel = Vec3f(1.00, 0.1, 0.1);
			//cout << pix[3 * (y*WIDTH + x)] <<"  "<< pix[3 * (y*WIDTH + x) + 1] <<"  "<< pix[y*WIDTH + x] << endl;
		}
	}

	double aftertime = glfwGetTime();
	double totaltime = aftertime - beforetime;
	cout << "�ܹ����ѵ�ʱ�䣺 " << totaltime << endl
		<< "׷�ٵĹ��������� " << aa
		<< "\n�����������: " << bb
		<< "\n�����������:  " << cc
		<< "\n������ཻ������ĸ�����  " << dir;

	//ofs.close();
	//delete[] image;
	//*/
	//�˴�Ϊ�Թ��������������ݵķ���
	//unsigned int �������⣬�޷���Ⱦ����ɫ�Ƚ�С�����أ�����ȫ����ɫ
	//unsigned int* pix = new unsigned int[WIDTH*HEIGHT * 3];
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
		//glRasterPos2f(-1,-1);   
		//glPixelZoom(2, 2); 
		//��Ⱦ��������ԭ����GLFW������ʼ�������Լ�TYPE��������ΪGL_UNSIGNED_BYTE��

		glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pix);
		glfwSwapBuffers(window); //swap buffer
	}
	delete[] pix;
	glfwTerminate();
}

//[comment]Sphere<double>
// In the main function, we will create the scene which is composed of 5 spheres
// and 1 light (which is also a sphere). Then, once the scene description is complete
// we render that scene, by calling the render() function.
//[/comment]


void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char **argv)
{

	// �����Լ��ĳ���

	// *******************************************************************************
	// YOUR CODES
	// position, radius, surface color, reflectivity, transparency, emission color
	vector<Sphere *> spheres;

	//spheres.push_back(new Sphere(Vec3f(0.0, 0.0, 0.0), 15000, Vec3f(0.5, 0.20, 0.10), 0.8, 0.0));
	//spheres.push_back(new Sphere(Vec3f(0, -10004, -20), 10000, Vec3f(0.10, 0.40, 0.40), 1.0, 0.0));

	//spheres.push_back(new Sphere(Vec3f(3, 0, -15), 1.5, Vec3f(1.00, 0.1, 0.1), 0.2, 0.95));
	//spheres.push_back(new Sphere(Vec3f(1, -1, -18), 1, Vec3f(1.0, 1.0, 1.0), 0.1, 0.9));
	//spheres.push_back(new Sphere(Vec3f(-2, 2, -15), 1, Vec3f(0.1, 0.1, 1.0), 0.5, 0.5));
	//spheres.push_back(new Sphere(Vec3f(-4, 3, -18), 1, Vec3f(0.1, 1.0, 0.1), 0.3, 0.7));

	//spheres.push_back(new Sphere(Vec3f(-4, 0, -25), 1, Vec3f(1.00, 0.1, 0.1), 0.15, 0.95));
	//spheres.push_back(new Sphere(Vec3f(-1, 1, -25), 2, Vec3f(1.0, 1.0, 1.0), 0.3, 0.0));
	//spheres.push_back(new Sphere(Vec3f(-2, -2, -25), 1, Vec3f(0.1, 0.1, 1.0), 0.15, 0.5));
	spheres.push_back(new Sphere(Vec3f(5, 5, -25), 2, Vec3f(0.1, 1.0, 0.1), 0.2, 0.7));

	//light
	//spheres.push_back(new Sphere(Vec3f(-10, 20, 0), 3, Vec3f(0.5, 0.7, 0.9), 0, 0, Vec3f(1)));
	//spheres.push_back(new Sphere(Vec3f(0, 10, 0), 3, Vec3f(1.0, 0.5, 0.6), 0, 0, Vec3f(1)));


	// ʾ����ʼ
	//srand(13);

	//vector<Sphere *> spheres;

	////����ͼ
	//Sphere* biggest = new Sphere(Vec3f(0.0, -10004, -20), 15000, Vec3f(0.20, 0.20, 0.20), 0.5, 0.0);

	//Sphere* big_right = new Sphere(Sphere(Vec3f(0.0, 5.0, -20), 3, Vec3f(0.20, 0.20, 0.20), 0.5, 0.0));
	//Sphere* big_left = new Sphere(Sphere(Vec3f(0.0, -10004, -20), 10005, Vec3f(0.20, 0.20, 0.20), 0.5, 0.0));

	//Sphere* big_left_left = new Sphere(Sphere(Vec3f(0.0, -3.0, -20), 3, Vec3f(0.20, 0.20, 0.20), 0.5, 0.0));

	//Sphere* a = new Sphere(Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.20, 0.60, 0.50), 1, 0.0));

	//Sphere* b = new Sphere(Sphere(Vec3f(1.0, -3.0, -20.0), 1, Vec3f(1.00, 0.32, 0.36), 0.05, 0.1));
	//Sphere* c = new Sphere(Sphere(Vec3f(-1.0, -3.0, -20.0), 1, Vec3f(0.90, 0.76, 0.46), 0.01, 0.0));

	//Sphere* d = new Sphere(Sphere(Vec3f(3.0, 5.0, -20), 2, Vec3f(1.00, 0.27, 0.15), 0.3, 0.0));
	//Sphere* e = new Sphere(Sphere(Vec3f(-1.0, 3.0, -20), 1, Vec3f(0.20, 0.8, 0.50), 0.2, 0.0));

	////big_right include d e
	//biggest->right = big_right;
	//big_right->left = d;
	//big_right->right = e;
	////big_left include a,b,c
	//biggest->left = big_left;

	//big_left->left = big_left_left;
	//big_left_left->left = b;
	//big_left_left->right = c;

	//big_left->right = a;

	///*position, radius, surface color, reflectivity, transparency, emission color*/
	//spheres.push_back(&Sphere(Vec3f(0.0, -10004, -20), 10000, Vec3f(0.10, 0.40, 0.40), 1, 0.0));
	//spheres.push_back(a);
	//spheres.push_back(b);
	//spheres.push_back(c);
	//spheres.push_back(d);
	//spheres.push_back(e);

	cout << glm::distance(glm::vec3(0.0, -10004, -20), glm::vec3(-3.0, 8.0, -30.0)) << endl
		<< glm::distance(glm::vec3(0.0, -10004, -20), glm::vec3(10.0, -3.0, -20.0)) << endl
		<< glm::distance(glm::vec3(0.0, -10004, -20), glm::vec3(-5.0, -3.0, -20.0)) << endl
		<< glm::distance(glm::vec3(0.0, -10004, -20), glm::vec3(-3.0, -1.0, -20.0));

	// light
	Sphere* light = new Sphere(Vec3f(0.0, 10, 0), 3, Vec3f(1.00, 0.50, 0.20), 0, 0.0, Vec3f(3));
	//spheres.push_back(light);
	//render(spheres);

	//glfwGetPrimaryMonitor();  //��Ϊ�����ڶ�����������ȫ��

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

	render(spheres, window, light);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}