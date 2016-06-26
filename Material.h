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

class Material {
public:
	Material() { }

	inline virtual const bool passesLight() const {
		return false;
	}

	inline Color colorAt(const Ray &ray,
						 const IntersectData &hit_data) const {
		Point hit_point = ray.o + ray.v * hit_data.dist;
		Color color = hit_data.hit_obj->colorAt(hit_point);
		return color;
	}

	virtual Color calculateColor(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const = 0;
};

class EmissiveMaterial : public Material {
protected:
	Color kEmissive;
public:
	EmissiveMaterial(const Color &emissive) : kEmissive(emissive) { }

	virtual Color calculateColor(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const;
};

class MirrorMaterial : public Material {
protected:
	Color kReflect;

	Color mirrorReflection(const Ray &ray, const IntersectData &hit_data,
						   int depth, const Scene &scene) const;
public:
	MirrorMaterial(const Color &reflect) : kReflect(reflect) { }

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

// Materials using Phong model

class DiffuseMaterial : public Material {
protected:
	Color kDiffuse, bDiffuse;        // diffuse_coef = kDiffuse .* color + bDiffuse
	double exp_phong;    // exponent in Phong model

	// Direct lighting with Phong model
	virtual Color directIllumination(const Ray &ray,
									 const IntersectData &hit_data,
									 const Scene &scene) const;
	// Importance sampling
	Vec3 sampleUpperHemisphere(const Vec3 &normal,
							   const double &exponent) const;
	// Indirect lighting through diffuse
	Color indirectDiffuse(const Ray &ray, const IntersectData &hit_data,
						  int depth, const Scene &scene) const;
public:
	DiffuseMaterial(const Color &diffuse, const Color &diffuse_bias)
			: kDiffuse(diffuse), bDiffuse(diffuse_bias) { }

	virtual Color calculateColor(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const;
};

class MetallicMaterial : public DiffuseMaterial {
protected:
	Color kSpecular, bSpecular;
	double exp_phong;
	double kD, kS;

	// Direct lighting with Phong model
	virtual Color directIllumination(const Ray &ray,
									 const IntersectData &hit_data,
									 const Scene &scene) const;

	// Indirect lighting through specular reflection
	Color indirectSpecular(const Ray &ray, const IntersectData &hit_data,
						   int depth, const Scene &scene) const;
public:
	MetallicMaterial(const Color &diffuse, const Color &diffuse_bias,
					 const Color &specular, const Color &specular_bias,
					 const double &_exp_phong)
			: DiffuseMaterial(diffuse, diffuse_bias),
			  kSpecular(specular), bSpecular(specular_bias),
			  exp_phong(_exp_phong) {
		kD = std::max(std::max(kDiffuse.val[0], kDiffuse.val[1]),
					  kDiffuse.val[2]);
		kS = std::max(std::max(kSpecular.val[0], kSpecular.val[1]),
					  kSpecular.val[2]);
	}

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

class GlossyMaterial : public MirrorMaterial {
public:
	GlossyMaterial(const Color &reflect) : MirrorMaterial(reflect) { }

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

class DielectricMaterial : public MirrorMaterial {
protected:
	Color kRefract;
	double ior;        // index of refraction
public:
	DielectricMaterial(const Color &reflect, const Color &refract,
					   const double &_ior)
			: MirrorMaterial(reflect), kRefract(refract), ior(_ior) { }

	inline virtual const bool passesLight() const {
		return true;
	}

	Color calculateColor(const Ray &ray, const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};

#endif //RENDERER_MATERIAL_H
