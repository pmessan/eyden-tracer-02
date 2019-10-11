#pragma once

#include "ShaderFlat.h"

class CScene;

class CShaderPhong : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhong(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhong(void) = default;

	virtual Vec3f Shade(const Ray& ray) const override
	{
		// --- PUT YOUR CODE HERE ---
		Ray I;
		Vec3f Li; 
		Vec3f R;
		Vec3f V; 
		Vec3f diffuse(0, 0, 0);
		Vec3f specular(0, 0, 0);
		Vec3f cs = RGB(1, 1, 1);
		Vec3f cd = CShaderFlat::Shade(ray);

		I.org = ray.org + ray.t * ray.dir;
		
		float i_N;
		Vec3f m_Normal = ray.hit->GetNormal(ray);

		Vec3f L_a(0, 0, 0);

		for(auto obj: m_scene.m_vpLights){	
			Li = obj->Illuminate(I).value();
			L_a += Li;
			i_N = std::max(0.f,I.dir.dot(ray.hit->GetNormal(ray)));
			V = -1 * ray.dir;
			diffuse += Li*i_N;
			R = normalize((2 * i_N * ray.hit->GetNormal(ray)) - I.dir);		
			specular += Li * pow(V.dot(R), m_ke);
		}
		
		Vec3f col;
		Vec3f m_ca = cd;

		for (int i = 0; i < 3; i++){
			col[i] = m_ca[i] * m_ka * L_a[i]+ m_kd * cd[i] * diffuse[i] + m_ks * specular[i]*cs[i];
		}

		
		for(auto obj: m_scene.m_vpLights){
			Li = obj->Illuminate(I).value();
			if (m_scene.Occluded(I) && (ray.hit != I.hit))
				col/=1.2;
		}


		return col;
	}

	
private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};