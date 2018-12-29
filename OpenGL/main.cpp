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

//球体
class Sphere
{
public:
	Vec3f center;                           // 球心
	float radius, radius2;                 // 半径，半径的平方
	Vec3f surfaceColor, emissionColor;      /// surface color and emission (light)
	float transparency, reflection;         /// surface transparency and reflectivity

	Sphere* left = NULL;
	Sphere* right = NULL;

	// 球体初始化
	Sphere() {}
	// c：球心， r：半径 ，sc：表面颜色， ec：发出的颜色 
	// transp：透明度， refl：反射
	Sphere(const Vec3f &c, const float &r, const Vec3f &sc,
		const float &refl = 0, const float &transp = 0, const Vec3f &ec = 0) :
		center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec),
		transparency(transp), reflection(refl) {}

	// 判断是否相交
	// rayorig：光线的原点
	// raydir:  光线的方向
	// t0: 第一个交点
	// t1: 第二个交点

	bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float *t0, float *t1) const
	{
		// ----------------------- 公式 ------------------------
		// r(t) = o + td
		// v：光线原点和球心的距离
		// v = o-d
		// DdotV = v·d
		// t^2 + (2v·d)t + (v^2-r^2) = 0
		// t0 = -v·d - sqrt((v·d)^2-(v^2-r^2)) = -DdotV -sqrt(DdotV^2-(v^2-r^2))
		// ----------------------- 公式 ------------------------

		Vec3f v = center - rayorig; 
		float DdotV = v.dot(raydir);   

		if (DdotV < 0) return false;
		float d2 = v.dot(v) - (DdotV * DdotV);
		if (d2 > radius2) return false;
		float thc = sqrt(radius2 - d2);
		if (t0 != NULL && t1 != NULL) {
			// 到前一个交点的距离
			*t0 = DdotV - thc;
			// 到后一个交点的距离
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
			// 如果光线在球的里面，就采用前面的交点
			if (t0 < 0) t0 = t1;
			if (t0 < tnear) {
				//判断 tnear 是否是最近的交点
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
		Vec3f nhit = phit - sphere->center; // 交点法向量
		nhit.normalize(); // 规范化
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

			// 反射
			float IdotN = raydir.dot(nhit); // 光线方向规范化
			float facingratio = max(float(0), -IdotN);
			// 不同光波分量被折射和反射 视线垂直于表面时，反射较弱，而当视线非垂直表面时，夹角越小，反射越明显 (菲涅尔效应)
			float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
			Vec3f refldir;
			//refldir = glm::reflect(raydir, nhit);          这个函数实际进行的计算就是下面的语句
			refldir = raydir - nhit * 2 * raydir.dot(nhit);
			refldir.normalize();
			// 递归调用
			Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1, my_sphere);

			bb++;																	//BB计算反射光线数量
			Vec3f refraction = 0;
			// 如果透明度不为零，进行折射计算
			if (sphere->transparency > 0) {
				// 折射
				float ior = 1.1;
				float eta = 1 / ior;												//玻璃的折射度1/1.2
				float k = 1 - eta * eta * (1 - IdotN * IdotN);
				Vec3f refrdir = raydir * eta - nhit * (eta *  IdotN + sqrt(k));
				refrdir.normalize();

				cc++;																//CC计算折射光线数量
				refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1, my_sphere);		// 递归调用,使用自己的参
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
							// 判断该点的光是否和源光线相交，如果相交，计算阴影

							if (spheres[j]->intersect(phit + (nhit * bias), lightDirection, &t0, &t1)) {
								shadow = max(0.0, shadow - (1.0 - spheres[j]->transparency));
								transmission = transmission * spheres[j]->surfaceColor * shadow;
								//break;
							}
						}
					}
					// *******************************************************************************

					// using phong model to calculate
					// 每一条光线，都会对这点像素造成影响
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
	cout << "总共花费的时间： " << totaltime << endl
		<< "追踪的光线数量： " << aa
		<< "\n反射光线数量: " << bb
		<< "\n折射光线数量:  " << cc
		<< "\n与光线相交过的球的个数：  " << dir;

	//ofs.close();
	//delete[] image;
	//*/
	//此处为试过的生成像素数据的方法
	//unsigned int 会有问题，无法渲染出颜色比较小的像素，导致全屏黑色
	//unsigned int* pix = new unsigned int[WIDTH*HEIGHT * 3];
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
		//glRasterPos2f(-1,-1);   
		//glPixelZoom(2, 2); 
		//渲染不出来的原因是GLFW环境初始化问题以及TYPE参数（需为GL_UNSIGNED_BYTE）

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

	// 构建自己的场景

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


	// 示例开始
	//srand(13);

	//vector<Sphere *> spheres;

	////场景图
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

	//glfwGetPrimaryMonitor();  //作为倒数第二个参数可以全屏

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

	render(spheres, window, light);

	system("pause");
	return 0;
}

void key_call_back(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}