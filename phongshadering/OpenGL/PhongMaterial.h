#pragma once
#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include "Material.h"
#include <cmath>

// �����
static Vec3f lightDir = Vec3f(1.75, 2, 1.5).normalize();
static Color La = Color::black();
static Color Ld = Color::white();
static Color Ls = Color::white();

// ����һ��phong����
class PhongMaterial :public Material
{
private:
	// Ka�������ⷴ��ϵ��  Kd:������ϵ��   Ks�����淴��ϵ��, shininess������ǿ��
	Color   Ka;
	Color   Kd;
	Color   Ks;
	float   shininess;
public:
	// ��ʼ��
	PhongMaterial() {};
	PhongMaterial(const Color& _Kd, const Color& _Ks, const float& _shininess, float _reflectiveness = 0):
		Kd(_Kd), Ks(_Ks), shininess(_shininess){reflectiveness = _reflectiveness;}

	// ---------------------- �����乫ʽ ------------------------
	//        Kd = Id��Kd��(N��L��
	// Id�ǹ�Դ������ĳɷ֣�Kd�������������ϵ����N�Ƿ��ߣ�L�����������

	// ---------------------- ���淴�乫ʽ  ------------------------
	//        Ks = Is��Ks����V��R)^shininess
	// R=2(L��N)��N-L
	// Is�ǹ�Դ���淴��ĳɷ֣�Ks������ľ��淴��ϵ����V���������������R�Ƿ�������
	// 
	// Ϊ����߼���Ч�ʣ���HalfVector H�����㾵�淴��
	//        Ks = Is��Ks����N��H)^shininess
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

	// ��������
	virtual ~PhongMaterial() {}

};

#endif