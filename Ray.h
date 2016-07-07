//
// Created by Kanari on 2016/6/23.
//

#ifndef RENDERER_RAY_H
#define RENDERER_RAY_H

#include "Utility.h"

struct Object;

struct Ray {
	Point o;
	Vec3 v;
	double dist;

	Ray() { }

	Ray(const Point &_o, const Vec3 &_v) : o(_o), v(cv::normalize(_v)) {
		dist = 0.0;
	}
};

// Calculate reflected ray
inline Vec3 reflect(const Vec3 &I, const Vec3 &N) {
	assert(is_normalized(I));
	assert(is_normalized(N));
	Vec3 ret = I - 2 * I.dot(N) * N;
	assert(is_normalized(ret));
	return ret;
}

// Calculate refracted ray
// Solve Fresnel's equation using Schlick's approximation
inline bool refract(const Vec3 &I, const Vec3 &_N, const double &ior,
                    Vec3 &refract_ray, double &schlick) {
	assert(is_normalized(I));
	assert(is_normalized(_N));
	double cosI = I.dot(_N);
	cosI = clamp(cosI, -1.0, 1.0);
	bool into = cosI < 0;
	double eta = ior;
	Vec3 N = into ? _N : -_N;
	if (into) {
		cosI = -cosI;
		eta = 1.0 / ior;
	}
	double k = 1 - sqr(eta) * (1 - sqr(cosI));
	if (k < 0) return false;
	refract_ray = eta * I + (eta * cosI - sqrt(k)) * N;
	assert(is_normalized(refract_ray));

//	Vec3 _r = refract_ray.cross(N), _i = I.cross(N);
//	double lr = sqrt(_r.dot(_r)), li = sqrt(_i.dot(_i));
//	assert(fabs(lr / li - eta) < kEps);

	double r0 = sqr((ior - 1) / (ior + 1));
	if (into) {
		cosI = 1 - cosI;
		assert(0 <= cosI && cosI <= 1);
	} else {
		cosI = 1 - refract_ray.dot(_N);
	}
	cosI = clamp(cosI, -1.0, 1.0);
	schlick = r0 + (1 - r0) * cosI * cosI * cosI * cosI * cosI;
	assert(0 <= schlick && schlick <= 1);
	return true;
}

struct IntersectData {
	Vec3 surface_normal;
	const Object *hit_obj;
	double dist;
};

#endif //RENDERER_RAY_H
