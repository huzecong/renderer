//
// Created by Kanari on 2016/6/24.
//

#include "Material.h"

using namespace std;

Vec3 Material::sampleUpperHemisphere(const Vec3 &normal,
									 const double &exponent) const {
	double r = Random::uniform_01();
	double phi = Random::uniform_01() * 2 * M_PI;
	double theta = exponent == 1 ? acos(sqrt(r)) :        // Uniform
				   acos(pow(r, 1 / (exponent + 1)));    // Importance

	double stheta = sin(theta);
	double x_coef = stheta * cos(phi), y_coef = stheta * sin(phi);

	// Spin above vector to surface with _normal_ as normal
	// aux is a auxiliary vector that is not collinear with normal
	static Vec3 aux = Random::vector();
	Vec3 x_axis = normal.cross(aux);
	Vec3 y_axis = normal.cross(x_axis);
	return x_axis * x_coef + y_axis * y_coef + r * normal;
}

Color Material::indirectDiffuse(const Ray &ray,
								const IntersectData &hit_data,
								int depth, const Scene &scene) const {
	Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
	Vec3 next_dir = sampleUpperHemisphere(hit_data.surface_normal, 1);
	Ray next_ray(hit_point, next_dir);
	next_ray.dist = ray.dist + hit_data.dist;
	Color color = scene.trace(next_ray, depth + 1);
	return (kDiffuse * colorAt(ray, hit_data) + One * bDiffuse).mul(color);
}

Color Material::indirectSpecular(const Ray &ray,
								 const IntersectData &hit_data, int depth,
								 const Scene &scene) const {
	Vec3 reflect_ray = reflect(ray.v, hit_data.surface_normal);
	Vec3 next_dir = sampleUpperHemisphere(reflect_ray, exp_phong);
	if (next_dir.dot(hit_data.surface_normal) < 0) return kColor::Black;
	Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
	Ray next_ray(hit_point, next_dir);
	next_ray.dist = ray.dist + hit_data.dist;
	Color color = scene.trace(next_ray, depth + 1);
	return (kSpecular * colorAt(ray, hit_data) + One * bSpecular).mul(color);
}

Color Material::directIllumination(const Ray &ray,
								   const IntersectData &hit_data,
								   const Scene &scene) const {
	Color color;
	double light_dist = ray.dist + hit_data.dist;
	for (auto light : scene.lights()) {
		Point light_pos = light->sample();
		Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
		Vec3 shadow_ray_v = light_pos - hit_point;
		Ray shadow_ray(hit_point, shadow_ray_v);
		double shadow_ray_length2 = shadow_ray_v.dot(shadow_ray_v);

		Vec3 light_col = light->color(light_dist + sqrt(shadow_ray_length2));

		IntersectData shadow_data;
		if (!scene.castShadowRay(shadow_ray, &shadow_data) ||
			sqr(shadow_data.dist) > shadow_ray_length2) {
			// Diffuse color
			const Vec3 &N = hit_data.surface_normal;
			const Vec3 &L = shadow_ray.v;
			Color diffuse_coef = kDiffuse * colorAt(ray, hit_data) +
								 bDiffuse * One;
			color += diffuse_coef.mul(
					max((Vec3::value_type)0.0, N.dot(L)) * light_col);

			// Specular color (using Blinn-Phong model)
			Vec3 H = cv::normalize(shadow_ray.v - ray.v);
			Color specular_coef = kSpecular * colorAt(ray, hit_data) +
								  bSpecular * One;
			color += specular_coef.mul(
					pow(max((Vec3::value_type)0.0, N.dot(H)), exp_phong) *
					light_col);
		}
	}
	return color;
}


Color Material::mirrorReflection(const Ray &ray, const IntersectData &hit_data,
								 int depth, const Scene &scene) const {
	Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
	Ray next_ray(hit_point, reflect(ray.v, hit_data.surface_normal));
	next_ray.dist = ray.dist + hit_data.dist;
	Color color = scene.trace(next_ray, depth + 1);
	return kReflect.mul(color);
}

Color Material::calculateColor(const Ray &ray, const IntersectData &hit_data,
							   int depth, const Scene &scene) const {
	if (kEmissive != Zero) return kEmissive;

	Vec3 refract_ray;
	double schlick;
	bool should_refract;

	double kR = 0.0;
	if (kReflect != Zero && kRefract != Zero) {
		should_refract = refract(ray.v, hit_data.surface_normal, kIor,
								 refract_ray, schlick);
		kR = max((1 - schlick) * kRefract + schlick * kReflect);
	} else if (kReflect != Zero) {
		should_refract = false;
		kR = max(kReflect);
	}

	Color color;
	Color obj_color = colorAt(ray, hit_data);
	double kD = max(kDiffuse * obj_color) + bDiffuse;
	double kS = max(kSpecular * obj_color) + bSpecular;
	double sum = kD + kS + kR;
	double prob = Random::uniform_01() * sum;
	if (prob <= kD) {
		// Diffuse
		color = sum / kD * indirectDiffuse(ray, hit_data, depth, scene);
	} else if (prob <= kD + kS) {
		// Specular
		color = sum / kS * indirectSpecular(ray, hit_data, depth, scene);
	} else {
		// Reflect and Refract
		Color re;
		if (!should_refract) {
			re = mirrorReflection(ray, hit_data, depth, scene);
		} else {
			if (roulette(1 - schlick, schlick)) {
				// Refract
				Point hit_point = ray.o + ray.v * (hit_data.dist + kEps);
				Ray next_ray(hit_point, refract_ray);
				next_ray.dist = ray.dist + hit_data.dist;
				re = kRefract.mul(scene.trace(next_ray, depth + 1));
			} else {
				// Reflect
				re = mirrorReflection(ray, hit_data, depth, scene);
			}
		}
		color = sum / kR * re;
	}

	return color;
}

