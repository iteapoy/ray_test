#pragma once
#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "color.h"
#include "Material.h"
#include <cmath>

// 定义光
static Vec3f lightDir = Vec3f(1, 1, 1).normalize();
static Color lightColor = Color::white();

// 定义一个phong材质
class PhongMaterial :public Material
{
private:
	// diffuse:漫反射的颜色， specular：镜面反射的颜色, shininess：反射强度
	Color   diffuse;
	Color   specular;
	float   shininess;
public:
	// 初始化
	PhongMaterial() {};
	PhongMaterial(const Color& _diffuse, const Color& _specular, const float& _shininess, float _reflectiveness = 0):
		diffuse(_diffuse), specular(_specular), shininess(_shininess){reflectiveness = _reflectiveness;}

	// ---------------------- 漫反射公式 ------------------------
	//        diffuse = Id·Kd·(N·L）
	// Id是光源漫反射的成分，Kd是物体的漫反射系数，N是法线，L是入射光向量

	// ---------------------- 镜面反射公式  ------------------------
	//        specular = Is·Ks·（V·R)^shininess
	// R=2(L·N)·N-L
	// Is是光源镜面反射的成分，Ks是物体的镜面反射系数，V是相机方向向量，R是反射向量
	// 
	// 为了提高计算效率，用HalfVector H来计算镜面反射
	//        specular = Is·Ks·（N·H)^shininess
	// H=(L+V)/2
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal) {
		float NdotL = normal.dot(lightDir); 
		Vec3f H = (lightDir - ray.direction).normalize();
		float NdotH = normal.dot(H);
		Color diffuseTerm = this->diffuse * max(NdotL,float(0));
		Color specularTerm = this->diffuse * pow(max(NdotH,float(0)),this->shininess);

		return lightColor * (diffuseTerm + specularTerm);
	}

	// 析构函数
	virtual ~PhongMaterial() {}

};

#endif