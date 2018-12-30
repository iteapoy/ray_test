#pragma once
#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "Material.h"
#include <cmath>

// 定义光
static Vec3f lightDir = Vec3f(1.75, 2, 1.5).normalize();
static Color La = Color::black();
static Color Ld = Color::white();
static Color Ls = Color::white();

// 定义一个phong材质
class PhongMaterial :public Material
{
private:
	// Ka：环境光反射系数  Kd:漫反射系数   Ks：镜面反射系数, shininess：反射强度
	Color   Ka;
	Color   Kd;
	Color   Ks;
	float   shininess;
public:
	// 初始化
	PhongMaterial() {};
	PhongMaterial(const Color& _Kd, const Color& _Ks, const float& _shininess, float _reflectiveness = 0):
		Kd(_Kd), Ks(_Ks), shininess(_shininess){reflectiveness = _reflectiveness;}

	// ---------------------- 漫反射公式 ------------------------
	//        Kd = Id·Kd·(N·L）
	// Id是光源漫反射的成分，Kd是物体的漫反射系数，N是法线，L是入射光向量

	// ---------------------- 镜面反射公式  ------------------------
	//        Ks = Is·Ks·（V·R)^shininess
	// R=2(L·N)·N-L
	// Is是光源镜面反射的成分，Ks是物体的镜面反射系数，V是相机方向向量，R是反射向量
	// 
	// 为了提高计算效率，用HalfVector H来计算镜面反射
	//        Ks = Is·Ks·（N·H)^shininess
	// H=(L+V)/2
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal) {
		float NdotL = max(normal.dot(lightDir), float(0));
		Vec3f H = (lightDir - ray.direction).normalize();
		float NdotH = max(normal.dot(H), float(0));

		Color ambient = this->Ka *La;
		Color diffuse = this->Kd * NdotL*Ld;
		Color specular = this->Ks * pow(NdotH,this->shininess)*Ls;

		return ambient + diffuse + specular;
	}

	// 析构函数
	virtual ~PhongMaterial() {}

};

#endif