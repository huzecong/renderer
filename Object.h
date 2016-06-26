//
// Created by Kanari on 2016/6/23.
//

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H

#include "Utility.h"
#include "Ray.h"

class Material;

class Object {
protected:
	Material *m_material;
	Color m_color;
public:
	Object(Material *material, const Color &color)
			: m_material(material), m_color(color) { }

	inline const Material *material() const {
		return m_material;
	}

	// Returns *normalized* normal vector at point
	virtual Vec3 normalAt(const Point &point) const = 0;
	// Finds intersection point with ray
	// @return If no intersection occurs, returns false
	virtual bool intersect(const Ray &ray, IntersectData &data) const = 0;

	// Returns color at point (whether pure color or texture)
	virtual Color colorAt(const Point &point) const {
		return m_color;
	}
};

class Sphere : public Object {
protected:
	Point center;
	double radius;
public:
	Sphere(const Point &_center, const double &_radius, Material *material,
		   const Color &color = kColor::Black)
			: center(_center), radius(_radius), Object(material, color) { }

	inline Vec3 normalAt(const Point &point) const {
//		assert(fabs(dist(point, center) - radius) < kEps);
		return cv::normalize(point - center);
	}

	inline bool intersect(const Ray &ray, IntersectData &data) const {
		Vec3 s = ray.o - center;
		assert(fabs(ray.v.dot(ray.v) - 1) < kEps); // a = 1
		double b = 2 * ray.v.dot(s);
		double c = s.dot(s) - sqr(radius);
		double delta = sqr(b) - 4 * c;
		if (delta > 0.0) {
			// Check if ray is inside sphere
			double sign = (c < 0.0) ? 1.0 : -1.0;
			double t = (-b + sign * sqrt(delta)) / 2.0;
			if (t > 0.0) {
				data.dist = t;
				data.surface_normal = normalAt(ray.o + ray.v * t);
				data.hit_obj = this;
				return true;
			} else return false;
		} else return false;
	}
};

class TexturedSphere : public Sphere {
protected:
	Mat texture;
	cv::Matx<float, 3, 3> base_rotation;
public:
	// Base normal: normal of point as upper left of texture image
	TexturedSphere(const Point &_center, const double &_radius,
				   Material *material, char *texture_path,
				   const Point &base_normal, const Vec3 &base_dir) :
			Sphere(_center, _radius, material, kColor::Black) {
		cv::imread(texture_path, CV_LOAD_IMAGE_COLOR)
				.convertTo(texture, CV_32FC3, 1.0 / 256);

		Vec3 x_axis = base_normal.cross(base_dir);
		base_rotation = cv::Matx<float, 3, 3>{
				x_axis.val[0], base_normal.val[0], base_dir.val[0],
				x_axis.val[1], base_normal.val[1], base_dir.val[1],
				x_axis.val[2], base_normal.val[2], base_dir.val[2]
		}.inv();
	}

	inline Color colorAt(const Point &point) const {
		Point bias_normal = cv::normalize(base_rotation * (point - center));
		double u = atan2(bias_normal.val[2], bias_normal.val[0]) / (2 * M_PI) +
				   0.5;
		double v = asin(bias_normal.val[1]) / M_PI + 0.5;
		int r = (int)floor(v * texture.rows), c = (int)floor(u * texture.cols);
		if (r == texture.rows) --r;
		if (c == texture.cols) --c;
		return texture.at<Vec3>(r, c);
	}
};

class Triangle {
	Point p[3];
	Vec3 n_normal;
	cv::Matx<float, 3, 3> inside_inv, inside;
public:
	// Points given in counter-clockwise direction
	Triangle(const Point &a, const Point &b, const Point &c) : p{a, b, c} {
		Vec3 x = p[0] - p[2], y = p[1] - p[2];
		n_normal = cv::normalize(x.cross(y));
		inside = cv::Matx<float, 3, 3>(
				x.val[0], y.val[0], n_normal.val[0],
				x.val[1], y.val[1], n_normal.val[1],
				x.val[2], y.val[2], n_normal.val[2]);
		inside_inv = inside.inv(cv::DECOMP_SVD);
	}

	Triangle(Point *const &_p) : Triangle(_p[0], _p[1], _p[2]) { }

	inline Vec3 normal() const {
		return n_normal;
	}

	inline bool intersect(const Ray &ray, IntersectData &data) const {
		double denom = ray.v.dot(n_normal);
		data.surface_normal = n_normal;
		double t = denom;    // Just an indicator of intersection
		if (fabs(denom) > kEps)
			t = (p[2] - ray.o).dot(n_normal) / denom;
		if (t > 0.0) {
			Point point = ray.o + t * ray.v - p[2];
			Vec3 coef = inside_inv * point;
			if (-kEps <= coef.val[0] && -kEps <= coef.val[1] &&
				coef.val[0] + coef.val[1] <= 1 && fabs(coef.val[2]) < kEps) {
				data.dist = t;
				data.surface_normal = normal();
				return true;
			} else return false;
		} else return false;
	}
};

class Plane : public Object {
protected:
	Triangle t[2];
	Vec3 axis[2];
	Point point;
public:
	Plane(const Point &p, const Vec3 &x_axis, const Vec3 &y_axis,
		  Material *material, const Color &color = kColor::Black)
			: point(p), axis{x_axis, y_axis},
			  t{{p,                   p + x_axis, p + y_axis},
				{p + x_axis + y_axis, p + y_axis, p + x_axis}},
			  Object(material, color) { }

	inline Vec3 normalAt(const Point &point) const {
		return t[0].normal();
	}

	inline bool intersect(const Ray &ray, IntersectData &data) const {
		bool ret = t[0].intersect(ray, data);
		if (!ret) ret = t[1].intersect(ray, data);
		if (ret) data.hit_obj = this;
		return ret;
	}
};

class TexturedPlane : public Plane {
protected:
	Mat texture;
public:
	TexturedPlane(const Point &p, const Vec3 &x_axis, const Vec3 &y_axis,
				  Material *material, char *path)
			: Plane(p, x_axis, y_axis, material, kColor::Black) {

	}
};

#endif //RENDERER_OBJECT_H
