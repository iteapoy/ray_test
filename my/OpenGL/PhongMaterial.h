#pragma once
#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "color.h"
#include "Material.h"
#include <cmath>

// �����
static Vec3f lightDir = Vec3f(1, 1, 1).normalize();
static Color lightColor = Color::white();

// ����һ��phong����
class PhongMaterial :public Material
{
private:
	// diffuse:���������ɫ�� specular�����淴�����ɫ, shininess������ǿ��
	Color   diffuse;
	Color   specular;
	float   shininess;
public:
	// ��ʼ��
	PhongMaterial() {};
	PhongMaterial(const Color& _diffuse, const Color& _specular, const float& _shininess, float _reflectiveness = 0):
		diffuse(_diffuse), specular(_specular), shininess(_shininess){reflectiveness = _reflectiveness;}

	// ---------------------- �����乫ʽ ------------------------
	//        diffuse = Id��Kd��(N��L��
	// Id�ǹ�Դ������ĳɷ֣�Kd�������������ϵ����N�Ƿ��ߣ�L�����������

	// ---------------------- ���淴�乫ʽ  ------------------------
	//        specular = Is��Ks����V��R)^shininess
	// R=2(L��N)��N-L
	// Is�ǹ�Դ���淴��ĳɷ֣�Ks������ľ��淴��ϵ����V���������������R�Ƿ�������
	// 
	// Ϊ����߼���Ч�ʣ���HalfVector H�����㾵�淴��
	//        specular = Is��Ks����N��H)^shininess
	// H=(L+V)/2
	virtual Color sample(const Ray& ray, const Vec3f& position, const Vec3f& normal) {
		float NdotL = normal.dot(lightDir); 
		Vec3f H = (lightDir - ray.direction).normalize();
		float NdotH = normal.dot(H);
		Color diffuseTerm = this->diffuse * max(NdotL,float(0));
		Color specularTerm = this->diffuse * pow(max(NdotH,float(0)),this->shininess);

		return lightColor * (diffuseTerm + specularTerm);
	}

	// ��������
	virtual ~PhongMaterial() {}

};

#endif