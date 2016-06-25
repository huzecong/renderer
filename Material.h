//
// Created by Kanari on 2016/6/24.
//

#ifndef RENDERER_MATERIAL_H
#define RENDERER_MATERIAL_H

#include "Utility.h"
#include "Object.h"
#include "Light.h"
#include "Scene.h"
#include <functional>

enum MaterialType {
	MTBase,
	MTMirror,
	MTDielectric,
	MTDiffuse,
	MTGlossy
};

class Material {
protected:
	MaterialType m_type;
	Color kEmissive;
public:
	Material(const Color &emissive)
			: kEmissive(emissive) {
		m_type = MTBase;
	}

	inline const MaterialType type() const {
		return m_type;
	}

	inline const Color &emissiveColor() const {
		return kEmissive;
	}

	virtual Color calculateColor(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const;
};

class MirrorMaterial : public Material {
protected:
	Color kReflect;

	Color mirrorReflection(const Ray &ray, const IntersectData &hit_data,
						   int depth, const Scene &scene) const;
public:
	MirrorMaterial(const Color &emissive, const Color &reflect)
			: Material(emissive), kReflect(reflect) {
		m_type = MTMirror;
	}

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

// Materials using Phong model

class DiffuseMaterial : public Material {
protected:
	Color kDiffuse;
	Color kSpecular;
	double exp_phong;    // exponent in Phong model

	// Direct lighting with Phong model
	Color directIllumination(const Ray &ray, const IntersectData &hit_data,
							 const Scene &scene) const;
	// Importance sampling
	Vec3 sampleUpperHemisphere(const Vec3 &normal,
							   const double &exponent) const;
	// Indirect lighting through diffuse
	Color indirectDiffuse(const Ray &ray, const IntersectData &hit_data,
						  int depth, const Scene &scene) const;
public:
	DiffuseMaterial(const Color &emissive, const Color &diffuse,
					const Color &specular, const double &_exp_phong)
			: Material(emissive), kDiffuse(diffuse), kSpecular(specular),
			  exp_phong(_exp_phong) {
		m_type = MTDiffuse;
	}

	virtual Color calculateColor(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const;
};

class GlossyMaterial : public DiffuseMaterial {
protected:
	Color kReflect;
public:
	GlossyMaterial(const Color &emissive, const Color &diffuse,
				   const Color &specular, const Color &reflect,
				   const double &_exp_phong)
			: DiffuseMaterial(emissive, diffuse, specular, _exp_phong),
			  kReflect(reflect) {
		m_type = MTGlossy;
	}

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

class DielectricMaterial : public MirrorMaterial {
protected:
	Color kRefract;
	double ior;        // index of refraction
public:
	DielectricMaterial(const Color &emissive, const Color &reflect,
					   const Color &refract, const double &_ior)
			: MirrorMaterial(emissive, reflect), kRefract(refract), ior(_ior) {
		m_type = MTDielectric;
	}

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};


namespace kMaterial {
	DiffuseMaterial *kPlastic(const Color &color);
}

#endif //RENDERER_MATERIAL_H
