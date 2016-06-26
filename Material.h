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
	Color kEmissive;
	Color kDiffuse, bDiffuse;    // diffuse_coef = kDiffuse .* color + bDiffuse
	Color kSpecular, bSpecular;    // specular_coef = kSpecular .* color + bSpecular
	double exp_phong;
	Color kReflect;
	Color kRefract;
	double kIor;
	double kS, kD;

	Color mirrorReflection(const Ray &ray, const IntersectData &hit_data,
						   int depth, const Scene &scene) const;

	// Direct lighting with Phong model
	Color directIllumination(const Ray &ray,
							 const IntersectData &hit_data,
							 const Scene &scene) const;
	// Importance sampling
	Vec3 sampleUpperHemisphere(const Vec3 &normal,
							   const double &exponent) const;
	// Indirect lighting through diffuse
	Color indirectDiffuse(const Ray &ray, const IntersectData &hit_data,
						  int depth, const Scene &scene) const;
	// Indirect lighting through diffuse
	Color indirectSpecular(const Ray &ray, const IntersectData &hit_data,
						   int depth, const Scene &scene) const;

public:
	Material() {
		exp_phong = 1;
	}

	inline Material &setEmissive(const Color &emissive) {
		kEmissive = emissive;
		return *this;
	}

	inline Material &setDiffuse(const Color &diffuse, const Color &diffuse_bias) {
		kDiffuse = diffuse;
		bDiffuse = diffuse_bias;
		kD = std::max(std::max(kDiffuse.val[0], kDiffuse.val[1]),
					  kDiffuse.val[2]);
		return *this;
	}

	inline Material &setSpecular(const Color &specular, const Color &specular_bias,
							const double &exponent) {
		kSpecular = specular;
		bSpecular = specular_bias;
		exp_phong = exponent;
		kS = std::max(std::max(kSpecular.val[0], kSpecular.val[1]),
					  kSpecular.val[2]);
		return *this;
	}

	inline Material &setReflect(const Color &reflect) {
		kReflect = reflect;
		return *this;
	}

	inline Material &setRefract(const Color &reflect, const Color &refract,
						   const double &ior) {
		kReflect = reflect;
		kRefract = refract;
		kIor = ior;
		return *this;
	}

	inline const bool passesLight() const {
		return length2(kRefract) > 0;
	}

	inline Color colorAt(const Ray &ray,
						 const IntersectData &hit_data) const {
		Point hit_point = ray.o + ray.v * hit_data.dist;
		Color color = hit_data.hit_obj->colorAt(hit_point);
		return color;
	}

	Color calculateColor(const Ray &ray,
						 const IntersectData &hit_data,
						 int depth, const Scene &scene) const;
};


inline Material *DiffuseMaterial(const Color &linear, const Color &bias) {
	Material *ret = new Material();
	ret->setDiffuse(linear, bias);
	return ret;
}

inline Material *MirrorMaterial(const Color &reflect) {
	Material *ret = new Material();
	ret->setReflect(reflect);
	return ret;
}

inline Material *DielectricMaterial(const Color &reflect, const Color &refract,
									const double &ior) {
	Material *ret = new Material();
	ret->setRefract(reflect, refract, ior);
	return ret;
}

inline Material *MetallicMaterial(const Color &diffuse, const Color &diffuse_bias,
								  const Color &specular, const Color &specular_bias,
								  const double &exponent) {
	Material *ret = DiffuseMaterial(diffuse, diffuse_bias);
	ret->setSpecular(specular, specular_bias, exponent);
	return ret;
}

inline Material *EmissiveMaterial(const Color &emissive) {
	Material *ret = new Material();
	ret->setEmissive(emissive);
	return ret;
}

#endif //RENDERER_MATERIAL_H
