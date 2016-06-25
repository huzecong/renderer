//
// Created by Kanari on 2016/6/24.
//

#include "Material.h"

using namespace std;


Color Material::calculateColor(const Ray &ray, const IntersectData &hit_data,
							   int depth, const Scene &scene) const {
	return kEmissive;
}

Color DiffuseMaterial::directIllumination(const Ray &ray,
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
			color += kDiffuse.mul(max((Vec3::value_type)0.0, N.dot(L)) * light_col);

			// Specular color (using Blinn-Phong model)
			Vec3 H = cv::normalize(shadow_ray.v - ray.v);
			color += kSpecular.mul(
					pow(max((Vec3::value_type)0.0, N.dot(H)), exp_phong) * light_col);
		}
	}
	return color;
}

Vec3 DiffuseMaterial::sampleUpperHemisphere(const Vec3 &normal,
											const double &exponent) const {
	double r = Random::uniform_01();
	double phi = Random::uniform_01() * 2 * M_PI;
	double theta = exponent == 1 ? acos(sqrt(r)) :		// Uniform
				   acos(pow(r, 1 / (exponent + 1)));	// Importance

	double stheta = sin(theta);
	double x_coef = stheta * cos(phi), y_coef = stheta * sin(phi);

	// Auxiliary vector that is not collinear with normal
	static Vec3 aux{(float)Random::uniform_01(),
					(float)Random::uniform_01(),
					(float)Random::uniform_01()};
	Vec3 x_axis = normal.cross(aux);
	Vec3 y_axis = x_axis.cross(normal);
	return x_axis * x_coef + y_axis * y_coef + r * normal;
}

Color DiffuseMaterial::indirectDiffuse(const Ray &ray,
									   const IntersectData &hit_data,
									   int depth, const Scene &scene) const {
	Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
	Vec3 next_dir = sampleUpperHemisphere(hit_data.surface_normal, 1);
	Ray next_ray(hit_point, next_dir);
	next_ray.dist = ray.dist + hit_data.dist;
	Color color = scene.trace(next_ray, depth + 1);
	return kDiffuse.mul(color);
}

Color DiffuseMaterial::calculateColor(const Ray &ray,
									  const IntersectData &hit_data,
									  int depth, const Scene &scene) const {
	Color color = kEmissive;
//	color += directIllumination(ray, hit_data, scene);
	color += indirectDiffuse(ray, hit_data, depth, scene);
	return color;
}


Color GlossyMaterial::calculateColor(const Ray &ray,
									 const IntersectData &hit_data,
									 int depth, const Scene &scene) const {
	return cv::Vec<float, 3>();
}


Color MirrorMaterial::mirrorReflection(const Ray &ray, const IntersectData &hit_data,
									   int depth, const Scene &scene) const {
	Point hit_point = ray.o + ray.v * (hit_data.dist - kEps);
	Ray next_ray(hit_point, reflect(ray.v, hit_data.surface_normal));
	next_ray.dist = ray.dist + hit_data.dist;
	Color color = scene.trace(next_ray, depth + 1);
	return kReflect.mul(color);
}

Color MirrorMaterial::calculateColor(const Ray &ray,
									 const IntersectData &hit_data,
									 int depth, const Scene &scene) const {
	return mirrorReflection(ray, hit_data, depth, scene);
}

Color DielectricMaterial::calculateColor(const Ray &ray,
										 const IntersectData &hit_data,
										 int depth, const Scene &scene) const {
	Vec3 refract_ray;
	double ior = dynamic_cast<const DielectricMaterial *>(
			hit_data.hit_obj->material())->ior;
	double schlick;
	bool should_refract = refract(ray.v, hit_data.surface_normal, ior,
								  refract_ray, schlick);
	if (!should_refract) {
		return mirrorReflection(ray, hit_data, depth, scene);
	} else {
		if (roulette(1 - schlick, schlick)) {
//		if (roulette(0.5, 0.5)) {
			// Refract
			Point hit_point = ray.o + ray.v * (hit_data.dist + kEps);
			Ray next_ray(hit_point, refract_ray);
			next_ray.dist = ray.dist + hit_data.dist;
			Color color = scene.trace(next_ray, depth + 1);
			return kRefract.mul(color);
//			return 2 * (1 - schlick) * kRefract.mul(color);
		} else {
			// Reflect
			return mirrorReflection(ray, hit_data, depth, scene);
//			return 2 * schlick * mirrorReflection(ray, hit_data, depth, scene);
		}
	}
}

DiffuseMaterial *::kMaterial::kPlastic(const Color &color) {
	return new DiffuseMaterial(color * 0.1, color, One, 30);
//	return new DiffuseMaterial(color.mul({0.11, 0.06, 0.11}), {0.43, 0.47, 0.54}, {0.33, 0.33, 0.52}, 10);
}

